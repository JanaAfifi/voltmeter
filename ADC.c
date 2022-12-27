#include "xc.h"
#include "ADC.h"


uint16_t do_ADC(void)
{
    uint16_t ADCvalue = 0; // to hold output of ADC
    
    //ADC INIT
    AD1CON1bits.ADON = 1;
    AD1CON1bits.ADSIDL = 0; //continue operation in idle mode
    AD1CON1bits.FORM = 00; //output as unsigned integer
    AD1CON1bits.SSRC = 0b111; //for SW triggered AD conversion
    AD1CON1bits.ASAM = 0; //sampling begins when SAMP bit is set
    
    AD1CON2bits.VCFG = 0b000; // Selects AVDD, AVSS (supply voltage) as Vref
    AD1CON2bits.CSCNA = 0; //do not scan inputs
    AD1CON2bits.BUFM = 0; //Buffer is one 16-word buffer
    AD1CON2bits.ALTS = 0; //Always use MUXA input multiplexer settings
    
    AD1CON3bits.ADRC = 0; //use system clock
    AD1CON3bits.SAMC = 0b1000; //setting sample time
    
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 0b0101; //CH0 positive input is AN5
    
    AD1PCFGbits.PCFG5 = 0;
    
    //START ADC SAMPLING
    
    AD1CON1bits.SAMP = 1; //start sampling
    while(AD1CON1bits.DONE == 0)
        {}
    
    ADCvalue = ADC1BUF0;
    AD1CON1bits.SAMP = 1;
    AD1CON1bits.ADON = 0; //turn off adc
    return (ADCvalue);
    
    
    
}