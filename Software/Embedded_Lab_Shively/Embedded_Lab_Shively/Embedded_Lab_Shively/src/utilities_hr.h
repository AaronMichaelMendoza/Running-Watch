/*
 * utilities_hr.h
 *
 * Created: 4/26/2022 2:39:55 PM
 *  Author: Aaron_Mendoza1
 */ 


#ifndef UTILITIES_HR_H_
#define UTILITIES_HR_H_

#include <twim.h>
#include "utilities.h"
#include "common.h"


/** initialize_hr_sensor
 *
 *  This function uses TWIM functions to initialize the HR 
 *  sensor.
 *
 *  Parameters:
 *		none
 *  
 *  Return: void
 */
/** set_device_mode()
 *
 *  This function sets the device mode
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x01
 *  Index Byte: 0x00
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void set_device_mode(void) {
	uint8_t data_buf_tx[3] = {0x01, 0x00, 0x00};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- Device mode failed to be set.\r\n");
	}
	else {
		printf("+++ Device mode set.\r\n");
	}
	
	mdelay(40);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
	
}

/** read_device_mode()
 *
 *  This function reads the current mode of the device.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x02
 *  Index Byte: 0x00
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 0x00 if device is operating
 */
void read_device_mode(void) {
	// Indicate read command
	uint8_t data_buf_tx[2] = {0x02,0x00};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101; //7 bits for chip address
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 2;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	twi_master_write(TWIM0, &packet_tx);
	
	mdelay(40);
	
	uint8_t data_buf_rx[2]; // 2 element array to hold 16 bytes
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 2;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	status_code_t status = twi_master_read(TWIM0, &packet_rx);
	if (status != STATUS_OK) {
		printf("--- Failed to read device mode.\r\n");
	}
	else {
		printf("+++ Device mode read.\r\n");
	}
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
	printf("Byte 2: %X\r\n", (data_buf_rx[1]));
}

/** get_register_attributes()
 *
 *  This function reads the current mode of the device.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x42
 *  Index Byte: 0x03
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 0x01 0x24 if device is operating
 */
void get_register_attributes(void) {
	uint8_t data_buf_tx[2] = {0x42,0x03};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 2;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	twi_master_write(TWIM0, &packet_tx);
	
	mdelay(40);
	
	uint8_t data_buf_rx[3]; // 4 element array to hold 32 bytes
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101; 
	packet_rx.addr[0] = 0; 
	packet_rx.addr[1] = 0; 
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 3;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	status_code_t status = twi_master_read(TWIM0, &packet_rx);
	if (status != STATUS_OK) {
		printf("--- Failed to read register attributes.\r\n");
	}
	else {
		printf("+++ Register attributes read.\r\n");
	}
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
	printf("Byte 2: %X\r\n", (data_buf_rx[1]));
	printf("Byte 3: %X\r\n", (data_buf_rx[2]));
}

