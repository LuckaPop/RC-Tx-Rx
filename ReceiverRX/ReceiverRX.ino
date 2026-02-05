#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "29235";
struct Signal{
  byte pitch;
  byte roll;
  byte yaw;
  byte motor;
};
Signal data;

Servo pitchServo;
Servo rollIServo;
Servo rollDServo;
Servo yawServo;
Servo motorESC;

#define pinMot 5

void setup() {
  Serial.begin(9600);
 
  pitchServo.attach(2);
  rollIServo.attach(3); 
  rollDServo.attach(4); 
  yawServo.attach(6);  


  motorESC.attach(pinMot, 1000, 2000);
  motorESC.writeMicroseconds(1000);
  delay(3000); 

  radio.begin();
  radio.openReadingPipe(0,address); 
  radio.setChannel(100); 
  radio.setAutoAck(false); 
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_HIGH); 
  radio.startListening();
  
  data.pitch = 127;
  data.roll = 127;
  data.yaw = 127;
  data.motor = 0;
}

void loop() {
  Serial.println("Loop Activo");
  delay(500);
  if (radio.available()) {

    radio.read(&data, sizeof(data));
    Serial.print("Pitch: ");
    Serial.print(data.pitch);
    Serial.print(" | Roll: ");
    Serial.print(data.roll);
    Serial.print(" | Yaw: ");
    Serial.print(data.yaw);
    Serial.print(" | Motor: ");
    Serial.println(data.motor);

    pitchServo.write(map(data.pitch, 0, 255, 0, 180));
    rollIServo.write(map(data.roll, 0, 255, 0, 180));
    rollDServo.write(map(data.roll, 0, 255, 0, 180));
    yawServo.write(map(data.yaw, 0, 255, 0, 180));
    
    int escValue = map(data.motor, 0, 255, 1000, 2000);
    motorESC.writeMicroseconds(escValue);
  } 
}
