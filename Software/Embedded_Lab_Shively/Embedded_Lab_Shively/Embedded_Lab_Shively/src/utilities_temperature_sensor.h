/*
 * utilities_temperature_sensor.h
 *
 * Created: 2/10/2022 2:37:31 PM
 *  Author: Seth_Shively1
 */ 


#ifndef UTILITIES_TEMPERATURE_SENSOR_H_
#define UTILITIES_TEMPERATURE_SENSOR_H_

// function prototypes
void configure_temperature_sensor(void);
float read_temp_data(TEMPERATURE_UNIT_TYPE);

void configure_temperature_sensor(void)
{
	// initialize config struct members
	struct twim_config twim_conf;
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
	
	// set config and callback
	twim_set_config(TWIM3, &twim_conf);
	twim_set_callback(TWIM3, 0, twim_default_callback, 1);	
	
	// bytes to write: pointer register (1st byte) and config register (2nd byte)
	// pointer value points to pointer register, config value tells sensor to use
	// 0.0625-degree precision
	uint8_t data_buf_tx2[2] = {0b00000001, 0b01100000};
		
	// create package and initialize members
	twi_package_t packet_tx;
	packet_tx.chip = 0b1001111;   // chip ID for temp sensor
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) &data_buf_tx2;  // 2-byte array to send
	packet_tx.length = 2;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	
	// write pointer and config data to temp sensor
	twi_master_write(TWIM3, &packet_tx);
	
	// pointer register byte to point to temperature register
	uint8_t data_buf_tx = 0b00000000;
	packet_tx.length = 1;
	packet_tx.buffer = (void*) &data_buf_tx;  // 1-byte data to send
	
	// write value to pointer register
	twi_master_write(TWIM3, &packet_tx);
}

float read_temp_data(TEMPERATURE_UNIT_TYPE unit)
{
	// 2-byte value to represent temperature data
	uint8_t data_buf_rx[2];
	
	// initialize package struct members
	twi_package_t packet_rx;
	packet_rx.chip = 0b1001111;   // chip address for temp sensor
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) &data_buf_rx;   // temp data to be read
	packet_rx.length = 2;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	
	// read temp data
	twi_master_read(TWIM3, &packet_rx);
	
	// value in first byte multiplied by 16 (4 zeros added to end)
	float dataUpper = (float)data_buf_rx[0] * 16;
	
	// value in second byte divided by 16 (4 least significant bits removed)
	float dataLower = (float)data_buf_rx[1] / 16;
	
	// complete data value from sensor
	float data = dataUpper + dataLower;
	
	// temperature in Celsius, data multiplied by precision
	float temp = data * 0.0625;
	
	// is unit set to Fahrenheit?
	if (unit == FAHRENHEIT)
	{
		// convert temperature to F
		temp = temp * (9.0 / 5.0) + 32.0;
	}
	return temp;
}


#endif /* UTILITIES_TEMPERATURE_SENSOR_H_ */