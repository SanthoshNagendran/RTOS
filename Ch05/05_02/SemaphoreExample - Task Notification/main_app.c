
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo include files */
#include "supporting_functions/supporting_functions.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xffffff )

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Handles for the tasks create by main_app(). */
static TaskHandle_t xTask1 = NULL, xTask2 = NULL;

/* Define the strings that will be passed in as the task parameters.  These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create one of the two tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 1, &xTask1);	

	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, &xTask2);

	vTaskStartScheduler();

	/* The following line should never be reached. */
	for (;; );
	return 0;
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	volatile uint32_t ul;
	char *pcTaskName;
	pcTaskName = (char *)pvParameters;

	for (;; )
	{
		vTaskDelay((TickType_t)500);
		/* Print out the name of this task. */
		vPrintString(pcTaskName);

		/* Send a notification to vTask2(), bringing it out of the Blocked
		state. */
		xTaskNotifyGive(xTask2);

		/* Block to wait for vTask2() to notify this task. */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	volatile uint32_t ul;

	char *pcTaskName;
	pcTaskName = (char *)pvParameters;

	for (;; )
	{
		vTaskDelay((TickType_t)500);
		/* Print out the name of this task. */
		vPrintString(pcTaskName);
	
		/* Block to wait for vTask1() to notify this task. */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		/* Send a notification to vTask1(), bringing it out of the Blocked
		state. */
		xTaskNotifyGive(xTask1);

	}
}
