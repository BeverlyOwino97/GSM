## Components in an MQTT Communication System

- The MQTT communication system contains 3 components with specific roles and objectives:
  - **Publisher** who generates and sends data to the MQTT broker. In our case, the publisher is the arduino MKR GSM which is connected to sensors and collects the sensor data then sends it via GSM/GPRS after a specified period of time to the thingspeak mqtt broker.
  - **Broker** which is like a server to collect the messages from all publishers, save the data and distribute the data messages to the right subscribers based on mqtt topics.
  - The **subscriber** is a component which subscribes to certain kinds of messages coming from publishers. In our case, the subscriber is our PC where the latest data updates are received.

- Using this code, we build up a basic MQTT communication with the Arduino MKR GSM board as publisher, collecting water level sensor information. 
From this publisher the data is sent with a topic and a payload via MQTT to the Thingspeak MQTT broker. We will then write a python script to subscribe to latest data updates from the channel.
- channels/channelID/publish/apikey publishes messages to the topic to update a channel feed. Replace 'channelID' with your channel ID and 'apikey' with the Write API Key of the thingspeak channel.
- Set up the IP of the MQTT broker. ThingSpeak™ has an MQTT broker at the URL mqtt.thingspeak.com and port 1883. 
- A topic contains the routing information for the broker. Each client that wants to send messages publishes them to a certain topic, and each client that wants to receive messages subscribes to a certain topic. The broker delivers all messages with the matching topic to the appropriate clients.The MQTT Topic is defined by the publisher. Thingspeak has a standard pattern of writing the topics as is available here: https://nl.mathworks.com/help/thingspeak/use-desktop-mqtt-client-to-subscribe-to-channel-updates.html. Topics have to be unique.
