// An arduino firmware that can be used with the Micro-Manager LStep Z-Stage device adaptor.
// Allows building simple 1-axis stage devices.
// version 0.1
// JM 2016-2018

// References
// https://micro-manager.org/wiki/MarzhauserLStep
// MM device adaptor code /DeviceAdapters/Marzhauser-LStep/LStep.cpp
// LStep documentation from www.marzhauser.com/nc/en/service/downloads.html

// in HCW, use serial port settings:
// AnswerTimeout,3000.0000
// BaudRate,9600
// DelayBetweenCharsMs,0.0000
// Handshaking,Off
// Parity,None
// StopBits,1
#include <ezButton.h>

const int enPin=8;
const int stepPin = 2;
const int dirPin = 5;

bool cal = false;

const int stepsPerRev = 1600; 
int pulseWidthMicros = 1600;
int millisBtwnSteps = 400;
String cmd = "" ;

ezButton limit(7);

float z = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(enPin, OUTPUT);
  //pinMode(7, INPUT_PULLUP);
  limit.setDebounceTime(100);
  digitalWrite(enPin, LOW);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  reply ("Vers:LS_z");
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
  } else if (s.startsWith("?pitch z")) {
    reply ("50.0");
  } else if (s.startsWith("?vel z")) {
    reply ("100.0");
  } else if (s.startsWith("?accel z")) {
    reply ("1.0");
  } else if (s.startsWith("!dim z 1")) {
    delay(5);
  } else if (s.startsWith("!dim z 2")) {
    delay(5);
  } else if (s.startsWith("?statusaxis")) {
    reply ("@");
  } else if (s.startsWith("!vel z")) {
    delay(5);
  } else if (s.startsWith("!accel z")) {
    delay(5);
  } else if (s.startsWith("?pos z")) {
    String zs = String(z, 1);
    reply (zs);
  } else if (s.startsWith("?lim z")) {
    reply ("0.0 100.0");
  } else if (s.startsWith("!pos z")) {
    delay(5);
  } else if (s.startsWith("?status")) {
    reply ("OK...");
  } else if (s.startsWith("!dim z 0")) {
    delay(5);
  } else if (s.startsWith("!speed z")) {
    delay(5);
  } else if (s.startsWith("!mor z")) {
    if (cal){
      String delta = s.substring(s.indexOf("z") + 1);
      z = z + delta.toFloat();
      if (delta.toFloat()>0){
        move(millisBtwnSteps, LOW, 1);}
      else if (delta.toFloat()<0){
        move(millisBtwnSteps, HIGH, 1);}
    }
    else{
      home(0);
      cal= true;
    }
  } else if (s.startsWith("!moa z")) {
    String apos = s.substring(s.indexOf("z") + 1);
    z = apos.toFloat();
  } else if (s.startsWith("?err")) {
    reply ("0");
  }
}

void reply(String s) {
  Serial.print(s);
  Serial.print("\r");
}

void move(int speed, bool dir, int dist) {


  digitalWrite(dirPin, dir);
  digitalWrite(enPin, LOW);

  for (int i = 0; i < dist; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  digitalWrite(enPin, HIGH); 
}

void home(int pos){
  limit.loop();
  digitalWrite(dirPin, HIGH);
  digitalWrite(enPin, LOW);
  while (true){
    if (limit.isPressed() == false){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(pulseWidthMicros);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(millisBtwnSteps);
    }
    else if (limit.isPressed()){
      digitalWrite(dirPin, LOW);
      for (int i = 0; i < 1000; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(pulseWidthMicros);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(millisBtwnSteps);
      }
    break;
    }
  digitalWrite(enPin, HIGH); 
}
