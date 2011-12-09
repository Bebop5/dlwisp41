/* See license.txt for license information. */

#ifndef NULL_SENSOR_H
#define NULL_SENSOR_H

#define SENSOR_DATA_TYPE_ID       0x0C

#define DATA_LENGTH_IN_WORDS      1
#define DATA_LENGTH_IN_BYTES      (DATA_LENGTH_IN_WORDS*2)

void init_sensor()
{
  return;
}

void read_sensor(unsigned char volatile *target) 
{
     
       // slow down clock
        BCSCTL1 = XT2OFF + RSEL1; // select internal resistor (still has effect when DCOR=1)
        DCOCTL = DCO1+DCO0; // set DCO step. 
            
        if(!is_power_good())
          sleep();
  
        P1OUT &= ~RX_EN_PIN;   // turn off comparator
        
        unsigned int k = 0;
        
#define USE_SENSOR_COUNTER      1
#if USE_SENSOR_COUNTER
        *(target + k + 1 ) = __swap_bytes(sensor_counter);
        *(target + k) = sensor_counter;
        if ( sensor_counter == 0xffff ) sensor_counter = 0; else sensor_counter++;
#else
        *(target + k + 1 ) = 0x03;
        // grab msb bits and store it
        *(target + k) = 0x02;
#endif
        
        return;
}

// used to sleep while taking adc, this wakes us
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  ADC10CTL0 &= ~ENC; // make sure this is off otherwise settings are locked.
  ADC10CTL0 &= ~(ADC10IFG | ADC10IE);
  LPM4_EXIT;
  return;
}

#endif // NULL_SENSOR_H
