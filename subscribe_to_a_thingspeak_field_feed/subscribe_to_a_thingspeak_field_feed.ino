#include "arduino_secrets.h"
//#include <MqttClient.h>

#include <HCSR04.h>

#include <MKRGSM.h>

#include <ThingSpeak.h>

#include <MQTT.h>
#include <MQTTClient.h>

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
char writeAPIKey[] = "WOS1ZSMH6IVRGBFS";
char readAPIKey[] = "5T98ROITW8SSRTU2";
long channelID = 1353435;
long ChannelID = 1353435;
long readChannelID = 000;

PubSubClient mqttClient(client);
String subscribeTopic = "channels/" + String(channelID) + "/subscribe/fields/field1";
String MQTTAPIKey = "NJOXL2SQHURNCH8Q";

void callback (char* topic, byte* payload, unsigned int length);

//mqttClient( "mqtt.thingspeak.com", 1883, callback);

int mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length);
int mqttSubscribe(long subChannelID, int field, char* readKey, int unSub);

//Messages relayed from the MQTT broker invoke the callback function to handle the message


const char* server = "mqtt.thingspeak.com";
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 20L * 1000L;

static const char alphanum[] ="0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"; // For random generation of client ID.

//#include "ThingSpeak.h"
//unsigned long myChannelNumber =x;
//const char * myWriteAPIKey = "x";
void setup() {
Serial.begin(115200);
mqttClient.setServer(server, 1883);
mqttClient.setCallback(callback); // set the MQTT message handler function

//each time the main loop executes, check to see if data from the MQTT subscription is available for processing.
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
  if (!mqttClient.connected())
{
reconnect();
if (mqttSubscribe( ChannelID,1,readAPIKey,0)==1){
    Serial.println("subscribed");
  }
}
mqttClient.loop(); //call the loop to maintain connection to the server
if (millis() - lastConnectionTime > postingInterval)
{
//mqttpublish();
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
}
void callback(char* topic, byte* payload, unsigned int length) {

  char p[ length+1]; //leave an extra space to null terminate the string.
  memcpy( p, payload, length);
  p[ length ] = NULL; //Terminate the string
  Serial.print( "Answer: ");
  Serial.println( String(p)); 
}
int mqttSubscribe( long subChannelID, int field, char* readKey, int unsubSub){
  String myTopic;
    
  //There is no field zero. So if field 0 is sent to subscribe to, then subscribe to the whole channel feed.
  if (field=0){
    myTopic = "channels/" + String(subChannelID)+ "/subscribe/json/" + String(readKey);
  } 
  else {
    myTopic = "channels/" + String(subChannelID)+ "/subscribe/fields/field"+ String(field) + "/" + String(readKey);
  }
  Serial.println("Subscribing to" +myTopic);
  Serial.println("State=" + String(mqttClient.state())); 
  char charBuf[myTopic.length()+1];
  myTopic.toCharArray(charBuf,myTopic.length()+1);
  if (unsubSub==1) {
    return mqttClient.unsubscribe(myTopic.c_str());
  }
    return mqttClient.subscribe(myTopic.c_str(), 0);
    return mqttClient.subscribe(charBuf);
  }

