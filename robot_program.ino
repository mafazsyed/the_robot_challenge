/*Library*/
#include <NewPing.h>              //Library for Ultrasonic
#include <Encoder.h>              //Library for Encoder
#include <Servo.h>                //Library for Servo               

/*Ultrasonic*/
#define TRIGGER_PIN1  40  // Ultrasonic sensor 1 trigger pin attaches Arduino Mega digitalPin 40
#define ECHO_PIN1     41  // Ultrasonic sensor 1 echo pin attaches Arduino Mega digitalPin 41

#define TRIGGER_PIN2  5  // Ultrasonic sensor 2 trigger pin attaches Arduino Mega digitalPin 5
#define ECHO_PIN2     4  // Ultrasonic sensor 2 echo pin attaches Arduino Mega digitalPin 4

#define MAX_DISTANCE 200 // Set maximum reading distance for both ultrasonic sensor

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); //NewPing.h library basic format for ultrasonic sensor 1
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); //NewPing.h library basic format for ultrasonic sensor 2

/*Button*/
int buttonPin1 = 8;                 //declare variable buttonPin as an integer and connect to Arduino Mega digitalPin 8
int buttonState1;                   //declare variable buttonState1 as an integer 

/*servo*/
Servo servo1;                       //declare a servo name servo1
Servo servo2;                       //declare a servo name servo2

/*Encoder*/
Encoder myEnc(2, 3);               //encoder.h library basic format, encoder attached to Arduino Mega digitalPin 2 and 3
long oldPosition  = -999;          //declare variable oldPosition as a long data type and set to -999
double newPosition;                //declare variable newPosition as a double data type 
double currentPos = 0;             //declare variable presentPos as an double and and set to 0


/*TB6612FNG Driver Board*/
//motor a
const int pinAI1 = 22;       //declare variable pinAI1 as a constant integer and attaches to Arduino Mega digitalPin 22
const int pinAI2 = 23;       //declare variable pinAI2 as a constant integer and attaches to Arduino Mega digitalPin 23
const int pinPWMA = 13;      //declare variable pinPWMA as a constant integer and attaches to Arduino Mega digitalPin 13
int PWMA = 0;                //declare variable PWMA as an integer and and set to 0

//motor b
const int pinBI1 = 24;       //declare variable pinBI1 as a constant integer and attaches to Arduino Mega digitalPin 24
const int pinBI2 = 25;       //declare variable pinBI2 as a constant integer and attaches to Arduino Mega digitalPin 25
const int pinPWMB = 12;      //declare variable pinPWMB as a constant integer and attaches to Arduino Mega digitalPin 12
int PWMB = 0;                //declare variable PWMB as an integer and and set to 0

//motor c
const int pinCI1 = 26;       //declare variable pinCI1 as a constant integer and attaches to Arduino Mega digitalPin 26
const int pinCI2 = 27;       //declare variable pinCI2 as a constant integer and attaches to Arduino Mega digitalPin 27
const int pinPWMC = 11;      //declare variable pinPWMC as a constant integer and attaches to Arduino Mega digitalPin 11
int PWMC = 0;                //declare variable PWMC as an integer and and set to 0

//motor d
const int pinDI1 = 28;       //declare variable pinDI1 as a constant integer and attaches to Arduino Mega digitalPin 28
const int pinDI2 = 29;       //declare variable pinDI2 as a constant integer and attaches to Arduino Mega digitalPin 29
const int pinPWMD = 10;      //declare variable pinPWMD as a constant integer and attaches to Arduino Mega digitalPin 10
int PWMD = 0;                //declare variable PWMD as an integer and and set to 0

//Standby pin is shorted to 5 volts. meaning that the board will always not in standby mode when system is powered on.

/*switch break declaration*/
int Task = 0;                      //declare variable Task as an integer(for switch case)and set to 0

