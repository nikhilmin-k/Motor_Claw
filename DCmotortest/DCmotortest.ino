#include <WiFi.h>
#include <HTTPClient.h>

#include <Stepper.h>
#include <AccelStepper.h>
#include <ESP32Servo.h>

#define motorPin1 19
#define motorPin2 21
#define motorPin3 22
#define motorPin4 23

#define MotorInterfaceType 8

//networking stuff
const char *ssid_Router  = "stationnebula"; //Enter the router name
const char *password_Router = "E30M20B25"; //Enter the router password
String address= "http://165.227.76.232:3000/nmk2146/running";

//Servo connection
Servo myServo;
int servoPin = 2;
int servoPos = 0;
const int servoRes = 16;

//PWM for Servo
const int servoChannel = 1;

//Stepper Motor steps per revolution
const int stepsPerRevolution = 2048;
//Stepper Motor initialization with pins
Stepper myStepper = Stepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);

//DC motor conenction
int enA = 13;
int in1 = 4;
int in2 = 5;

//potentiometer
int potControl = 14;
int potControl2 = 12;
int potControl3 = 25;
int lastPot3 = 0;
int potDelta = 0;

//button pin
int buttonPin = 15;
boolean rotstate = false;

//PWM properties for enA pin
const int freq = 5000;
const int channel = 5;
const int resolution = 10;



void setup() {
  //Serial Communication setup
  Serial.begin(115200);

  //wifi setup
  /*WiFi.begin(ssid_Router, password_Router);
  Serial.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup End");*/

  //Stepper setup
  myStepper.setSpeed(15);

  //Servo PWM setup
  ledcSetup(servoChannel, 50, servoRes);
  ledcAttachPin(servoPin, servoChannel);  

  //Servo motor 1 setup
  pinMode(servoPin, OUTPUT);
  //myServo.setPeriodHertz(50);
  //myServo.attach(servoPin, 500, 2400);

  //DC PWM setup
  //ledcSetup(channel, freq, resolution);
  
  //DC motor 1 setup
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  //pot setup
  pinMode(potControl, INPUT);
  pinMode(potControl2, INPUT);
  pinMode(potControl3, INPUT);

  //button setup
  pinMode(buttonPin, INPUT);

  //attach control pin to motor channel
  ledcAttachPin(enA, channel);

  //set analog read resolution to match resolution of PWM functions
  analogReadResolution(resolution);
}

void loop() {

  Serial.println("loop");
  //Servo motor pot analogRead
  servoPos = analogRead(potControl2);
  servoPos = map(servoPos, 0, 1023, 0, 180);
  setServo(servoPos);
  
  //DC motor pot analogRead
  int rotationSpeed = analogRead(potControl);

  //DC motor rotation direction button query
  if(digitalRead(buttonPin) == HIGH){
    rotstate = !rotstate;
    motorSwitchDir(rotstate);
    delay(200);
  }

  //Drive DC motor
  //driveDCMotor(rotationSpeed);

  
  //Drive Stepper Motor
  int currPot = analogRead(potControl3);
  int potDelta = currPot - lastPot3;
  lastPot3 = currPot;
  driveStepperMotor(myStepper, potDelta);
  
  //debugging print statements
  //Serial.println(rotationSpeed);
  //Serial.println(constrain(rotationSpeed, 0, 1024));
}

void motorSwitchDir(boolean dir){
  if(dir){
    Serial.println("Dir forward");
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);  
  }
  else {
    Serial.println("Dir backward");
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
}

void setServo(float pos){//16 bit servo requires 16 bit input
  uint32_t duty = (((pos/180.0)*2000)/20000.0*65536.0) + 1634;
     // convert 0-180 degrees to 0-65536

  ledcWrite(servoChannel,duty);
    // set channel to pos
}

void driveDCMotor(int spd){
  if(spd>20){ 
    //algortihm to convert 0-1024 spd range to 550-1024 write range
    ledcWrite(channel, 550+((float)spd/1024)*(1024-550));
  }
  else{
    ledcWrite(channel, 0);  
  }
}

void driveStepperMotor(Stepper stepper, int steps){
  // Step one revolution in one direction:
  myStepper.step(steps);
}
