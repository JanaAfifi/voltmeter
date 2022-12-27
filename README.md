In this project we designed a voltmeter. The voltage will be set using buttons. These
buttons are connected to the microcontroller. Then the voltage will be shown on the
terminal. There are three buttons that are used in this project.
At first the system is in sleep mode. When we press button three it wakes the system up
and then it displays the current voltage as well as the threshold. The lowest value is zero
volts and the highest voltage is 3.250V and the voltage threshold can be manually set to
a desired value.
Moreover, we have 2 buttons to change the threshold. The first button is used to change
the threshold by incrementing it. The second button is used to decrement the threshold.
The third button puts the system to sleep or wakes the system up if it was already in
sleep mode. Additionally, we are using a watchdog timer in this project as a power
saving feature. Also, we will have an LED which lights up when the measured voltage
exceeds the threshold voltage.

Our voltmeter uses the microcontroller's ADC to measure voltage on the input pin. The
output is the voltage and the voltage is displayed on the UART terminal. When you turn
the potentiometer it changes the value of the voltage in the terminal. When voltage is
below the set threshold the LED is Off. However, when the voltage is above the set
threshold, the LED is turned On. The first button in the circuit is used to increment the
threshold by 0.100V and the second button is used to decrement the threshold by
0.100V. Furthermore, we can manually put the system to sleep by pressing the third
button.That button can also be used to wake up the system so it is no longer in sleep
mode. When the measured voltage or the set threshold voltage does not change for a
period of time (8 seconds) the system will go into sleep mode.

Authors: Jana Afifi, Myles, Ethan
