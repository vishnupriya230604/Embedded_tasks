##Overview:##
This project reads values from three sensors�LM35 (temperature), MPX4115 (pressure), and a potentiometer�and displays the data on a 16x2 LCD. The ATmega32p reads analog signals from the sensors using its ADC, processes the data, and updates the LCD every 500ms.

##Components:##
Microcontroller: AVR (e.g., ATmega32)
Sensors: LM35 (Temperature), MPX4115 (Pressure), Potentiometer
Display: 16x2 LCD
Others: Resistors (optional), Power supply

##Pin Connections:##
LM35: ADC0 (PA0), VCC to +5V, GND to ground.
MPX4115: ADC1 (PA1), VCC to +5V, GND to ground.
Potentiometer: ADC2 (PA2), VCC to +5V, GND to ground.
LCD: RS to PD0, E to PD1, Data pins to PD2-PD7.

##Functionality:##
ADC Initialization: Configures ADC to read analog inputs from sensors.
Sensor Readings: Converts ADC values from LM35 (�C), MPX4115 (kPa), and potentiometer (V) into physical values.
LCD Display: Updates the LCD with the latest readings every 500ms.

##Code Workflow:##
ADC_Init(): Initializes ADC with AVCC as the reference voltage.
ADC_Read(channel): Reads analog input from the specified ADC channel.
LCD_Init() and LCD_Char(): Initializes and updates the LCD display.
Read_Temperature(), Read_Pressure(), Read_Potentiometer(): Read sensor data and display it on the LCD.
Main Program: Continuously reads sensor data and updates the LCD.

