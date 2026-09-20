/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 * Modifications Copyright 2026 OSPTEK
 * https://github.com/osptek
*/

#include "app_init.h"
#include "demo_ui.h"
#include "esp_log.h"
#include "lvgl_port.h"
#include "modbus_port.h"

void app_main(void)
{
    app_init();

    esp_err_t modbus_err = modbus_port_start();
    if (modbus_err != ESP_OK)
    {
        ESP_LOGE("main", "RS485 Modbus start failed: %s", esp_err_to_name(modbus_err));
    }

    if (lvgl_port_lock(-1))
    {
        demo_ui_create();
        lvgl_port_unlock();
    }
}