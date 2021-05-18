//#include <MqttClient.h>

#include <HCSR04.h>

#include <MKRGSM.h>

#include <ThingSpeak.h>

//#include <MQTT.h>
//#include <MQTTClient.h>

#include <PubSubClient.h>
#include <SPI.h>

//The sensitive data are set in the in the arduino_secrets.h tab
const char PINNUMBER[]     = SECRET_PIN;
// APN data

const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASS;

// Initialize the library instance
//GSMSSLClient net; 
GSMClient client;//Client service for TCP connection
GPRS gprs; //GPRS access
GSM gsmAccess; //GSM access: include a 'true' parameter for debug enabled

char mqttUserName[] = "gsm2";
char mqttPass[] = "NJOXL2SQHURNCH8Q";
char writeAPIKey[] = "L5SYQNCEAMKWJ5HK";
long channelID = 1352833;

PubSubClient mqttClient(client);

const char* server = "mqtt.thingspeak.com";
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 20L * 1000L;

static const char alphanum[] ="0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"; // For random generation of client ID.

//#include "ThingSpeak.h"
//unsigned long myChannelNumber =x;
//const char * myWriteAPIKey = "x";

#define echoPIN 7 
#define trigPIN 8
long duration;
int distance;

int analogInput = A3;
int analogInput2 = A2;
float vout = 0.0;
float vout1 = 0.0;
float Vout3 = 0.0;
float vin = 0.0;
float vin1 = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; //
float R3 = 30000.0; //  
float R4 = 7500.0; // 
int value = 0;


// Variables for Measured Voltage and Calculated Current
double Vout = 0;
double Current = 0;
 
// Constants for Scale Factor
 
//const double scale_factor = 0.185; // 5A
const double scale_factor = 0.1; // 20A
//const double scale_factor = 0.066; // 30A
 
// Constants for A/D converter resolution
// Arduino has 10-bit ADC, so 1024 possible values
// Reference voltage is 5V if not using AREF external reference
// Zero point is half of Reference Voltage
 
const double vRef = 5.00;
const double resConvert = 1024;
double resADC = vRef/resConvert;
double zeroPoint = vRef/2;
 


void setup() {
Serial.begin(115200);
mqttClient.setServer(server, 1883);

//ThingSpeak.begin(client);

pinMode(analogInput, INPUT);
pinMode(analogInput2, INPUT);

pinMode(trigPIN,OUTPUT);
  pinMode(echoPIN,INPUT);
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

//mqttClient.setServer(server, 1883);

void loop() {
     long duration, inches, cm;
     digitalWrite(trigPIN, LOW);
     delayMicroseconds(2);
     digitalWrite(trigPIN, HIGH);
     delayMicroseconds(10);
     digitalWrite(trigPIN, LOW);
     duration = pulseIn(echoPIN, HIGH);
     inches = microsecondsToInches(duration);
     cm = microsecondsToCentimeters(duration);
     distance = duration*(0.034/2);
     //return distance;
     Serial.print(distance);
     delay(1000); 


     // read the value at analog input
   value = analogRead(analogInput);
   vout = (value * 5.0) / 1024.0; // see text
   vin = vout / (R2/(R1+R2)); 
   
   Serial.print("INPUT V= ");
   Serial.println(vin,2);
   delay(500);


   // read the value at analog input 2
   value = analogRead(analogInput2);
   vout1 = (value * 5.0) / 1024.0; // see text
   vin1 = vout1 / (R4/(R3+R4)); 
   
   Serial.print("INPUT V2= ");
   Serial.println(vin1,2);
   delay(500);


   // Vout is read 1000 Times for precision
  for(int i = 0; i < 1000; i++) {
    Vout3 = (Vout3 + (resADC * analogRead(A0)));   
    delay(1);
  }
  
  // Get Vout in mv
  Vout3 = Vout3 /1000;
  
  // Convert Vout into Current using Scale Factor
  Current = (Vout3 - zeroPoint)/ scale_factor;                   
 
  // Print Vout and Current to two Current = ");                  
 
  Serial.print("Vout = ");           
  Serial.print(Vout3,2); 
  Serial.print(" Volts");                            
  Serial.print("\t Current = ");                  
  Serial.print(Current,2);
  Serial.println(" Amps");                             
 
  delay(1000); 
         
     
if (!mqttClient.connected())
{
reconnect();
}
mqttClient.loop();
if (millis() - lastConnectionTime > postingInterval)
{
mqttpublish();
}
  
}

void reconnect()
{

while (!mqttClient.connected())
{
Serial.print("Attempting MQTT connection...");

char clientID[9];
// Generate ClientID
for (int i = 0; i < 8; i++) {
clientID[i] = alphanum[random(51)];
}

clientID[8]='\0';
        
// Connect to the ThingSpeak MQTT broker.
  
//char clientID[]="x";
if (mqttClient.connect(clientID,mqttUserName,mqttPass))
{
Serial.print("Connected with Client ID: ");
Serial.print(String(clientID));
Serial.print(", Username: ");
Serial.print(mqttUserName);
Serial.print(" , Password: ");
Serial.println(mqttPass);
} else
{
Serial.print("failed, rc=");
Serial.print(mqttClient.state());
Serial.println(" try again in 5 seconds");
delay(5000);
}
} 
//sendDistance(cm);
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
void mqttpublish() {


// Create data string to send to ThingSpeak
//String data = String("field1=" + String(distance, DEC) + "&field2=" + String(vin, DEC) + "&field3=" + String(vin1, DEC));
String data = String("field1=" + String(distance, DEC) + "&field2=" + String(vin, DEC) + "&field3=" + String(Current, DEC)+ + "&field4=" + String(vin1, DEC));
int length = data.length();
char msgBuffer[length];
data.toCharArray(msgBuffer,length+1);
Serial.println(msgBuffer);

// Create a topic string and publish data to ThingSpeak channel feed.
String topicString ="channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
length=topicString.length();
char topicBuffer[length];
topicString.toCharArray(topicBuffer,length+1);

mqttClient.publish( topicBuffer, msgBuffer );

lastConnectionTime = millis();
}
// Use this function instead to publish to a single field directly. 
/*
void mqttpublish() {
  //void sendDistance(float cm) { 
  //float cm;}

//String data = String(cm, DEC);
String data = String(distance, DEC);
int length = data.length();
char msgBuffer[length];
data.toCharArray(msgBuffer,length+1);
Serial.println(msgBuffer);

// Create a topic string and publish data to ThingSpeak channel feed.
String topicString ="channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
length=topicString.length();
char topicBuffer[length];
topicString.toCharArray(topicBuffer,length+1);

mqttClient.publish( topicBuffer, msgBuffer );

lastConnectionTime = millis();
}
*/
