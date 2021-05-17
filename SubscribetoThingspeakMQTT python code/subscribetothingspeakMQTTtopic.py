# -*- coding: utf-8 -*-
"""
Created on Fri Apr 16 13:51:09 2021

@author: bevakinyi
"""

#Create a python file for subscribing to MQTT data

import paho.mqtt.client as mqtt


WRITE_API_KEY = "L5SYQNCEAMKWJ5HK"
CHANNEL_ID = 1352833
READ_API_KEY = "Y0DQ8NS2S1P4UC29"
MQTT_ADDRESS = 'mqtt.thingspeak.com'
MQTT_USER = 'gsm2'
MQTT_PASSWORD = 'NJOXL2SQHURNCH8Q'


MQTT_TOPIC  = 'channels/1352833/subscribe/json/Y0DQ8NS2S1P4UC29'

'''
The on_connect function handles what happens when the MQTT client connects to the broker. 
If our clients connect to the broker we want to subscribe to the distance topics and print the message that the connection was established. 
The variable rc holds and error code if the connection is not successful so the debugging is easier.

'''
def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    print("message received " ,str(msg.payload.decode("utf-8")))
   

#we create a client object and set the username and password for the MQTT client. 
#Then we tell the client which function is called when we want to connect to the MQTT broker and receive messages.

#Once everything has been set up, we can connect to the broker with the broker IP and the broker port.
# Once we have told the client to connect, the client object runs in the loop function forever.
def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    # tell the client which function is called when we want to connect to the MQTT broker and receive messages.
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
