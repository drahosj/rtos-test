#ifndef _uart_io_h
#define _uart_io_h
#include "main.h"
#include "rtos.h"

struct WriteRequest {
    size_t len;
    osMemoryPoolId_t pool;
    uint8_t * buf;
};

void uartDmaTransmit(UART_HandleTypeDef *huart);

#endif
