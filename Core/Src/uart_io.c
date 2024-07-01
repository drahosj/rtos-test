#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "main.h"
#include "uart_io.h"
#include "usart.h"
#include "rtos.h"
#include "string.h"

void uartDmaTransmit(UART_HandleTypeDef *huart)
{
    if (huart != &huart2) {
        return;
    }
    
    static struct WriteRequest wr;
    if (wr.buf != NULL) {
        ASSERT(osMemoryPoolFree(wr.pool, wr.buf) == osOK);
        wr.buf = NULL;
    }
    
    osStatus_t res;
    res = osMessageQueueGet(consoleWriteQueueHandle, &wr, NULL, 0);
    if (res == osOK) {
        HAL_UART_Transmit_DMA(huart, wr.buf, wr.len);
    } else if (res == osErrorResource) {
        wr.buf = NULL;
        osEventFlagsSet(consoleHandle, 1);
    }
}

    
int _write(int file, char *ptr, int len)
{
    if ((file == 1) || (file == 2)) {
        ASSERT(osMutexAcquire(atomicWriteMutexHandle, osWaitForever) == osOK);
        do {
            int wrlen = (len > 64 ? 64 : len);
            uint8_t * buf  = osMemoryPoolAlloc(consoleWritePoolHandle, osWaitForever);
            ASSERT(buf != NULL);
            memcpy(buf, ptr, wrlen);
            struct WriteRequest wr;
            wr.pool = consoleWritePoolHandle;
            wr.buf = buf;
            wr.len = wrlen;
            
            ASSERT(osMessageQueuePut(consoleWriteQueueHandle, &wr, 0, osWaitForever) == osOK);
            
            if (osEventFlagsClear(consoleHandle, 1) == 1) {
                uartDmaTransmit(&huart2);
            }
            len -= 64;
            ptr += 64;
        } while (len > 0);
        
        ASSERT(osMutexRelease(atomicWriteMutexHandle) == osOK);
        return len;
    }
    return -1;
}
