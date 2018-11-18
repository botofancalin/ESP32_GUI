/*
This is the subclass graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.)

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Based on ESP8266 library https://github.com/Sermus/ESP8266_Adafruit_lcd

Copyright (c) 2015-2016 Andrey Filimonov.  All rights reserved.

Additions for ESP32 Copyright (c) 2016-2017 Espressif Systems (Shanghai) PTE LTD

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

#include "iot_lcd.h"
#include "spi_lcd.h"

#include "esp_partition.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "freertos/semphr.h"
#include "freertos/task.h"

/*Rotation Defines*/
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04


#define SWAPBYTES(i) ((i>>8) | (i<<8))

CEspLcd::CEspLcd(lcd_conf_t* lcd_conf, int height, int width, bool dma_en, int dma_word_size, int dma_chan)
{
    m_height = height;
    m_width  = width;
    tabcolor = 0;
    dma_mode = dma_en;
    dma_buf_size = dma_word_size;
    spi_mux = xSemaphoreCreateRecursiveMutex();
    m_dma_chan = dma_chan;
    setSpiBus(lcd_conf);
}

CEspLcd::~CEspLcd()
{
    spi_bus_remove_device(spi_wr);
    vSemaphoreDelete(spi_mux);
}

void CEspLcd::acquireBus()
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
}

void CEspLcd::releaseBus()
{
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::setSpiBus(lcd_conf_t *lcd_conf)
{
    cmd_io = (gpio_num_t) lcd_conf->pin_num_dc;
    dc.dc_io = cmd_io;
    id.id = lcd_init(lcd_conf, &spi_wr, &dc, m_dma_chan);
    id.mfg_id = (id.id >> (8 * 1)) & 0xff ;
    id.lcd_driver_id = (id.id >> (8 * 2)) & 0xff;
    id.lcd_id = (id.id >> (8 * 3)) & 0xff;
}

void CEspLcd::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    transmitCmdData(LCD_CASET, MAKEWORD(x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF));
    transmitCmdData(LCD_PASET, MAKEWORD(y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF));
    transmitCmd(LCD_RAMWR); // write to RAM
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::transmitData(uint16_t data)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_data(spi_wr, (uint8_t *)&data, 2, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::transmitData(uint8_t data)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_data(spi_wr, (uint8_t *)&data, 1, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::transmitCmdData(uint8_t cmd, uint32_t data)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_cmd(spi_wr, cmd, &dc);
    lcd_data(spi_wr, (uint8_t *)&data, 4, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}
void CEspLcd::transmitData(uint16_t data, int32_t repeats)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_send_uint16_r(spi_wr, data, repeats, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}
void CEspLcd::transmitData(uint8_t* data, int length)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_data(spi_wr, (uint8_t *)data, length, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}
void CEspLcd::transmitCmd(uint8_t cmd)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_cmd(spi_wr, cmd, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::transmitCmdData(uint8_t cmd, const uint8_t data, uint8_t numDataByte)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    lcd_cmd(spi_wr, (const uint8_t) cmd, &dc);
    lcd_data(spi_wr, &data, 1, &dc);
    xSemaphoreGiveRecursive(spi_mux);
}

uint32_t CEspLcd::getLcdId()
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    uint32_t id = lcd_get_id(spi_wr, &dc);
    xSemaphoreGiveRecursive(spi_mux);
    return id;
}

void CEspLcd::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if ((x < 0) || (x >= m_width) || (y < 0) || (y >= m_height)) {
        return;
    }
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    setAddrWindow(x, y, x + 1, y + 1);
    transmitData((uint16_t) SWAPBYTES(color));
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::_fastSendBuf(const uint16_t* buf, int point_num, bool swap)
{
    if ((point_num * sizeof(uint16_t)) <= (16 * sizeof(uint32_t))) {
        transmitData((uint8_t*) buf, sizeof(uint16_t) * point_num);
    } else {
        int gap_point = dma_buf_size;
        uint16_t* data_buf = (uint16_t*) malloc(gap_point * sizeof(uint16_t));
        int offset = 0;
        while (point_num > 0) {
            int trans_points = point_num > gap_point ? gap_point : point_num;

            if (swap) {
                for (int i = 0; i < trans_points; i++) {
                    data_buf[i] = SWAPBYTES(buf[i + offset]);
                }
            } else {
                memcpy((uint8_t*) data_buf, (uint8_t*) (buf + offset), trans_points * sizeof(uint16_t));
            }
            transmitData((uint8_t*) (data_buf), trans_points * sizeof(uint16_t));
            offset += trans_points;
            point_num -= trans_points;
        }
        free(data_buf);
        data_buf = NULL;
    }
}

void CEspLcd::_fastSendRep(uint16_t val, int rep_num)
{
    int point_num = rep_num;
    int gap_point = dma_buf_size;
    gap_point = (gap_point > point_num ? point_num : gap_point);

    uint16_t* data_buf = (uint16_t*) malloc(gap_point * sizeof(uint16_t));
    int offset = 0;
    while (point_num > 0) {
        for (int i = 0; i < gap_point; i++) {
            data_buf[i] = val;
        }
        int trans_points = point_num > gap_point ? gap_point : point_num;
        transmitData((uint8_t*) (data_buf), sizeof(uint16_t) * trans_points);
        offset += trans_points;
        point_num -= trans_points;
    }
    free(data_buf);
    data_buf = NULL;
}

void CEspLcd::drawBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h)
{
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    if (dma_mode) {
        _fastSendBuf(bitmap, w * h);
    } else {
        for (int i = 0; i < w * h; i++) {
            transmitData(SWAPBYTES(bitmap[i]), 1);
        }
    }
    xSemaphoreGiveRecursive(spi_mux);
}

void CEspLcd::fillScreen(uint16_t color)
{
    fillRect(0, 0, m_width, m_height, color);
}

void CEspLcd::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= m_width) || (y >= m_height)) {
        return;
    }
    if ((x + w - 1) >= m_width) {
        w = m_width - x;
    }
    if ((y + h - 1) >= m_height) {
        h = m_height - y;
    }
    xSemaphoreTakeRecursive(spi_mux, portMAX_DELAY);
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    if (dma_mode) {
        _fastSendRep(SWAPBYTES(color), h * w);
    } else {
        transmitData(SWAPBYTES(color), h * w);
    }
    xSemaphoreGiveRecursive(spi_mux);
}

uint16_t CEspLcd::color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

