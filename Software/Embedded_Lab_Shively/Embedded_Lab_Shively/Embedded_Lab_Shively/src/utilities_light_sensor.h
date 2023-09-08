/*
 * utilities_light_sensor.h
 *
 * Created: 2/1/2022 3:24:44 PM
 *  Author: Seth_Shively1
 */ 


#ifndef UTILITIES_LIGHT_SENSOR_H_
#define UTILITIES_LIGHT_SENSOR_H_

// function prototypes
void light_sensor_handler(void);
void configure_light_sensor(void);

void configure_light_sensor(void)
{
	// setup light sensor pin as output
	ioport_set_pin_dir(LIGHT_SENSOR_PIN, IOPORT_DIR_INPUT);
	
	// enable interrupt on light sensor pin
	gpio_enable_pin_interrupt(LIGHT_SENSOR_PIN);
	
	// specify callback function
	gpio_set_pin_callback(LIGHT_SENSOR_PIN, light_sensor_handler, 1);
}

void light_sensor_handler()
{
	// if light sensor pin is high
	if (ioport_get_pin_level(LIGHT_SENSOR_PIN) == GPIO_INPUT_LEVEL_HIGH)
	{
		// turn light on
		set_lcd_backlight(LCD_BACKLIGHT_ON);
	}
	else
	{
		// turn light off
		set_lcd_backlight(LCD_BACKLIGHT_OFF);
	}
}

#endif /* UTILITIES_LIGHT_SENSOR_H_ */