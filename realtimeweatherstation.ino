#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

// WiFi Credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Define Pins
#define DHTPIN D4  // DHT11 sensor pin
#define DHTTYPE DHT11  // DHT Type
#define LDRPIN A0  // LDR connected to Analog pin A0
#define RAINPIN D3  // Rain sensor connected to GPIO D3

// Initialize DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// Create Async Web Server on port 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize sensors
  dht.begin();
  pinMode(LDRPIN, INPUT);
  pinMode(RAINPIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected.");
  Serial.print("ESP8266 IP Address: ");
  Serial.println(WiFi.localIP());

  // Web Server Route
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int ldrValue = analogRead(LDRPIN);
    int rainStatus = digitalRead(RAINPIN); // 0 = Rain Detected, 1 = No Rain

    String response = "<html><head><title>Weather Station</title></head><body>";
    response += "<h1>ESP8266 Weather Station</h1>";
    response += "<p><b>Temperature:</b> " + String(temperature) + "°C</p>";
    response += "<p><b>Humidity:</b> " + String(humidity) + "%</p>";
    response += "<p><b>Light Intensity (LDR):</b> " + String(ldrValue) + "</p>";
    response += "<p><b>Rain Status:</b> " + (rainStatus == 0 ? "Rain Detected" : "No Rain") + "</p>";
    response += "</body></html>";

    request->send(200, "text/html", response);
  });

  // Start Server
  server.begin();
}

void loop() {
  // No need for code in loop() since the web server handles requests asynchronously
}
