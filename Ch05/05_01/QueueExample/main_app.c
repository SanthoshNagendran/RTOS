/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" /* Queue include file */

/* Supporting function include files */
#include "supporting_functions/supporting_functions.h"

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Handles for the tasks created in main_app(). */
static TaskHandle_t xTask1 = NULL, xTask2 = NULL;

/* Define the strings that will be passed in as the task parameters. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/* Queue definitions */
typedef struct Message /* Define the data type that will be queued. */
{
	uint32_t ucId;
	char ucData[15];
};

#define QUEUE_LENGTH 1
#define QUEUE_ITEM_SIZE sizeof( struct Message *  )

QueueHandle_t xQueue1;

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create two tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 1, &xTask1);	
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, &xTask2);

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
char *pcTaskString;
uint32_t counter = 0;
struct Message *pxMessage;

	pcTaskString = (char *)pvParameters;

	/* Create the queue, storing the returned handle in the xQueue variable. */
	xQueue1 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

	if (xQueue1 == NULL)
	{
		configASSERT(pdFALSE);
	}

	for ( ;; )
	{
		vTaskDelay((TickType_t)500);
		/* Print out the string of this task. */
		vPrintString(pcTaskString);

		struct Message msg = { counter++, "Hello, IoT!" };

		pxMessage = &msg;

		xQueueSendToBack(xQueue1, (void *)&pxMessage, (TickType_t)0);

	}
}
/*-----------------------------------------------------------*/


void vTask2(void *pvParameters)
{
struct Message *xReceivedFromQueue1;
char *pcTaskString;

	pcTaskString = (char *)pvParameters;

	for ( ;; )
	{
		vTaskDelay((TickType_t)200);
		/* Print out the string of this task. */
		vPrintString(pcTaskString);
	
		/* Receive and print data from Queue1 after a brief waiting for 10 ticks. */
		if (xQueueReceive(xQueue1, &(xReceivedFromQueue1), (TickType_t)10))
		{
			vPrintString((char *) "Received from Queue1: ");

			/* Print received data received. */
			vPrintStringAndNumber((char *)((struct Message *)xReceivedFromQueue1)->ucData,
				((struct Message *)xReceivedFromQueue1)->ucId );
			
		}

	}
}
