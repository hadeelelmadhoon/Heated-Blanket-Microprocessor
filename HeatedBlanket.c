#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050

volatile float desiredTemp;
volatile float currTemp;

void decreaseTemp(unsigned int temp) {
	temp -= 1;
}

void increaseTemp(unsigned int temp) {
	temp += 1;
}
//declare and initialize pointers to the addresses
volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE;//Address of LED
volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;

//turn off all LEDs to start
*LED_ptr = 0x0;

int lookUpTable[10] = 
	{0x3F, 0x6, 0x5B, 0x4F, 0x66, 
	0x6D, 0x7D, 0x7, 0x7F, 0x67};
	




*HEX_ptr = lookUpTable[0];
*HEX_ptr |= lookUpTable[0] << 16;
*HEX_ptr |= lookUpTable[0] << 24;

//functions

float HexToDecimal(){

	unsigned int decimalValue = *HEX_ptr & 0xff;
	unsigned int unitsValue = *HEX_ptr & 0xff0000;
	unsigned int tensValue = *HEX_ptr & 0xff000000;

}

void stimulateHeat(){
    float currentTemp = hexToDecimal();
    
    //turn on LED
    if(currentTemp <= desiredTemp - 2.0){
        *LED_ptr = 0x1;
        increaseTemp();
    }
    //turn off LED
    else{
        *LED_ptr = 0x0;
    }
}

int main(){
    
}
