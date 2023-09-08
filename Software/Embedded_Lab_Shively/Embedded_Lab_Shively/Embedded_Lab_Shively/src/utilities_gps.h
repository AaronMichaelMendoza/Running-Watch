/*
 * utilities_temperature_sensor.h
 *
 * Created: 2/10/2022 2:37:31 PM
 *  Author: Seth_Shively1
 */ 
#include <asf.h>


#ifndef UTILITIES_GPS_H_
#define UTILITIES_GPS_H_

// function prototypes
void configure_gps(void);
void read_gps_data(int*, int*);

void configure_gps(void)
{
		
	
	// 9-byte array to initially write to the module
	uint8_t data_buf_tx9[9] = {0xB5, 0x62, 0x06, 0x00, 0x01, 0x00, 0x00, 0x07, 0x21};
		
	// Initialize write package
	twi_package_t packet_tx;
	packet_tx.chip = 0x42;   // chip ID for GPS module
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) &data_buf_tx9;  // initial array to send
	packet_tx.length = 0;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	
	// first execute "empty" write statement
	twi_master_write(TWIM3, &packet_tx);
	mdelay(50);

	
	// set length to 9 so array can be sent
	packet_tx.length = 9;
	// send array
	twi_master_write(TWIM3, &packet_tx);
	mdelay(50);
	
	// 2-byte package to read
	uint8_t data_buf_rx2[2];
	twi_package_t packet_rx;
	packet_rx.chip = 0x42;   // chip address for temp sensor
	packet_rx.addr[0] = 0xFD; // "write" byte for start-restart command
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 1;
	packet_rx.buffer = (void *) &data_buf_rx2;
	packet_rx.length = 2;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;

	
	// read 2-byte array
	twi_master_read(TWIM3, &packet_rx);
	mdelay(50);
	// 32-byte and 6-byte arrays into which to read, function does not work unless this data is read
	uint8_t data_buf_rx32[32];
	uint8_t data_buf_rx6[6];
	
	// read 32-byte and 6-byte arrays
	packet_rx.buffer = (void*) data_buf_rx32;
	packet_rx.length = 32;
	packet_rx.addr[0] = 0;
	packet_rx.addr_length = 0;
	twi_master_read(TWIM3, &packet_rx);
	mdelay(50);
	packet_rx.buffer = data_buf_rx6;
	packet_rx.length = 6;
	twi_master_read(TWIM3, &packet_rx);
	mdelay(50);
	
	
}

void read_gps_data(int* lat, int* lon)
{
	bool timeoutReached = false;
	// Send following 8-byte array to initialize data transfer
	uint8_t data_buf_tx8[8] = {0xB5, 0x62, 0x01, 0x07, 0x00, 0x00, 0x08, 0x19};
		
	// Address of register containing number of bytes available
	uint8_t data_bux_tx1 = 0xFD;
	twi_package_t packet_tx;
	packet_tx.chip = 0x42;
	packet_tx.addr[0] = 0;
	packet_tx.addr[1] = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer = (void *) &data_buf_tx8;
	packet_tx.length = 8;
	packet_tx.high_speed = 0;
	packet_tx.high_speed_code = 0;
	packet_tx.ten_bit = 0;
	
	// Send 8-byte array
	twi_master_write(TWIM3, &packet_tx);
	
	// set up packet to read number of bytes available
	uint8_t data_buf_rx2[2];
	data_buf_rx2[1] = 0x00;
	twi_package_t packet_rx;
	packet_rx.chip = 0x42; 
	packet_rx.addr[0] = 0xFD;
	packet_rx.addr[1] = 0;
	packet_rx.addr_length = 1;
	packet_rx.buffer = (void *) &data_buf_rx2;
	packet_rx.length = 2;
	packet_rx.high_speed = 0;
	packet_rx.high_speed_code = 0;
	packet_rx.ten_bit = 0;
	
	// Check number of bytes available until device returns 100 (64 in hex)
	uint32_t start = ticks;
	do {
		twi_master_read(TWIM3, &packet_rx);
		mdelay(100);
		if (ticks - start >= 1000) {
			timeoutReached = true;
			c42412a_show_icon(C42412A_ICON_WLESS_LEVEL_0);
			break;

		}
	} while(data_buf_rx2[1] != 0x64);
	if (!timeoutReached) {
		c42412a_clear_icon(C42412A_ICON_WLESS_LEVEL_0);
	}
	
	// 4 arrays into which to read 100 bytes
	uint8_t data_buf_rx32_1[32];
	uint8_t data_buf_rx32_2[32];
	uint8_t data_buf_rx32_3[32];
	uint8_t data_buf_rx4[4];
	
	// read data into 4 arrays
	packet_rx.buffer = (void*) data_buf_rx32_1;
	packet_rx.length = 32;
	packet_rx.addr_length = 0;
	twi_master_read(TWIM3, &packet_rx);
/*	mdelay(50);*/
	packet_rx.buffer = data_buf_rx32_2;
	twi_master_read(TWIM3, &packet_rx);
/*	mdelay(50);*/
	packet_rx.buffer = data_buf_rx32_3;
	twi_master_read(TWIM3, &packet_rx);
/*	mdelay(50);*/
	packet_rx.buffer = data_buf_rx4;
	packet_rx.length = 4;
	twi_master_read(TWIM3, &packet_rx);
/*	mdelay(50);*/
	
	// longitude = last two bytes from first array (MSB's) and first two from second array (LSB's)
	uint32_t lon_int = 0;
	lon_int |= (uint32_t)data_buf_rx32_1[30];
	lon_int |= (uint32_t)data_buf_rx32_1[31] << 8;
	lon_int |= (uint32_t)data_buf_rx32_2[0] << 16;
	lon_int |= (uint32_t)data_buf_rx32_2[1] << 24;
	
	// latitude = bytes 2 through 5 from 2nd array
	uint32_t lat_int = 0;
	lat_int |= (uint32_t)data_buf_rx32_2[2];
	lat_int |= (uint32_t)data_buf_rx32_2[3] << 8;
	lat_int |= (uint32_t)data_buf_rx32_2[4] << 16;
	lat_int |= (uint32_t)data_buf_rx32_2[5] << 24;
	
	// convert to signed int
	*lon = (int)lon_int;
	*lat = (int)lat_int;
}



#endif /* UTILITIES_GPS_H_ */