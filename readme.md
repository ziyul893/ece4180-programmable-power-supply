# ECE 4180 Project: Programmable Power Supply 

**Team member: Eric Shi, Ziyu Liu, Tomas Gowens**

**Geogia Tech**

**Watch the Demo:**
## Table of Content 
* [Project Description](#project-description)
* [Repo Navigatation](#repo-navigation)
* [Parts List](#parts-list)
* [Schematic](#shcematic)
* [Connection Guide](#connection-guide)
* [Source Code](#source-code)
* [Future Work](#future-work)


## Project Description
The project is inspired from PD trigger that is avalibale request different voltages with I/O control. While PD Controller is widely available on market. However, those devices have doesn't have monitor to show the real output of voltage of current. Out project is trying to solve this issue. An add-on display is integrated in our project to demonstrate requested voltage (V_req), measured voltage (V_meas), measure current (I_meas).

It is a mini “benchtop-style” variable voltage power supply that is powered by a
standard USB-C PD-enabled AC to DC adapter. The unit should be able to output [5V, 9V, 12V, 15V,
20V]. The device will include an mbed-enabled microcontroller (LPC1768, STM32F4-Series, or
STM32F7-Series), which will connect to an LCD screen via UART, the USB-C PHY controller via
I2C, buttons or an RPG with GPIO, and status LEDs with PWM-enabled GPIO. The screen will
display the output voltage and current. This project will most likely require the creation of a custom
PCB. We may use an additional small IC such as the Atmega32u4 as a coprocessor to hardware
offload specialized functions such as USB-C CC/PD communications.

## Repo Navigation
[4DGL-uLCD-SE](4DGL-uLCD-SE), [INA260](INA260), [mbed](mbed) are three library folders that include required API for this project. 

[main.cpp](main.cpp) contains the main logic of controlling the mini power supply using mbed command.

[Schematic](Schematic_ECE4180_USBPD_2022-12-01.svg) shows the layout of the project's customized break out board. 

## Parts List 
* Mbed LPC1768
* LCD Display: uLCD-144G2

## Schematic 
![schematic break out board](Schematic_ECE4180_USBPD_2022-12-09.svg)

## PCB Layout
![pcbrender](PCB_PCB_ECE4180_USBPD_2022-12-09.svg)

## Connection Guide

## Source Code
```
#include "mbed.h"
#include "INA260.hpp"
#include "uLCD_4DGL.h"
#include "PinDetect.h"


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
        if ((VCmonitor.getVoltage(&V) == 0) && (VCmonitor.getCurrent(&C) == 0) && (VCmonitor.getPower(&P) == 0)) {
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
```
## Future Work
Due to the scope of the project, we only made the power supply support 5,9, 12, 15, 20V. In the future, we want to integrate some analog voltage regulator to the break out unit in order to enable to fine control. Ideally, we want the future work to have a 20mV increment step, therefore, the power supply become more accurate. 
