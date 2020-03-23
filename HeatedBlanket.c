#define LED_BASE 0xFF200000
#define SW_BASE 0xFF200040
#define JP1_BASE 0xFF200060
#define ADC_BASE 0xFF204000
#define HEX3_HEX0_BASE 0xFF200020
#define BTN_BASE 0xFF200050

volatile float desiredTemp;
volatile float currTemp;

volatile unsigned int * HEX_ptr = (unsigned int *)HEX3_HEX0_BASE;
volatile unsigned int * BTN_ptr = (unsigned int *)BTN_BASE;

int lookUpTable[10] = 
	{0x3F, 0x6, 0x5B, 0x4F, 0x66, 
	0x6D, 0x7D, 0x7, 0x7F, 0x67};
	
float HexToDecimal(){
	
	
	
	
}

