
#include "ST7789.h"

// extern SPI_HandleTypeDef hspi1;
//
// extern *GPIO_TypeDef cs_port;
// extern uint16_t cs_pin;
//
// extern *GPIO_TypeDef dc_port;
// extern uint16_t dc_pin;

void ST7789_DC_Set(int value) {
  if (!value)
    HAL_GPIO_WritePin(dc_port, dc_pin, GPIO_PIN_RESET);
  else
    HAL_GPIO_WritePin(dc_port, dc_pin, GPIO_PIN_SET);
}

void ST7789_CS_Set(int value) {
  if (!value)
    HAL_GPIO_WritePin(cs_port, dc_pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(cs_port, dc_pin, GPIO_PIN_RESET);
}

void ST7789_WriteCommand(uint8_t cmd) {
  ST7789_CS_Set(1);
  ST7789_DC_Set(0);
  HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
  ST7789_CS_Set(0);
}

void ST7789_WriteData(uint8_t *buf, uint16_t buf_size) {
  ST7789_CS_Set(1);
  ST7789_DC_Set(1);

  while (buf_size > 0) {
    uint16_t chunk_size = buf_size > 65535 ? 65535 : buf_size;
    HAL_SPI_Transmit(&hspi1, buf, chunk_size, HAL_MAX_DELAY);
    buf += chunk_size;
    buf_size -= chunk_size;
  }

  ST7789_CS_Set(0);
}

void ST7789_WriteSmallData(uint8_t data) {
  ST7789_CS_Set(1);
  ST7789_DC_Set(1);
  HAL_SPI_Transmit(&hspi1, &data, sizeof(data), HAL_MAX_DELAY);
  ST7789_CS_Set(0);
}

void ST7789_Init(void) {
  ST7789_WriteCommand(ST7789_COLMOD);
  ST7789_WriteSmallData(ST7789_COLOR_MODE_16bit);

  ST7789_WriteCommand(ST7789_PORCTRL);
  {
    uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
    ST7789_WriteData(data, sizeof(data));
  }
  ST7789_SetRotation(ST7789_ROTATION);

  ST7789_WriteCommand(0XB7);         // Gate Control
  ST7789_WriteSmallData(0x35);      // Default value
  ST7789_WriteCommand(0xBB);         // VCOM setting
  ST7789_WriteSmallData(0x19);      // 0.725v (default 0.75v for 0x20)
  ST7789_WriteCommand(0xC0);         // LCMCTRL
  ST7789_WriteSmallData (0x2C);     // Default value
  ST7789_WriteCommand (0xC2);        // VDV and VRH command Enable
  ST7789_WriteSmallData (0x01);     // Default value
  ST7789_WriteCommand (0xC3);        // VRH set
  ST7789_WriteSmallData (0x12);     // +-4.45v (defalut +-4.1v for 0x0B)
  ST7789_WriteCommand (0xC4);        // VDV set
  ST7789_WriteSmallData (0x20);     // Default value
  ST7789_WriteCommand (0xC6);        // Frame rate control in normal mode
  ST7789_WriteSmallData (0x0F);     // Default value (60HZ)
  ST7789_WriteCommand (0xD0);        // Power control
  ST7789_WriteSmallData (0xA4);     // Default value
  ST7789_WriteSmallData (0xA1);     // Default value

  ST7789_WriteCommand(0xE0);
  {
    uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
    ST7789_WriteData(data, sizeof(data));
  }

  ST7789_WriteCommand(0xE1);
  {
    uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
    ST7789_WriteData(data, sizeof(data));
  }

  ST7789_WriteCommand(ST7789_INVON);
  ST7789_WriteCommand(ST7789_SLPOUT);
  ST7789_WriteCommand(ST7789_NORON);
  ST7789_WriteCommand(ST7789_DISPON);

  HAL_Delay(50);
  ST7789_Fill_Color(COLOR_BLACK);
}

void ST7789_SetRotation(uint8_t m) {
  ST7789_WriteCommand(ST7789_MADCTL);
  ST7789_WriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
}

void ST7789_Fill_Color(uint16_t color) {
  uint16_t i;
  ST7789_SetAddrWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
  ST7789_CS_Set(1);

  uint16_t j;
  for (i = 0; i < ST7789_WIDTH; i++) {
    for (j = 0; j < ST7789_HEIGHT; j++) {
      uint8_t data[] = {color >> 8, color & 0xFF};
      ST7789_WriteData(data, sizeof(data));
    }
  }

  ST7789_CS_Set(0);
}

void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if ((x < 0) || (x >= ST7789_WIDTH) || (y < 0) || (y >= ST7789_HEIGHT))
    return;

  ST7789_SetAddrWindow(x, y, x, y);
  uint8_t data[] = {color >> 8, color & 0xFF};
  ST7789_CS_Set(1);
  ST7789_WriteData(data, sizeof(data));
  ST7789_CS_Set(0);
}

void ST7789_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
  if ((x0 < 0) || (x1 < 0) || (x0 >= ST7789_WIDTH) || (x1 >= ST7789_WIDTH)
      || (y0 < 0) || (y1 < 0) || (y1 >= ST7789_HEIGHT) || (y1 >= ST7789_HEIGHT))
    return;

  ST7789_CS_Set(1);
  uint16_t i, j;
  ST7789_SetAddrWindow(x0, y0, x1, y1);
  for (i = y0; i <= y1; i++) {
    for (j = x0; j <= x1; j++) {
      uint8_t data[] = {color >> 8, color & 0xFF};
      ST7789_WriteData(data, sizeof(data));
    }
  }

  ST7789_CS_Set(0);
}

void ST7789_Square(uint16_t x, uint16_t y, uint16_t dim, uint16_t color) {
  if ((x < 0) || (x * dim + dim - 1 >= ST7789_WIDTH) || (y < 0) || (y * dim + dim - 1 >= ST7789_HEIGHT))
    return;

  ST7789_CS_Set(1);
  ST7789_Fill(x, y, x * dim + dim - 1, y * dim + dim - 1, color);
}

void ST7789_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  ST7789_CS_Set(1);
  uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
  uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

  ST7789_WriteCommand(ST7789_CASET);
  {
    uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
    ST7789_WriteData(data, sizeof(data));
  }

  ST7789_WriteCommand(ST7789_RASET);
  {
    uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
    ST7789_WriteData(data, sizeof(data));
  }

  ST7789_WriteCommand(ST7789_RAMWR);
  ST7789_CS_Set(0);
}

uint16_t ST7789_ColorSelect(uint8_t r, uint8_t g, uint8_t b) {
  return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}
