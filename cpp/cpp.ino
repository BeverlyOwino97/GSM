#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(2,3);
#include<String.h>
#define echoPIN 7
#define trigPIN 8
long duration;
int distance;

void setup(){
  gprsSerial.begin(9600);
  Serial.begin(9600);
  while (!Serial){
    ;//wait for serial port to connect
  }
  gprsSerial.begin(9600);
  pinMode(echoPIN, INPUT);
  pinMode(trigPIN, OUTPUT);
  //send SMS("0706242893", "Hi");
}
void loop(){
 
  digitalWrite(trigPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(echoPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPIN, LOW);
  duration = pulseIn(echoPIN, HIGH);
  distance = duration*(0.034/2);
  return distance;
  Serial.print(distance);
 


if (gprsSerial.available())
   Serial.write(gprsSerial.read());
   
Serial.print("AT\\r\\n");
gprsSerial.println("AT");/* Check Communication */
delay(1000);

ShowSerialData();
delay(1000);
Serial.print("AT+CPIN?\\r\\n");
gprsSerial.println("AT+CPIN?");
delay(1000);

ShowSerialData();
delay(1000);
Serial.print("AT+CGATT=1\\r\\n");
gprsSerial.println("AT+CGATT=1");  /* Attach to GPRS Service */
delay(1000);

ShowSerialData();
delay(1000);
Serial.print("AT+CREG?\\r\\n");
gprsSerial.println("AT+CREG?");/* Network registration status */
delay(1000);

ShowSerialData();
delay(1000);
Serial.print("AT+CGATT?\\r\\n");/* Attach to GPRS Service */
gprsSerial.println("AT+CGATT?");
delay(1000);

ShowSerialData();
delay(1000);
gprsSerial.println("AT+CIPSHUT");
delay(1000);

ShowSerialData();
delay(1000);
Serial.print("AT+CIPSTATUS\\r\\n");
gprsSerial.println("AT+CIPSTATUS");
delay(2000);

ShowSerialData();
delay(1000);
Serial.print("AT+CIPMUX=0\\r\\n");
gprsSerial.println("AT+CIPMUX = 0");/* Single TCP/IP connection mode */
delay(2000);

ShowSerialData();

Serial.print("AT+CSTT=\"airtelgprs.com\",\"\",\"\"\\r\\n");
gprsSerial.println("AT + CSTT= \"airtelgprs.com\",\"\",\"\"");/*connect to internet*/
//setting the APN

ShowSerialData();

Serial.print("AT+CIICR\\r\\n");
gprsSerial.println("AT+CIICR"); //wireless connection
delay(3000);

ShowSerialData();

Serial.print("AT+CIFSR\\r\\n");
gprsSerial.println("AT+CIFSR"); // get local IP address
delay(2000);

ShowSerialData();

Serial.print("AT+CIPSPRT=0\\r\\n");
gprsSerial.println("AT+CIPSPRT=0");
delay(3000);

ShowSerialData();

Serial.print("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"\\r\\n");
gprsSerial.println("AT + CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\""); //starting up the connection
delay(6000);

ShowSerialData();

Serial.print("AT+CIPSEND\\r\\n");
gprsSerial.println("AT+CIPSEND");/* Send data through TCP connection */
delay(4000);

ShowSerialData();

String str = "GET https://api.thingspeak.com/update?api_key=O13AOCHYYNU2LQ19&field1=" + String(distance);/* URL for data to be sent to */
Serial.println(str);
gprsSerial.println(str);
delay(4000);

ShowSerialData();

gprsSerial.println((char)26);
delay(5000);
gprsSerial.println();

ShowSerialData();
Serial.print("AT+CIPSHUT\\r\\n");
gprsSerial.println("AT+CIPSHUT");
delay(100);

ShowSerialData();
}
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000);
}
