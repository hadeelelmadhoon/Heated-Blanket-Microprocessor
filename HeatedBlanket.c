#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020

//declare and initialize pointers to the addresses
volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE;//Address of LED

//functions

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

