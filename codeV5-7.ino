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
const int forwardPinA = 3;
const int forwardPinB = 11;
const int leftPin = 5;
const int rightPin = 6;
const int DIRA = 12;
const int DIRB = 13;

// status
const int maxWalls = 5000;
const int irthreshold = 180;
const int irtooclose = 700;

int wallCount = 0;
int lrdirection;
bool leftHit = 0;
bool rightHit = 0;
int ir = 0;
int moveahead = 0;
long lasttime = -1000;
long currenttime;

void setup() {
  Serial.begin(9600);

  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(forwardPinA, OUTPUT);
  pinMode(forwardPinB, OUTPUT);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  analogWrite(forwardPinA, 0);
  analogWrite(forwardPinB, 0);
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, HIGH);

  lrdirection = 0;
}

void loop() {
  if (wallCount >= maxWalls) {
    analogWrite(forwardPinA, 0);
    analogWrite(forwardPinB, 0);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, LOW);
    Serial.println("Maze solved, system stop");
    while (true);
  }

  // read sensors
  leftHit = digitalRead(leftButtonPin) == LOW;
  rightHit = digitalRead(rightButtonPin) == LOW;
  ir = analogRead(infraredSensorPin);

  Serial.print("IR: ");
  Serial.print(ir);
  Serial.print("\tLeft: ");
  Serial.print(leftHit);
  Serial.print("\tRight: ");
  Serial.println(rightHit);

  if (moveahead == 0) {
    if (leftHit) lrdirection = 1;
    else if (rightHit) lrdirection = 0;
  }

  currenttime = millis();

  if (moveahead == 1) {
    analogWrite(forwardPinA, 40);
    analogWrite(forwardPinB, 40);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, LOW);
    Serial.println("action: move forward");
    delay(50);
    lasttime += 50;
    if (ir > irthreshold) {
      analogWrite(forwardPinA, 0);
      analogWrite(forwardPinB, 0);
      moveahead = 0;
      wallCount++;
      delay(200);
    }
  } else {
    if (lrdirection == 1) {
      analogWrite(forwardPinA, 0);
      analogWrite(forwardPinB, 0);
      digitalWrite(leftPin, LOW);
      digitalWrite(rightPin, HIGH);
      Serial.println("action: move right");
    } else {
      analogWrite(forwardPinA, 0);
      analogWrite(forwardPinB, 0);
      digitalWrite(leftPin, HIGH);
      digitalWrite(rightPin, LOW);
      Serial.println("action: move left");
    }
  }

  // Enhanced gap detection and center-return logic
  if (ir < irthreshold) {
    if (currenttime - lasttime >= 1000) {
      lasttime = currenttime;
    } else if (currenttime - lasttime > 275) {
      long gaptime = currenttime - lasttime;

      // Step 1: move backward half duration
      digitalWrite(DIRA, LOW);
      digitalWrite(DIRB, LOW);
      analogWrite(forwardPinA, 50);
      analogWrite(forwardPinB, 50);
      delay(gaptime / 2);
      analogWrite(forwardPinA, 0);
      analogWrite(forwardPinB, 0);

      // Step 2: move forward
      digitalWrite(DIRA, HIGH);
      digitalWrite(DIRB, HIGH);
      moveahead = 1;
      digitalWrite(leftPin, LOW);
      digitalWrite(rightPin, LOW);
    }
  } else if (ir > irtooclose) {
    moveahead = 1;
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, LOW);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, LOW);
  }
}
