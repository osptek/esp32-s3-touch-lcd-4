#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Modbus RTU slave: UART1, external RS485 transceiver with hardware direction. */
#define MODBUS_SLAVE_ADDR          1
#define MODBUS_UART_BAUD_RATE      115200
#define MODBUS_UART_PORT           UART_NUM_1
#define MODBUS_UART_TX_GPIO        GPIO_NUM_13
#define MODBUS_UART_RX_GPIO        GPIO_NUM_45
#define MODBUS_HOLDING_REG_COUNT   4
#define MODBUS_EVENT_TASK_PRIORITY 3
#define MODBUS_EVENT_TASK_STACK    4096

esp_err_t modbus_port_start(void);

bool modbus_port_is_running(void);
size_t modbus_port_get_holding_count(void);
uint16_t modbus_port_get_holding(size_t index);
esp_err_t modbus_port_set_holding(size_t index, uint16_t value);
uint32_t modbus_port_get_event_count(void);

#ifdef __cplusplus
}
#endif
