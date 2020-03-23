#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050


//declare and initialize pointers to the addresses

volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE;//Address of LED
volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;

//declare gloabl variables

volatile float desiredTemp;
volatile float currTemp;

int lookUpTable[10] = 
	{0x3F, 0x6, 0x5B, 0x4F, 0x66, 
	0x6D, 0x7D, 0x7, 0x7F, 0x67};
	

//functions

void decreaseTemp() {
    currTemp -= 1;
}

void increaseTemp() {
    currTemp += 1;
}

float HexToDecimal(){
}

void stimulateHeat(){
    float currentTemp = hexToDecimal();
    unsigned int LED_value = 0x0;
    
    //turn on LED
    if(currentTemp <= desiredTemp - 2.0){
        LED_value = 0x1;
        *LED_ptr = LED_value;
        increaseTemp();
    }
    //turn off LED
    else{
        *LED_ptr = LED_value;
    }
}

int main(){
    //turn off all LEDs to start
    unsigned int LED_value = 0x0;
    *LED_ptr = LED_value;
    
}
