- To use MQTT subscribe, supply any unique user name and the MQTT API Key as your password when you connect. MQTT publish does not require the API key. To see your MQTT API key, choose Account > My Profile.

- The first thing in the python script is to import the paho MQTT library. After the library is imported, all necessary variables are defined. Make sure that the MQTT  usename and password are correctly written.

- The on_connect function handles what happens when the MQTT client connects to the broker. If our client connects to the broker we want to subscribe to the MQTTtopic on water level measurement and print the message that the connection was established. The variable rc holds an error code if the connection is not successful so the debugging is easier:

  0: Connection successful.
  1: Connection refused – incorrect protocol version.
  2: Connection refused – invalid client identifier.
  3: Connection refused – server unavailable.
  4: Connection refused – bad username or password.
  5: Connection refused – not authorized.
  6-255: Currently unused.
  
- The on_message function is called every time the topic is published to. In this case the topic and the message are printed to the terminal. We expect that at this point we will get the current distance measurements taken by the ultrasonic sensor printed to the terminal.

- The main function holds the main part of the python script where all defined functions are executed. First we create a client object and set the username and password for the MQTT client. Then we tell the client which function is called when we want to connect to the MQTT broker and receive messages.

- Once everything has been set up, we can connect to the broker with the broker IP and the broker port. Once we have told the client to connect, the client object runs in the loop function forever.
