#ifndef COMMON_H_
#define COMMON_H_
// breadboard LED macros
#define BREADBOARD_LED_PIN EXT1_PIN_6
#define BREADBOARD_LED_OFF 0
#define BREADBOARD_LED_ON  1
#define BREADBOARD_BUTTON_PIN EXT1_PIN_7
#define SW1_PIN BREADBOARD_BUTTON_PIN
#define SW2_PIN EXT1_PIN_8
#define SW3_PIN EXT1_PIN_9
#define NUM_MICROCONTROLLER_PINS 100
#define LCD_BACKLIGHT_PIN PIN_PC05D_TC1_B2
#define LIGHT_SENSOR_PIN PIN_PA07
#define RST_PIN EXT1_PIN_5
#define MFIO_PIN EXT1_PIN_6

// gpio input level enumeration
// "level" indicates the voltage level of
// the input, either high or low
typedef enum{
	GPIO_INPUT_LEVEL_LOW  = 0,
	GPIO_INPUT_LEVEL_HIGH = 1
}GPIO_INPUT_LEVEL_TYPE;
// gpio input state enumeration
// "state" indicates the state of the input
// when taking the previous level into account
typedef enum{
	GPIO_INPUT_STATE_LOW = 0, // level is low and was previously low
	GPIO_INPUT_STATE_RISING_EDGE = 1, // level is high and was previously low
	GPIO_INPUT_STATE_HIGH = 2, // level is high and was previously high
	GPIO_INPUT_STATE_FALLING_EDGE = 3 // level is low and was previously high
}GPIO_INPUT_STATE_TYPE;
// led state enumeration
// this is a generic term to describe whether the
// LED is on or off, independent of the circuitry
// and whether 0 or 1 turns the LED on
typedef enum{
	LED_STATE_OFF = 0,
	LED_STATE_ON  = 1
}LED_STATE_TYPE;

typedef enum{
	LCD_BACKLIGHT_OFF = 0,
	LCD_BACKLIGHT_ON = 1
} LCD_BACKLIGHT_TYPE;
typedef enum{
	CELSIUS = 0,
	FAHRENHEIT = 1
}TEMPERATURE_UNIT_TYPE;

// Global temperature unit indicator, is updated with EIC interrupt
TEMPERATURE_UNIT_TYPE tempUnit = CELSIUS;
// watch state
typedef enum{
	IDLE  = 0,
	RUNNING = 1,
	PAUSED = 2,
	DONE = 3
}WATCH_STATE_TYPE;

// display state
typedef enum{
	TIMER  = 0,
	HR = 1,
	GPS = 2,
}DISPLAY_STATE_TYPE;
WATCH_STATE_TYPE watchState = IDLE;
DISPLAY_STATE_TYPE displayState = TIMER;

uint32_t radius = 3963;
uint32_t startTime = 0;
uint32_t curTime = 0;
uint32_t pressTime = 0;
uint32_t prevTime = 0;
#endif /* COMMON_H_ */