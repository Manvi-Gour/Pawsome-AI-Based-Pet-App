#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif


#include <Arduino_FreeRTOS.h>
#include <Stepper.h>
#include <Wire.h>
#include <ds3231.h>
int trigPin = 2;    // Trigger
int echoPin = 3;    // Echo
int sensorPin= A3;
int thresholdDown = 310;
long duration, cm;
struct ts t; 
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 9, 10, 11, 12);
// define two tasks for Blink & AnalogRead
void TaskWaterdisp( void *pvParameters );
void Taskfooddisp( void *pvParameters );
static int outputValue=100;
 
// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskWaterdisp
    , NULL   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    Taskfooddisp
    ,  NULL   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskWaterdisp(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

   pinMode(13, OUTPUT);
    pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // initialize digital LED_BUILTIN on pin 13 as an output.
 
  for (;;) // A Task shall never return or exit.
  {  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
   pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1; 
  int sensorValue = analogRead(sensorPin);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
       vTaskDelay(outputValue/portTICK_PERIOD_MS);  // one tick delay (15ms) in between reads for stability
      if(cm>=10 && cm<=20 && sensorValue > thresholdDown){
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW 
  delay(5000);
    }  
    
      }
    
      // loop from the highest pin to the lowest:
    
        vTaskDelay(outputValue/portTICK_PERIOD_MS);  // one tick delay (15ms) in between reads for stability
        // turn the pin off:
        
}

void Taskfooddisp(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
myStepper.setSpeed(60);
  Wire.begin();
  /*DS3231_init(DS3231_CONTROL_INTCN);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
 /*t.hour=13; 
  t.min=23;
  t.sec=50;
  t.mday=27;
  t.mon=3;
  t.year=2022; 
 
  DS3231_set(t);
 
  delay(2000);
  Serial.println("Setup was running"); */
  for (;;)
  {
    DS3231_get(&t);
    vTaskDelay(10); // one tick delay (15ms) in between reads for stability
    Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);
  delay(1000);

  if((t.hour==8 && t.min==43 && t.sec==10) || (t.hour==11 && t.min==30 && t.sec==10) || (t.hour==20 && t.min==00 && t.sec==10)){
  myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  myStepper.step(stepsPerRevolution);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW 
  delay(1000);
  }

  }
}
