#ifndef UTILITIES_TC_H_
#define UTILITIES_TC_H_

// function prototypes
static void configure_tc(void);

static void configure_tc(void)
{
	// channel 2 (divides frequency by 2)
	uint32_t ul_tcclks = 1;
	// get clock speed
	uint32_t ul_sysclk = sysclk_get_pba_hz();
	//TCO Config
	sysclk_enable_peripheral_clock(TC0);
	// calculation: 2K Hz means 500ms per full wave, which means 250ms per level
	// timer channel 2 divides system frequency by 2
	// Therefore, output should be (ul_sysclk / 2)Hz / 10^6ms/sec * 250 ms
	float tc_a_val_f = ((float)ul_sysclk/2000000.0) * 250.0;
	uint32_t tc_a_val = (uint32_t) tc_a_val_f; //values to write
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG); //3 timer channels option 1
	tc_write_rc(TC0, 0, tc_a_val); //writing value into counter a
	//enable interrupt
	NVIC_EnableIRQ((IRQn_Type) TC00_IRQn);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
	//start counters 0,1,2
	tc_start(TC0, 0);
}
// interrupt handler for the Timer Counter interrupt we configured
void TC00_Handler(void)
{
	// tc_get_status must be called to acknowledge that the
	// interrupt was handled
	tc_get_status(TC0, 0);
	// TODO: fill in the code to create the square wave
	
	// tells us whether led is currently on
	static bool ledIsOn = false;
	
	// is LED on?
	if (ledIsOn)
	{
		// turn LED off
		ioport_set_pin_level(BREADBOARD_LED_PIN, BREADBOARD_LED_OFF);
		ledIsOn = false;
	}
	else
	{
		// turn LED on
		ioport_set_pin_level(BREADBOARD_LED_PIN, BREADBOARD_LED_ON);
		ledIsOn = true;
	}
	
}
#endif /* UTILITIES_TC_H_ */