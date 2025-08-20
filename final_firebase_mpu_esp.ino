#include <Arduino.h>

// Wi-Fi credentials
#define WIFI_SSID "..."
#define WIFI_PASSWORD "..."

// Firebase libraries
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>

#define DATABASE_URL "..."  
#define FIREBASE_AUTH "..."
#define API_KEY "..."
#define USER_EMAIL "..."
#define USER_PASSWORD "..."

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// MPU-6500
#include <MPU9250_WE.h>
#include <Wire.h>
#define MPU9250_ADDR 0x68
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

// Motor pins
// int motor1pin1 = 0;
// int motor1pin2 = 2;
// int motor2pin1 = 12;
// int motor2pin2 = 13;

unsigned long sendDataPrevMillis = 0;

//********************************************************************

void setup() {
  
  Serial.begin(9600);

  // Motor pin setup
  // pinMode(motor1pin1, OUTPUT);
  // pinMode(motor1pin2, OUTPUT);
  // pinMode(motor2pin1, OUTPUT);
  // pinMode(motor2pin2, OUTPUT);

  // MPU setup
  Wire.begin();
  if (!myMPU9250.init()) {
    Serial.println("MPU9250 does not respond");
  } else {
    Serial.println("MPU9250 is connected");
  }

  Serial.println("Calibrating MPU...");
  delay(200);
  myMPU9250.autoOffsets();
  Serial.println("Calibration done!");

  myMPU9250.setSampleRateDivider(5);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
}

//********************************************************************

void loop() {

  // Read MPU sensor data
  xyzFloat gValue = myMPU9250.getGValues();
  xyzFloat gyr = myMPU9250.getGyrValues();
  float resultantG = myMPU9250.getResultantG(gValue);

  // Calculate Roll, Pitch (in degrees)
  float roll = atan2(gValue.y, gValue.z) * 180 / PI;
  float pitch = atan2(-gValue.x, sqrt(gValue.y * gValue.y + gValue.z * gValue.z)) * 180 / PI;
  float yaw = gyr.z; // Approximate, for stable yaw you'll need magnetometer data

  // Determine status
  String status;
  if (roll > 20 || roll < -20) {
    status = "UNSTABLE";
  } else if (pitch > 15 || pitch < -15) {
    status = "INCLINE";
  } else {
    status = "STABLE";
  }

  // Send data to Firebase every 100 ms
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setFloat(&fbdo, "Acc/ResultantG", resultantG);
    Firebase.RTDB.setFloat(&fbdo, "Gyro/X", gyr.x);
    Firebase.RTDB.setFloat(&fbdo, "Gyro/Y", gyr.y);
    Firebase.RTDB.setFloat(&fbdo, "Gyro/Z", gyr.z);
    Firebase.RTDB.setFloat(&fbdo, "Orientation/Roll", roll);
    Firebase.RTDB.setFloat(&fbdo, "Orientation/Pitch", pitch);
    Firebase.RTDB.setFloat(&fbdo, "Orientation/Yaw", yaw);
    Firebase.RTDB.setString(&fbdo, "Status", status);

    Serial.print("Roll:"); Serial.print(roll);
    Serial.print("Pitch:"); Serial.print(pitch);
    Serial.print(" Yaw:"); Serial.println(yaw);

  }
}
