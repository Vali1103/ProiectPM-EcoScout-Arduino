#include <SoftwareSerial.h>

int mid =10;
int z1 =0;
int z2 = 0;

const byte bluePin = 4;
const byte redPin = 2;
const byte greenPin = 3;

const int buttonPinSt = 5;
const int buttonPinDr = 6;

int stareSt = 0;
int stareDr = 0;
SoftwareSerial btSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  btSerial.begin(38400);
  Serial.println("btSerial started at 38400");
  // joystick
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);

  z1 = analogRead(A0); 
  z2 = analogRead(A1); 

  // RGB directie
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  // Semnalizari directii
  pinMode(buttonPinSt, INPUT);
  pinMode(buttonPinDr, INPUT);

}

void loop() {

  int xAxis = analogRead(A0); // Read Joysticks X-axis
  int yAxis = analogRead(A1); // Read Joysticks Y-axis
  int bt = analogRead(A2);

  // // Resetare culori
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);

  // // Citire valoare buton semnalizare apoi trimitere
  stareSt = digitalRead(buttonPinSt);
  stareDr = digitalRead(buttonPinDr);

  if (stareSt == HIGH) {
    // Serial.println("Trimite Semnalizare STANGA");
    btSerial.write("l");
  } 
  if (stareDr == HIGH) {
    // Serial.println("Trimite Semnalizare DREAPTA");
    btSerial.write('r');
  }

  

  // Serial.print("Axa X ");
  // Serial.println(xAxis);
  // Serial.print("Axa Y ");
  // Serial.println(yAxis);

  // // Serial.print("Button ");
  // // Serial.println(bt);

  // // Send directions
  if(bt == 0){
    btSerial.write('b');
  }
  if (xAxis < z1 - mid && yAxis < z2 - mid) {
    btSerial.write('I'); // Top right
    analogWrite(redPin, 255);
    analogWrite(greenPin, 128);
    analogWrite(bluePin, 255); // Magenta
  } else if (xAxis < z1 - mid && yAxis > z2 + mid) {
    btSerial.write('J'); // Top left
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 128); // Pink
  } else if (xAxis > z1 + mid && yAxis < z2 - mid) {
    btSerial.write('K'); // Bottom right
    analogWrite(redPin, 128);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255); // Cyan
  } else if (xAxis > z1 + mid && yAxis > z2 + mid) {
    btSerial.write('M'); // Bottom left
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 64); // Light Yellow
  } else if (xAxis < z1 - mid) {
    btSerial.write('F'); // Forward
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255); // White
  } else if (xAxis > z1 + mid) {
    btSerial.write('B'); // Backward
    analogWrite(redPin, 255); // Red
  } else if (yAxis < z2 - mid) {
    btSerial.write('L'); // Right
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255); // Yellow
  } else if (yAxis > z2 + mid) {
    btSerial.write('R'); // Rigth
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255); // Yellow
  }
  if (xAxis <= 510 and xAxis >= 500 and yAxis >= 500 and yAxis <= 510) {
    btSerial.write("S");
  }
  
  
  // if (Serial.available()) {
  //   btSerial.write(Serial.read());
  // }

  // // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (btSerial.available()) {
    char t = btSerial.read();
    // Serial.println(t);
    switch(t) {
      case 'T':
        int hum = btSerial.read();
        Serial.print(F("Humidity: "));
        Serial.print(hum);
        int temp = btSerial.read();
        Serial.print(F("%  Temperature: "));
        Serial.print(temp);
        Serial.print(F("°C "));
        int cv = btSerial.read();
        Serial.println(cv);

        int sensorValueHigh = btSerial.read();
        int sensorValueLow = btSerial.read();
        int sensorValue = (sensorValueHigh << 8) | sensorValueLow;
        Serial.print("PPM: ");
        Serial.println(sensorValue);
    }
  }
  delay(100);
}

