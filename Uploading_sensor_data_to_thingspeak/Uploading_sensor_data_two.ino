#include "arduino_secrets.h"
/*
  Write Sensor data to a field on ThingSpeak
  
  Description: Writes distance read from HCSR04 to a channel on ThingSpeak every 60 seconds.
  
  Hardware: Arduino MKR GSM 1400, Ultrasonic Sensor.
  
  -Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires MKRGSM library, Thingspeak library, HCSR04 library.
  - Reqires GSM access (SIM card or credentials).
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  
*/
#include <ThingSpeak.h>

#include <HCSR04.h>
#include <MKRGSM.h>
#include <OneWire.h>
#include <PubSubClient.h>

//The sensitive data are set in the arduino_secrets.h tab
const char PINNUMBER[]     = SECRET_PIN;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASS;

// Initialize the library instance
GSMClient client; //Client service for TCP connection
GPRS gprs; //GPRS access
GSM gsmAccess; //GSM access: include a 'true' parameter for debug enabled

#define SECRET_CH_ID "1353435"
String myWriteAPIKey = SECRET_WRITE_APIKEY;

// Setting URL to http://thingspeak.com
char thingSpeakAddress[] = "api.thingspeak.com";
//char url[] = "api.thingspeak.com";
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

void setup() 
{
  Serial.begin(115200);         //Recommended speed is 115200
  pinMode(trigPIN,OUTPUT);      //Sets the TrigPIN as an output
  pinMode(echoPIN,INPUT);       //Sets the echoPIN as an input
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  Serial.println("Starting Arduino web client.");
  boolean connected = false;

  // wait 10 seconds for connection:
  delay(1000);
  
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


void loop() 
{
      long duration, inches, cm;
      //clears the trigPIN
      digitalWrite(trigPIN, LOW);
      delayMicroseconds(2);
      
      // Sets the trigPin on HIGH state.
      digitalWrite(trigPIN, HIGH);
      delayMicroseconds(10);
      
      digitalWrite(trigPIN, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPIN, HIGH);
      inches = microsecondsToInches(duration);
      cm = microsecondsToCentimeters(duration);

      Serial.print(inches);
      Serial.print("in, ");
      Serial.print(cm);
      Serial.print("cm");
      Serial.println();

  delay(100);
    

  sendDistance(cm);
}
long microsecondsToInches(long microseconds) 
{ // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

 void sendDistance(float cm)
{  
  GSMClient client;//not to be confused with "client" in PubSub{}, and wclient for mqtt
   if (client.connect(thingSpeakAddress, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   //Serial.println("GSM Client connected ");
   
   String postStr = myWriteAPIKey;
   postStr += "&field1=";
   postStr += String(cm);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.println("User-Agent: Arduino MKRGSM");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + myWriteAPIKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if

 client.stop();
}//end send to ts
