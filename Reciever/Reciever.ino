#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

    // Right Wheel:
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);

  // Left Wheel:
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

int leftSpeed;
int rightSpeed;
char Vals[9] = "";

void loop() {
  if (radio.available()) {
    radio.read(&Vals, sizeof(Vals));
  }
  
  String ValsString(Vals);
  int ySpeed = ValsString.substring(0,4).toInt();
  int xSpeed = ValsString.substring(4,9).toInt();

  // y-axis logic
  if (ySpeed > 550) { // Forward
    rightSpeed = map(ySpeed,550,1023,0,255);
    leftSpeed = map(ySpeed,550,1023,0,255);

    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);

    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);   
  } else if (ySpeed < 470) { // Backwards
    rightSpeed = map(ySpeed,470,0,0,255);
    leftSpeed = map(ySpeed,470,0,0,255);

    digitalWrite(4, LOW);
    digitalWrite(2, HIGH);

    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
  } else { // Middle
    rightSpeed = 0;
    leftSpeed = 0;

    digitalWrite(2, LOW);
    digitalWrite(4, LOW);

    digitalWrite(5, LOW);
    digitalWrite(6, LOW);  
  }

  // x-axis logic
  if (xSpeed > 550) {
    rightSpeed = rightSpeed - map(xSpeed,550,1023,0,255);
    leftSpeed = leftSpeed + map(xSpeed,550,1023,0,255);
    
    if (ySpeed > 470 && ySpeed < 550) {
      digitalWrite(2, LOW);
      digitalWrite(4, HIGH);

      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
    }
  }
  if (xSpeed < 470) {
    rightSpeed = rightSpeed + map(xSpeed,470,0,0,255);
    leftSpeed = leftSpeed - map(xSpeed,470,0,0,255);

    if (ySpeed > 470 && ySpeed < 550) {
      digitalWrite(2, LOW);
      digitalWrite(4, HIGH);

      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
    }
  }

  if (rightSpeed > 255) {
    rightSpeed = 255;
  } else if (rightSpeed < 70) {
    rightSpeed = 0;
  }
  if (leftSpeed < 70) {
    leftSpeed = 0;
  } else if (leftSpeed > 255) {
    leftSpeed = 255;
  }
  
  analogWrite(9, rightSpeed);
  analogWrite(3, leftSpeed);
//  Serial.print("R:");
//  Serial.print(rightSpeed);
//  Serial.print(",L:");
//  Serial.println(leftSpeed);
}
