//**************************************************
//***              LFP PID controller            ***
//**************************************************

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "mbed.h"
#include "sleep_api.h"

/* define the sample size */
#define     sample  100
#define     taps    50

/* pin output headers */
// https://developer.mbed.org/teams/Freescale/wiki/frdm-k64f-pinnames
AnalogIn        input(PTC10);               // analog input pin : PTC10
AnalogOut       dac0out(DAC0_OUT);          // DAC ouput pin
Serial          pc(USBTX, USBRX);           // tx, rx serial pins

uint16_t    input_sin;

LowPowerTimeout timeout;
bool expired = false;

void callback(void)
{
    expired = true;
}

int main(void)
{
    //  PID Gain Set Up
    //uint8_t       kp = 0;
    //uint8_t       ki = 0;
    //uint8_t       kd = 1;
    uint16_t        diff_buffer[50];    // buffer to hold the ADC input values
    int             output;

    //  Derivative Convolution Kernal Buffer - Sine Wave values * 100 to avoid the use of floats
    int16_t derivative_taps [taps]= {125,    249,   368,   482,   588,   685,   771,   844,   905,   951,   982,   998,   998,   982,   951,   905,   844,   771,   685,   588,   482,   368,   249,   125,   0,   -125,  -249,  -368,  -482,  -588,  -685,  -771,  -844,  -905,  -951,  -982,  -998,  -998,  -982,  -951,  -905, -844,  -771,  -685, -588,  -482,  -368,  -249,  -125,  0};

//DAC-setup
    uint32_t mask16=1<<16;                  // mask 16 bits
    uint32_t dat1=(PTC->PDOR) | mask16;     // set Port Data Output Register
    uint32_t dat0=(PTC->PDOR) & (!mask16);  // clear

    DAC0->C0 = 0; //reset state
    DAC0->C1 = 0;
    DAC0->C0 = DAC_C0_DACEN_MASK            // Enable
               | DAC_C0_DACSWTRG_MASK       // Software Trigger
               | DAC_C0_DACRFS_MASK;        // VDDA selected

    // main loop
    while (1) {
        input_sin = input.read_u16() ;
        // step1 : shift register
        for(uint8_t i = taps - 1 ; i > 0 ; i--) {
            diff_buffer[i] = diff_buffer[i-1];
        }
        // step2 : read analog data into buffer
        diff_buffer[0] = input_sin ;
        // step3 : integration
        for (uint8_t j= 0 ; j<taps ; j++) {
            output += ((diff_buffer[j]*derivative_taps[j])/1000);     
        }

        // 32 bits >> 16 bits output
        output = output * 0.03676;
        // normalise the uint upper and lower values. Negative values appear >= 32768
        if (output > 32768) output = output - 32768;
        if (output < 32768) output = output + 32768;
        dac0out.write_u16(output);

        // 32 bits >> 12 bits output
        //output = output * 0.00229755;
        //if (output > 2047) output = output - 2047;
        //if (output < 2047) output = output + 2047;
        //DAC0->DAT[0].DATL = (output & 0xFF);                  // set low half of register
        //DAC0->DAT[0].DATH = ((output >> 8) & 0x0F);           // set high half

        // sampling a 10Hz sine => 500 samples/s = 2ms
        wait_ms ( 2 );

        output = 0;
    }
}