/** set_output_mode()
 *
 *  This function sets the output mode to raw and algorithm data.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x10
 *  Index Byte: 0x00
 *  Data Byte: 0x03
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void set_output_mode(void) {
	uint8_t data_buf_tx[3] = {0x10,0x00,0x02};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	twi_master_write(TWIM0, &packet_tx);
	
	mdelay(40);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101; 
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	status_code_t status = twi_master_read(TWIM0, &packet_rx);
	if (status != STATUS_OK) {
		printf("--- Output mode failed to be set.\r\n");
	}
	else {
		printf("+++ Output mode set.\r\n");
	}
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
}

/** set_fifo()
 *
 *  This function sets the FIFO threshold as almost full.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x10
 *  Index Byte: 0x01
 *  Data Byte: 0x0F
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void set_fifo(void) {
	uint8_t data_buf_tx[3] = {0x10,0x01,0x01};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- FIFO threshold failed to be set.\r\n");
	}
	else {
		printf("+++ FIFO threshold set.\r\n");
	}
	
	mdelay(40);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
}

/** enable_agc
 *
 *  This function enables the AGC.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x52
 *  Index Byte: 0x00
 *  Data Byte: 0x01
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void enable_agc(void) {
	uint8_t data_buf_tx[3] = {0x52,0x00,0x01};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- AGC failed to be enabled.\r\n");
	}
	else {
		printf("+++ AGC enabled.\r\n");
	}
	
	mdelay(40);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
}

/** enable_hr_sensor()
 *
 *  This function enables the MAX30101 HR sensor.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x44
 *  Index Byte: 0x03
 *  Data Byte: 0x01
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void enable_hr_sensor(void) {
	uint8_t data_buf_tx[3] = {0x44,0x03,0x01};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- HR sensor failed to be enabled.\r\n");
	}
	else {
		printf("+++ HR sensor enabled.\r\n");
	}
	
	mdelay(40);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
}

/** enable_algo()
 *
 *  This function enables the WHRM/MaximFast 10.x algorithm
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x52
 *  Index Byte: 0x02
 *  Data Byte: 0x01
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void enable_algo(void) {
	uint8_t data_buf_tx[3] = {0x52,0x02,0x01};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- Algorithm failed to be set.\r\n");
	}
	else {
		printf("+++ Algorithm set.\r\n");
	}
	
	mdelay(100);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
}

/** algo_config_read()
 *
 *  This function reads the number of samples to average for the AGC algorithm.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x51
 *  Index Byte: 0x00
 *  Data Byte: 0x03
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void algo_config_read(void) {
	uint8_t data_buf_tx[3] = {0x51,0x00,0x03};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- Failed to read number of samples for AGC algorithm.\r\n");
	}
	else {
		printf("+++ Read number of samples for AGC algorithm.\r\n");
	}
	
	mdelay(100);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
}

/** read_sensor_status()
 *
 *  This function reads the current status of the sensor.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x00
 *  Index Byte: 0x00
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 0x08 if device is operating
 */
uint8_t read_sensor_status(void) {
	// Indicate read command
	uint8_t data_buf_tx[2] = {0x00,0x00};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101; 
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 2;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	twi_master_write(TWIM0, &packet_tx);
	
	mdelay(40);
	
	uint8_t data_buf_rx[2]; // 2 element array to hold 16 bytes
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 2;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	status_code_t status = twi_master_read(TWIM0, &packet_rx);
	if (status != STATUS_OK) {
		printf("--- Sensor status not read.\r\n");
	}
	else {
		printf("+++ Sensor status read.\r\n");
	}
	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
	printf("Byte 2: %X\r\n", (data_buf_rx[1]));
	return data_buf_rx[1];
}

/** get_num_samples()
 *
 *  This function gets the number of samples in FIFO
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x12
 *  Index Byte: 0x00
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, return value changes based on FIFO threshold
 */
void get_num_samples(void) {
	// Indicate read command
	uint8_t data_buf_tx[2] = {0x12,0x00};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 2;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	twi_master_write(TWIM0, &packet_tx);
	
	mdelay(1000);
	
	uint8_t data_buf_rx[2]; // 2 element array to hold 16 bytes
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 2;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	status_code_t status = twi_master_read(TWIM0, &packet_rx);
	if (status != STATUS_OK) {
		printf("--- Num samples failed to be read.\r\n");
	}
	else {
		printf("+++ Num samples read.\r\n");
	}
 	printf("Byte 1: %X\r\n", (data_buf_rx[0]));
 	printf("Byte 2: %X\r\n", (data_buf_rx[1]));
}

/** read_data()
 *
 *  This function reads the data stored in the FIFO
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x12
 *  Index Byte: 0x01
 *
 *  Parameters:
 *		none
 *  
 *  Return: void
 */
