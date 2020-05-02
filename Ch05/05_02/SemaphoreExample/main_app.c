/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" /* Semaphore include file */

#include "supporting_functions/supporting_functions.h"

/* This is the interrupt that should execute whenever taskYIELD() / portYIELD() is called */
#define portYIELD()	vPortGenerateSimulatedInterrupt( portINTERRUPT_YIELD ) 
#define portINTERRUPT_YIELD 2

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Define the strings that will be passed in as the task parameters.  These are
defined. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/* Define the string as the shared data accessed by Task 1 and Task 2. */
static char *sharedData = "It's the shared data.";

/* Create the semaphore handle. */
SemaphoreHandle_t xSemaphore1 = NULL;

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 2, NULL); /* The 2nd last parmeter means the task priority. */
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, NULL); /* The 2nd last parmeter means the task priority. */

	/* Start the scheduler to start the tasks executing. */
	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xDelay300ms = pdMS_TO_TICKS(300UL);
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	/* The xLastWakeTime variable needs to be initialized with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	/* Create a sempahore for mutex. */
	xSemaphore1 = xSemaphoreCreateMutex();

	/* Check if the semaphore is created successfully. */
	if (xSemaphore1 == NULL)
	{
		configASSERT(pdFALSE);
	}

	for ( ;; )
	{
		/* Print out the name of this task. */
		vPrintString(pcTaskString);

		//taskENTER_CRITICAL();
		if (xSemaphore1 != NULL)
		{
			/* Obtain the semaphore without blocking the task if the semaphore isn't
			   immediately available. */
			if(xSemaphoreTake(xSemaphore1, (TickType_t)0))
			{
				/* Now the task can access the shared resource. Do something here. */
				sharedData = (char *) "Shared data changed by Task1";
				if (strcmp(sharedData, "Shared data changed by Task1") != 0)
				{
					configASSERT(pdFALSE);
				}

				{
					printf("%s\r\n", sharedData);
					fflush(stdout);
				}

				/* Delay for a period of time. */
				vTaskDelayUntil(&xLastWakeTime, xDelay300ms);

				/* When finished accessing the shared resource, free the
				   semaphore. */
				if(xSemaphoreGive(xSemaphore1) != pdTRUE)
				{
					configASSERT(pdFALSE);
				}
			}
		}


	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	xLastWakeTime = xTaskGetTickCount();

	for ( ;; )
	{
		/* Print out the name of this task. */
		//vPrintString(pcTaskString);

		//		taskENTER_CRITICAL();

		sharedData = (char *) "Shared data changed by Task2";
		{
			printf("%s\r\n", sharedData);
			fflush(stdout);
		}

		//		taskEXIT_CRITICAL();

		/* Delay for a period of time. */
		vTaskDelayUntil(&xLastWakeTime, xDelay500ms);

	}
}
