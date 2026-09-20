#include "modbus_port.h"

#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mbcontroller.h"

#define MB_PARAM_INFO_TIMEOUT_MS 10
#define MB_ACCESS_MASK (MB_EVENT_INPUT_REG_RD | MB_EVENT_HOLDING_REG_RD | MB_EVENT_HOLDING_REG_WR)

static const char *TAG = "modbus_port";

static void *s_slave_handle;
static uint16_t s_holding_regs[MODBUS_HOLDING_REG_COUNT];
static volatile uint32_t s_event_count;
static bool s_running;

static void modbus_event_task(void *arg)
{
    (void)arg;
    mb_param_info_t reg_info = {0};

    for (;;) {
        (void)mbc_slave_check_event(s_slave_handle, MB_ACCESS_MASK);
        if (mbc_slave_get_param_info(s_slave_handle, &reg_info, MB_PARAM_INFO_TIMEOUT_MS) != ESP_OK) {
            continue;
        }

        ++s_event_count;
        ESP_LOGI(TAG, "RS485 event=%lu type=0x%lx addr=%u size=%u",
                 (unsigned long)s_event_count,
                 (unsigned long)reg_info.type,
                 (unsigned)reg_info.mb_offset,
                 (unsigned)reg_info.size);
    }
}

bool modbus_port_is_running(void)
{
    return s_running;
}

size_t modbus_port_get_holding_count(void)
{
    return MODBUS_HOLDING_REG_COUNT;
}

uint16_t modbus_port_get_holding(size_t index)
{
    if (index >= MODBUS_HOLDING_REG_COUNT || !s_slave_handle) {
        return 0;
    }

    uint16_t value = 0;
    if (mbc_slave_lock(s_slave_handle) == ESP_OK) {
        value = s_holding_regs[index];
        (void)mbc_slave_unlock(s_slave_handle);
    }
    return value;
}

esp_err_t modbus_port_set_holding(size_t index, uint16_t value)
{
    if (index >= MODBUS_HOLDING_REG_COUNT || !s_slave_handle) {
        return ESP_ERR_INVALID_STATE;
    }

    ESP_RETURN_ON_ERROR(mbc_slave_lock(s_slave_handle), TAG, "lock holding registers");
    s_holding_regs[index] = value;
    return mbc_slave_unlock(s_slave_handle);
}

uint32_t modbus_port_get_event_count(void)
{
    return s_event_count;
}

esp_err_t modbus_port_start(void)
{
    if (s_running) {
        return ESP_OK;
    }

    mb_communication_info_t comm = {
        .ser_opts.port = MODBUS_UART_PORT,
        .ser_opts.mode = MB_RTU,
        .ser_opts.baudrate = MODBUS_UART_BAUD_RATE,
        .ser_opts.parity = MB_PARITY_NONE,
        .ser_opts.uid = MODBUS_SLAVE_ADDR,
        .ser_opts.data_bits = UART_DATA_8_BITS,
        .ser_opts.stop_bits = UART_STOP_BITS_1,
    };

    ESP_RETURN_ON_ERROR(mbc_slave_create_serial(&comm, &s_slave_handle),
                        TAG, "create serial slave");

    for (size_t i = 0; i < MODBUS_HOLDING_REG_COUNT; ++i) {
        s_holding_regs[i] = (uint16_t)(i + 1);
    }

    const mb_register_area_descriptor_t area = {
        .type = MB_PARAM_HOLDING,
        .start_offset = 0,
        .address = s_holding_regs,
        .size = sizeof(s_holding_regs),
        .access = MB_ACCESS_RW,
    };
    ESP_RETURN_ON_ERROR(mbc_slave_set_descriptor(s_slave_handle, area),
                        TAG, "set holding registers");

    ESP_RETURN_ON_ERROR(uart_set_pin(MODBUS_UART_PORT,
                                     MODBUS_UART_TX_GPIO,
                                     MODBUS_UART_RX_GPIO,
                                     UART_PIN_NO_CHANGE,
                                     UART_PIN_NO_CHANGE),
                        TAG, "set UART pins");
    ESP_RETURN_ON_ERROR(uart_set_mode(MODBUS_UART_PORT, UART_MODE_RS485_HALF_DUPLEX),
                        TAG, "set RS485 mode");
    ESP_RETURN_ON_ERROR(mbc_slave_start(s_slave_handle), TAG, "start slave");

    BaseType_t task_created = xTaskCreate(modbus_event_task,
                                          "modbus_evt",
                                          MODBUS_EVENT_TASK_STACK,
                                          NULL,
                                          MODBUS_EVENT_TASK_PRIORITY,
                                          NULL);
    ESP_RETURN_ON_FALSE(task_created == pdPASS, ESP_ERR_NO_MEM, TAG,
                        "create event task");

    s_running = true;
    ESP_LOGI(TAG, "Modbus RTU slave ready: ID=%d, %d 8N1, TX=GPIO%d, RX=GPIO%d",
             MODBUS_SLAVE_ADDR, MODBUS_UART_BAUD_RATE,
             MODBUS_UART_TX_GPIO, MODBUS_UART_RX_GPIO);
    return ESP_OK;
}
