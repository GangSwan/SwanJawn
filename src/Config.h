#ifndef CONFIG_H
#define CONFIG_H

// --- Hardware Pins ---
#define BUZZER_PIN 25
#define ENCODER_CLK 33
#define ENCODER_DT 32
#define ENCODER_BTN 34
#define RED_LED 16
#define GREEN_LED 17
#define BLUE_LED 4 

// SD Card SPI Pins
#define SD_CS        5
#define SD_MOSI      23
#define SD_MISO      19
#define SD_SCK       18

// --- OLED Display Settings ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// --- Buzzer Note Definitions ---
#define Fb5 740
#define D5 587
#define E5 659
#define F5 698
#define Cb5 554
#define B5 988
#define B4 494
#define A5 880
#define G5 784
#define Cb6 1109
#define D6 1175
#define E6 1319
#define Fb6 1480
#define F6 1397
#define B6 1976
#define A6 1760
#define G6 1568
#define Ab6 1865

#endif // CONFIG_H
