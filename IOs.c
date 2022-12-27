/*
 * File:   IOs.c
 * Author: Rushi V
 *
 * Created on September 30, 2020, 11:33 AM
 */


#include "xc.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"
#include <stdio.h>

//voltThresh: Threshold voltage, Light will turn 
//on if meassured voltage is higher
double voltThresh = 3.0;

//sysON: flag that determines if the system is in the on state
//it will run if it is in the power saving off mode it will 
//need to be turned back on before anything will happen
int sysOn = 1;

//firstTime: flag that is used just once at the start to make sure 
//button 3 was pressed to turn on the system befor we continue
//int firstTime = 1;

void goToSleep()
{
    RCONbits.SWDTEN = 0; //disable WDT
    Idle();
    RCONbits.SWDTEN = 1;
    
}


void volt2String(double currentVolt){
	char message[32];   // Buffer for storing min and sec 
	sprintf(message, "\rVoltage: %.3lf   Threshold: %.3lf ", currentVolt, voltThresh);
	Disp2String(message);   // Display message
	return;
}

void IOcheck(void)
{
	//Disable individual button CN interrupts
	IEC1bits.CNIE = 0;      
    
    
	int32_t oldVal = 0;
    double voltage = 0;
	
	//While the system is on we will poll voltage and respond
	while(sysOn == 1){
		//if only RA2 pb1 is pressed
		if((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)
		&& sysOn==1)
		{//button 1
			if(voltThresh<=3.10)//TODO: this might not be the actual max needs changed
				voltThresh+=.1; //increment the voltage threshold
			delay_ms(200,1);
            oldVal += 50;
		}
		//if only RA4 pb2 is pressed
		if((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)
		&& sysOn==1) 
		{//button 2
			if(voltThresh>=0.1)
				voltThresh-=.1; //decrement the voltage threshold
			delay_ms(200, 1);  
            oldVal += 50;
		}
		//if only RB4 pb3 is pressed
		if((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) 
		{//button 3

            delay_ms(200, 1);
            LATBbits.LATB8 = 0; //turn off LED
             
			sysOn ^= sysOn;//sets sysOn off if on and vice versa
		}
		
		if(sysOn == 0){
            XmitUART2('\r',1);
            XmitUART2(' ', 40);
            XmitUART2('\r',1);
            
            Disp2String("Sleep");
			delay_ms(100, 1);
            
            //IFS1bits.CNIF = 0;
			IEC1bits.CNIE = 1;      
			//goToSleep();
            //Idle();
            return;
			//IEC1bits.CNIE = 0;      
		}
        
        //check voltage
        uint16_t val = do_ADC();
        //Disp2String("\nLoop");
        //Disp2Dec(RCONbits.SWDTEN);
        if((val > oldVal + 2|| val < oldVal - 2) && sysOn)
        {
            //Disp2String("\nClear WDT");

            //clear line
            XmitUART2('\r',1);
            XmitUART2(' ', 40);
            XmitUART2('\r',1);
            
            oldVal = val;
            
            voltage = val * (3.25 / 1023 );
            
            volt2String(voltage);
            
            
            //if the voltage exceeds the threshold the light will turn on
            if(voltage >= voltThresh){
                LATBbits.LATB8 = 1;
            } else {
                LATBbits.LATB8 = 0;
            }
            //Disp2String(" TOO");
        }
        
        //Disp2String(" HERE");
        //Disp2Dec(PORTBbits.RB4);
        //ClrWdt();
        delay_ms(100, 1);
	}
	
	delay_ms(200,1);
	
	//re-enable the individual interrupts 
	IEC1bits.CNIE = 1;      
	
	return;
}

//// IOinit() with IO interrupts
void IOinit(void)
{
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  // enable CN on CN0
    
    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1;  // enable CN on CN1
    
    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1;  // enable CN on CN30
    
    //CN Interrupt settings
    IPC4bits.CNIP = 1; //7 is highest priority, 1 is lowest, 0 is disabled interrupt
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    
    return;
}

///// Change of pin Interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
	// clear IF flag
	IFS1bits.CNIF = 0;   
    
    //Disp2String("\nCN int ");
	//if(PORTBbits.RB4 == 1)
        //Disp2String("Button 3");
    
	//check that the first time we come here is because
	//button 3 is pressed to turn the system on after boot
//	if(firstTime == 1){
//		if(PORTBbits.RB4 == 1) return;
//		firstTime = 0;
//	}
	
	//run the iocheck which loop polls until off again
	
    if (PORTBbits.RB4 == 0){
		sysOn = 1;
		return;
	}
    if(sysOn == 1){
		IOcheck();
		delay_ms(200,1);
	}
    
	return;
}

