#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
 
void main() 
{
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
 
	// Read 6 bytes of data
	// temp msb, temp lsb, temp CRC, humidity msb, humidity lsb, humidity CRC
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Error : Input/output Error \n");
	}
	else
	{
	// Convert the data
	double cTemp = (((data[0] * 256) + data[1]) * 175.0) / 65535.0  - 45.0;
	double fTemp = (((data[0] * 256) + data[1]) * 315.0) / 65535.0 - 49.0;
	double humidity = (((data[3] * 256) + data[4])) * 100.0 / 65535.0;
 
	// Output data to screen
	printf("Temperature in Celsius : %.2f C \n", cTemp);
	printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
	printf("Relative Humidity is : %.2f RH \n", humidity);
	}
}