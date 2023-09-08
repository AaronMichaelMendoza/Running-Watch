#ifndef UTILITIES_H_
#define UTILITIES_H_
#include "common.h"
#include "conf_uart_serial.h"
// create function prototypes based on the function definitions below
GPIO_INPUT_STATE_TYPE check_gpio_input_state(int);
void display_time(uint32_t ms_value);
void configure_twim();
static void mdelay(uint32_t delay_ms);
/////////////////////////////////////////////////////////////////////////////
// Name: check_gpio_input_state
// Inputs: int pin_number - the pin number of the gpio input pin
// Outputs: GPIO_INPUT_STATE_TYPE gpio_input_state
// See definition of GPIO_INPUT_STATE_TYPE in common.h for possible values
// IMPORTANT TERMINOLOGY NOTE: the term "level" simply indicates the 0 or 1
// level of the input pin.  The term "state" is more complex.  It indicates
// the state of the input pin such as FALLING EDGE, LOW, RISING EDGE, or HIGH
// the state takes into account history, whereas level is just a 0 or 1
/////////////////////////////////////////////////////////////////////////////
GPIO_INPUT_STATE_TYPE check_gpio_input_state(int pin_number)
{
	// create static variables for current and previous pin input levels
	// Note on static variables: static variables retain their values even after
	// the function exits because they are stored in the static section of memory
	// rather than the stack.  They are initialized to their initial values the
	// first time the function is called, but then they are not re-initialized
	// each time the function is called
	static GPIO_INPUT_LEVEL_TYPE current_gpio_input_level[NUM_MICROCONTROLLER_PINS] = {GPIO_INPUT_LEVEL_HIGH};	
	static GPIO_INPUT_LEVEL_TYPE prev_gpio_input_level[NUM_MICROCONTROLLER_PINS] = {GPIO_INPUT_LEVEL_HIGH};
	// the following few lines of code are intended to be executed one time to
	// determine the initial state of the pin.  This will prevent incorrectly
	// detecting an edge the first time the code is run
	static bool first_time = true;
	if(first_time)
	{
		current_gpio_input_level[pin_number] = ioport_get_pin_level(pin_number);
		prev_gpio_input_level[pin_number] = current_gpio_input_level[pin_number];
		first_time = false;
	}
	// create and initialize gpio_input_state
	// the initialization value is arbitrary because it will be
	// updated by the logic of the function
	GPIO_INPUT_STATE_TYPE gpio_input_state = GPIO_INPUT_STATE_LOW;
	// save the input level that you read last time you called the function
	// into prev_gpio_input_level
	prev_gpio_input_level[pin_number] = current_gpio_input_level[pin_number];
	// read a new value from the input pin
	current_gpio_input_level[pin_number] = ioport_get_pin_level(pin_number);
	// TODO: use the current and previous levels to set gpio_input_state
	// use the UML Activity Diagram from the lab manual to guide your logic
	
	// is current pin low?
	if (current_gpio_input_level[pin_number] == GPIO_INPUT_LEVEL_LOW)
	{
		// is previous pin also low?
		if (prev_gpio_input_level[pin_number] == GPIO_INPUT_LEVEL_LOW)
		{
			// set input state to low
			gpio_input_state = GPIO_INPUT_STATE_LOW;
		}
		else
		{
			// set input state to falling edge
			gpio_input_state = GPIO_INPUT_STATE_FALLING_EDGE;
		}
	}
	else
	{
		// is previous pin low?
		if (prev_gpio_input_level[pin_number] == GPIO_INPUT_LEVEL_LOW)
		{
			// set input state to rising edge
			gpio_input_state = GPIO_INPUT_STATE_RISING_EDGE;
		}
		else
		{
			// set input state to high
			gpio_input_state = GPIO_INPUT_STATE_HIGH;
		}
	}
	
	// return input state
	return gpio_input_state;	
}

// counts system time from when program begins
volatile uint32_t ticks = 0;

// update ticks on interrupt
void SysTick_Handler(void)
{
	ticks++;
}

// Displays time to 2 decimal places in seconds to LCD
void display_time(uint32_t ms_value)
{
	// truncated value to represent seconds to hundredth place
	uint32_t ms_rounded = ms_value / 10;
	
	// display value to lcd
	c42412a_show_numeric_dec(ms_rounded);
	
	// is ms_rounded less than 5 digits?
	if (ms_rounded < 10000)
	{
		// display decimal at position 3
		c42412a_show_icon(C42412A_ICON_DOT_3);
	}
	// is ms_rounded less than 6 digits?
	else if (ms_rounded < 100000)
	{
		// clear previous decimal point
		c42412a_clear_icon(C42412A_ICON_DOT_3);
		
		// display decimal at position 4
		c42412a_show_icon(C42412A_ICON_DOT_4);
	}
}

static void mdelay(uint32_t delay_ms)
{
	// time when function is called
	uint32_t start_time = ticks;
	
	// increment until amount of time specified by start_time has passed
	while(ticks <= start_time + delay_ms){}
}

static void configure_console(void)
{
	// initialize struct members for configuration
	const usart_serial_options_t uart_serial_options =
	{
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS,

	};
	
	/* Configure console. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

void configure_twim() {
	// initialize config struct members
	struct twim_config twim_conf;
	twim_conf.twim_clk = sysclk_get_cpu_hz();
	twim_conf.speed = TWI_STD_MODE_SPEED;
	twim_conf.smbus = false;
	twim_conf.hsmode_speed = 0;
	twim_conf.data_setup_cycles = 4;
	twim_conf.hsmode_data_setup_cycles = 0;
	twim_conf.clock_slew_limit = 0;
	twim_conf.clock_drive_strength_low = 0;
	twim_conf.data_slew_limit = 0;
	twim_conf.data_drive_strength_low = 0;
	twim_conf.hs_clock_slew_limit = 0;
	twim_conf.hs_clock_drive_strength_high = 0;
	twim_conf.hs_clock_drive_strength_low = 0;
	twim_conf.hs_data_slew_limit = 0;
	twim_conf.hs_data_drive_strength_low = 0;
	
	// set config and callback
	twim_set_config(TWIM3, &twim_conf);
	twim_set_callback(TWIM3, 0, twim_default_callback, 1);
	
	twim_conf.twim_clk = sysclk_get_cpu_hz();
	twim_conf.speed = TWI_STD_MODE_SPEED;
	twim_conf.smbus = false;
	twim_conf.hsmode_speed = 0;
	twim_conf.data_setup_cycles = 0;
	twim_conf.hsmode_data_setup_cycles = 0;
	twim_conf.clock_slew_limit = 0;
	twim_conf.clock_drive_strength_low = 0;
	twim_conf.data_slew_limit = 0;
	twim_conf.data_drive_strength_low = 0;
	twim_conf.hs_clock_slew_limit = 0;
	twim_conf.hs_clock_drive_strength_high = 0;
	twim_conf.hs_clock_drive_strength_low = 0;
	twim_conf.hs_data_slew_limit = 0;
	twim_conf.hs_data_drive_strength_low = 0;
	twim_set_config(TWIM0, &twim_conf);
	twim_set_callback(TWIM0, 0, twim_default_callback, 1);
}

#endif /* UTILITIES_H_ */