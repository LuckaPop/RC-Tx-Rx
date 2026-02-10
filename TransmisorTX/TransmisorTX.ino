#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "29235";
struct Signal {
  byte pitch;
  byte roll;
  byte yaw;
  byte motor;
};
Signal data;
const int motSw = 3;



void setup() {
  radio.begin(); 
  radio.openWritingPipe(address); 
  radio.setChannel(76); 
  radio.setAutoAck(false); 
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_HIGH); 
  radio.stopListening();
  
  pinMode(motSw, INPUT_PULLUP);
  data.pitch = 127;
  data.roll = 127;
  data.yaw = 127;
  data.motor = 0;
}

int Border_Map(int val, int lower, int middle, int upper, bool reverse){
  val = constrain(val, lower, upper);
  if (val < middle) val = map(val, lower, middle, 0, 128);
  else val = map(val, middle, upper, 128, 255);
  return (reverse ? 255 - val : val);
}

void loop() {
  int valP = analogRead(A1);
  int valR = analogRead(A0);
  int valY = analogRead(A3);
  int valM = analogRead(A2);


  data.pitch = map(valP, 0, 1023, 0 ,180);
  data.roll = map(valR, 0, 1023, 180 ,0);
  data.yaw = map(valY, 0, 1023, 0 ,180);

  if (digitalRead(motSw) == HIGH) {
  data.motor = 0;
  } else {
    data.motor = Border_Map(valM, 0, 512, 1023, true);
  }
  radio.write(&data, sizeof(data));
  delay(15); 
}
