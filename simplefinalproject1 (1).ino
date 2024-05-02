#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include<ESP32_Servo.h>
//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Shantanu"
#define WIFI_PASSWORD "sHantanu@892"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyApW649AuleAHtVyFAbiHJn-WgRNPBsasY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://atomicpaw-b3179-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

// NTP Server config
const char* ntpServer = "pool.ntp.org";
struct tm t;
String Hr1,Hr2,Hr3,Hr4,Min1,Min2,Min3,Min4;
int h1,h2,h3,h4,m1,m2,m3,m4;
void printLocalTime();
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//Define Pins
const int trigPin = 25;
int sensorPin= 33;
int thresholdDown = 2000;
const int echoPin = 26;
long duration,cm;

void printLocalTime();

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

void setup()
{

  Serial.begin(115200);
 // digitalWrite(27, HIGH);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  pinMode(27,OUTPUT);
  // Sets the echoPin as an Input
delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
myservo1.attach(14);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getLocalTime(&t);
  printLocalTime();


  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;



  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(DATABASE_URL, API_KEY);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
 // Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

}

void loop()
{
     digitalWrite(27, HIGH);
   
  if (Firebase.ready()) 
  {
    
    //Firebase.setInt(fbdo, main, 5);
//    Firebase.setInt(fbdo, "/test/a", x);
//    Firebase.setInt(fbdo, "/test/b", y);
//    delay(200);

   if (Firebase.getString(fbdo, "/Compartment/Hr1")) {
  Hr1 = fbdo.stringData();
  Serial.println("Retrieved string value (c1 h1): " + Hr1);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}
    
     if (Firebase.getString(fbdo, "/Compartment/Hr2")) {
 Hr2 = fbdo.stringData();
  Serial.println("Retrieved string value (c2 h2): " + Hr2);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

 if (Firebase.getString(fbdo, "/Compartment/Hr3")) {
  Hr3 = fbdo.stringData();
  Serial.println("Retrieved string value (c3 h3): " + Hr3);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

 if (Firebase.getString(fbdo, "/Compartment/Hr4")) {
  Hr4 = fbdo.stringData();
  Serial.println("Retrieved string value (c4 h4): " + Hr4);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

 if (Firebase.getString(fbdo, "/Compartment/Min1")) {
  Min1 = fbdo.stringData();
  Serial.println("Retrieved string value (C1 m1): " + Min1);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

 if (Firebase.getString(fbdo, "/Compartment/Min2")) {
  Min2 = fbdo.stringData();
  Serial.println("Retrieved string value (C2 m2): " + Min2);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

 if (Firebase.getString(fbdo, "/Compartment/Min3")) {
  Min3 = fbdo.stringData();
  Serial.println("Retrieved string value(C3 m3): " + Min3);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

 if (Firebase.getString(fbdo, "/Compartment/Min4")) {
  Min4 = fbdo.stringData();
  Serial.println("Retrieved string value(C4 M4): " + Min4);
} else {
  Serial.println("Failed to retrieve string value. Reason: " + fbdo.errorReason());
}

h1 = Hr1.toInt();
h2 = Hr2.toInt();
h3 = Hr3.toInt();
h4 = Hr4.toInt();
m1 = Min1.toInt();
m2 = Min2.toInt();
m3 = Min3.toInt();
m4 = Min4.toInt();
 
  delay(2000);
  }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  cm= duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  

  Serial.print("Distance in cm: ");
    Serial.print(cm);

  printLocalTime();
  delay(3000);
    if(cm>=10 && cm<=20 && sensorValue > thresholdDown){
      Serial.println("The relay is triggered");
    digitalWrite(27, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(5000);                       // wait for a second
  digitalWrite(27, HIGH);    // turn the LED off by making the voltage LOW 
  delay(10000);
    }
    else {
      Serial.println("The bowl is full/ no pet detected");
      }
     if(t.tm_hour==h1 && t.tm_min==m1)
     {
      pos1=150;
      Serial.print("Compartment 1 Opened");
      myservo1.write(pos1);
     }
      if(t.tm_hour==h1+1 && t.tm_min==m1)
     {
      pos1=0;
      Serial.print("Compartment 1 closed");
      myservo1.write(pos1);
     }
 if(t.tm_hour==h2 && t.tm_min==m2)
     {
      pos2=150;
      Serial.print("Compartment 2 Opened");
      myservo2.write(pos2);
     }
      if(t.tm_hour==h2+1 && t.tm_min==m2)
     {
      pos2=0;
      Serial.print("Compartment 2 closed");
      myservo2.write(pos2);
     }
if(t.tm_hour==h3 && t.tm_min==m3)
     {
      pos3=150;
      Serial.print("Compartment 3 opened");
      myservo3.write(pos3);
     }
      if(t.tm_hour==h3+1 && t.tm_min==m3)
     {
      pos3=0;
      Serial.print("Compartment 3 closed");
      myservo3.write(pos3);
     }
     if(t.tm_hour==h4 && t.tm_min==m4)
     {
      pos4=150;
      Serial.print("Compartment 4 Opened");
      myservo4.write(pos4);
     }
      if(t.tm_hour==h4+1 && t.tm_min==m4)
     {
      pos4=0;
      Serial.print("Compartment 4 closed");
      myservo4.write(pos4);
     }

    }

void printLocalTime(){
  if(!getLocalTime(&t)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&t, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&t, "%A");
  Serial.print("Month: ");
  Serial.println(&t, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&t, "%d");
  Serial.print("Year: ");
  Serial.println(&t, "%Y");
  Serial.print("Hour: ");
  Serial.println(&t, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&t, "%I");
  Serial.print("Minute: ");
  Serial.println(&t, "%M");
  Serial.print("Second: ");
  Serial.println(&t, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &t);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &t);
  Serial.println(timeWeekDay);
  Serial.println();
  delay(1000);
}
