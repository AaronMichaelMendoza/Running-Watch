/*
 * utilities_lcd.h
 *
 * Created: 2/1/2022 3:12:11 PM
 *  Author: Seth_Shively1
 */ 


#ifndef UTILITIES_LCD_H_
#define UTILITIES_LCD_H_

// function prototypes
void configure_lcd_backlight(void);
void set_lcd_backlight(LCD_BACKLIGHT_TYPE level);

void configure_lcd_backlight(void)
{
	// setup LCD backlight pin as output
	ioport_set_pin_dir(LCD_BACKLIGHT_PIN, IOPORT_DIR_OUTPUT);
}

void set_lcd_backlight(LCD_BACKLIGHT_TYPE level)
{
	// set LCD backlight on or off depending on parameter
	ioport_set_pin_level(LCD_BACKLIGHT_PIN, level);
}

#endif /* UTILITIES_LCD_H_ */