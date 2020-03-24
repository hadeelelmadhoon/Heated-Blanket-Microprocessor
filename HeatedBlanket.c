#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050



//declare global variables

volatile float desiredTemp;
volatile float currTemp;

int lookUpTable[10] =
{ 0x3F, 0x6, 0x5B, 0x4F, 0x66,
0x6D, 0x7D, 0x7, 0x7F, 0x67 };


//functions

void displayHex(volatile unsigned int *HEX_ptr, float currTemp) {
	unsigned int decimalValue = (currTemp - ((unsigned int)currTemp)*1.0) * 10;
	unsigned int unitsValue = (unsigned int)currTemp % 10;
	unsigned int tensValue = ((unsigned int)currTemp - unitsValue) / 10;

	*HEX_ptr = lookUpTable[decimalValue];
	*HEX_ptr |= lookUpTable[unitsValue] << 16;
	*HEX_ptr |= lookUpTable[tensValue] << 24;


}

void decreaseTemp(volatile unsigned int *HEX_ptr) {
	currTemp -= 1;
	displayHex(HEX_ptr, currTemp);
}

void increaseTemp(volatile unsigned int *HEX_ptr) {
	currTemp += 2;
	displayHex(HEX_ptr, currTemp);

}

void setDesiredTemp(volatile unsigned int *SW_ptr, volatile unsigned int *BTN_ptr, volatile unsigned int *HEX_ptr) {
	// value to determine if desired temp should be changes
	int setTemp = 0;

	// variables to hold each number 
	int decimal = 0, ones = 0, tens = 0;

	// check if B0 is pressed
	if (*SW_ptr & 0x2) {
		// enter state to edit desired temp
		setTemp = 1;

	}

	// loop while setTemp is 1
	while (setTemp) {


		if (*BTN_ptr & 0x04) {
			// if current value is 3, reset to 0 (max temperature is 30.0)
			if (tens == 4) {
				tens = 0;
				ones = 0;
				decimal = 0;
				*HEX_ptr = 0x3F3F003F;
			}
			// set tens display to value of tens
			*HEX_ptr = (lookUpTable[tens++] << 24) | (*HEX_ptr & 0xffffff);
			if (tens == 4) {
				*HEX_ptr = 0x4F3F003F;

			}
			// wait until button turns off
			while (*BTN_ptr & 0x04);
		}


		// check if B2 is pressed
		if (*BTN_ptr & 0x02) {
			// if current value is 9, reset to 0
			if (tens == 4 || ones == 10) {
				ones = 0;
			}
			// set ones display to value of ones
			*HEX_ptr = (lookUpTable[ones++] << 16) | (*HEX_ptr & 0xff0000ff);

			// wait until button turns off
			while (*BTN_ptr & 0x02);
		}
		if (*BTN_ptr & 0x01) {

			// if current value is 9, reset to 0
			if (tens == 4 || decimal == 10) {
				decimal = 0;
			}
			// set decimal display to value of decimal
			*HEX_ptr = lookUpTable[decimal++] | (*HEX_ptr & 0xffffff00);

			// wait until button turns off
			while (*BTN_ptr & 0x01);
		}




		// check if B0 is pressed
		if ((*SW_ptr & 0x02) == 0) {
			// set boolean to false to exit the function and save desired temperature
			setTemp = 0;

		}
	}

	// set desiredTemp to current values
	desiredTemp = (tens * 10.0) + ones + (decimal / 10.0);
}

void stimulateHeat(volatile unsigned int *LED_ptr, volatile unsigned int *JP1_ptr, volatile unsigned int*HEX_ptr) {
	unsigned int LED_value = 0x0;

	//turn on LED
	if (currTemp <= (desiredTemp - 2.0)) {
		LED_value = 0x1;
		increaseTemp(HEX_ptr);
	}
	*LED_ptr = LED_value;
	*JP1_ptr = LED_value;
}

unsigned int linear_search(int *pointer, unsigned int n, unsigned int find)
{
	for (int c = 0; c < n; c++)
	{
		if (*(pointer + c) == find)
			return c;
	}

	return -1;
}

float HexToDecimal(volatile unsigned int* HEX_ptr) {

	unsigned int decimalValue = *HEX_ptr & 0xff;
	unsigned int unitsValue = *HEX_ptr & 0xff0000;
	unsigned int tensValue = *HEX_ptr & 0xff000000;

	decimalValue = linear_search(lookUpTable, 10, decimalValue);
	unitsValue = linear_search(lookUpTable, 10, unitsValue);
	tensValue = linear_search(lookUpTable, 10, tensValue);

	//Conversion to float
	return tensValue * 10.0 + unitsValue * 1.0 + (decimalValue / 10.0);

}

int main() {

	// declare and initialize pointers to the addresses
	volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE; // Address of LED
	volatile unsigned int * JP1_ptr = (unsigned int *)JP1_BASE; // Address of GPIO
	volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
	volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;
	volatile unsigned int * SW_ptr = (unsigned int *)SW_BASE;


	// set bit as output, bit = 1, 0001
	*(JP1_ptr + 1) |= 0x1;

	// turn off LEDs and GPIO ports to start
	unsigned int LED_value = 0x0;
	*LED_ptr = LED_value;
	*JP1_ptr = LED_value;

	// set display to be 00 0
	*HEX_ptr = lookUpTable[0];
	*HEX_ptr |= lookUpTable[0] << 16;
	*HEX_ptr |= lookUpTable[0] << 24;


	while (1) {
		if (*SW_ptr & 0x1) {
			setDesiredTemp(SW_ptr, BTN_ptr, HEX_ptr);
			//stimulateHeat(LED_ptr, JP1_ptr,HEX_ptr);
			//decreaseTemp(HEX_ptr);
		}
		else {

		}
	}

}
