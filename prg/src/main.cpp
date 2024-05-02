#include <Arduino.h>
#include <ArduinoJson.h>

#include "max6675.h"


// Settings
static const uint16_t timer_devider = 80; // Clock tics at 1 MHz
static const uint64_t timer_max_count = 1000; // 1 ms period
static const uint16_t pwm_max_count = 1000;

//Mutex
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


// Globals
static hw_timer_t *timer = NULL;
static volatile uint16_t pwm_counter = 0;

//******************************************************************************
//Interrupt Service Routines (ISRs)

void IRAM_ATTR onTimer() {

  portENTER_CRITICAL_ISR(&timerMux);
  ++pwm_counter;
   portEXIT_CRITICAL_ISR(&timerMux);

}

int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;

const int pin = 22;  // GPIO pin number

const int pwm_pin = 18; //PWM pin

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

StaticJsonDocument<200> doc; 

void setup() {
  pinMode(pin, OUTPUT);  // Set the GPIO as an output
  pinMode(pwm_pin, OUTPUT);  // Set the PWM pin as an output
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  doc["sensor"] = "K-Thermocouple";
  doc["unit"] = "°C";
  delay(500);

  //Timer setup for PWM
  timer = timerBegin(0, timer_devider, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, timer_max_count, true);
  timerAlarmEnable(timer);

}

const float AVG = 0.25;
unsigned int temp_index = 0;
float temperature[4] = {0, 0, 0, 0};

int pin_state = 0;
uint16_t pwm_value = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //doc["value"] = thermocouple.readCelsius();
  //temperature[temp_index % 4] = thermocouple.readCelsius()*AVG;
  //serializeJson(doc, Serial);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  if (Serial.available() > 0) {  // Check if data is available to read
    char received = Serial.read(); // Read the incoming byte
    if (received == 's') {
      digitalWrite(pin, HIGH);  // Set the GPIO high
      pin_state = 1;
    } else if (received == 'e') {
      digitalWrite(pin, LOW);   // Set the GPIO low
      pin_state = 0;
    } else if (received == 'i') {
      digitalWrite(pin, LOW);   // Increment pwm value
      pin_state = 0;
    } else if (received == 'd') {
      digitalWrite(pin, LOW);   // Decrement pwm value
      pin_state = 0;
    }
  }
  
  //PWM
  portENTER_CRITICAL(&timerMux);
  if (pwm_counter < pwm_max_count) {
    
    if (pwm_counter >= pwm_value) {

      digitalWrite(pwm_pin, LOW);

    } else {

      digitalWrite(pwm_pin, HIGH);

    }
  } else {

    pwm_counter = 0;

  }
  portEXIT_CRITICAL(&timerMux);

  // Start a new line
  //Serial.println();
  /*if((temp_index % 4) == 0) {
    Serial.print(temperature[0] + temperature[1] + temperature[2] + temperature[3]);
    Serial.print(";");
    Serial.print(pin_state);
    Serial.println();
  }
  ++temp_index;*/
  //vTaskDelay(250/portTICK_PERIOD_MS);
}

