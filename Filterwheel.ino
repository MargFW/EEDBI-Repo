// References
// Ludl Filter wheel from Micromanager

#include <Stepper.h>
#include <AccelStepper.h>
int pos;

const int GFP = 2;
const int RFP = 3;
const int BF = 4;

const int stepsPerRevolution = 2038;

const int enPin=8;
const int stepPin = 10;
const int dirPin = 9;

AccelStepper myStepper(stepsPerRevolution, enPin, stepPin, dirPin, 11);

String cmd = "" ;

float TransDelay = 10.0;

void setup() {
  Serial.begin(9600);

  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, HIGH);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(GFP, OUTPUT);
  pinMode(RFP, OUTPUT);
  pinMode(BF, OUTPUT);

  digitalWrite(GFP, LOW);
  digitalWrite(RFP, LOW);
  digitalWrite(BF, LOW);
  delay(1000);

  myStepper.setSpeed(20);
  
  reply("Vers:dd_Filterwheel");
}

char c = '*';

void loop() {
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\r');
    processCommand(cmd);
    cmd = "";
  }
}

void reply(String s) {
  Serial.print(s);
  Serial.print("\n");
}

void processCommand(String s) {

  if (s.startsWith("VER")) {
    reply("Vers: DD");
  }
  else if (s.startsWith("Rconfig")) {
    reply("BigMode");
  }
  else if (s.startsWith("Remres")) {
    delay(10);
  }
  
  else if (s.startsWith("TRXDEL")) {  
    if (s.length() > 6) {
      String cmd = s.substring(s.indexOf("TRXDEL ") + 8);
      TransDelay = cmd.toFloat();
      reply(":A");
    }
    else {
      reply("AA");
    }
  }


  else if (s.startsWith("STATUS S")) {
    Serial.print("N");
  }
  else if (s.startsWith("Rotat S")) {
    String cmd = s.substring(s.indexOf("M") + 1, s.length());
    if (cmd == "H") {
      setChannel(0);
      reply("AA");
    }
    else {
      setChannel(cmd.toInt());
      reply("AA");
    }
  }

  //Creating a sudo-id response to trick MM
  else if (s.endsWith("63:")) {
    Serial.print("66:");
  }
  else { //Useful for debugging with Micromanager, just to see what Micromanager has sent to Arduino board
    Serial.print(s);
  } 

}

void rotateStepper(int degrees) {
  myStepper.setSpeed(10);
  int stepsToMove = map(degrees, 0, 360, 0, stepsPerRevolution);
  myStepper.moveTo(stepsToMove);
  myStepper.run();
}

// 0 is nothing, 1 is BF, 2 is GFP, 3 is RFP
void setChannel(int channel) {

  if (channel == 0) {
    pos = 0;
    digitalWrite(GFP, LOW);
    digitalWrite(RFP, LOW);
    digitalWrite(BF, LOW);
    rotateStepper(pos);
  } else if (channel == 1) {
    pos = 120;
    digitalWrite(GFP, LOW);
    digitalWrite(RFP, LOW);
    digitalWrite(BF, LOW);
    digitalWrite(GFP, HIGH);
    rotateStepper(pos);
  } else if (channel == 2) {
    pos = 240;
    digitalWrite(GFP, LOW);
    digitalWrite(RFP, LOW);
    digitalWrite(BF, LOW);
    digitalWrite(GFP, HIGH);
    rotateStepper(pos);
  } else if (channel == 3) {
    pos = 360;
    digitalWrite(GFP, LOW);
    digitalWrite(RFP, LOW);
    digitalWrite(BF, LOW);
    digitalWrite(RFP, HIGH);
    rotateStepper(pos);
  } else {
  }
}

