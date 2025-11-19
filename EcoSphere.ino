// #include <WiFi.h>
// #include <Adafruit_Sensor.h>
// #include <DHT.h>
// #include <WiFiClientSecure.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <PubSubClient.h>
// #include <ESP32Servo.h>

// const char* ssid = "Dialog 4G 287";  
// const char* password = "5E8Fd0f3";  
// const char* server = "mongodb+srv://thilakshanapiumika:yMt3CfdGuElIYAxb@sensordht.zhpwycr.mongodb.net/?retryWrites=true&w=majority&appName=sensorDHT";
// const char* mqtt_broker = "test.mosquitto.org";  // Public MQTT broker
// const int mqtt_port = 1883;
// const char* topic = "esp32/sensors";

// #define DHT11_PIN 4
// #define DHT22_PIN 5
// #define DHTTYPE11 DHT11
// #define DHTTYPE22 DHT22
// #define SERVO_PIN 25

// DHT dht11(DHT11_PIN, DHTTYPE11);
// DHT dht22(DHT22_PIN, DHTTYPE22);
// WiFiClient espClient;
// PubSubClient client(espClient);
// Servo windowServo;

// // **Function to Calculate Heat Index (NOAA Formula)**
// float calculateHeatIndex(float temp, float hum) {
//     float HI = 0.5 * (temp + 61.0 + ((temp - 68.0) * 1.2) + (hum * 0.094));
    
//     if (HI > 79) { 
//         HI = -42.379 + 2.04901523 * temp + 10.14333127 * hum 
//             - 0.22475541 * temp * hum - 0.00683783 * temp * temp 
//             - 0.05481717 * hum * hum + 0.00122874 * temp * temp * hum 
//             + 0.00085282 * temp * hum * hum - 0.00000199 * temp * temp * hum * hum;

//         if ((hum < 13) && (temp >= 80.0) && (temp <= 112.0)) {
//             HI -= ((13 - hum) * 0.25) * sqrt((17 - abs(temp - 95)) / 17);
//         } else if ((hum > 85) && (temp >= 80.0) && (temp <= 87.0)) {
//             HI += ((hum - 85) * 0.1) * ((87 - temp) * 0.2);
//         }
//     }
//     return HI;
// }

// String windowStatus = "Unknown";

// // **Function to Control Servo Motor (Window Control)**
// void controlWindow(float heatIndex) {
//     if (heatIndex > 35) {  
//         windowStatus = "Fully open window";
//         Serial.println(windowStatus);
//         windowServo.write(180);
//     } else if (heatIndex > 30) { 
//         windowStatus = "Open window partially";
//         Serial.println(windowStatus);
//         windowServo.write(90);
//     } else {
//         windowStatus = "Close window";
//         Serial.println(windowStatus);
//         windowServo.write(0);
//     }
// }

// void sendToMongoDB(float temp1, float hum1, float temp2, float hum2) {
//     WiFiClient client;
//     HTTPClient http;

//     http.begin(client, "http://192.168.8.161:5000/sensor-data");  // Replace with your server IP
//     http.addHeader("Content-Type", "application/json");

//     StaticJsonDocument<200> doc;
//     doc["DHT11_Temp"] = temp1;
//     doc["DHT11_Hum"] = hum1;
//     doc["DHT22_Temp"] = temp2;
//     doc["DHT22_Hum"] = hum2;

//     String jsonString;
//     serializeJson(doc, jsonString);

//     int httpResponseCode = http.POST(jsonString);
//     Serial.print("HTTP Response code: ");
//     Serial.println(httpResponseCode);

//     http.end();
// }


// void setup() {
//     Serial.begin(115200);
//     WiFi.begin(ssid, password);
//     client.setServer(mqtt_broker, mqtt_port);

//     Serial.print("Connecting to WiFi");
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.print(".");
//     }
//     Serial.println("\nConnected to WiFi!");

//     while (!client.connected()) {
//         Serial.println("Connecting to MQTT...");
//         if (client.connect("ESP32_Client")) {
//             Serial.println("Connected to MQTT broker!");
//         } else {
//             Serial.print("Failed, rc=");
//             Serial.print(client.state());
//             delay(5000);
//         }
//     }

