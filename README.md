# STM32_ST7789
Creates an STM32 library for use with the ST7789 (SPI TFT display),
based heavily off of @Floyd-Fish's [ST7789-STM32](https://github.com/Floyd-Fish/ST7789-STM32), as 
well as [Adafruit-ST7735-Library](https://github.com/adafruit/Adafruit-ST7735-Library) 
and [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library/tree/master).
Specifically, this is for use with my [BasicBoard-STM32G473CET6 board](https://github.com/kjpeng/BasicBoard-STM32G473CET6), which uses SPI1 pins PA7 (SPI1_MOSI), PA5 (SPI1_SCK), PA10 (SPI1_CS), and PB10 (DC).
