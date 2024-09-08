#define BLYNK_TEMPLATE_ID "TMPL6FfLnUPfe"
#define BLYNK_TEMPLATE_NAME "smart waste bin"
#define BLYNK_AUTH_TOKEN "D2LcHHDbsgouEVOFpxgtRpmsSguxhxWs"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>

// Blynk credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "GAYANGA";
char pass[] = "Gayangaa";


#define VPIN_DISTANCE V0  // Virtual Pin V0

// Max distance in centimeters//here i am mentioning the maximum distance is 50cm
#define MAX_DISTANCE 50


Adafruit_VL53L0X lox = Adafruit_VL53L0X();

BlynkTimer timer;

void setup() {
  
  Serial.begin(115200);
  
  Blynk.begin(auth, ssid, pass);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  
  Serial.println(F("VL53L0X sensor started"));

  
  timer.setInterval(1000L, sendDistanceToBlynk);
}

void loop() {
  
  Blynk.run();
  timer.run();
}


void sendDistanceToBlynk() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); 
  
  if (measure.RangeStatus != 4) {  
    int distanceMm = measure.RangeMilliMeter;
    float distanceCm = distanceMm / 10.0;  
    float displayValue = 0;  
    
    if (distanceCm >= MAX_DISTANCE) {
      displayValue = 0;  
    } else {
      
      displayValue = MAX_DISTANCE - distanceCm;
    }
    
    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
    Serial.print("Displayed value: ");
    Serial.println(displayValue);

  
    Blynk.virtualWrite(VPIN_DISTANCE, displayValue);
  } else {
    Serial.println("Out of range");
  }
}
