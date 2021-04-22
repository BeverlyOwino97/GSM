#include "arduino_secrets.h"
/*
  Write Sensor data to a single field on ThingSpeak
  
  Description: Writes distance read from HCSR04 to a channel on ThingSpeak every 20 seconds.
  
  Hardware: Arduino MKR GSM 1400, Ultrasonic Sensor.
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires MKRGSM library, Thingspeak library, HCSR04 library.
  - Reqires GSM access (SIM card or credentials).
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/
// libraries
#include <MKRGSM.h>
#include <HCSR04.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

//The sensitive data are set in the in the arduino_secrets.h tab
const char PINNUMBER[]     = SECRET_PIN;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASS;

// Initialize the library instance
GSMClient client;//Client service for TCP connection
GPRS gprs; //GPRS access
GSM gsmAccess; //GSM access: include a 'true' parameter for debug enabled

#define SECRET_CH_ID "1352833"
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
const int ChannelField = 1; // Which channel to write data

// Setting URL to http://thingspeak.com
char thingSpeakAddress[] = "api.thingspeak.com";
char url[] = "api.thingspeak.com";
char urlproxy[] = "http://api.thingspeak.com";
char path[] = "/";


// Various variables used in this Sketch
String response = "";
String okmsg = "ok";
String errormsg = "error";

#define echoPIN 7 
#define trigPIN 8
long duration;
int distance;

void setup() {
  pinMode(echoPIN, INPUT);
  pinMode(trigPIN, OUTPUT);
  Serial.begin(9600);  //Initialize serial communication
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  Serial.println("Starting Arduino web client.");
  boolean connected = false;

  // wait 10 seconds for connection:
  delay(10000);
  
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connected");
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
}

void loop() {
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  //reading sensor values
      digitalWrite(trigPIN, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPIN, LOW);
      duration = pulseIn(echoPIN, HIGH);
      distance = duration*(0.034/2);
      
      delay(2000); 

      if(Serial) Serial.print("distance in cm:");
      if(Serial) Serial.print(distance);
  
  ThingSpeak.writeField(1352833, ChannelField, distance, myWriteAPIKey);
  delay(1000);
  }

  
  
  
