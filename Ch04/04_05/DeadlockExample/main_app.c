/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Supporting functions include file */
#include "supporting_functions/supporting_functions.h"

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Handles for the tasks create by main_app(). */
static TaskHandle_t xTask1 = NULL, xTask2 = NULL;

/* Define the strings that will be passed in as the task parameters. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create one of the two tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 1, &xTask1);
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, &xTask2);

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	volatile uint32_t ul;
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	for (;; )
	{
		vTaskDelay((TickType_t)500);
		vPrintString(pcTaskString);

		/* Send a notification to Task2, bringing it out of the Blocked
		state. */
		xTaskNotifyGive(xTask2);

		/* Block to wait for Task2 to notify this task. */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	for ( ;; )
	{
		vTaskDelay((TickType_t)500);
		vPrintString(pcTaskString);

		/* Block to wait for Task1 to notify this task. */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		/* Send a notification to Task1, bringing it out of the Blocked
		state. */
		xTaskNotifyGive(xTask1);

	}
}
