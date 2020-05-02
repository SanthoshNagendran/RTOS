/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h" /* Include this file for this example. */

/* Supporting functions include file */
#include "supporting_functions/supporting_functions.h"

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Define the strings that will be passed in as the task parameters. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/* The number of bytes of storage in the buffer. */
#define BUFFER_LENGTH_BYTES	( ( size_t ) 100 )
StreamBufferHandle_t xStreamBuffer;

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create one of the two tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 1, NULL);
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 2, NULL);

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	char *pcTaskName;
	const size_t xTriggerLevel = 10;
	size_t xBytesSent;
	char cStringToSend[20];
	int32_t iCounter = 0;

	pcTaskName = (char *)pvParameters;

	/* Create a stream buffer in the heap memory */
	xStreamBuffer = xStreamBufferCreate(BUFFER_LENGTH_BYTES, xTriggerLevel);

	if (xStreamBuffer == NULL)
	{
		/* There was not enough heap memory space available to create the
		stream buffer. */
		configASSERT(pdFAIL);
	}

	for ( ;; )
	{
		vTaskDelay((TickType_t)500);
		vPrintString(pcTaskName);

		memset(cStringToSend, 0x00, sizeof(cStringToSend));
		sprintf(cStringToSend, "%s%d", "Hello, IoT ", (int)iCounter);

		/* Send the string to the stream buffer.  Return immediately if there is not
		enough space in the buffer. */
		xBytesSent = xStreamBufferSend(xStreamBuffer,
			(void *)cStringToSend,
			strlen(cStringToSend), 0);

		if (xBytesSent != strlen(cStringToSend))
		{
			/* The entire string could not be added to the stream buffer because
			there was not enough free space in the buffer, but xBytesSent bytes
			were sent.  Could try again to send the remaining bytes. */
			configASSERT(pdFAIL);
		}

		iCounter++;

	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	char *pcTaskName;
	char *pucBuffer;
	size_t xReceivedBytes;

	pcTaskName = (char *)pvParameters;

	/* Create the buffer from the heap. */
	pucBuffer = (char *)pvPortMalloc(BUFFER_LENGTH_BYTES);
	configASSERT(pucBuffer);

	for ( ;; )
	{
		vTaskDelay((TickType_t)500);
		/* Print out the name of this task. */
		vPrintString(pcTaskName);

		/* Receive up to another BUFFER_LENGTH_BYTES bytes from the stream buffer.
		Wait in the Blocked state (not using any CPU processing time) indefinitely
		till the full number of BUFFER_LENGTH_BYTES bytes to be available. */
		xReceivedBytes = xStreamBufferReceive(xStreamBuffer,
			(void *)pucBuffer,
			BUFFER_LENGTH_BYTES,
			portMAX_DELAY);

		if (xReceivedBytes > 0)
		{
			/* A ucRxData contains another xRecievedBytes bytes of data, which can
			be processed here... */
			vPrintString((char *)"Buffer received: ");
			vPrintString((char *)pucBuffer);
			vPrintStringAndNumber((char *)"\r\nNum. of bytes:", xReceivedBytes);
			vPrintString((char *)"\r\n");
		}

	}
}
