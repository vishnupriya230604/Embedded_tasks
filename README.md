# Embedded_tasks

## Overview
This repository contains various embedded system tasks covering microcontroller programming, RTOS-based development, Linux kernel driver development, and custom embedded Linux image creation using Yocto.

## Task Descriptions

### Task 1: Data Acquisition and LCD Display

#### Overview
Reads values from LM35 (temperature), MPX4115 (pressure), and a potentiometer, displaying data on a 16x2 LCD via an ATmega32p microcontroller. The ADC processes inputs and updates the LCD every 500ms.

#### Components
- **Microcontroller**: AVR (e.g., ATmega32)
- **Sensors**: LM35, MPX4115, Potentiometer
- **Display**: 16x2 LCD
- **Power**: 5V supply

#### Functionality
- **ADC Initialization**: Configured to read analog inputs.
- **Sensor Readings**: Converts ADC values into meaningful data.
- **LCD Display**: Updates every 500ms.

---

### Task 2: FreeRTOS in STM32

#### Overview
This task involves working with **FreeRTOS** on STM32 microcontrollers, focusing on task creation, synchronization mechanisms like semaphores, queues, and mutexes.

#### Key Features
- **Task Creation**: Blinking an LED using separate FreeRTOS tasks.
- **Semaphores**: Synchronizing tasks.
- **Queues**: Inter-task communication.
- **Mutexes**: Preventing resource conflicts.

#### Expected Outcome
- Understanding RTOS task scheduling and synchronization in an embedded system.

---

### Task 3: Linux Character Driver for BMP280 Temperature Sensor

#### Overview
This project implements a Linux kernel module for interacting with a BMP280 sensor over I2C, along with a user-space application to read temperature data.

#### Features
- **I2C Communication**: Uses I2C protocol to interface with the BMP280 sensor.
- **Character Device Driver**: Exposes `/dev/bmp280` for user-space access.
- **User-Space Application**: Reads temperature values and displays them.

#### Hardware Requirements
- Raspberry Pi (or any Linux system with I2C support)
- BMP280 sensor connected via I2C (default address: `0x76`)

#### Software Requirements
- Linux with I2C support enabled
- `gcc` for compiling the user-space program
- `make` for building the kernel module

#### Steps
1. **Enable I2C on Raspberry Pi**
    ```bash
    sudo raspi-config
    ```
    Navigate to Interfacing Options → I2C → Enable.

2. **Build the Kernel Module and User-Space Program**
    ```bash
    make
    ```

3. **Insert the Kernel Module**
    ```bash
    sudo insmod bmp280_driver.ko
    ```

4. **Verify Device File Creation**
    ```bash
    ls /dev/bmp280
    ```

5. **Run the User-Space Program**
    ```bash
    sudo ./bmp280_read
    ```

6. **Remove the Kernel Module (Optional)**
    ```bash
    sudo rmmod bmp280_driver
    ```

---

### Task 4: Yocto Custom Image Creation

#### Overview
This task focuses on building a custom Linux image using **Yocto** for embedded systems. The goal is to create a minimal, optimized image tailored for specific hardware.

#### Steps
1. **Set Up Yocto Environment**
    ```bash
    git clone git://git.yoctoproject.org/poky.git
    cd poky
    git checkout -b dunfell origin/dunfell
    ```

2. **Initialize Build Environment**
    ```bash
    source oe-init-build-env
    ```

3. **Customize `local.conf`**
    - Set `MACHINE` to the target hardware.
    - Configure package selection.

4. **Build the Image**
    ```bash
    bitbake core-image-minimal
    ```

5. **Deploy to Target Hardware**
    - Flash the generated image to an SD card or eMMC.
    - Boot the custom image on the target board.

---


## License
This repository is GNU general public licence.

