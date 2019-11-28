#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#include "semphr.h"

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);
SemaphoreHandle_t xSemaphore = 0;

void thread1(void*);

struct Task {
	char name[10];
	int priority;
	int c;
	int t;
	int d;
};

Task setr[3] = { { "t1", 1, 2, 4, 4 }, { "t2", 2, 1, 5, 5 },
		{ "t3", 3, 1, 6, 6 } };

void wait_without_block(TickType_t ticks) {
	TickType_t currentTick = xTaskGetTickCount();

	while (xTaskGetTickCount() - currentTick < ticks) {
	}

	return;
}

void access_shared_resource(TickType_t ticks) {
	wait_without_block(ticks);
}

int main() {
	// Initializes the trace recorder, but does not start the tracing.
	vTraceEnable( TRC_INIT);

	xSemaphore = xSemaphoreCreateMutex(); // @suppress("Function cannot be resolved")

	for (int i = 0; i < 3; i++) {
		Task *task = &(setr[i]);
		xTaskCreate(thread1, task->name, 256, (void*) task,
		configMAX_PRIORITIES - task->priority,
		NULL);
	}

	vTraceEnable( TRC_START);

	pc.printf("Iniciando planificador...\n\r");

	vTaskStartScheduler();
	for (;;)
		;
}

void thread1(void *params) {
	// get current task
	Task *task = (Task*) params;
	TickType_t startTime = 0;

	const TickType_t xFrequency = task->t * 1000;
	startTime = xTaskGetTickCount();

	while (1) {
		led1 = !led1;
		pc.printf("Tarea [%s]\n\r", task->name);

		if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
			pc.printf("La tarea %s accede al recurso\n\r", task->name);
			access_shared_resource(task->c * 1000);

			xSemaphoreGive(xSemaphore); // @suppress("Invalid arguments")
		}

		vTaskDelayUntil(&startTime, xFrequency);
	}
}
