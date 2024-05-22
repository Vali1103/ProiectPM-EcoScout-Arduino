#include <SoftwareSerial.h>
#include <AFMotor.h>
#include "DHT.h"

//initial motors pin
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

SoftwareSerial btSerial(11, 10); // RX | TX

int val;
int Speeed = 255;

// timer
unsigned long startTime;
const unsigned long interval = 5000; // 5 seconds

#define obstacle 13
#define Buzzer A5
#define DHTPIN A3   // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define led_stanga A1
#define led_dreapta A2

DHT dht(DHTPIN, DHTTYPE);

int gradient_st = 255;
int gradient_dr = 255;

int status_st = 0;
int status_dr = 0;

bool blinkActive_st = false;
bool blinkActive_dr = false;
unsigned long lastBlinkTime_st = 0;
unsigned long lastBlinkTime_dr = 0;
int blinkCount_st = 0;
int blinkCount_dr = 0;
const unsigned long blinkInterval = 500; // Interval between blinks in milliseconds

void setup() 
{
  Serial.begin(9600);

  btSerial.begin(38400);      
  Serial.println("btSerial started at 38400");

  pinMode(obstacle, INPUT);

  // temp
  dht.begin();

  // timer start
  startTime = millis();

  // leds
  pinMode(led_stanga, OUTPUT);
  pinMode(led_dreapta, OUTPUT);

}
 
void loop()
{
  // Keep reading from Arduino Serial Monitor and send to HC-05
  // if (Serial.available())
  // {
  //   btSerial.write(Serial.read()); 
  // }

  // mq-135
  int sensorValue = analogRead(A0);
  // merge
  if(sensorValue > 500){
      analogWrite(Buzzer, 255);
    } else {
      analogWrite(Buzzer, 0);
    }
  // temp
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (millis() - startTime >= interval) {
    // Reset the start time to create a continuous 5-second timer
    startTime = millis();
    btSerial.write("T");
    btSerial.write(h);

    btSerial.write(t);

    btSerial.write(f);

    btSerial.write(sensorValue);

  }

  int statusP = digitalRead(obstacle);
  if (btSerial.available()) {
    char val = btSerial.read();
    Serial.write(val);
    switch(val) {
      case 'F':
        if (statusP == 1) {
          forward();
        } else {
          Stop();
        }
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'I':
        topright();
        break;
      case 'J':
        topleft();
        break;
      case 'K':
        bottomright();
        break;
      case 'M':
        bottomleft();
        break;
      case 'S':
        Stop();
        break;
      case 'l':
        semnalizare_stanga();
        break;
      case 'r':
        semnalizare_dreapta();
        break;
      case 'b':
        claxon();
        break;
      default:
        Stop();
        break;
    }
  }

  // Handle left blinker
    if (blinkActive_st && millis() - lastBlinkTime_st >= blinkInterval) {
        toggleBlinker(led_stanga, status_st);
        lastBlinkTime_st = millis();
        if (++blinkCount_st >= 6) { // 3 full blinks, on and off counted separately
            blinkActive_st = false;
            blinkCount_st = 0;
        }
    }

    // Handle right blinker
    if (blinkActive_dr && millis() - lastBlinkTime_dr >= blinkInterval) {
        toggleBlinker(led_dreapta, status_dr);
        lastBlinkTime_dr = millis();
        if (++blinkCount_dr >= 6) { // 3 full blinks, on and off counted separately
            blinkActive_dr = false;
            blinkCount_dr = 0;
        }
    }
  
  delay(60);
}

void toggleBlinker(int pin, int &status) {
    if (status == 0) {
        analogWrite(pin, gradient_st);
        status = 1;
    } else {
        analogWrite(pin, 0);
        status = 0;
    }
}
            
void semnalizare_stanga() {
    blinkActive_st = true;
    lastBlinkTime_st = millis() - blinkInterval; // Start immediately
}

void semnalizare_dreapta() {
    blinkActive_dr = true;
    lastBlinkTime_dr = millis() - blinkInterval; // Start immediately
}

void claxon() {
  analogWrite(Buzzer,255);
}
  
  
void forward()
{
  motor1.setSpeed(Speeed); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(Speeed); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(Speeed);//Define maximum velocity
  motor3.run(FORWARD); //rotate the motor clockwise
  motor4.setSpeed(Speeed);//Define maximum velocity
  motor4.run(FORWARD); //rotate the motor clockwise
}
  
void back()
{
  motor1.setSpeed(Speeed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(Speeed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(Speeed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(Speeed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}
  
void left()
{
  motor1.setSpeed(Speeed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(Speeed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(Speeed); //Define maximum velocity
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.setSpeed(Speeed); //Define maximum velocity
  motor4.run(FORWARD);  //rotate the motor clockwise
}
  
void right()
{
  motor1.setSpeed(Speeed); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(Speeed); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(Speeed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(Speeed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}
  
void topleft(){
  motor1.setSpeed(Speeed); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(Speeed); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(Speeed/3.1);//Define maximum velocity
  motor3.run(FORWARD); //rotate the motor clockwise
  motor4.setSpeed(Speeed/3.1);//Define maximum velocity
  motor4.run(FORWARD); //rotate the motor clockwise
}
  
void topright()
{
  motor1.setSpeed(Speeed/3.1); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(Speeed/3.1); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(Speeed);//Define maximum velocity
  motor3.run(FORWARD); //rotate the motor clockwise
  motor4.setSpeed(Speeed);//Define maximum velocity
  motor4.run(FORWARD); //rotate the motor clockwise
}
  
void bottomleft()
{
  motor1.setSpeed(Speeed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(Speeed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(Speeed/3.1); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(Speeed/3.1); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}
  
void bottomright()
{
  motor1.setSpeed(Speeed/3.1); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(Speeed/3.1); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(Speeed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(Speeed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}
  
  
void Stop()
{
  motor1.setSpeed(0); //Define minimum velocity
  motor1.run(RELEASE); //stop the motor when release the button
  motor2.setSpeed(0); //Define minimum velocity
  motor2.run(RELEASE); //rotate the motor clockwise
  motor3.setSpeed(0); //Define minimum velocity
  motor3.run(RELEASE); //stop the motor when release the button
  motor4.setSpeed(0); //Define minimum velocity
  motor4.run(RELEASE); //stop the motor when release the button
}


/*
Slave Command:
AT
AT+RMAAD
AT+ROLE=0
AT+UART=38400,0,0
AT+ADDR


Master Commands:
AT
AT+RMAAD
AT+ROLE=1
AT+UART=38400,0,0
AT+CMODE=0
AT+BIND=98D3:61:F613EA  (This should be your HC05 Slave address)
*/