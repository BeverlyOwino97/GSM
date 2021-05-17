## GSM Technology.

**Global System for Mobile Communication(GSM)** is a digital mobile network that is widely used by mobile phone users in all parts of the world. One big advantage of GSM/GPRS connectivity is that it covers a wide area and connectivity is available almost everywhere. The Arduino MKR GSM 1400 takes advantage of the cellular network as a means to communicate. The GSM/3G network covers the highest percentage of the world's surface, making this connectivity option very attractive. The board's main processor is a low power **Arm Cortex -MO 32-bit SAMD21** like in the other boards within the Arduino MKR family. The GSM/3G connectivity is performed with a module from u-blox, the SARA-U201,  a low power chipset operating in the different bands of the cellular range (GSM 850 MHz, E-GSM 1900MHz, DCS 1800MHz, PCS 1900MHz). In addition to that, secure communication is ensured through the Microchip ECC508 crypto chip. The board makes it possible to set or receive calls, send and receive text messages, and access data networks to exchange data with different types of servers. Arduino MKR GSM has a modem that transfers data from a serial port to the GSM network. The modem executes operations via a series of AT commands.

# Using the MKR GSM Module.

# Powering the board.

The board runs at 3.3V. The maximum voltage that its I/O pins can tolerate is 3.3V. The mkr gsm has a USB port that can be used to supply 5V power to the board. During cellular transmissions, the peak current required by the board will exceed 500mA. This is in excess of what can be provided by a astndard USB port and therefore, it is mandatory to have a 1500mAh or  higher LiPo battery plugged all the time. When powering the board using Vin, a 5V power supply that can supply atleast 2A is required.

# MKR GSM Library. 

To operate the MKR GSM module, you need to install Arduino Software (IDE) which provides an environment on which code to run in the module can be written, compiled and uploaded to the board. You can also write your code on the Arduino Web Editor. The Arduino MKR GSM board uses the MKR GSM library which can be installed using the Arduino Library Manager**.The library abstracts low level communications between the modem and SIM card. It relies on the Serial library** for communication between the modem and the Arduino. This library is based on the GSM library of the Arduino GSM shield, adding new features such as UDP, SSL and DTMF tone recognition.

# Board Manager.

To program your MKR GSM 1400 offline, you need to add the Atmel SAMD Core to the Arduino Desktop IDE. This is done by selecting Tools menu, then Boards and finally Board Manager from where you can search Zero or MKR GSM to find the core. Click on its box and then install.

# Select Board Type and Port.

From Tools, select the Board Arduino MKR GSM 1400 and then the port that is labelled with the same name.


In this project, the arduino mkr gsm 1400 board was used to collect data from the ultrasonic sensor and this data is sent wirelessly to the **thingspeak IoT platform**.The connection was done as follows:

Digital pin 8 on Arduino mkr gsm - Trigger Pin on HCSR04
Digital pin 7 on Arduino mkr gsm - Echo Pin on HCSR04
5V output on Arduino mkr gsm - Vcc on HCSR04
GND - GND 

ThingSpeak is an open data platform and an API for the IoT that enables you to collect, store, analyze, visualize and act on data collected from sensors.  

# Setting up Thingspeak.

First, you need to sign up to ThingSpeak on their website https://thingspeak.com and create a Mathworks account through which you will get access to the site.
Next, create a new channel and set up what you want. After creating your channel, ThingSpeak will provide you with a unique channel ID, read and Write API keys which are required for programming modifications. The channel ID, ThingSpeak read/write API keys are stored in an arduino_secrets file.
