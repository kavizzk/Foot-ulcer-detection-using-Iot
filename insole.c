// Define Blynk Template ID and Name
#define BLYNK_TEMPLATE_ID "TMPL3mIIQhKuo"
#define BLYNK_TEMPLATE_NAME "insole"
#define BLYNK_AUTH_TOKEN "H9fLE7xlcNq305fBf_NhbHOQ9Z9dzKFD"

// Include libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
const char* ssid = "Kavitha";
const char* password = "kavi2004";

// Define pins
#define DHTPIN 27      
#define HEEL_FSR_PIN 35 
#define META_FSR_PIN 32 

// Sensor thresholds
#define TEMPERATURE_THRESHOLD 37.0  
#define HUMIDITY_THRESHOLD 60      
#define PRESSURE_THRESHOLD 500     

// DHT11 setup
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

// Function to check sensors
void checkSensors() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read pressure values from FSR sensors
  int heelPressure = analogRead(HEEL_FSR_PIN);
  int metaPressure = analogRead(META_FSR_PIN);

  // Send readings to Blynk
  Blynk.virtualWrite(V0, temperature); 
  Blynk.virtualWrite(V1, humidity);  
  Blynk.virtualWrite(V2, heelPressure); 
  Blynk.virtualWrite(V3, metaPressure); 

  // Display readings in Serial Monitor
  Serial.println("Temperature: " + String(temperature) + " °C");
  Serial.println("Humidity: " + String(humidity) + " %");
  Serial.println("Heel Pressure: " + String(heelPressure));
  Serial.println("Metatarsal Pressure: " + String(metaPressure));

  // Check thresholds and send alerts
  if (temperature > TEMPERATURE_THRESHOLD) {
    Blynk.logEvent("high_temperature", "ALERT!");
    Blynk.setProperty(V0, "color", "#FF0000");
  } else {
    Blynk.setProperty(V0, "color", "#00FF00");
  }

  if (humidity > HUMIDITY_THRESHOLD) {
    Blynk.logEvent("high_humidity", "ALERT!");
  }

  if (heelPressure > PRESSURE_THRESHOLD) {
    Blynk.logEvent("high_heel_pressure", "ALERT!");
  }

  if (metaPressure > PRESSURE_THRESHOLD) {
    Blynk.logEvent("high_metatarsal_pressure", "ALERT!");
  }

  // Log general event for insole check
  Blynk.logEvent("insole", "foot ulcer detected slightly, Risk of ulcer!!");
  Serial.println(); 
}

// BLYNK_WRITE function to handle commands from Blynk app
BLYNK_WRITE(V0) {
  int userCommand = param.asInt(); 
  if (userCommand == 1) {
    Serial.println("Manual Check Initiated from App");
    checkSensors();
  }
}

void setup() {
  // Start Serial and DHT
  Serial.begin(115200);
  dht.begin();
  
  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Set up a timer to check sensor data every 5 seconds
  timer.setInterval(5000L, checkSensors);

  Serial.println("Diabetic Foot Ulcer Monitoring System Initialized.");
}

void loop() {
  Blynk.run();
  timer.run();
}
