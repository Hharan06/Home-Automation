#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Base64.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// WiFi credentials
const char* ssid = "Oneplus";
const char* password = "t2nykkva";

// Twilio credentials
const char* account_sid = "***************";
const char* auth_token = "************";
const char* twilio_number = "**********";
const char* recipient_number = "***********";

const char* host = "api.twilio.com";
const int httpsPort = 443;

unsigned int message_count = 0;
const unsigned int max_messages = 2;
WiFiClientSecure client;

const int ledPin = D1;  // GPIO pin for the LED
const int dhtPin = D2;  // GPIO pin for the DHT sensor

DHT dht(dhtPin, DHT11); // Initialize DHT sensor for DHT11

ESP8266WebServer server(80);

String urlencode(const char* str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (uint16_t i = 0; i < strlen(str); i++) {
    c = str[i];
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if ((c & 0xf) > 9) {
        code0 = (c & 0xf) - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}

void sendSMS(String message) {
  String auth = String(account_sid) + ":" + String(auth_token);

  client.setInsecure();

  if (client.connect(host, httpsPort)) {
    String data = "To=" + urlencode(recipient_number) +
                  "&From=" + urlencode(twilio_number) +
                  "&Body=" + urlencode(message.c_str());

    client.println("POST /2010-04-01/Accounts/" + String(account_sid) + "/Messages.json HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Authorization: Basic " + base64::encode(auth));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(data.length()));
    client.println("Connection: close");
    client.println();
    client.println(data);

    delay(100); // Wait for response
  } else {
    Serial.println("Connection to Twilio failed!");
  }

  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.println(line);
    }
  }
  client.stop();
}

void handleRoot() {
  // Read the current state of the LED
  String ledState = (digitalRead(ledPin) == HIGH) ? "ON" : "OFF";

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Generate the HTML with dynamic LED state, temperature, and humidity
  String html = "<!DOCTYPE html><html><head><title>Home Automation</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>body { font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #f0f0f0; }";
  html += ".container { text-align: center; background: #fff; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  html += ".button { display: inline-block; margin: 10px; padding: 15px 25px; font-size: 16px; cursor: pointer; text-align: center; text-decoration: none; outline: none; color: #fff; background-color: #4CAF50; border: none; border-radius: 5px; box-shadow: 0 9px #999; }";
  html += ".button:hover { background-color: #3e8e41; }";
  html += ".button:active { background-color: #3e8e41; box-shadow: 0 5px #666; transform: translateY(4px); }";
  html += ".button.off { background-color: #f44336; }</style></head><body>";
  html += "<div class=\"container\"><h1>Home Automation</h1>";
  html += "<h2>Temperature: " + String(temperature) + " °C</h2>";
  html += "<h2>Humidity: " + String(humidity) + " %</h2>";
  html += "<button class=\"button\" onclick=\"sendData('LED_ON')\">Turn ON</button>";
  html += "<button class=\"button off\" onclick=\"sendData('LED_OFF')\">Turn OFF</button>";
  html += "</div><script>function sendData(command) { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/' + command, true); xhttp.send(); }</script></body></html>";

  server.send(200, "text/html", html);  // Send the updated HTML response
}

void handleLEDOn() {
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleLEDOff() {
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Serial.println();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int wifiConnectAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiConnectAttempts < 40) {  // Increased attempts to 40
    delay(500);
    Serial.print(".");
    wifiConnectAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected to WiFi. IP Address: ");
    Serial.println(WiFi.localIP());

    if (message_count < max_messages) {
      String ipMessage = "ESP8266 IP Address: " + WiFi.localIP().toString();
      sendSMS(ipMessage);
      message_count++;
    }

    dht.begin();  // Initialize the DHT sensor

    server.on("/", handleRoot);
    server.on("/LED_ON", handleLEDOn);
    server.on("/LED_OFF", handleLEDOff);

    server.begin();
    Serial.println("HTTP server started");
  } else {
    Serial.println();
    Serial.println("Failed to connect to WiFi");
  }
}

void loop() {
  server.handleClient();
}