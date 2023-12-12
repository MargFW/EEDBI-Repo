#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Encoder.h>

const int enPin = 8;
const int stepXPin = 2;
const int dirXPin = 5;
const int stepYPin = 3;
const int dirYPin = 6;
int stepPin = stepYPin;
int dirPin = dirYPin;

int firstSpace;
int secondSpace;

const int stepsPerRev = 800;
int pulseWidthMicros = 100;
int millisBtwnSteps = 1000;
String cmd = "" ;
float x = 0.0;
float y = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  pinMode(stepXPin, OUTPUT);
  pinMode(stepYPin, OUTPUT);
  pinMode(dirXPin, OUTPUT);
  pinMode(dirYPin, OUTPUT);
  delay(1000);
  reply("Vers:LS_xy");
}
char c = '*';

void loop()
{
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\r');
    processCommand(cmd);
    cmd = "";
  }
}

void processCommand(String s) {
  if (s.startsWith("?ver")) {
    reply ("Vers:LS");
  } else if (s.startsWith("!autostatus 0")) {
    delay(5);
  } else if (s.startsWith("?det")) {
    reply ("60");
  } else if (s.startsWith("?pitch x")) {
    reply ("50.0");
  } else if (s.startsWith("?pitch y")) { 
    reply ("50.0");
  } else if (s.startsWith("?vel x")) {
    reply ("100.0");
  } else if (s.startsWith("?vel y")) {
    reply ("100.0");
  } else if (s.startsWith("?accel x")) {
    reply ("1.0");
  } else if (s.startsWith("?accel y")) {
    reply ("1.0");
  } else if (s.startsWith("!dim x 1")) {
    delay(5);
  } else if (s.startsWith("!dim y 1")) {
    delay(5);
  } else if (s.startsWith("!dim x 2")) {
    delay(5);
  } else if (s.startsWith("!dim y 2")) {
    delay(5);
  } else if (s.startsWith("?statusaxis")) {
    reply ("@");
  } else if (s.startsWith("!vel x")) {
    delay(5);
  } else if (s.startsWith("!vel y")) {
    delay(5);
  } else if (s.startsWith("!accel x")) {
    delay(5);
  } else if (s.startsWith("!accel y")) {
    delay(5);
  } else if (s.startsWith("?pos x")) {
    String xs = String(x, 1);
    reply (xs);
  } else if (s.startsWith("?pos y")) {
    String ys = String(y, 1);
    reply (ys);
  } else if (s.startsWith("?lim x")) {
    reply ("0.0 100.0");
  } else if (s.startsWith("?lim y")) {
    reply ("0.0 100.0");
  } else if (s.startsWith("?pos")) {
    reply(String(x)+" "+String(y));
  //  delay(5);
  //} else if (s.startsWith("!pos y")) {
  //  delay(5);
  } else if (s.startsWith("?status")) {
    reply ("OK...");
  } else if (s.startsWith("!dim x 0")) {
    delay(5);
  } else if (s.startsWith("!dim y 0")) {
    delay(5);
  } else if (s.startsWith("!speed x")) {
    delay(5);
  } else if (s.startsWith("!speed y")) {
    delay(5);
  } else if (s.startsWith("!mor ")) {
    firstSpace = s.indexOf(' ');
    secondSpace = s.indexOf(' ', firstSpace+1);
    String deltax = s.substring(firstSpace, secondSpace);
    String deltay = s.substring(secondSpace);
    float delta_x = deltax.toFloat();
    float delta_y = deltay.toFloat();
    if ((x + delta_x)>=-100 and (x+delta_x)<=100){
    x = x + delta_x;
    if (delta_x>0) {
      move(stepXPin, millisBtwnSteps, dirXPin, HIGH, 1);}
    else if (delta_x<0){
      move(stepXPin, millisBtwnSteps, dirXPin, LOW, 1);}
    } if ((y + delta_y)>=-100 and (y + delta_y)<=100){
      y = y + delta_y;
    if (delta_y>0){
      move(stepYPin, millisBtwnSteps, dirYPin, HIGH, 1);}
    else if (delta_y<0){
      move(stepYPin, millisBtwnSteps, dirYPin, LOW, 1);}
    }
  } else if (s.startsWith("!moa x")) {
    firstSpace = s.indexOf(' ');
    secondSpace = s.indexOf(' ', firstSpace + 1);
    String apos_x = s.substring(firstSpace, secondSpace);
    String apos_y = s.substring(secondSpace);//Serial.print(apos_y);
    float apos_x_num = apos_x.toFloat();
    float apos_y_num = apos_y.toFloat(); 
  } else if (s.startsWith("?err")) {
    reply ("0");
  }
}

void reply(String s) {
  Serial.print(s);
  Serial.print("\r");
}

void move(int motorPin, int speed, int dirPin, bool dir, int dist) {
  digitalWrite(dirPin, dir);

  for (int i = 0; i <dist; i++){
    digitalWrite(motorPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(motorPin, LOW);
    delayMicroseconds(speed);
  }
}