//     Serial.println("ESP32 Started");
//     dht11.begin();
//     dht22.begin();
//     windowServo.attach(SERVO_PIN);
//     windowServo.write(0);  // Ensure window starts closed
//     delay(2000);
// }

// void publishMQTT(float temp1, float hum1, float temp2, float hum2) {
//     StaticJsonDocument<256> doc;
//     doc["DHT11_Temp"] = temp1;
//     doc["DHT11_Hum"] = hum1;
//     doc["DHT22_Temp"] = temp2;
//     doc["DHT22_Hum"] = hum2;
//     doc["Window_Status"] = windowStatus;

//     String jsonString;
//     serializeJson(doc, jsonString);

//     client.publish(topic, jsonString.c_str());
//     Serial.println("Published data to MQTT!");
// }

// void loop() {
//     float temp1 = dht11.readTemperature();
//     float hum1 = dht11.readHumidity();
//     float temp2 = dht22.readTemperature();
//     float hum2 = dht22.readHumidity();

//     if (isnan(temp1) || isnan(hum1) || isnan(temp2) || isnan(hum2)) {
//         Serial.println("Failed to read from DHT sensors!");
//     }else{
//       Serial.print("DHT11 - Temp: "); Serial.print(temp1); Serial.print("°C  Hum: "); Serial.print(hum1); Serial.println("%");
//       Serial.print("DHT22 - Temp: "); Serial.print(temp2); Serial.print("°C  Hum: "); Serial.print(hum2); Serial.println("%");
//     }

//     // **Calculate Heat Index (HI)**
//     float hi1 = calculateHeatIndex(temp1, hum1);
//     float hi2 = calculateHeatIndex(temp2, hum2);

//     Serial.print("DHT11 - Heat Index: "); Serial.println(hi1);
//     Serial.print("DHT22 - Heat Index: "); Serial.println(hi2);

//     // **Control Window (Servo Motor) Based on HI**
//     controlWindow(hi2);  // Use DHT22 HI for control (more accurate)

//     delay(2500);

//     if (!isnan(temp1) && !isnan(hum1) && !isnan(temp2) && !isnan(hum2)) {
//         sendToMongoDB(temp1, hum1, temp2, hum2);
//     }

//     if (!isnan(temp1) && !isnan(hum1) && !isnan(temp2) && !isnan(hum2)) {
//         publishMQTT(temp1, hum1, temp2, hum2);
//     }

//     delay(6000);
// }

#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "Galaxy A04s 483E";  
const char* password = "ngla7235";  

// MQTT broker
const char* mqtt_broker = "test.mosquitto.org";  
const int mqtt_port = 1883;
const char* topic = "esp32/sensors";

// MongoDB Server (via API)
const char* server = "mongodb+srv://thilakshanapiumika:yMt3CfdGuElIYAxb@sensordht.zhpwycr.mongodb.net/?retryWrites=true&w=majority&appName=sensorDHT";

// Sensor and actuator setup
#define DHT11_PIN 4
#define DHT22_PIN 5
#define DHTTYPE11 DHT11
#define DHTTYPE22 DHT22
#define SERVO_PIN 25

DHT dht11(DHT11_PIN, DHTTYPE11);
DHT dht22(DHT22_PIN, DHTTYPE22);
Servo windowServo;

WiFiClient espClient;
PubSubClient client(espClient);

String windowStatus = "Unknown";

// Heat Index Calculation
float calculateHeatIndex(float temp, float hum) {
    float HI = 0.5 * (temp + 61.0 + ((temp - 68.0) * 1.2) + (hum * 0.094));
    if (HI > 79) { 
        HI = -42.379 + 2.04901523 * temp + 10.14333127 * hum 
            - 0.22475541 * temp * hum - 0.00683783 * temp * temp 
            - 0.05481717 * hum * hum + 0.00122874 * temp * temp * hum 
            + 0.00085282 * temp * hum * hum - 0.00000199 * temp * temp * hum * hum;
        if ((hum < 13) && (temp >= 80.0) && (temp <= 112.0)) {
            HI -= ((13 - hum) * 0.25) * sqrt((17 - abs(temp - 95)) / 17);
        } else if ((hum > 85) && (temp >= 80.0) && (temp <= 87.0)) {
            HI += ((hum - 85) * 0.1) * ((87 - temp) * 0.2);
        }
    }
    return HI;
}

