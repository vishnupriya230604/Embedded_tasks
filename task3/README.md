# BMP280 I2C Driver for Raspberry Pi

This project implements a kernel module and a user-space program for reading temperature data from a BMP280 sensor connected via I2C on a Raspberry Pi.

## Overview

The kernel module (`bmp280_driver.c`) interacts with the BMP280 sensor over I2C to read temperature data. The user-space program (`bmp280_read.c`) communicates with the driver through a device file (`/dev/bmp280`) to read and display the current temperature.

## Features

- **I2C Communication**: The driver communicates with the BMP280 sensor using the I2C protocol.
- **Temperature Reading**: Reads the current temperature from the BMP280 sensor and converts it to degrees Celsius.
- **User-Space Interaction**: A simple C program that interacts with the kernel driver to fetch temperature data.

## Hardware Requirements

- Raspberry Pi (with I2C enabled)
- BMP280 sensor connected via I2C (using the default address 0x76)

## Software Requirements

- Raspberry Pi OS (or any Linux-based OS with I2C support)
- `gcc` for compiling the user-space program
- `make` for building the kernel module and user-space program

## Directory Structure

```plaintext
bmp280-driver/
├── Makefile                # Makefile to build kernel module and user-space program
├── bmp280_driver.c         # Kernel module source code
├── bmp280_read.c           # User-space program to interact with BMP280 sensor
└── README.md               # Documentation for the project
```

## Setup and Installation

### 1. Clone the Repository

Clone this repository to your Raspberry Pi or any Linux system with I2C support.

```bash
git clone https://github.com/yourusername/bmp280-driver.git
cd bmp280-driver
```

### 2. Enable I2C on Raspberry Pi

If you haven't already, enable I2C on your Raspberry Pi:

```bash
sudo raspi-config
```

Navigate to Interfacing Options → I2C → Enable.

### 3. Build the Kernel Module and User-Space Program

You can build both the kernel module and the user-space program using the provided Makefile.

```bash
make
```

### 4. Insert the Kernel Module

Insert the kernel module into the system:

```bash
sudo insmod bmp280_driver.ko
```

This will load the driver and create the device file `/dev/bmp280`.

### 5. Verify Device File Creation

Check if the device file `/dev/bmp280` has been created:

```bash
ls /dev/bmp280
```

If the device file exists, you're ready to interact with the BMP280 sensor.

### 6. Run the User-Space Program

Now you can run the user-space program to read temperature data from the BMP280 sensor:

```bash
sudo ./bmp280_read
```

The program will print the current temperature in Celsius to the terminal, e.g.,

```plaintext
Current Temperature: 25.50
```

### 7. Remove the Kernel Module (Optional)

Once you're done, you can remove the kernel module:

```bash
sudo rmmod bmp280_driver
```

### 8. Clean Up (Optional)

To clean up generated files (e.g., compiled object files and binaries):

```bash
make clean
```

## Troubleshooting

- **I2C Bus Issues**: If you cannot communicate with the BMP280 sensor, ensure I2C is enabled, and the sensor is connected to the correct bus and address (0x76).

    Use the following command to check for connected devices:

    ```bash
    sudo i2cdetect -y 1
    ```

- **Permission Issues**: If you face permission issues while accessing `/dev/bmp280`, try running the program with `sudo` or change the permissions of the device file.

- **Kernel Module Not Loading**: If the module fails to load, check the kernel logs for any error messages:

    ```bash
    dmesg | tail -n 20
    ```

## License