/*procedural declaration*/
double spinPos1 = 33.50;           //declare variable spinPos1 as an double and set to 33.50
double forwardPos1 = 65.00;        //declare variable forwardPos1 as an double and set to 65.00 
double spinPos2 = -10.00;          //declare variable spinPos2 as an double and set to -10.00
double forwardPos2 = 71.50;        //declare variable forwardPos2 as an double and set to 71.50 
double backwardPos1 = -5.00;       //declare variable backwardPos1 as an double and set to -5.00 
double spinPos3 = -16.75;          //declare variable spinPos3 as an double and set to -16.75
double forwardPos3 = 65.00;        //declare variable forwardPos3 as an double and set to 65.00
double spinPos4 = -9.50;           //declare variable spinPos4 as an double and set to -9.50
double forwardPos4 = 52.50;        //declare variable forwardPos4 as an double and set to 52.50


void setup()                            //Setup
{
  //Below Pin Mode for buttonPin
  pinMode(buttonPin1,INPUT_PULLUP);     //Set buttonPin1 as an input with pull up condition
  
  //Motor
  //A motor
  pinMode(pinAI1, OUTPUT);              //Set pinAI1 as an output 
  pinMode(pinAI2, OUTPUT);              //Set pinAI2 as an output 
  pinMode(pinPWMA, OUTPUT);             //Set pinPWMA as an output 
  //B motor
  pinMode(pinBI1, OUTPUT);              //Set pinBI1 as an output 
  pinMode(pinBI2, OUTPUT);              //Set pinBI2 as an output 
  pinMode(pinPWMB, OUTPUT);             //Set pinPWMB as an output 
  //C motor
  pinMode(pinCI1, OUTPUT);              //Set pinCI1 as an output 
  pinMode(pinCI2, OUTPUT);              //Set pinCI2 as an output 
  pinMode(pinPWMC, OUTPUT);             //Set pinPWMC as an output 
  //D motor
  pinMode(pinDI1, OUTPUT);              //Set pinDI1 as an output 
  pinMode(pinDI2, OUTPUT);              //Set pinDI2 as an output 
  pinMode(pinPWMD, OUTPUT);             //Set pinPWMD as an output 


  //servo
  servo1.attach(5);                     //servo1 attaches to Arduino Mega digitalPin 5
  servo2.attach(6);                     //servo2 attaches to Arduino Mega digitalPin 6

  /*intterupt*/
  attachInterrupt(digitalPinToInterrupt(2), readEncoder, CHANGE); //attaches interrupt to Arduino Mega digitalPin 2 and set mode to change

  //Serial initialization
  Serial.begin(9600);                   //Begin Serial with 9600
}

