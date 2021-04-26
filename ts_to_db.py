# -*- coding: utf-8 -*-
"""
Created on Sun Apr 18 09:33:12 2021

@author: bevakinyi
"""


#import re
#from typing import NamedTuple

import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
#INFLUXDB_ADDRESS = '192.168.137.46'
#INFLUXDB_USER = 'mqtt'
#INFLUXDB_PASSWORD = 'mqtt'
INFLUXDB_DATABASE = 'gsmtech'

WRITE_API_KEY = 'L5SYQNCEAMKWJ5HK'
CHANNEL_ID = 1352833;
READ_API_KEY = 'Y0DQ8NS2S1P4UC29'

MQTT_ADDRESS = 'mqtt.thingspeak.com'
MQTT_USER = 'gsm2'
MQTT_PASSWORD = 'NJOXL2SQHURNCH8Q'
# MQTT_TOPIC = 'home/+/+'
MQTT_TOPIC = 'channels/1352833/subscribe/json/Y0DQ8NS2S1P4UC29'
#MQTT_REGEX = 'channels/([^/]+)/([^/]+)'
#MQTT_REGEX = 'channels/([^/]1352833)/([^/]subscribe/([^/]json/([^/]Y0DQ8NS2S1P4UC29'
MQTT_CLIENT_ID = 'MQTTInfluxDBBridge'

influxdb_client = InfluxDBClient(host='instance ip', port=8086, username='admin', password='instance password',ssl=True)

'''
class SensorData(NamedTuple):
    channel: str
    #channelid: str
    measurement: str
    value: float '''

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)

'''def _parse_mqtt_message(topic, payload):
    
    match = re.match(MQTT_REGEX, topic)
    if match:
        channelid = match.group(1)
        distance = match.group(3)
        if distance == 'status':
            return None
    return SensorData(channelid, distance, float(payload))
   # else:
       # return None
       '''

def _send_sensor_data_to_influxdb(topic, data):
    json_body = [
        {
            "measurement": "distance",
            "tags": {
                "topic": topic
            },
            "fields": data
            }
    ]
    influxdb_client.write_points(json_body)

def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    print(msg.topic + ' ' + str(msg.payload))
    #sensor_data = _parse_mqtt_message(msg.topic, msg.payload.decode('utf-8'))
    data = _send_sensor_data_to_influxdb(msg.topic , msg.payload.decode('utf-8'))
    if data is not None:
       _send_sensor_data_to_influxdb(data)

def _init_influxdb_database():
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)

def main():
    _init_influxdb_database()

    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
