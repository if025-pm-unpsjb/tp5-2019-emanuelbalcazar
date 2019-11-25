#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);

// kkkkk
const int SYSTEM = 0;

void thread1(void*);

struct Task {
	char name[10];
	int priority;
	int c;
	int t;
	int d;
};

Task setr1[3] = { { "t1", 1, 2, 4, 4 }, { "t2", 2, 1, 5, 5 },
		{ "t3", 3, 1, 6, 6 } };

Task setr2[3] = { { 1, 3, 5, 5 }, { 2, 1, 7, 7 }, { 3, 2, 10, 10 } };

Task setr3[4] = { { 1, 2, 5, 5 }, { 2, 1, 6, 6 }, { 3, 2, 10, 10 }, { 4, 1, 15,
		15 } };

Task *all[] = { setr1, setr2, setr3 };

Task* get_system(int number) {
	Task *setr;
	setr = all[number];
	return setr;
}

int main() {
	// Initializes the trace recorder, but does not start the tracing.
	vTraceEnable( TRC_INIT);

	Task *setr = get_system(SYSTEM);

	// TODO cambiar el 3 por una variable calculada
	for (int i = 0; i < 3; i++) {
		Task *task = &(setr[i]);
		xTaskCreate(thread1, task->name, 128, (void*) task, configMAX_PRIORITIES - task->priority,
		NULL);
	}

	vTraceEnable( TRC_START);

	vTaskStartScheduler();
	for (;;)
		;
}

void thread1(void *params) {
	// get current task
	Task *task = (Task*) params;

	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = task->t * 100;
	int instance = 0;

	while (1) {
		xLastWakeTime = xTaskGetTickCount();

		/*printf("\nTarea: %s - inicio: %d - fin: %d - instancia: %d\n\r",
				task->name, xLastWakeTime, xFrequency, instance);*/
		led1 = !led1;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		instance++;
	}
}
