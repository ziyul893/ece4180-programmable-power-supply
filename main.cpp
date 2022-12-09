#include "mbed.h"
#include "INA260.hpp"
#include "uLCD_4DGL.h"
#include "PinDetect.h"

// Host PC Communication channels
//Serial pc(USBTX, USBRX); // tx, rx
// i2c setup
I2C i2c(p28, p27);
INA260 VCmonitor(i2c);
DigitalIn ALT(p29);            // Alert pin
uLCD_4DGL uLCD(p13, p14, p12); // serial tx, serial rx, reset pin;

// USB PD Trigger Actuators: S1, S2, S3
DigitalOut s1(p18);
DigitalOut s2(p19);
DigitalOut s3(p20);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Button In
PinDetect pb(p8, PullUp);


int vReq = 5;

void buttonPressed(void)
{
    switch (vReq)
    {
    case 5:
        vReq = 9;
        s1 = 1;
        s2 = 1;
        s3 = 1;
        
        
        led1 = 1;
        led2 = 0;
        led3 = 0;
        led4 = 0;
        
        break;

    case 9:
        vReq = 12;
        s1 = 1;
        s2 = 0;
        s3 = 1;
        
        led1 = 1;
        led2 = 1;
        led3 = 0;
        led4 = 0;
        break;

    case 12:
        vReq = 15;
        s1 = 0;
        s2 = 0;
        s3 = 1;
        
        led1 = 1;
        led2 = 1;
        led3 = 1;
        led4 = 0;
        break;

    case 15:
        vReq = 20;
        s1 = 0;
        s2 = 1;
        s3 = 1;
        
        led1 = 1;
        led2 = 1;
        led3 = 1;
        led4 = 1;
        break;

    case 20:
        vReq = 5;
        s1 = 1;
        s2 = 1;
        s3 = 0;
        
        led1 = 0;
        led2 = 0;
        led3 = 0;
        led4 = 0;
        break;
    }
}

int main() {
    s1 = 1;
    s2 = 1;
    s3 = 0;
    
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
    double V, C, P;
    int count = 1;
    // Sets 4 samples average and sampling time for voltage and current to 8.244ms
    VCmonitor.setConfig(0x0600 | 0x01C0 | 0x0038 | 0x0007); // INA260_CONFIG_AVGRANGE_64|INA260_CONFIG_BVOLTAGETIME_8244US|INA260_CONFIG_SCURRENTTIME_8244US|INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS
//    pc.printf("INA260 TEST!\n");
//    pc.printf(""__DATE__
//              " , "__TIME__
//              "\n");
//    pc.printf("%d Config register\n", 0x0600 | 0x01C0 | 0x0038 | 0x0007); // prints the COnfig reg value to PC COM port
    VCmonitor.setAlert(0x8001);                                           // set current value as alert, latch alert pin
    VCmonitor.setLim(0x8);                                                // set limit to 10mA (10/1.25)

    wait_ms(3000);

    uLCD.text_width(2);
    uLCD.text_height(2);

    uLCD.color(BLUE);
    uLCD.locate(0, 3);
    uLCD.printf("Meas V");
    uLCD.locate(0, 6);
    uLCD.printf("Meas I");
    uLCD.locate(0, 2);
    uLCD.color(WHITE);
    uLCD.printf("---------");


    while (1) {
        
        if (!pb) {
            buttonPressed();
        }
        
        

        // get ina260 settings0
        if ((VCmonitor.getVoltage(&V) == 0) && (VCmonitor.getCurrent(&C) == 0) && (VCmonitor.getPower(&P) == 0))
        {
            // pc.printf("%d,V,%f,C,%f,P,%f\n",count,V,C,P);
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.locate(0, 0);
            uLCD.color(BLUE);
            uLCD.printf("Req V");
            
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.color(WHITE);

            uLCD.locate(0, 1);
            uLCD.printf("\r%d V ", vReq);

            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.locate(0, 4);
            uLCD.color(WHITE);
            uLCD.printf("\r%.2f V ", V);
            uLCD.locate(0, 7);
            uLCD.color(WHITE);
            uLCD.printf("\r%.1f mA  ", C);
        }
        count++;

        wait_ms(100);
    }
}