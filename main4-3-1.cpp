#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#include "semphr.h"
#include "queue.h"

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);

SemaphoreHandle_t xSemaphore = 0;

QueueHandle_t xQueue1, xQueue2, xQueue3;

struct Task {
	char name[10];
	int priority;
	int c;
	int t;
	int d;
};

Task task1 = { "t1", 1, 1, 3, 4 };
Task task2 = { "t2", 1, 2, 3, 5 };
Task task3 = { "t3", 1, 3, 3, 6 };

Task *t1 = &task1;
Task *t2 = &task2;
Task *t3 = &task3;

void wait_without_block(TickType_t ticks) {
	TickType_t currentTick = xTaskGetTickCount();

	while (xTaskGetTickCount() - currentTick < ticks) {
	}

	return;
}

void access_shared_resource(TickType_t ticks) {
	wait_without_block(ticks);
}

void thread1(void *params) {
	Task *task = (Task*) params;

	int message = 1;
	int instance = 0;

	TickType_t startTime;
	startTime = xTaskGetTickCount();

	while (1) {

		if (instance == 0) {
			// ejecuto
			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name,
					instance);
			wait_without_block(1000);

			if ( xQueueSend(xQueue2, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
				pc.printf("\nMensaje de T1 a T2\n\r");
			} else {
				pc.printf("\nNo se pudo enviar el mensaje de T1 a T2\n\r");
			}
		}

		if (xQueueReceive(xQueue1, &message, portMAX_DELAY)) { // @suppress("Invalid arguments")
			// ejecuto
			pc.printf("\nT1 Recibio un mensaje, ya puede ejecutar.\n\r");

			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name, instance);

			wait_without_block(1000);

			if ( xQueueSend(xQueue2, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
				pc.printf("\nMensaje de T1 a T2\n\r");
			} else {
				pc.printf("\nNo se pudo enviar el mensaje de T1 a T2\n\r");
			}
		}

		vTaskDelayUntil(&startTime, 2000);
		instance++;
	}
}

void thread2(void *params) {
	Task *task = (Task*) params;

	int message = 1;
	int instance = 0;

	TickType_t startTime;
	startTime = xTaskGetTickCount();

	while (1) {

		if (xQueueReceive(xQueue2, &message, portMAX_DELAY)) { // @suppress("Invalid arguments")
			pc.printf("\nT2 Recibio un mensaje, ya puede ejecutar.\n\r");

			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name,
					instance);
			wait_without_block(1000);

			if ( xQueueSend(xQueue3, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
				pc.printf("\nMensaje de T2 a T3\n\r");
			} else {
				pc.printf("\nNo se pudo enviar el mensaje de T2 a T3\n\r");
			}

		}

		vTaskDelayUntil(&startTime, 2000);
		instance++;
	}
}

void thread3(void *params) {
	Task *task = (Task*) params;

	int message = 1;
	int instance = 0;

	TickType_t startTime;
	startTime = xTaskGetTickCount();

	while (1) {

		if (xQueueReceive(xQueue3, &message, portMAX_DELAY)) { // @suppress("Invalid arguments")
			pc.printf("\nT3 Recibio un mensaje, ya puede ejecutar.\n\r");

			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name,
					instance);
			wait_without_block(1000);

			if ( xQueueSend(xQueue1, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
				pc.printf("\nMensaje de T3 a T1\n\r");
			} else {
				pc.printf("\nNo se pudo enviar el mensaje de T3 a T1\n\r");
			}
		}

		vTaskDelayUntil(&startTime, 2000);
		instance++;
	}
}

/**
 * Tarea 1 → Tarea 2 → Tarea 3
 * */
int main() {
// Initializes the trace recorder, but does not start the tracing.
	vTraceEnable( TRC_INIT);

	xSemaphore = xSemaphoreCreateMutex(); // @suppress("Function cannot be resolved")
	xQueue1 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")
	xQueue2 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")
	xQueue3 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")

	xTaskCreate(thread1, t1->name, 256, (void*) t1,
	configMAX_PRIORITIES - t1->priority, NULL);

	xTaskCreate(thread2, t2->name, 256, (void*) t2,
	configMAX_PRIORITIES - t2->priority, NULL);

	xTaskCreate(thread3, t3->name, 256, (void*) t3,
	configMAX_PRIORITIES - t3->priority, NULL);

	vTraceEnable( TRC_START);

	pc.printf("Iniciando planificador...\n\r");

	vTaskStartScheduler();
	for (;;)
		;
}
