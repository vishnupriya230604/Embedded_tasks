/**
 * @file main.c
 * @brief AVR-based ADC readings displayed on an LCD
 *
 * This program reads temperature from an LM35 sensor, pressure from an MPX4115 sensor,
 * and voltage from a potentiometer using an AVR microcontroller and displays the values on an LCD.
 *
 * @author Vishnupriya
 * @date 2025
 */

 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <stdio.h>
 
 #define LCD_PORT PORTD  /**< LCD connected to PORTD */
 #define LCD_DDR DDRD    /**< Data Direction Register for LCD */
 #define RS PD0          /**< Register Select pin */
 #define E PD1           /**< Enable pin */
 #define VREF 5          /**< Reference voltage for ADC */
 
 /**
  * @brief Initialize the ADC
  *
  * Configures the ADC with AVCC as reference voltage and a prescaler of 64.
  */
 void ADC_Init() {
     ADMUX = (1 << REFS0); // AVCC as reference voltage
     ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, prescaler = 64
 }
 
 /**
  * @brief Read ADC value from a given channel
  * @param channel ADC channel (0-7)
  * @return 10-bit ADC conversion result
  */
 uint16_t ADC_Read(uint8_t channel) {
     channel &= 0x07; // Ensure channel is within range (0-7)
     ADMUX = (ADMUX & 0xF8) | channel; // Select ADC channel
     ADCSRA |= (1 << ADSC); // Start conversion
     while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
     return ADC; // Return ADC value
 }
 
 /**
  * @brief Send a command to the LCD
  * @param cmd Command byte to send
  */
 void LCD_Command(unsigned char cmd) {
     LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0); // Higher nibble
     LCD_PORT &= ~(1 << RS); // RS = 0 for command
     LCD_PORT |= (1 << E); // Enable pulse
     _delay_ms(1);
     LCD_PORT &= ~(1 << E);
 
     LCD_PORT = (LCD_PORT & 0x0F) | (cmd << 4); // Lower nibble
     LCD_PORT |= (1 << E);
     _delay_ms(1);
     LCD_PORT &= ~(1 << E);
 }
 
 /**
  * @brief Display a character on the LCD
  * @param data Character to display
  */
 void LCD_Char(unsigned char data) {
     LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0); // Higher nibble
     LCD_PORT |= (1 << RS); // RS = 1 for data
     LCD_PORT |= (1 << E);
     _delay_ms(1);
     LCD_PORT &= ~(1 << E);
 
     LCD_PORT = (LCD_PORT & 0x0F) | (data << 4); // Lower nibble
     LCD_PORT |= (1 << E);
     _delay_ms(1);
     LCD_PORT &= ~(1 << E);
 }
 
 /**
  * @brief Initialize the LCD
  */
 void LCD_Init() {
     LCD_DDR = 0xFF; // Configure LCD port as output
     _delay_ms(50);
     LCD_Command(0x02); // Initialize in 4-bit mode
     LCD_Command(0x28); // 2 lines, 5x7 matrix
     LCD_Command(0x0C); // Display on, cursor off
     LCD_Command(0x06); // Increment cursor
     LCD_Command(0x01); // Clear display
 }
 
 /**
  * @brief Display a string on the LCD
  * @param str Pointer to string
  */
 void LCD_String(char *str) {
     while (*str) {
         LCD_Char(*str++);
     }
 }
 
 /**
  * @brief Set the cursor position on the LCD
  * @param row Row position (0 or 1)
  * @param column Column position (0-15)
  */
 void LCD_SetCursor(unsigned char row, unsigned char column) {
     unsigned char pos[] = {0x80, 0xC0}; // Row 0 and 1 positions
     LCD_Command(pos[row] + column);
 }
 
 /**
  * @brief Read and display temperature from LM35 sensor
  */
 void Read_Temperature() {
     uint16_t adc_value = ADC_Read(0); // Read from ADC0
     float temperature = (adc_value * VREF * 100.0) / 1024.0;
     int temperature_int = (int)(temperature + 0.5);
     char buffer[16];
     snprintf(buffer, 16, "Temp: %d C", temperature_int);
     LCD_SetCursor(0, 0);
     LCD_String(buffer);
 }
 
 /**
  * @brief Read and display pressure from MPX4115 sensor
  */
 void Read_Pressure() {
     uint16_t adc_value = ADC_Read(1); // Read from ADC1
     float voltage = (adc_value * VREF) / 1024.0;
     float pressure = ((voltage - 0.2) / 4.6) * 100.0 + 15.0;
     int pressure_int = (int)(pressure + 0.5);
     char buffer[16];
     snprintf(buffer, 16, "Pre:%dkPa", pressure_int);
     LCD_SetCursor(1, 0);
     LCD_String(buffer);
 }
 
 /**
  * @brief Read and display potentiometer voltage
  */
 void Read_Potentiometer() {
     uint16_t adc_value = ADC_Read(2); // Read from ADC2
     float voltage = (adc_value * VREF) / 1024.0;
     int voltage_int = (int)(voltage * 100 + 0.5);
     char buffer[16];
     snprintf(buffer, 16, "Pot:%dV", voltage_int / 100);
     LCD_SetCursor(1, 8);
     LCD_String(buffer);
 }
 
 /**
  * @brief Main function
  *
  * Initializes the ADC and LCD, then continuously reads and displays sensor data.
  * @return int Exit status (never returns in embedded systems)
  */
 int main() {
     ADC_Init();
     LCD_Init();
 
     while (1) {
         Read_Temperature();
         Read_Pressure();
         Read_Potentiometer();
         _delay_ms(500);
     }
 }
 