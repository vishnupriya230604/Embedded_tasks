#include<stdio.h>
int main() {
    int adc_value = 10; // Read from ADC0
    float temperature = (adc_value * 5.0* 100.0) / 1024.0; // Convert to Celsius
    int temperature_int = (int)(temperature + 0.5); // Round to nearest integer

    printf("Temp: %d C", temperature_int); // Display integer temperature
}
