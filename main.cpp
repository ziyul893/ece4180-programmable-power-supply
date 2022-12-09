/* INA260 test app*/

#include "mbed.h"
#include "INA260.hpp"
#include "uLCD_4DGL.h"

// Host PC Communication channels
Serial pc(USBTX, USBRX); // tx, rx
//i2c setup
I2C i2c(p28,p27);
INA260 VCmonitor(i2c); 
DigitalIn ALT(p29); //Alert pin
uLCD_4DGL uLCD(p13,p14,p12); // serial tx, serial rx, reset pin;

int main() { 

    double V,C,P;
    int count = 1;  
    // Sets 4 samples average and sampling time for voltage and current to 8.244ms
    VCmonitor.setConfig(0x0600 | 0x01C0 | 0x0038 | 0x0007); //INA260_CONFIG_AVGRANGE_64|INA260_CONFIG_BVOLTAGETIME_8244US|INA260_CONFIG_SCURRENTTIME_8244US|INA260_CONFIG_MODE_SANDBVOLT_CONTINUOUS
    pc.printf("INA260 TEST!\n");
    pc.printf(""__DATE__" , "__TIME__"\n");
    pc.printf("%d Config register\n",0x0600 | 0x01C0 | 0x0038 | 0x0007); //prints the COnfig reg value to PC COM port
    VCmonitor.setAlert(0x8001); //set current value as alert, latch alert pin
    VCmonitor.setLim(0x8); //set limit to 10mA (10/1.25)

    wait_ms(3000);
    
        uLCD.text_width(2);     
        uLCD.text_height(2);
        uLCD.locate(0,0);
        uLCD.color(BLUE);
        uLCD.printf("Req V");
        uLCD.locate(0,3);
        uLCD.printf("Meas V");
        uLCD.locate(0,6);
        uLCD.printf("Meas I");
        uLCD.locate(0,2);
        uLCD.color(WHITE);
        uLCD.printf("---------");
        
while(1)
{

         
         //get ina260 settings0
         if((VCmonitor.getVoltage(&V) == 0) && (VCmonitor.getCurrent(&C) == 0) && (VCmonitor.getPower(&P) == 0))
         {
             //pc.printf("%d,V,%f,C,%f,P,%f\n",count,V,C,P);
             uLCD.text_width(2);     
            uLCD.text_height(2);
            uLCD.color(WHITE);
            if (V <= 6.5) {
                uLCD.locate(0,1);
                uLCD.printf("\r5 V ");
            }
            else if (V >= 6.8 && V <= 10.5) {
                uLCD.locate(0,1);
                uLCD.printf("\r9 V ");
            }
            else if (V >= 10.7 && V <= 12.5) {
                uLCD.locate(0,1);
                uLCD.printf("\r12 V ");
            }
            else if (V >= 13.5 && V <= 15.5) {
                uLCD.locate(0,1);
                uLCD.printf("\r15 V ");
            }
            else if (V >= 18) {
                uLCD.locate(0,1);
                uLCD.printf("\r20 V ");
            }
            
            uLCD.text_width(2);     
            uLCD.text_height(2);
            uLCD.locate(0,4);
            uLCD.color(WHITE);
            uLCD.printf("\r%.2f V ",V);
            uLCD.locate(0,7);
            uLCD.color(WHITE);
            uLCD.printf("\r%.1f mA  ",C);
         }
         count++;
         
         //if(ALT==0)
         //{
         //   pc.printf("Overcurrent!!!!");  
             
         //   uLCD.text_string("OVERCURRENT", 0, 4, FONT_7X8, WHITE);
         //}
         
         wait_ms(100);
}   
}