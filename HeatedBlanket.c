#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define BTN_BASE 0xFF200050
#define TIMER_BASE 0xFF202000


//declare global variables

volatile float desiredTemp;
volatile float currTemp;

int lookUpTable[10] =
{ 0x3F, 0x6, 0x5B, 0x4F, 0x66,
0x6D, 0x7D, 0x7, 0x7F, 0x67 };


//functions

void displayTemp(volatile unsigned int *HEX5_ptr, volatile unsigned int *HEX_ptr, float currTemp, float desiredTemp) {
	unsigned int decimalValue_current = (currTemp - ((unsigned int)currTemp)*1.0) * 10 + 0.1;
	unsigned int unitsValue_current = (unsigned int)currTemp % 10;
	unsigned int tensValue_current = ((unsigned int)currTemp - unitsValue_current) / 10;

	unsigned int decimalValue_desired = (desiredTemp - (unsigned int)desiredTemp)*10.0 + 0.1;
	unsigned int unitsValue_desired = (unsigned int)desiredTemp % 10;
	unsigned int tensValue_desired = ((unsigned int)desiredTemp - unitsValue_desired) / 10;

	*HEX_ptr = lookUpTable[decimalValue_current];
	*HEX_ptr |= lookUpTable[unitsValue_current] << 8;
	*HEX_ptr |= lookUpTable[tensValue_current] << 16;
	*HEX_ptr |= lookUpTable[decimalValue_desired] << 24;
	*HEX5_ptr = lookUpTable[unitsValue_desired];
	*HEX5_ptr |= lookUpTable[tensValue_desired] << 8;

}


void setDesiredTemp(volatile unsigned int*HEX5_ptr, volatile unsigned int *SW_ptr, volatile unsigned int *BTN_ptr, volatile unsigned int *HEX_ptr) {
	// value to determine if desired temp should be changes
	int setTemp = 0;

	// variables to hold each number 
	int decimal = 0, ones = 0, tens = 1;

	// check if B0 is pressed
	if (*SW_ptr & 0x2) {
		// enter state to edit desired temp
		setTemp = 1;
		// set display to be 00 0
		*HEX_ptr = lookUpTable[0];
		*HEX_ptr |= lookUpTable[0] << 16;
		*HEX_ptr |= lookUpTable[1] << 24;
		*HEX5_ptr = 0x0;

	}

	// loop while setTemp is 1
	while (setTemp) {


		if (*BTN_ptr & 0x04) {
			// if current value is 3, reset to 0 (max temperature is 30.0)
			tens++;
			if (tens == 4) {
				tens = 1;
				ones = 0;
				decimal = 0;
				*HEX_ptr = 0x3F3F003F;
			}

			// set tens display to value of tens
			*HEX_ptr = (lookUpTable[tens] << 24) | (*HEX_ptr & 0xffffff);
			if (tens == 3) {
				ones = 0;
				decimal = 0;
				*HEX_ptr = 0x4F3F003F;

			}
			// wait until button turns off
			while (*BTN_ptr & 0x04);
		}


		// check if B2 is pressed
		if (*BTN_ptr & 0x02) {
			// if current value is 9, reset to 0
			ones++;
			if (tens == 3 || ones == 10) {
				ones = 0;


			}
			// set ones display to value of ones
			*HEX_ptr = (lookUpTable[ones] << 16) | (*HEX_ptr & 0xff0000ff);

			// wait until button turns off
			while (*BTN_ptr & 0x02);
		}
		if (*BTN_ptr & 0x01) {
			decimal++;
			// if current value is 9, reset to 0
			if (tens == 3 || decimal == 10) {
				decimal = 0;
			}
			// set decimal display to value of decimal
			*HEX_ptr = lookUpTable[decimal] | (*HEX_ptr & 0xffffff00);

			// wait until button turns off
			while (*BTN_ptr & 0x01);
		}




		// check if B0 is pressed
		if ((*SW_ptr & 0x02) == 0) {
			// set boolean to false to exit the function and save desired temperature
			setTemp = 0;
			// set desiredTemp to current values
			desiredTemp = (tens * 10.0) + ones + (decimal / 10.0);
		}
	}


}




int main() {

	// declare and initialize pointers to the addresses
	volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE; // Address of LED
	volatile unsigned int * JP1_ptr = (unsigned int *)JP1_BASE; // Address of GPIO
	volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
	volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;
	volatile unsigned int * SW_ptr = (unsigned int *)SW_BASE;
	volatile unsigned int * HEX5_ptr = (unsigned int *)HEX5_HEX4_BASE;
	volatile unsigned int *timer_ptr = (unsigned int *)TIMER_BASE;

	int counter = 1000000;
	// set bit as output, bit = 1, 0001
	*(JP1_ptr + 1) |= 0x1;

	// turn off LEDs and GPIO ports to start
	unsigned int LED_value = 0x0;
	*LED_ptr = LED_value;
	*JP1_ptr = LED_value;

	// set display to be 00 0
	*HEX_ptr = lookUpTable[0];
	*HEX_ptr |= lookUpTable[0] << 8;
	*HEX_ptr |= lookUpTable[0] << 16;

	*(timer_ptr + 1) = 0x2;
	*(timer_ptr + 2) = (counter & 0xFFFF);
	*(timer_ptr + 3) = (counter >> 16) & 0xFFFF;
	*(timer_ptr + 1) |= 0x4;

	int ms = 0;

	desiredTemp = 25.9;
	currTemp = 25;

	while (1) {
		setDesiredTemp(HEX5_ptr, SW_ptr, BTN_ptr, HEX_ptr);

		while (*timer_ptr & 0x1 == 0);
		*timer_ptr &= ~1;
		ms += 1;
		if (ms == 100000) {
			ms = 0;
			if (currTemp <= (desiredTemp - 0.5)) {
				*LED_ptr = 0x1;
				*JP1_ptr = 0x1;
				currTemp += 0.1;

			}
			else {
				currTemp -= 1;
				*LED_ptr = 0x0;
				*JP1_ptr = 0x0;

			}
			displayTemp(HEX5_ptr, HEX_ptr, currTemp, desiredTemp);


		}

	}

}
