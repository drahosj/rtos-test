#ifndef _rtos_h
#define _rtos_h

#include "cmsis_os.h"

/* Thread IDs */
extern osThreadId_t defaultTaskHandle;

/* Queues */
extern osMessageQueueId_t consoleWriteQueueHandle;

/* Pools */
extern osMemoryPoolId_t consoleWritePoolHandle;

/* Mutexes */
extern osMutexId_t atomicWriteMutexHandle;

/* Flags */
extern osEventFlagsId_t consoleHandle;

#endif
