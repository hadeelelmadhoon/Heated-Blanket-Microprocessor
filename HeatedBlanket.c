#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050



//declare gloabl variables

volatile float desiredTemp;
volatile float currTemp;


void increaseTemp() {
	currTemp += 1;
}

void stimulateHeat(unsigned int *LED_ptr){
    unsigned int LED_value = 0x0;
    
    //turn on LED
    if(curr <= desiredTemp - 2.0){
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


float HexToDecimal() {

	unsigned int decimalValue = *HEX_ptr & 0xff;
	unsigned int unitsValue = *HEX_ptr & 0xff0000;
	unsigned int tensValue = *HEX_ptr & 0xff000000;

}

int main(){
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

	int lookUpTable[10] =
	{ 0x3F, 0x6, 0x5B, 0x4F, 0x66,
	0x6D, 0x7D, 0x7, 0x7F, 0x67 };


	*HEX_ptr = lookUpTable[0];
	*HEX_ptr |= lookUpTable[0] << 16;
	*HEX_ptr |= lookUpTable[0] << 24;



    
}
