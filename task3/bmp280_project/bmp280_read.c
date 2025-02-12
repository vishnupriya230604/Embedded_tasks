/**
 * @file bmp280_read.c
 * @brief This file contains the implementation for reading data from the BMP280 sensor.
 * 
 * The BMP280 is a barometric pressure sensor that can be used for measuring atmospheric pressure and temperature.
 * This file includes functions to initialize the sensor, read raw data, and convert it to human-readable values.
 * 
 * @note Ensure that the sensor is properly connected and configured before attempting to read data.
 * 
 * @date 2023
 * @version 1.0
 */


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

#define DEVICE_PATH "/dev/bmp280"

/**
 * @brief Main function to read temperature data from the BMP280 sensor
 * 
 * This function opens the device file, reads the temperature data, and prints it to the console.
 * 
 * @return 0 on success, -1 on failure
 */
int main() {
    int fd, ret;
    char buffer[20];  // Buffer to store temperature data

    // Open the device file
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open device file");
        return -1;
    }

    // Read the temperature data from the device
    ret = read(fd, buffer, sizeof(buffer) - 1);
    if (ret == -1) {
        perror("Failed to read from device");
        close(fd);
        return -1;
    }

    // Null-terminate the read data
    buffer[ret] = '\0';

    // Print the temperature read from the sensor
    printf("Current Temperature: %s\n", buffer);

    // Close the device file
    close(fd);
    return 0;
}
