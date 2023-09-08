/*
 * utilities_eic.h
 *
 * Created: 2/1/2022 4:11:07 PM
 *  Author: Seth_Shively1
 */ 


#ifndef UTILITIES_EIC_H_
#define UTILITIES_EIC_H_

// function prototypes
static void eic_callback(void);
static void eic_setup(void);

static void eic_callback(void)
{
	// has interrupt occurred?
	if (eic_line_interrupt_is_pending(EIC, GPIO_PUSH_BUTTON_EIC_LINE)) {
		
		// switch unit to opposite of what it was at time of interrupt
		switch (displayState) {
			case TIMER:
				//printf("Display state: %d\r\n", displayState);
				c42412a_clear_all();
				displayState = HR;
				break;
			case HR:
				//printf("Display state: %d\r\n", displayState);
				c42412a_clear_all();
				displayState = GPS;
				break;
			case GPS:
				//printf("Display state: %d\r\n", displayState);
				c42412a_clear_all();
				displayState = TIMER;
				break;
		}
		
		// clear interrupt line
		eic_line_clear_interrupt(EIC, GPIO_PUSH_BUTTON_EIC_LINE);

	}	

}
static void eic_setup(void)
{
	// setup function from documentation
	eic_enable(EIC);
	struct eic_line_config eic_line_conf;
	eic_line_conf.eic_mode = EIC_MODE_EDGE_TRIGGERED;
	eic_line_conf.eic_edge = EIC_EDGE_FALLING_EDGE;
	eic_line_conf.eic_level = EIC_LEVEL_LOW_LEVEL;
	eic_line_conf.eic_filter = EIC_FILTER_DISABLED;
	eic_line_conf.eic_async = EIC_ASYNCH_MODE;
	eic_line_set_config(EIC, GPIO_PUSH_BUTTON_EIC_LINE, &eic_line_conf);
	
	eic_line_set_callback(EIC, GPIO_PUSH_BUTTON_EIC_LINE, eic_callback,
	GPIO_PUSH_BUTTON_EIC_IRQ, 1);
	
	eic_line_enable(EIC, GPIO_PUSH_BUTTON_EIC_LINE);
}


#endif /* UTILITIES_EIC_H_ */