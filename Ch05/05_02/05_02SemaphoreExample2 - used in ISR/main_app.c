
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo include files */
#include "supporting_functions/supporting_functions.h"

/* Must be greater than any priority used by the kernel and less than 32 */
#define portINTERRUPT_YIELD 5

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

BaseType_t xHigherPriorityTaskWoken = pdFALSE;

static unsigned long prvProcessYieldInterrupt(void)
{

	/* xHigherPriorityTaskWoken must be initialised to pdFALSE.  If calling
	vTaskNotifyGiveFromISR() unblocks the handling task, and the priority of
	the handling task is higher than the priority of the currently running task,
	then xHigherPriorityTaskWoken will automatically get set to pdTRUE. */
	xHigherPriorityTaskWoken = pdFALSE;

	/* Send a notification to vTask2(), bringing it out of the Blocked
	state. */
	vTaskNotifyGiveFromISR(xTask2, &xHigherPriorityTaskWoken);

	vPrintString((char *)"Notification was sent to Task2.\r\n");

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}
/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create one of the two tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 1, &xTask1);	

	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, &xTask2);

	vPortSetInterruptHandler(portINTERRUPT_YIELD, prvProcessYieldInterrupt);

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

		/* Simulate the periodic interrupt hanlded by 
		prvProcessYieldInterrupt. */
		portYIELD();

	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{

	char *pcTaskName;
	pcTaskName = (char *)pvParameters;

	for (;; )
	{
		//vTaskDelay((TickType_t)500);
		/* Print out the name of this task. */
		//vPrintString(pcTaskName);
	
		/* Block to wait for vTask1() to notify this task. */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		vPrintString((char *)"Task2 was notified!\r\n");

	}
}
