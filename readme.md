# ECE 4180 Project: Programmable Power Supply 

** Team member: Eric Shi, Ziyu Liu, Tomas Gowens **
** Geogia Tech **

**Watch the Demo: **
## Table of Content 
* [Project Description](#project-description)
* [Repo Navigatation](#repo-navigation)
* [Parts List](#parts-list)
* [Schematic](#shcematic)
* [Connection Guide](#connection-guide)
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
[4DGL-uLCD-SE](4DGL-uLCD-SE), [INA260](INA260), [mbed](mbed)are three library folders that include required API for this project. 

[main.cpp ](main.cpp) contains the main logic of controlling the mini power supply using mbed command.

[Schematic](Schematic_ECE4180_USBPD_2022-12-01.svg) shows the layout of the project's customized break out board. 

## Parts List 
* Mbed LPC1768
* LCD Display: uLCD-144G2

## Schematic 

## Connection Guide

## Future Work
Due to the scope of the project, we only made the power supply support 5,9, 12, 15, 20V. In the future, we want to integrate some analog voltage regulator to the break out unit in order to enable to fine control. Ideally, we want the future work to have a 20mV increment step, therefore, the power supply become more accurate. 