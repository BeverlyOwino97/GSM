# -*- coding: utf-8 -*-
"""
Created on Thu Apr 22 16:21:34 2021

@author: bevakinyi
"""


import paho.mqtt.client as mqtt
import json
from influxdb import InfluxDBClient

#INFLUXDB_ADDRESS = '192.168.137.46'
#INFLUXDB_USER = 'mqtt'
#INFLUXDB_PASSWORD = 'mqtt'
INFLUXDB_DATABASE = 'techgsm'

WRITE_API_KEY = "L5SYQNCEAMKWJ5HK";
CHANNEL_ID = 1352833;
READ_API_KEY = 'Y0DQ8NS2S1P4UC29'

MQTT_ADDRESS = 'mqtt.thingspeak.com'
MQTT_USER = 'gsm2'
MQTT_PASSWORD = 'NJOXL2SQHURNCH8Q'
MQTT_TOPIC = 'channels/1352833/subscribe/json/Y0DQ8NS2S1P4UC29'
MQTT_CLIENT_ID = 'MQTTInfluxDBBridge'

influxdb_client = InfluxDBClient(host='35.230.100.15', port=8086, username='admin', password='bK2eCWuuAJHTgPRE',ssl=True)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    print(msg.topic+" "+ msg.payload.decode('utf-8'))
    savedata(msg.topic , msg.payload.decode('utf-8'))

# influxdb
def savedata(topic, data):
    #client = InfluxDBClient(INFLUXDB_ADDRESS, 8086, 'root', '', 'Sensor') 
    
    data = json.loads(data)
    datas = [
        {
            "measurement": "distance",
            "tags": {
                "topic": topic
            },
            "fields": data
        }
    ]
    
    influxdb_client.write_points(datas)
    
def _init_influxdb_database():
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)
    '''result = list(influxdb_client.query('select * from distance').get_points())
    print(result) '''

def main():
    _init_influxdb_database()


    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.username_pw_set(MQTT_USER,MQTT_PASSWORD)
    client.connect(MQTT_ADDRESS, 1883, 60)
    client.loop_forever()
    
if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()