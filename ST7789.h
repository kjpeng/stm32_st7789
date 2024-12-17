
#ifndef _ST7789_H_
#define _ST7789_H_

#include <stdint.h>
#include "stm32g4xx_hal.h"

#define ST7789_WIDTH 240
#define ST7789_HEIGHT 135
#define X_SHIFT 40
#define Y_SHIFT 53

#define COLOR_WHITE    0xFFFF
#define COLOR_BLACK    0x0000
#define COLOR_RED      0xF800
#define COLOR_GREEN    0x07D0
#define COLOR_BLUE     0x001F

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_PORCTRL 0xB2

/**
 * Memory Data Access Control Register (0x36H)
 * MAP:     D7  D6  D5  D4  D3  D2  D1  D0
 * param:   MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00

#define ST7789_COLOR_MODE_16bit 0x55    //  RGB565 (16bit)

#define ABS(x) ((x) > 0 ? (x) : -(x))

extern SPI_HandleTypeDef hspi1;

extern GPIO_TypeDef *cs_port;
extern uint16_t cs_pin;

extern GPIO_TypeDef *dc_port;
extern uint16_t dc_pin;

// this implementation does not use the rst pin

void ST7789_DC_Set(int value);
void ST7789_CS_Set(int value);

void ST7789_WriteCommand(uint8_t cmd);
void ST7789_WriteData(uint8_t *buf, uint16_t buf_size);
void ST7789_WriteSmallData(uint8_t data);

void ST7789_Init(void);
void ST7789_SetRotation(void);
void ST7789_Fill_Color(uint16_t color);
void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void ST7789_Square(uint16_t x, uint16_t y, uint16_t dim, uint16_t color);

void ST7789_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

uint16_t ST7789_ColorSelect(uint8_t r, uint8_t g, uint8_t b);

#endif
