#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas

void thread1 (void*);

int main()
{
    xTaskCreate(thread1, "t1", 128, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();
    for( ;; );
}

void thread1 (void* params)
{
    DigitalOut led1(LED1);

    while(1)
    {
        led1 = !led1;
        vTaskDelay(1000);
    }
}
