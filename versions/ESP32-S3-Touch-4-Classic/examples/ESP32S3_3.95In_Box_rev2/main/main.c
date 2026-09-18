/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 * Modifications Copyright 2026 OSPTEK
 * https://github.com/osptek
*/

#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_io_additions.h"
#include "esp_lcd_touch_ft5x06.h"
#include "esp_lcd_st7701.h"
#include "st7701_init_cmds.h"
#include "lv_demos.h"
#include "lvgl_port.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_H_RES (480)
#define EXAMPLE_LCD_V_RES (480)
#define EXAMPLE_LCD_BIT_PER_PIXEL (18)
#define EXAMPLE_RGB_BIT_PER_PIXEL (16)
#define EXAMPLE_RGB_DATA_WIDTH (16)
#define EXAMPLE_RGB_BOUNCE_BUFFER_SIZE (EXAMPLE_LCD_H_RES * CONFIG_EXAMPLE_LCD_RGB_BOUNCE_BUFFER_HEIGHT)
#define EXAMPLE_LCD_IO_SPI_CS (GPIO_NUM_45)
#define EXAMPLE_LCD_IO_SPI_SCL (GPIO_NUM_38)
#define EXAMPLE_LCD_IO_SPI_SDA (GPIO_NUM_39)

#define EXAMPLE_LCD_IO_RGB_DISP (-1) // -1 if not used
#define EXAMPLE_LCD_IO_RGB_VSYNC (GPIO_NUM_21)
#define EXAMPLE_LCD_IO_RGB_HSYNC (GPIO_NUM_14)
#define EXAMPLE_LCD_IO_RGB_DE (GPIO_NUM_47)
#define EXAMPLE_LCD_IO_RGB_PCLK (GPIO_NUM_48)

#define EXAMPLE_LCD_IO_RGB_DATA0 (GPIO_NUM_0)
#define EXAMPLE_LCD_IO_RGB_DATA1 (GPIO_NUM_12)
#define EXAMPLE_LCD_IO_RGB_DATA2 (GPIO_NUM_11)
#define EXAMPLE_LCD_IO_RGB_DATA3 (GPIO_NUM_10)
#define EXAMPLE_LCD_IO_RGB_DATA4 (GPIO_NUM_9)
#define EXAMPLE_LCD_IO_RGB_DATA5 (GPIO_NUM_46)
#define EXAMPLE_LCD_IO_RGB_DATA6 (GPIO_NUM_3)
#define EXAMPLE_LCD_IO_RGB_DATA7 (GPIO_NUM_20)
#define EXAMPLE_LCD_IO_RGB_DATA8 (GPIO_NUM_19)
#define EXAMPLE_LCD_IO_RGB_DATA9 (GPIO_NUM_8)
#define EXAMPLE_LCD_IO_RGB_DATA10 (GPIO_NUM_18)
#define EXAMPLE_LCD_IO_RGB_DATA11 (GPIO_NUM_17)
#define EXAMPLE_LCD_IO_RGB_DATA12 (GPIO_NUM_16)
#define EXAMPLE_LCD_IO_RGB_DATA13 (GPIO_NUM_15)
#define EXAMPLE_LCD_IO_RGB_DATA14 (GPIO_NUM_7)
#define EXAMPLE_LCD_IO_RGB_DATA15 (GPIO_NUM_6)

#define EXAMPLE_LCD_IO_RST (-1)                // -1 if not used
#define EXAMPLE_PIN_NUM_BK_LIGHT (GPIO_NUM_13) // -1 if not used
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL (1)
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

// #if EXAMPLE_LCD_TOUCH_CONTROLLER_FT5x06

#define EXAMPLE_TOUCH_I2C_SCL (GPIO_NUM_4)
#define EXAMPLE_TOUCH_I2C_SDA (GPIO_NUM_5)
#define EXAMPLE_TOUCH_RST     (GPIO_NUM_NC)
#define EXAMPLE_TOUCH_INT     (GPIO_NUM_NC)

/* Touch settings */
#define EXAMPLE_TOUCH_I2C_NUM       (I2C_NUM_0)
#define EXAMPLE_TOUCH_I2C_CLK_HZ    (400 * 1000)

// #endif


static const char *TAG = "example";

IRAM_ATTR static bool rgb_lcd_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)
{
    return lvgl_port_notify_rgb_vsync();
}

