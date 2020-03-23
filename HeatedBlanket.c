#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050



//declare gloabl variables

volatile float desiredTemp;
volatile float currTemp;

int lookUpTable[10] =
{ 0x3F, 0x6, 0x5B, 0x4F, 0x66,
0x6D, 0x7D, 0x7, 0x7F, 0x67 };

void increaseTemp() {
	currTemp += 1;
}


//functions

void setDesiredTemp(unsigned int *BTN_ptr) {
	// value to determine if desired temp should be changes
	bool setTemp = false;
	// variables to hold each number 
	int decimal = 0, ones = 0, tens = 0;

	// check if B0 is pressed
	if (BTN_ptr &= 0x01) {
		setTemp = true;
	}

	// loop while setTemp is true
	while (setTemp) {

		// check if B3 is pressed
		if (BTN_ptr & 0x04) {
			// if current value is 3, reset to 0 (max temperature is 30.0)
			if (tens == 3) {
				tens = 0;
			}
			// set tens display to value of tens
			*HEX_ptr |= lookUpTable[tens++] << 24;
		}
		// check if B1 is pressed
		else if (BTN_ptr & 0x02) {
			// if current value is 9, reset to 0
			if (tens == 3 || decimal == 9) {
				decimal = 0;
			}
			// set decimal display to value of decimal
			*HEX_ptr |= lookUpTable[decimal++];
		}
		// check if B2 is pressed
		else if (BTN_ptr & 0x03) {
			// if current value is 9, reset to 0
			if (tens == 3 || ones == 9) {
				ones = 0;
			}
			// set ones display to value of ones
			*HEX_ptr |= lookUpTable[ones++] << 16;
		}

		// check if B0 is pressed
		else if (BTN_ptr & 0x01) {
			// set boolean to false to exit the function and save desired temperature
			setTemp = false;
		}
	}

	// set desiredTemp to current values
	desiredTemp = (tens * 10.0) + ones + (decimal / 10.0);
}

void stimulateHeat(unsigned int *LED_ptr, unsigned int *JP1_ptr) {
	unsigned int LED_value = 0x0;

	//turn on LED
	if (currTemp <= desiredTemp - 2.0) {
		LED_value = 0x1;
		increaseTemp();
	}
	*LED_ptr = LED_value;
	*JP1_ptr = LED_value;
}

//functions

void decreaseTemp() {
	currTemp -= 1;
}

unsigned int linear_search(unsigned int *pointer, unsigned int n, unsigned int find)
{
	for (int c = 0; c < n; c++)
	{
		if (*(pointer + c) == find)
			return c;
	}

	return -1;
}


float HexToDecimal(unsigned int* HEX_ptr) {

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
	//declare and initialize pointers to the addresses

	volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE;//Address of LED
	volatile unsigned int * JP1_ptr = (unsigned int *)JP1_BASE;//Address of GPIO
	volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
	volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;

	//set bit as output, bit = 1, 0001
	*(JP1_ptr + 1) |= 0x1;

	//turn off LEDs and GPIO ports to start
	unsigned int LED_value = 0x0;
	*LED_ptr = LED_value;
	*JP1_ptr = LED_value;


	*HEX_ptr = lookUpTable[0];
	*HEX_ptr |= lookUpTable[0] << 16;
	*HEX_ptr |= lookUpTable[0] << 24;




}
