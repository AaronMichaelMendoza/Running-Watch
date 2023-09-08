/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <math.h>
#include "common.h"
#include "utilities.h"
#include "utilities_lcd.h"
#include "utilities_light_sensor.h"
//#include "utilities_eic.h"
#include "utilities_hr.h"
#include "interrupt_buttons.h"
#include "utilities_gps.h"

int main (void)
{
	// initialize board
	board_init();
	
	// initialize and configure sysclk
	sysclk_init();	
	SysTick_Config(sysclk_get_cpu_hz() / 1000);
	
	// initialize lcd
	c42412a_init();
	
	// initialize EIC
	//eic_setup();
	
	// configure Putty console output
	configure_console();
	// set up twim, gps and HR sensor
	configure_twim();
	configure_gps();
	mdelay(20);
	configure_hr();
	
	configure_buttons();
	// highest priority for systick IRQ
	NVIC_SetPriority(SysTick_IRQn, 1);
	NVIC_SetPriority(TWIM3_IRQn, 2);
	
	// second highest priority for GPIO IRQ
	NVIC_SetPriority(GPIO_0_IRQn, 3);
	NVIC_SetPriority(GPIO_1_IRQn, 4);
	NVIC_SetPriority(GPIO_2_IRQn, 5);
	
	// current and previous latitude and longitude and change variables
	int curLat, curLon, prevLat, prevLon, changeLat, changeLon;
	
	// lat and lon variables converted to actual (floating point scaled) values
	double curLatF, curLonF, prevLatF, prevLonF, changeLonF, changeLatF;
	
	// lat and lon variables in radians
	double curLatR, curLonR, changeLatR, changeLonR, prevLatR, prevLonR;
	
	// value used in distance calculation
	double a;
	
	// distance between current and previous lat and lon points
	double distance;
	
	// total distance traveled
	double totalDist = 0.0;
	
	// value to convert degrees to radians
	double p = 0.017453292519943295;
	
	// heartrate value
	int hr;
	
	// indicates whether current run is the first run
	bool firstRun = true;
	
	// minute and second values
	uint32_t minutes, seconds;
	
	
	c42412a_show_text("START");		
	
	
	
	while(1) 
	{
			if (watchState == RUNNING || watchState == PAUSED) {
				// read HR data
				hr = read_data();
				// read GPS data
 				read_gps_data(&curLat, &curLon);
			}
		
			switch (watchState) {
				case RUNNING:
					
					
					// calculate GPS coordinates in radians
					curLonF = (double)curLon / 10000000.0;
					curLatF = (double)curLat / 10000000.0;
					prevLonF = (double)prevLon / 10000000.0;
					prevLatF = (double)prevLat / 10000000.0;
					printf("lat: %d\r\n", curLat);
					printf("lon: %d\r\n", curLon);
					changeLat = abs(curLat - prevLat);
					changeLon = abs(curLon - prevLon);
					changeLatF = (double)changeLat / 10000000.0;
					changeLonF = (double)changeLon / 10000000.0;
					curLonR = p * curLonF;
					curLatR = p * curLatF;
					prevLonR = p * prevLonF;
					prevLatR = p * prevLatF;
					changeLatR = p * changeLatF;
					changeLonR = p * changeLonF;
					printf("changeLat: %d\r\n", changeLat);
					
					// calculate distance
					a = 0.5 - cos((changeLatR))/2.0 + cos(prevLatR) * cos(curLatR) * ((1-cos(changeLonR))/2);
					printf("a: %3.5f\r\n", a);
					distance = 2 * radius * asin(sqrt(a));
					printf("Distance: %3.2f\r\n", distance);
					prevLat = curLat;
					prevLon = curLon;
	
					if (firstRun) {
						firstRun = false;
					}
					else {
						totalDist += distance;
					}
					
					printf("Total distance: %10.10f\r\n", totalDist);
					prevLat = curLat;
					prevLon = curLon;
					curTime = prevTime + (ticks - pressTime);
					minutes = (curTime / 1000) / 60.0;
					seconds = (curTime / 1000) % 60;
					switch (displayState) {
						case TIMER:
							c42412a_show_numeric_dec(100*minutes + seconds);
							c42412a_show_icon(C42412A_ICON_COLON);
							break;
						case HR:
							if (hr == 0) {
								c42412a_show_text("RDING");
							}
							else {
								c42412a_clear_text();
								c42412a_show_numeric_dec(hr);	
							}
							break;
						case GPS:
							c42412a_show_numeric_dec((int)(totalDist * 100.0));
							c42412a_show_icon(C42412A_ICON_DOT_3);
							break;
					}
					break;
				case DONE:
					c42412a_clear_icon(C42412A_ICON_COLON);
					c42412a_show_text("DONE");
					break;
				case PAUSED:
					switch (displayState) {
						case TIMER:
							c42412a_show_numeric_dec(100*minutes + seconds);
							c42412a_show_icon(C42412A_ICON_COLON);
							break;
						case HR:
							c42412a_clear_icon(C42412A_ICON_COLON);
							if (hr == 0) {
								c42412a_show_text("RDING");
							}
							else {
								c42412a_clear_text();
								c42412a_show_numeric_dec(hr);
							}
							break;
						case GPS:
							c42412a_show_numeric_dec((int)(totalDist * 100.0));
							c42412a_show_icon(C42412A_ICON_DOT_3);;
							break;
					}
	
					break;
				default:
					break;
			}
			mdelay(10);		
   	}		
}
