#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020

void decreaseTemp(unsigned int temp) {
	temp -= 1;
}

void increaseTemp(unsigned int temp) {
	temp += 1;
}