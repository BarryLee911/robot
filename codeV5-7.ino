# Maze-Solving Robot

A Year 1 team project at University College London (UCL) to design and build an autonomous robot capable of solving an unknown maze using Arduino-based sensing and control.

## 📌 Project Summary

This project was part of Scenario B in UCL's Electronic and Electrical Engineering curriculum. The robot was required to:

- Navigate a maze with unknown layout
- Detect and avoid unstable front walls using IR proximity sensors
- Sense rigid side walls using mechanical push buttons
- Control motion using omni-wheels driven by DC motors

## 🛠️ Features & Implementation

### Hardware:
- **IR Proximity Sensors**: Detect distance to front walls
- **Push Buttons**: Detect contact with side walls
- **Motor Drivers**: Custom-built using NPN transistors and diodes
- **Arduino**: Central controller for sensors and actuators

### Software:
- **Arduino C** for motor control, wall detection, and decision logic
- Modular code structure with setup, readSensors, decide, and drive functions
- Non-hardcoded logic to allow general maze-solving behavior

## 👨‍🔧 My Contribution

- Designed and assembled all hardware circuits including sensor input and motor driver stages  
- Assisted in writing the main Arduino code framework and handling sensor integration  
- Created the navigation **flowchart** and contributed to pathfinding algorithm logic  
- Participated in testing and debugging throughout the integration phase

## 🔧 Tools & Technologies

- Arduino Uno  
- Breadboard, NPN transistors, IR emitter/receiver pairs, push buttons  
- DC power supply, oscilloscope, multimeter  
- Arduino IDE  

## 📷 Demo

(Insert images or video links of your robot navigating the maze)

## 📄 Report

(If available, attach or link to final project report PDF)

---
// inputs
const int leftButtonPin = 2;    // left button
const int rightButtonPin = 7;   // right button
const int infraredSensorPin = A2;  // IR sensor
// outputs
const int forwardPinA=3;   // PIN moving foward
const int forwardPinB=11;   // PIN moving foward
const int leftPin=5;      // PIN moving leftward
const int rightPin=6;     // PIN moving rightward
const int DIRA = 12; 
const int DIRB = 13; 
// sensor values
bool leftHit=0; //the left button status
bool rightHit=0; //the right button status
int ir=0;  //IR sensor value
// variables
const int irGap=150; //IR sensor threshold value for wall
const int irSetDis=450; //IR sensor threshold value for when the wall is too close
// status code
int lrDirec=1; //0-left 1-right
int statusCode=0;  //0-LeftRight 1-Move forward to the next stage

void setup(){
  Serial.begin(9600);

  pinMode(leftButtonPin,INPUT_PULLUP);
  pinMode(rightButtonPin,INPUT_PULLUP);
  pinMode(forwardPinA,OUTPUT);
  pinMode(forwardPinB,OUTPUT);
  pinMode(leftPin,OUTPUT);
  pinMode(rightPin,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  analogWrite(forwardPinA,0);
  analogWrite(forwardPinB,0);
  analogWrite(leftPin,0);
  analogWrite(rightPin,0);
  digitalWrite(DIRA,HIGH);
  digitalWrite(DIRB,HIGH);
}

void moveF(int spd){
  digitalWrite(DIRA,HIGH);
  digitalWrite(DIRB,HIGH);
  analogWrite(forwardPinA,spd);
  analogWrite(forwardPinB,spd);
}
void moveB(int spd){
  digitalWrite(DIRA,LOW);
  digitalWrite(DIRB,LOW);
  analogWrite(forwardPinA,spd);
  analogWrite(forwardPinB,spd);
}
void moveL(int spd){
  analogWrite(leftPin,spd);
  analogWrite(rightPin,0);
}
void moveR(int spd){
  analogWrite(leftPin,0);
  analogWrite(rightPin,spd);
}
void movelrStop(){
  analogWrite(leftPin,0);
  analogWrite(rightPin,0);
}
void movefStop(){
  digitalWrite(DIRA,HIGH);
  digitalWrite(DIRB,HIGH);
  analogWrite(forwardPinA,0);
  analogWrite(forwardPinB,0);
}

void loop() {
  //read sensor status
  leftHit=digitalRead(leftButtonPin)==HIGH;   //read from buttons
  rightHit=digitalRead(rightButtonPin)==HIGH;
  ir = analogRead(infraredSensorPin);        //read from IR sensor
  Serial.print("IR sensor: ");
  Serial.print(ir);
  Serial.print("\tleft button: ");
  Serial.print(leftHit);
  Serial.print("\tright button: ");
  Serial.println(rightHit);
  //status and their coresponding movement
  if(statusCode==0){  //status 0
    if(lrDirec==0){ //move left
      moveL(180);
      if(leftHit==1){
        lrDirec=1;
        moveR(180);
      }
    }else if(lrDirec==1){ //move right
      moveR(180);
      if(rightHit==1){
        lrDirec=0;
        moveL(180);
      }
    }
    if(ir<irSetDis&&ir>irGap){  //control left-back distance
      int fspeed=10+0.03*(irSetDis-ir);
      moveF(fspeed);
    }else if(ir>irSetDis){
      int fspeed=10+0.03*(ir-irSetDis);
      moveB(fspeed);
    }else{
      movefStop();
    }
    if(ir<irGap){
      movefStop();
      if(lrDirec==0){ //move left
        moveL(100);
        lrDirec=1;
      }else if(lrDirec==1){ //move right
        moveR(100);
        lrDirec=0;
      }
      delay(350); 
      statusCode=1;
    }
  }else if(statusCode==1){  //status 1
    movelrStop();
    moveF(100);
    if(ir>irGap){
      statusCode=0;
    }
  }
}