int read_data(void) {
	// Indicate read command
	uint8_t data_buf_tx[2] = {0x12,0x01};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101; //7 bits for chip address
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 2;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	twi_master_write(TWIM0, &packet_tx);
	
	mdelay(40);
	
	uint8_t data_buf_rx[7]; 
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 7;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	status_code_t status = twi_master_read(TWIM0, &packet_rx);
	if (status != STATUS_OK) {
		printf("--- Failed to read data.\r\n");
	}
	else {
		printf("+++ Data read.\r\n");
	}
	
	uint16_t temp = (data_buf_rx[1] << 8 | data_buf_rx[2]);
	double HR = (double)temp / 10.0;
	int intHR = HR;
	printf("HR: %i\r\n", intHR);
	printf("Status: %x\r\n\r\n", data_buf_rx[6]);
	return intHR;
}

/** disable_hr_sensor()
 *
 *  This function disables the HR sensor
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x44
 *  Index Byte: 0x03
 *  Data Byte: 0x00
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void disable_hr_sensor(void) {
	uint8_t data_buf_tx[3] = {0x44,0x03,0x00};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- HR sensor failed to be disabled.\r\n");
	}
	else {
		printf("+++ HR sensor disabled.\r\n");
	}
	
	mdelay(40);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
}

/** disable_algo()
 *
 *  This function disables the algorithm.
 *
 *  Address Byte: 0xAA
 *  Family Byte: 0x52
 *  Index Byte: 0x02
 *  Data Byte: 0x00
 *
 *  Parameters:
 *		none
 *  
 *  Return: void, should return 0x00 if device is operating
 */
void disable_algo(void) {
	uint8_t data_buf_tx[3] = {0x52,0x02,0x00};
	twi_package_t packet_tx;
	packet_tx.chip = 0b1010101;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) data_buf_tx;
	packet_tx.length = 3;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	status_code_t status = twi_master_write(TWIM0, &packet_tx);
	if (status != STATUS_OK) {
		printf("--- Algorithm failed to be disabled.\r\n");
	}
	else {
		printf("+++ Algorithm disabled.\r\n");
	}
	
	mdelay(100);
	
	uint8_t data_buf_rx[1]; // 1 element array to hold 1 byte
	twi_package_t packet_rx;
	packet_rx.chip = 0b1010101;
	packet_rx.addr[0] = 0;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 0;
	packet_rx.buffer = (void *) data_buf_rx;
	packet_rx.length = 1;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	twi_master_read(TWIM0, &packet_rx);
}

void configure_hr(void) {
	ioport_set_pin_dir(RST_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(MFIO_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RST_PIN, LOW);
	ioport_set_pin_level(MFIO_PIN, HIGH);
	mdelay(10);
	ioport_set_pin_level(RST_PIN, HIGH);
	mdelay(1000);
	ioport_set_pin_dir(MFIO_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(MFIO_PIN, IOPORT_MODE_PULLUP);
	
	printf("\r\n********** STARTING INITIALIZATION **********\r\n");
	
	
	// Read MAX32664 mode
	printf("\r\nDevice mode read:\r\n");
	read_device_mode();
	mdelay(1000);
	
	// Get register attributes
	printf("\r\nRegister attributes: \r\n");
	get_register_attributes();
	mdelay(1000);
	
	// Set output mode of MAX32664.
	printf("\r\nSetting output mode: \r\n");
	set_output_mode();
	mdelay(1000);
	
	// Set FIFO threshold
	printf("\r\nSetting threshold: \r\n");
	set_fifo();
	mdelay(1000);
	
	// Enable automatic gain control
	printf("\r\nEnabling agc: \r\n");
	enable_agc();
	mdelay(1000);
	
	// Enable HR sensor
	printf("\r\nEnabling HR sensor: \r\n");
	enable_hr_sensor();
	mdelay(1000);
	
// 	// Enable algorithm
	printf("\r\nEnabling algorithm: \r\n");
	enable_algo();
	
	// Configure algorithm samples
	printf("\r\nConfiguring algorithm samples: \r\n");
	algo_config_read();
	
	mdelay(1000);
	
	printf("\r\n********** INITIALIZATION COMPLETE **********\r\n");
}

#endif /* UTILITIES_HR_H_ */