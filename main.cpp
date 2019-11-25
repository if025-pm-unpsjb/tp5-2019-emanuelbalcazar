#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas

void thread1(void*);

int main() {
	// Initializes the trace recorder, but does not start the tracing.
	vTraceEnable( TRC_INIT);

	xTaskCreate(thread1, "t1", 128, NULL, configMAX_PRIORITIES - 1, NULL);

	vTraceEnable( TRC_START);

	vTaskStartScheduler();
	for (;;)
		;
}

void thread1(void *params) {
	DigitalOut led1(LED1);

	while (1) {
		led1 = !led1;
		vTaskDelay(1000);
	}
}
