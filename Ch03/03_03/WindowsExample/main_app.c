/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Supporting functions include file */
#include "supporting_functions/supporting_functions.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xffffff )

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Define the strings that will be passed in as the task parameters. These are
defined const and off the stack to ensure they remain valid when the tasks are
executing. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create two tasks and pass string parameters to the task functions. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 1, NULL);	
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 1, NULL);

	/* Start the scheduler to start the tasks executing. */
	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	volatile uint32_t ul;

	/* The string to print out is passed via the parameter.  Cast this to a
	character pointer. */
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for ( ;; )
	{
		/* Print out the string parameter of this task. */
		vPrintString(pcTaskString);

		/* Delay for a period of time. */
		for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++)
		{

		}
	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	volatile uint32_t ul;
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for ( ;; )
	{
		/* Print out the string parameter of this task. */
		vPrintString(pcTaskString);

		/* Delay for a period of time. */
		for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++)
		{

		}
	}
}
