#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#include "semphr.h"
#include "queue.h"

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);

SemaphoreHandle_t xSemaphore = 0;

QueueHandle_t xQueue1, xQueue2, xQueue3, xQueue4, xQueue5;

struct Task {
	char name[10];
	int priority;
	int c;
	int t;
	int d;
};

Task task1 = { "t1", 1, 1, 3, 4 };
Task task2 = { "t2", 1, 4, 3, 5 };
Task task3 = { "t3", 1, 2, 3, 6 };
Task task4 = { "t4", 1, 4, 3, 6 };
Task task5 = { "t5", 1, 4, 3, 6 };

Task *t1 = &task1;
Task *t2 = &task2;
Task *t3 = &task3;
Task *t4 = &task4;
Task *t5 = &task5;

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

		pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name, instance);
		wait_without_block(1000);

		if ( xQueueSend(xQueue2, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
			pc.printf("\nMensaje de T1 a T2\n\r");
		} else {
			pc.printf("\nNo se pudo enviar el mensaje de T1 a T2\n\r");
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
			pc.printf("\n T2 Recibio un mensaje, ya puede ejecutar.\n\r");

			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name,
					instance);
			wait_without_block(1000);

			if ( xQueueSend(xQueue5, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
				pc.printf("\nMensaje de T2 a T5\n\r");
			} else {
				pc.printf("\nNo se pudo enviar el mensaje de T2 a T5\n\r");
			}

			if ( xQueueSend(xQueue4, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
				pc.printf("\nMensaje de T2 a T4\n\r");
			} else {
				pc.printf("\nNo se pudo enviar el mensaje de T2 a T4\n\r");
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

		pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name, instance);
		wait_without_block(1000);

		if ( xQueueSend(xQueue5, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
			pc.printf("\nMensaje de T3 a T5\n\r");
		} else {
			pc.printf("\nNo se pudo enviar el mensaje de T3 a T5\n\r");
		}

		vTaskDelayUntil(&startTime, 2000);
		instance++;
	}
}

void thread4(void *params) {
	Task *task = (Task*) params;

	int message = 1;
	int instance = 0;

	TickType_t startTime;
	startTime = xTaskGetTickCount();

	while (1) {
		if (xQueueReceive(xQueue4, &message, portMAX_DELAY)) { // @suppress("Invalid arguments")
			pc.printf("\n T4 Recibio un mensaje, ya puede ejecutar.\n\r");

			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name,
					instance);
			wait_without_block(1000);
		}

		vTaskDelayUntil(&startTime, 2000);
		instance++;
	}
}

void thread5(void *params) {
	Task *task = (Task*) params;

	int message = 1;
	int instance = 0;

	TickType_t startTime;
	startTime = xTaskGetTickCount();

	while (1) {
		if (xQueueReceive(xQueue5, &message, portMAX_DELAY)) { // @suppress("Invalid arguments")
			pc.printf("\n T5 Recibio un mensaje, ya puede ejecutar.\n\r");

			pc.printf("\n> Tarea [%s] - instancia: %d\n\r", task->name,
					instance);
			wait_without_block(1000);
		}

		vTaskDelayUntil(&startTime, 2000);
		instance++;
	}
}

/**
 * Tarea 1 → Tarea 2 → Tarea 5; Tarea 2 → Tarea 4; Tarea 3 → Tarea 5
 * */
int main() {
// Initializes the trace recorder, but does not start the tracing.
	vTraceEnable( TRC_INIT);

	xSemaphore = xSemaphoreCreateMutex(); // @suppress("Function cannot be resolved")
	xQueue1 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")
	xQueue2 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")
	xQueue3 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")
	xQueue4 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")
	xQueue5 = xQueueCreate(5, sizeof(int)); // @suppress("Function cannot be resolved")

	xTaskCreate(thread1, t1->name, 256, (void*) t1,
	configMAX_PRIORITIES - t1->priority, NULL);

	xTaskCreate(thread2, t2->name, 256, (void*) t2,
	configMAX_PRIORITIES - t2->priority, NULL);

	xTaskCreate(thread3, t3->name, 256, (void*) t3,
	configMAX_PRIORITIES - t3->priority, NULL);

	xTaskCreate(thread4, t4->name, 256, (void*) t4,
	configMAX_PRIORITIES - t4->priority, NULL);

	xTaskCreate(thread5, t5->name, 256, (void*) t5,
	configMAX_PRIORITIES - t5->priority, NULL);

	vTraceEnable( TRC_START);

	pc.printf("Iniciando planificador...\n\r");

	vTaskStartScheduler();
	for (;;)
		;
}