void app_main()
{
    if (EXAMPLE_PIN_NUM_BK_LIGHT >= 0)
    {
        ESP_LOGI(TAG, "Turn off LCD backlight");
        gpio_config_t bk_gpio_config = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT};
        ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    }

    ESP_LOGI(TAG, "Install 3-wire SPI panel IO");
    spi_line_config_t line_config = {
        .cs_io_type = IO_TYPE_GPIO,
        .cs_gpio_num = EXAMPLE_LCD_IO_SPI_CS,
        .scl_io_type = IO_TYPE_GPIO,
        .scl_gpio_num = EXAMPLE_LCD_IO_SPI_SCL,
        .sda_io_type = IO_TYPE_GPIO,
        .sda_gpio_num = EXAMPLE_LCD_IO_SPI_SDA,
        .io_expander = NULL,
    };
    esp_lcd_panel_io_3wire_spi_config_t io_config = ST7701_PANEL_IO_3WIRE_SPI_CONFIG(line_config, 0);
    esp_lcd_panel_io_handle_t io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_3wire_spi(&io_config, &io_handle));

    ESP_LOGI(TAG, "Install ST7701 panel driver");
    esp_lcd_panel_handle_t lcd_handle = NULL;
    esp_lcd_rgb_panel_config_t rgb_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .psram_trans_align = 64,
        .data_width = EXAMPLE_RGB_DATA_WIDTH,
        .bits_per_pixel = EXAMPLE_RGB_BIT_PER_PIXEL,
        .de_gpio_num = EXAMPLE_LCD_IO_RGB_DE,
        .pclk_gpio_num = EXAMPLE_LCD_IO_RGB_PCLK,
        .vsync_gpio_num = EXAMPLE_LCD_IO_RGB_VSYNC,
        .hsync_gpio_num = EXAMPLE_LCD_IO_RGB_HSYNC,
        .disp_gpio_num = EXAMPLE_LCD_IO_RGB_DISP,
        .data_gpio_nums = {
            EXAMPLE_LCD_IO_RGB_DATA0,
            EXAMPLE_LCD_IO_RGB_DATA1,
            EXAMPLE_LCD_IO_RGB_DATA2,
            EXAMPLE_LCD_IO_RGB_DATA3,
            EXAMPLE_LCD_IO_RGB_DATA4,
            EXAMPLE_LCD_IO_RGB_DATA5,
            EXAMPLE_LCD_IO_RGB_DATA6,
            EXAMPLE_LCD_IO_RGB_DATA7,
            EXAMPLE_LCD_IO_RGB_DATA8,
            EXAMPLE_LCD_IO_RGB_DATA9,
            EXAMPLE_LCD_IO_RGB_DATA10,
            EXAMPLE_LCD_IO_RGB_DATA11,
            EXAMPLE_LCD_IO_RGB_DATA12,
            EXAMPLE_LCD_IO_RGB_DATA13,
            EXAMPLE_LCD_IO_RGB_DATA14,
            EXAMPLE_LCD_IO_RGB_DATA15,
        },
        .timings ={                                 \
        .pclk_hz = 21 * 1000 * 1000,                \
        .h_res = 480,                               \
        .v_res = 480,                               \
        .hsync_pulse_width = 10,                    \
        .hsync_back_porch = 10,                     \
        .hsync_front_porch = 20,                    \
        .vsync_pulse_width = 10,                    \
        .vsync_back_porch = 10,                     \
        .vsync_front_porch = 10,                    \
        .flags.pclk_active_neg = false,             \
        },
        .flags.fb_in_psram = true,
        .num_fbs = LVGL_PORT_LCD_RGB_BUFFER_NUMS,
        .bounce_buffer_size_px = EXAMPLE_RGB_BOUNCE_BUFFER_SIZE,
    };
    rgb_config.timings.h_res = EXAMPLE_LCD_H_RES;
    rgb_config.timings.v_res = EXAMPLE_LCD_V_RES;
    st7701_vendor_config_t vendor_config = {
        .rgb_config = &rgb_config,
        .init_cmds = lcd_init_cmds, 
        .init_cmds_size = lcd_init_cmds_size,
        .flags = {
            .auto_del_panel_io = 0, /**
                                     * Set to 1 if panel IO is no longer needed after LCD initialization.
                                     * If the panel IO pins are sharing other pins of the RGB interface to save GPIOs,
                            #include "driver/i2c_master.h"         * Please set it to 1 to release the pins.
                                     */
            .mirror_by_cmd = 1,     // Set to 0 if `auto_del_panel_io` is enabled
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_IO_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = EXAMPLE_LCD_BIT_PER_PIXEL,
        .vendor_config = &vendor_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7701(io_handle, &panel_config, &lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_handle));
    esp_lcd_panel_disp_on_off(lcd_handle, true);

    esp_lcd_touch_handle_t tp_handle = NULL;
// #if EXAMPLE_LCD_TOUCH_CONTROLLER_FT5x06
    i2c_master_bus_handle_t i2c_handle = NULL;
    const i2c_master_bus_config_t i2c_config = {
        .i2c_port = EXAMPLE_TOUCH_I2C_NUM,
        .sda_io_num = EXAMPLE_TOUCH_I2C_SDA,
        .scl_io_num = EXAMPLE_TOUCH_I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        // .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &i2c_handle));

    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_FT5x06_CONFIG();
    tp_io_config.scl_speed_hz = EXAMPLE_TOUCH_I2C_CLK_HZ;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_handle, &tp_io_config, &tp_io_handle));

    ESP_LOGI(TAG, "Initialize touch controller FT5x06");
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = EXAMPLE_TOUCH_RST,
        .int_gpio_num = EXAMPLE_TOUCH_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_ft5x06(tp_io_handle, &tp_cfg, &tp_handle));
// #endif

    ESP_ERROR_CHECK(lvgl_port_init(lcd_handle, tp_handle));

    esp_lcd_rgb_panel_event_callbacks_t cbs = {
#if EXAMPLE_RGB_BOUNCE_BUFFER_SIZE > 0
        .on_bounce_frame_finish = rgb_lcd_on_vsync_event,
#else
        .on_vsync = rgb_lcd_on_vsync_event,
#endif
    };
    esp_lcd_rgb_panel_register_event_callbacks(lcd_handle, &cbs, NULL);

    if (EXAMPLE_PIN_NUM_BK_LIGHT >= 0)
    {
        ESP_LOGI(TAG, "Turn on LCD backlight");
        gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
    }

    ESP_LOGI(TAG, "Display LVGL demos");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1))
    {
        // lv_demo_music();
        lv_demo_widgets();

        lv_obj_t *scr = lv_scr_act();
        ESP_LOGI(TAG, "scr: %p", scr);

        // Release the mutex
        lvgl_port_unlock();
    }
}
