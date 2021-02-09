#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

//double cTemp, humidity;
int temp, humi;
void i2c_reading(){
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, SHT31 I2C address is 0x44(68)
	ioctl(file, I2C_SLAVE, 0x44);
 	//PAGE8, Table 8 COMMANDS:
	//Clock stretching ENABLED:  0x2C,  Repeatability: 0x06(high), 0x0D(medium), 0x10(low)
	//Clock strechhing DISABLED: 0x24,  Repeatability: 0x00(high), 0x0B(medium), 0x16(low)

	char config[2] = {0};
	config[0] = 0x24;
	config[1] = 0x0B;
	write(file, config, 2);
	sleep(1);

	char data[6] = {0};
	read(file,data,6);
	//cTemp = (((data[0] * 256) + data[1]) * 175.0) / 65535.0  - 45.0;
	//humidity = (((data[3] * 256) + data[4])) * 100.0 / 65535.0;
	temp = 0;
	temp = data[0];
	temp <<= 8;
	temp = temp + data[1];

	humi = 0;
	humi = data[3];
	humi <<= 8;
	humi = humi + data[4];
}


int get_temp(void){

	return temp;
}
int get_humi(void){

	return humi;
}