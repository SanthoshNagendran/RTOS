/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Suuporting functions include file */
#include "supporting_functions/supporting_functions.h"

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Define the strings that will be passed in as the task parameters. 
   They are to be printed to the console when tasks are running. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 3, NULL); /* The 2nd last parmeter means the task priority. */
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, NULL); /* The 2nd last parmeter means the task priority. */

	/* Start the scheduler to start the tasks executing. */
	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for ( ;; )
	{
		/* Print out the string parameter of this task. */
		vPrintString(pcTaskString);

		/* Delay the task a bit to better see the console output. */
		vTaskDelay( pdMS_TO_TICKS(200UL) );
	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	for ( ;; )
	{
		/* Print out the string parameter of this task. */
		vPrintString(pcTaskString);

		/* Delay for a period of time. Time is measured in ticks. 
		The pdMS_TO_TICKS macro is used to convert milliseconds to ticks, 
		where 100UL means 100 milliseconds. */
		//vTaskDelay( pdMS_TO_TICKS( 100UL ) );
	}
}

