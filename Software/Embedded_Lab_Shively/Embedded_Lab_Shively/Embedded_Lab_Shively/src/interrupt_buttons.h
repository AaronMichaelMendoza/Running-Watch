/*
 * utilities_light_sensor.h
 *
 * Created: 2/1/2022 3:24:44 PM
 *  Author: Seth_Shively1
 */ 


#ifndef UTILITIES_INTERRUPT_BUTTONS_H_
#define UTILITIES_INTERRUPT_BUTTONS_H_

// function prototypes
void button_1_callback(void);
void button_2_callback(void);
void button_3_callback(void);
void configure_buttons(void);

void configure_buttons(void)
{
	// setup SW1 pin
	ioport_set_pin_dir(SW1_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(SW1_PIN, IOPORT_MODE_PULLDOWN);
	gpio_enable_pin_interrupt(SW1_PIN);
	gpio_set_pin_callback(SW1_PIN, button_1_callback, 1);
	
	// setup SW2 pin
	ioport_set_pin_dir(SW2_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(SW2_PIN, IOPORT_MODE_PULLDOWN);
	gpio_enable_pin_interrupt(SW2_PIN);
	gpio_set_pin_callback(SW2_PIN, button_2_callback, 1);
	
	// setup SW3 pin
	ioport_set_pin_dir(SW3_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(SW3_PIN, IOPORT_MODE_PULLDOWN);
	gpio_enable_pin_interrupt(SW3_PIN);
	gpio_set_pin_callback(SW3_PIN, button_3_callback, 1);
}


// Controls watch state
void button_1_callback()
{	
	// if SW1 is low (falling edge)
	if (ioport_get_pin_level(SW1_PIN) == GPIO_INPUT_LEVEL_LOW)
	{
		// clear any previous LCD text
		c42412a_clear_all();
		
		switch(watchState) {
			case IDLE:
				// idle to running
				c42412a_clear_text();				
				watchState = RUNNING;
				pressTime = ticks;
				prevTime = 0;
				printf("Watch state: %d\r\n", watchState);
				break;
			case RUNNING:
				// running to paused
				watchState = PAUSED;
				printf("Watch state: %d\r\n", watchState);
				break;
			case PAUSED:
				// paused back to running
				pressTime = ticks;
				prevTime = curTime;			
				watchState = RUNNING;
				printf("Watch state: %d\r\n", watchState);
				break;
			};
	}
}


// switches state to DONE
void button_2_callback()
{
	
	// if SW2 is low
	if (ioport_get_pin_level(SW2_PIN) == GPIO_INPUT_LEVEL_LOW)
	{
		c42412a_clear_all();
		// switch state to DONE if it is paused or running
		if (watchState == RUNNING || watchState == PAUSED) {			
			watchState = DONE;
			printf("Watch state: %d\r\n", watchState);
		}
	}
}

// controls display state
void button_3_callback() {
	// if state is running or paused
	if (ioport_get_pin_level(SW3_PIN) == GPIO_INPUT_LEVEL_LOW && watchState != IDLE && watchState != DONE) {
		
		// clear previous input
		c42412a_clear_all();
		c42412a_clear_icon(C42412A_ICON_COLON);
		
		switch (displayState) {
			case TIMER:
				// timer to HR
				printf("Display state: %d\r\n", displayState);
				displayState = HR;
				break;
			case HR:
				// HR to GPS
				printf("Display state: %d\r\n", displayState);
				c42412a_clear_text();
				displayState = GPS;
				break;
			case GPS:
				// GPS to timer
				printf("Display state: %d\r\n", displayState);
				displayState = TIMER;
				break;
		}
	} 
}

#endif /* UTILITIES_LIGHT_SENSOR_H_ */