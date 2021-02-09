# Project: cloud_project
# Author: Miodrag Vukovic
# Date: 25.8.2020
# Revision: 1.0

# Read temperature and humidity value from SHT3 sensor.
# These values are measured every 5s and published to WolkAbout IoT platform.

#import necessary libraries
import os
import sys
import smbus
import time
import datetime
from io import StringIO
import wolk
import ctypes

_i2c = ctypes.CDLL('/home/pi/Desktop/I2C_cython/i2c_read.so')


# define measurement period in seconds
MEASUREMENT_PERIOD = 5
  
  
C1 = -45.0
C2 = 175.0
C3 = 65535.0

def main():
    global _i2c
    # Define device which will be connected to colud
    # key and password defined by WolkAbout platform
    device = wolk.Device(key="05e67f85-1f9c-49f8-82b6-5f2860f775ec", password="PN4QDITBF3")
    
    # Asign our deviced to connecting to WolkAbout platform
    wolk_device = wolk.WolkConnect(device)

    # Start program
    print("Connecting to WolkAbout IoT Platform")
    
    # Connect to device to WolkAbout Platform, in case that is not possible, exit system
    try:
        wolk_device.connect()
    except RuntimeError as e:
        print(str(e))
        sys.exit(1)

    while True:
        try:
            _i2c.i2c_reading()
            
            # sensor need 20ms to measure values
            time.sleep(0.2)
            
            
            temp_var = _i2c.get_temp()
            humi_var = _i2c.get_humi() 
            
            temperature = C1 + C2*(temp_var / C3)
            humidity = 100.0 * humi_var / C3
            
            # upload measured values to WolkAbout platform        
            wolk_device.add_sensor_reading("Temperature", temperature)
            wolk_device.add_sensor_reading("Humidity", humidity)
            wolk_device.publish()
            
            # show measured values
            print ("Temperature in Celsius: %.2f C  " %temperature)
            print ("Humidity: %.2f %% " % humidity)


            
            # wait until next cycle of measurement
            time.sleep(MEASUREMENT_PERIOD)
         
        # if program execution is interrupted, then disconnect device from WolkAbout platform 
        except KeyboardInterrupt:
            print("Received KeyboardInterrupt, quitting")
            wolk_device.disconnect()
            sys.exit()


if __name__ == "__main__":
    main()