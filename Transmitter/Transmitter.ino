#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Defining the joystick
#define xVal A0
#define yVal A1

// Declaring the radio
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
//  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  String X = String(analogRead(xVal));
  String Y = String(analogRead(yVal));

  while (X.length() < 4) {
    X = "0" + X;
  }

  while (Y.length() < 4) {
    Y = "0" + Y;
  }

  String val = X + Y;
  char Vals[9];
  val.toCharArray(Vals, 9);
  radio.write(&Vals, sizeof(Vals));
//  Serial.println(Vals);
  delay(100);
}
