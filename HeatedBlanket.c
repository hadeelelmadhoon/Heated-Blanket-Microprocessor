#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050

volatile float desiredTemp;

//declare and initialize pointers to the addresses
volatile unsigned int * LED_ptr = (unsigned int *)LED_BASE;//Address of LED
volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;

//turn off all LEDs to start
*LED_ptr = 0x0;

int lookUpTable[10] = 
	{0x3F, 0x6, 0x5B, 0x4F, 0x66, 
	0x6D, 0x7D, 0x7, 0x7F, 0x67};

//functions

float HexToDecimal(){
	
}

void setDesiredTemp(){
	// value to determine if desired temp should be changes
	bool setTemp = false;
	// variables to hold each number 
	int decimal = 0, ones = 0, tens = 0;
	
	// check if B0 is pressed
	if(BTN_ptr &= 0x01){
		setTemp = true;
	}
	
	// loop while setTemp is true
	while(setTemp){
		
		// check if B3 is pressed
		if(BTN_ptr &= 0x04){
			// if current value is 3, reset to 0 (max temperature is 30.0)
			if(tens == 3){
				tens = 0;
			}
			// set tens display to value of tens
			*HEX_ptr |= lookUpTable[tens++] << 24;
		}
		// check if B1 is pressed
		else if (BTN_ptr &= 0x02){
			// if current value is 9, reset to 0
			if(tens == 3 || decimal == 9){
				decimal = 0;
			}
			// set decimal display to value of decimal
			*HEX_ptr |= lookUpTable[decimal++];
		}
		// check if B2 is pressed
		else if(BTN_ptr &= 0x03){
			// if current value is 9, reset to 0
			if(tens == 3 || ones == 9){
				ones = 0;
			}
			// set ones display to value of ones
			*HEX_ptr |= lookUpTable[ones++] << 16;
		}
		
		// check if B0 is pressed
		else if(BTN_ptr &= 0x01){
			// set boolean to false to exit the function and save desired temperature
			setTemp = false;
		}
	}
	
	// set desiredTemp to current values
	desiredTemp = (tens * 10.0) + ones + (decimal / 10.0);
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

