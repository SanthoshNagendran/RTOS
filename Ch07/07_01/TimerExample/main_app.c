/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h" //Include this file to use timer API functions

//* Suuporting functions include file */
#include "supporting_functions/supporting_functions.h"

/* Defined one-short timer period in ticks */
#define ONE_SHOT_TIMER_PERIOD	( ( TickType_t ) 800 ) //800 ticks

/* The task functions. */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

/* Define the strings that will be passed to the task parameters. */
const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\r\n";

/* The one-shot timer is configured to use a callback function that increments
ucTimerCounter each time it's called. */
static TimerHandle_t xOneShotTimer = NULL;  //Timer handle
static void timerCallback(TimerHandle_t pxExpiredTimer); //Timer callback function
static uint8_t ucTimerCounter = (uint8_t)0; //Timer counter

/*-----------------------------------------------------------*/

int main_app(void)
{
	/* Create two tasks. */
	xTaskCreate(vTask1, "Task 1", 100, (void*)pcTextForTask1, 2, NULL);	
	xTaskCreate(vTask2, "Task 2", 100, (void*)pcTextForTask2, 1, NULL);

	/* Start the scheduler to start the tasks executing. */
	vTaskStartScheduler();

}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters)
{
	volatile uint32_t ul;
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	for ( ;; )
	{
		/* Print out the string parameter of this task. */
		vPrintString(pcTaskString);

		vTaskDelay((TickType_t)500);
	}
}
/*-----------------------------------------------------------*/

void vTask2(void *pvParameters)
{
	char *pcTaskString;
	pcTaskString = (char *)pvParameters;

	/* Create a one-shot timer. */
	xOneShotTimer = xTimerCreate("Oneshot Timer",/* Text name to facilitate debugging. */
		ONE_SHOT_TIMER_PERIOD,	/* The period for the timer. */
		pdFALSE,				/* Don't auto-reload - hence a one shot timer. */
		(void *)0,				/* The timer identifier.  Initialise to 0, then increment each time it is called. */
		timerCallback);		    /* The callback to be called when the timer expires. */

	if (xOneShotTimer == NULL)
	{
		configASSERT(pdFAIL);
	}

	/* Start the one shot timer and check that it reports its state correctly. */
	xTimerStart(xOneShotTimer, (TickType_t)0);
	if (xTimerIsTimerActive(xOneShotTimer) == pdFALSE)
	{
		configASSERT(pdFAIL);
	}

	for ( ;; )
	{
		/* Print out the string parameter of the task and timer counter. */
		vPrintStringAndNumber(pcTaskString, ucTimerCounter);

		/* Delay the task for 100 ticks*/
		vTaskDelay((TickType_t)500);
	}
}
/*-----------------------------------------------------------*/

static void timerCallback(TimerHandle_t pxExpiredTimer)
{
	vPrintString((char*)"Timer callback called!\r\n");
	ucTimerCounter++;
}
