# ESP8266 Twilio SMS IP Address Notifier
This project utilizes an ESP8266 microcontroller to send an SMS containing its IP address via Twilio, enabling remote control and monitoring of home devices through a secure web interface.
# Introduction
This project is designed to help users remotely monitor and control their home devices. By sending an SMS with its current IP address, the ESP8266 can be accessed from anywhere, facilitating a secure connection through a web interface.

# Requirements
Hardware
ESP8266 development board (e.g., NodeMCU, Wemos D1 Mini)
USB cable for programming the ESP8266
Optional: Breadboard and jumper wires for additional components
Software
Arduino IDE (with ESP8266 board support installed)
Twilio account (with a verified phone number)
A web server (optional, if you want to host a secure interface)
Hardware Setup
Connect the ESP8266 to your computer using a USB cable.

Install the ESP8266 board package in Arduino IDE:

Go to File > Preferences, and in the "Additional Boards Manager URLs" field, add: http://arduino.esp8266.com/stable/package_esp8266com_index.json
Open Tools > Board > Boards Manager, search for "ESP8266" and install it.
Select the Board:

Go to Tools > Board, and select your ESP8266 board.
# Software Setup
Install required libraries in Arduino IDE:

Go to Sketch > Include Library > Manage Libraries.
Search for and install the following libraries:
ESP8266WiFi
WiFiClient
ArduinoJson (for handling JSON, if needed)
Download the provided code (see Code Explanation section for a sample code snippet).

Configure the code:

Update the WiFi credentials.
Add your Twilio credentials (Account SID, Auth Token, and Twilio phone number).
Update the recipient's phone number where the IP address will be sent.
# Twilio Configuration
Sign up for a Twilio account at Twilio.com.
Get a Twilio phone number:
Navigate to the Phone Numbers section and buy a number.
Find your Account SID and Auth Token in the Twilio Console Dashboard.
Note: Ensure that your Twilio account is set up to send SMS messages to the phone number you want to use.

# License
This project is open-source and free to use. Feel free to modify it to suit your needs. Please give credit if you use parts of this project in your own work.