// Window control based on HI
void controlWindow(float heatIndex) {
    if (heatIndex > 35) {  
        windowStatus = "Fully open window";
        Serial.println(windowStatus);
        windowServo.write(180);
    } else if (heatIndex > 30) { 
        windowStatus = "Open window partially";
        Serial.println(windowStatus);
        windowServo.write(90);
    } else {
        windowStatus = "Close window";
        Serial.println(windowStatus);
        windowServo.write(0);
    }
}

// Send sensor data to MongoDB server (via HTTP POST)
void sendToMongoDB(float temp1, float hum1, float temp2, float hum2) {
    WiFiClient client;
    HTTPClient http;666666666666666666666666666666666

    http.begin(client, "http://192.168.118.181:5000/sensor-data");  // Replace with your server IP
    http.addHeader("Content-Type", "application/json");
66666666666666666666
    StaticJsonDocument<200> doc;
    doc["DHT11_Temp"] = temp1;
    doc["DHT11_Hum"] = hum1;
    doc["DHT22_Temp"] = temp2;
    doc["DHT22_Hum"] = hum2;

    String jsonString;
    serializeJson(doc, jsonString);

    int httpResponseCode = http.POST(jsonString);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
}

// Publish sensor data to MQTT
void publishMQTT(float temp1, float hum1, float temp2, float hum2) {
    StaticJsonDocument<256> doc;
    doc["DHT11_Temp"] = temp1;
    doc["DHT11_Hum"] = hum1;
    doc["DHT22_Temp"] = temp2;
    doc["DHT22_Hum"] = hum2;
    doc["Window_Status"] = windowStatus;

    String jsonString;
    serializeJson(doc, jsonString);

    client.publish(topic, jsonString.c_str());
    Serial.println("Published data to MQTT!");
}

// Callback when a message arrives (subscriber functionality)
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  float dht11_temp = doc["DHT11_Temp"];
  float dht11_hum = doc["DHT11_Hum"];
  float dht22_temp = doc["DHT22_Temp"];
  float dht22_hum = doc["DHT22_Hum"];
  const char* window_status = doc["Window_Status"];

  Serial.println("Received Sensor Data (from MQTT):");
  Serial.print("DHT11 - Temperature: "); Serial.print(dht11_temp); Serial.println("°C");
  Serial.print("DHT11 - Humidity: "); Serial.print(dht11_hum); Serial.println("%");
  Serial.print("DHT22 - Temperature: "); Serial.print(dht22_temp); Serial.println("°C");
  Serial.print("DHT22 - Humidity: "); Serial.print(dht22_hum); Serial.println("%");
  Serial.print("Window Status: "); Serial.println(window_status);
}

// MQTT Reconnect
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Client")) {
      Serial.println("connected");
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    dht11.begin();
    dht22.begin();
    windowServo.attach(SERVO_PIN);
    windowServo.write(0); // Start closed
    delay(2000);
}

unsigned long lastPublishTime = 0;
const long publishInterval = 60000; // 60 seconds publishing

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastPublishTime > publishInterval) {
        lastPublishTime = now;

        float temp1 = dht11.readTemperature();
        float hum1 = dht11.readHumidity();
        float temp2 = dht22.readTemperature();
        float hum2 = dht22.readHumidity();

        if (isnan(temp1) || isnan(hum1) || isnan(temp2) || isnan(hum2)) {
            Serial.println("Failed to read from DHT sensors!");
            return;
        } else {
            Serial.print("DHT11 - Temp: "); Serial.print(temp1); Serial.print("°C  Hum: "); Serial.print(hum1); Serial.println("%");
            Serial.print("DHT22 - Temp: "); Serial.print(temp2); Serial.print("°C  Hum: "); Serial.print(hum2); Serial.println("%");
        }

        float hi1 = calculateHeatIndex(temp1, hum1);
        float hi2 = calculateHeatIndex(temp2, hum2);

        Serial.print("DHT11 - Heat Index: "); Serial.println(hi1);
        Serial.print("DHT22 - Heat Index: "); Serial.println(hi2);

        controlWindow(hi2); // Use DHT22 HI for control

        sendToMongoDB(temp1, hum1, temp2, hum2);
        publishMQTT(temp1, hum1, temp2, hum2);

        delay(10000);
    }
}