void loop()                             //Program loop
{
  switch (Task)
  {
  case 0:                                 //initiate
  buttonState1 = digitalRead(buttonPin1); //buttonState1 is equal to digitalRead(buttonPin1)
  currentPos = newPosition;               //currentPos is equal to newPosition

  // if the button state is high then print ready on serial monitor, if low then call void Start()
  if (buttonState1 == HIGH) 
  {
    Serial.print("System ready ");        //printing System ready in serial monitor
    Serial.println(currentPos);           //printing currentPos value in serial monitor 
    stop();                               //Call void stop()
  } 
  else 
  {
    servoDefault();                       //Call void servoDefault()
    delay(1000);                          //delay 1 s
    Start();                              //Call void Start()
  }
  delay(10);                              //delay 10 miliseconds
  break;
  


  case 1:                                   
  currentPos = newPosition;               //currentPos is equal to newPosition
  //if currentPos value is smaller or equal than spinPos1, turn right. else Task value is equal to 2
  if(currentPos<=spinPos1)
    {
      turnRight();                        //Call void turnRight()
      Serial.print("i am 1 ");            //printing i am 1  in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 2;                           //Task is equal to 2
      myEnc.write(0);                     //reset encoder value
    }
  delay(10);                              //delay 10 miliseconds
  break;


  case 2:
  //if currentPos value is smaller or equal than forwardPos1, move forward. else Task value is euqal to 3                                     
  currentPos = newPosition;              //currentPos is equal to newPosition
  if(currentPos<=forwardPos1)
    {
      moveForward();                     //Call void moveForward()
      Serial.print("i am 2 ");           //printing i am 2  in serial monitor
      Serial.println(currentPos);        //printing currentPos value in serial monitor
    }
  else
    {
      Task = 3;                          //Task is equal to 3
      myEnc.write(0);                    //reset encoder value
    }
  delay(10);                             //delay 10 miliseconds
  break;

  case 3:
  //if currentPos value is bigger or equal than spinPos2, turn left. else Task value is euqal to 4                                    
  currentPos = newPosition;               //currentPos is equal to newPosition
  if(currentPos>=spinPos2)
    {
      turnLeft();                         //Call void turnLeft()
      Serial.print("i am 3 ");            //printing i am 3 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 4;                           //Task is equal to 4
      myEnc.write(0);                     //reset encoder value
    }
  delay(10);                              //delay 10 miliseconds  
  break;

  case 4:
   //if currentPos value is smaller or equal than forwardPos2, move forward. else Task value is euqal to 5 and move the servos                                   
  currentPos = newPosition;               //currentPos is equal to newPosition
  if(currentPos<=forwardPos2)
    {
      moveForward();                      //Call void moveForward()
      Serial.print("i am 4 ");            //printing i am 4 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else  
    {
      stop();                             //Call void stop()
      delay(2000);                        //delay 2 s
      Task = 5;                           //Task is equal to 5
      servoUp();                          //Call void servoUp()
      delay(2000);                        //delay 2 s
      servoDefault();                     //Call void servoDefault()
      delay(2000);                        //delay 2 s
      myEnc.write(0);                     //reset encoder value
    }
  delay(10);                              //delay 10 miliseconds  
  break;
  
  case 5:
  //if currentPos value is bigger or equal than backwardPos1, move backward. else Task value is euqal to 6
  currentPos = newPosition;               //currentPos is equal to newPosition
  if(currentPos>=backwardPos1)
    {
      moveBackward();                     //Call void moveBackward()
      Serial.print("i am 5 ");            //printing i am 5 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 6;                           //Task is equal to 6
      myEnc.write(0);                     //reset encoder value
    }
  delay(10);                              //delay 10 miliseconds  
  break;
  

  case 6:
  //if currentPos value is bigger or equal than spinPos3, turn left. else Task value is euqal to 7
  currentPos = newPosition;               //currentPos is equal to newPosition  
  if(currentPos>=spinPos3)
    {
      turnLeft();                         //Call void turnLeft()
      Serial.print("i am 6 ");            //printing i am 6 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 7;                           //Task is equal to 7 
      myEnc.write(0);                     //reset encoder value
    }
  delay(10);                              //delay 10 miliseconds  
  break;
  

  case 7:
  //if currentPos value is smaller or equal than forwardPos3, move forward. else Task value is euqal to 8                                    
  currentPos = newPosition;               //currentPos is equal to newPosition  
  if(currentPos<=forwardPos3)
    {
      moveForward();                      //Call void moveForward()
      Serial.print("i am 7 ");            //printing i am 7 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 8;                           //Task is equal to 8
      myEnc.write(0);                     //reset encoder value 
    }
  delay(10);                              //delay 10 miliseconds  
  break;
  

  case 8:
  //if currentPos value is bigger or equal than spinPos4, turn left. else Task value is euqal to 9                                   
  currentPos = newPosition;               //currentPos is equal to newPosition    
  if(currentPos>=spinPos4)
    {
      turnLeft();                         //Call void turnLeft()
      Serial.print("i am 8 ");            //printing i am 8 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 9;                           //Task is equal to 9 
      myEnc.write(0);                     //reset encoder value  
    }
  delay(10);                              //delay 10 miliseconds  
  break;

  case 9:
  //if currentPos value is smaller or equal than forwardPos4, move forward. else Task value is euqal to 0                                   
  currentPos = newPosition;               //currentPos is equal to newPosition    
  if(currentPos<=forwardPos4)
    {
      moveForward();                      //Call void moveForward()
      Serial.print("i am 9 ");            //printing i am 9 in serial monitor
      Serial.println(currentPos);         //printing currentPos value in serial monitor
    }
  else
    {
      Task = 0;                           //Task is equal to 0  
      myEnc.write(0);                     //reset encoder value  
    }
  delay(10);                              //delay 10 miliseconds  
  break;
  }
}

//Call funtion//

void moveForward()         
{
  //A motor
  PWMA = 95;                        //set PWMA to 95
  //B motor
  PWMB = 100;                       //set PWMB to 100
  //C motor
  PWMC = 95;                        //set PWMC to 95
  //D motor
  PWMD = 100;                       //set PWMD to 100
  
  digitalWrite(pinAI1, 1);          //set pinAI1 to high
  digitalWrite(pinAI2, 0);          //set pinAI2 to low
  analogWrite(pinPWMA, PWMA);       //set pinPWMA pwm to PWMA
  digitalWrite(pinBI1, 1);          //set pinBI1 to high
  digitalWrite(pinBI2, 0);          //set pinBI2 to low
  analogWrite(pinPWMB, PWMB);       //set pinPWMB pwm to PWMB
  
  digitalWrite(pinCI1, 1);          //set pinCI1 to high
  digitalWrite(pinCI2, 0);          //set pinCI2 to low
  analogWrite(pinPWMC, PWMC);       //set pinPWMC pwm to PWMC
  digitalWrite(pinDI1, 1);          //set pinDI1 to high
  digitalWrite(pinDI2, 0);          //set pinDI2 to low
  analogWrite(pinPWMD, PWMD);       //set pinPWMD pwm to PWMD

}
void moveBackward()         
{
  //A motor
  PWMA = 100;                       //set PWMA to 100
  //B motor
  PWMB = 100;                       //set PWMB to 100
  //C motor
  PWMC = 100;                       //set PWMC to 100
  //D motor
  PWMD = 100;                       //set PWMD to 100
  
  digitalWrite(pinAI1, 0);          //set pinAI1 to low
  digitalWrite(pinAI2, 1);          //set pinAI2 to high
  analogWrite(pinPWMA, PWMA);       //set pinPWMA pwm to PWMA
  digitalWrite(pinBI1, 0);          //set pinBI1 to low
  digitalWrite(pinBI2, 1);          //set pinBI2 to high
  analogWrite(pinPWMB, PWMB);       //set pinPWMB pwm to PWMB
  
  digitalWrite(pinCI1, 0);          //set pinCI1 to low
  digitalWrite(pinCI2, 1);          //set pinCI2 to high
  analogWrite(pinPWMC, PWMC);       //set pinPWMC pwm to PWMC
  digitalWrite(pinDI1, 0);          //set pinDI1 to low
  digitalWrite(pinDI2, 1);          //set pinDI2 to high
  analogWrite(pinPWMD, PWMD);       //set pinPWMD pwm to PWMD

}
void turnRight()        
{
  //A motor
  PWMA = 100;                       //set PWMA to 100
  //B motor
  PWMB = 100;                       //set PWMB to 100
  //C motor
  PWMC = 100;                       //set PWMC to 100
  //D motor
  PWMD = 100;                       //set PWMD to 100
  
  digitalWrite(pinAI1, 1);          //set pinAI1 to high
  digitalWrite(pinAI2, 0);          //set pinAI2 to low
  analogWrite(pinPWMA, PWMA);       //set pinPWMA pwm to PWMA
  digitalWrite(pinBI1, 0);          //set pinBI1 to low
  digitalWrite(pinBI2, 1);          //set pinBI2 to high
  analogWrite(pinPWMB, PWMB);       //set pinPWMB pwm to PWMB
  
  digitalWrite(pinCI1, 1);          //set pinCI1 to high
  digitalWrite(pinCI2, 0);          //set pinCI2 to low
  analogWrite(pinPWMC, PWMC);       //set pinPWMC pwm to PWMC
  digitalWrite(pinDI1, 0);          //set pinDI1 to low
  digitalWrite(pinDI2, 1);          //set pinDI2 to high
  analogWrite(pinPWMD, PWMD);       //set pinPWMD pwm to PWMD

}
void turnLeft()         
{
  //A motor
  PWMA = 100;                       //set PWMA to 100
  //B motor
  PWMB = 100;                       //set PWMB to 100
  //C motor
  PWMC = 100;                       //set PWMC to 100
  //D motor
  PWMD = 100;                       //set PWMD to 100
  
  digitalWrite(pinAI1, 0);          //set pinAI1 to low
  digitalWrite(pinAI2, 1);          //set pinAI2 to high
  analogWrite(pinPWMA, PWMA);       //set pinPWMA pwm to PWMA
  digitalWrite(pinBI1, 1);          //set pinBI1 to high
  digitalWrite(pinBI2, 0);          //set pinBI2 to low
  analogWrite(pinPWMB, PWMB);       //set pinPWMB pwm to PWMB
  
  digitalWrite(pinCI1, 0);          //set pinCI1 to low
  digitalWrite(pinCI2, 1);          //set pinCI2 to high
  analogWrite(pinPWMC, PWMC);       //set pinPWMC pwm to PWMC
  digitalWrite(pinDI1, 1);          //set pinDI1 to high
  digitalWrite(pinDI2, 0);          //set pinDI2 to low
  analogWrite(pinPWMD, PWMD);       //set pinPWMD pwm to PWMD
}


void stop()         
{
  //A motor
  PWMA = 255;                       //set PWMA to 255
  //B motor
  PWMB = 255;                       //set PWMB to 255
  //C motor
  PWMC = 255;                       //set PWMC to 255
  //D motor
  PWMD = 255;                       //set PWMD to 255

  digitalWrite(pinAI1, 1);          //set pinAI1 to high
  digitalWrite(pinAI2, 1);          //set pinAI2 to high
  analogWrite(pinPWMA, PWMA);       //set pinPWMA pwm to PWMA
  digitalWrite(pinBI1, 1);          //set pinBI1 to high
  digitalWrite(pinBI2, 1);          //set pinBI2 to high
  analogWrite(pinPWMB, PWMB);       //set pinPWMB pwm to PWMB

  digitalWrite(pinCI1, 1);          //set pinCI1 to high
  digitalWrite(pinCI2, 1);          //set pinCI2 to high
  analogWrite(pinPWMC, PWMC);       //set pinPWMC pwm to PWMC
  digitalWrite(pinDI1, 1);          //set pinDI1 to high
  digitalWrite(pinDI2, 1);          //set pinDI2 to high
  analogWrite(pinPWMD, PWMD);       //set pinPWMD pwm to PWMD
}

void Start()        //Function Start
{
  Task = 1;         //Task is equal to 1
}

void readEncoder()      //Function readEncoder
{
  newPosition = (double)myEnc.read()*0.0064692248; //library encoder basic form, times normalizing from the wheel circumference
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
  } 
}

void servoDefault()          //servo default sequence
{
  servo1.write(0);           //servo1 angle to 0
  delay(1500);               //delay 1.5 s
  servo2.write(180);         //servo2 angle to 180
  delay(1500);               //delay 1.5 s
}

void servoUp()               //servo up sequence
{
  servo1.write(90);          //servo1 angle to 90
  delay(1500);               //delay 1.5 s
  servo2.write(0);           //servo2 angle to 0
  delay(1500);               //delay 1.5 s
}
