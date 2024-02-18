//Team Nemo
//Members: Joanna Ma, Briana Luna, Alondra Alfaro, Alyssa Gomez, Madeline Benensohn, Mariam Haroun
//date of last edit: 3/17/2023
//Summary of code: this code will allow a rover to follow a black line using IR sensors and follow and grab a can at the end of the track



//Library
int IR_L = 2; //left sensor connected to pin 2
int IR_R = 3; //right sensor connected to pin 3
int IR_B = 8; //bottom sensor on pin 8

#include "CytronMotorDriver.h"
//motor driver
CytronMD motor1(PWM_DIR, 5, 4); //PWM 1 = pin 5, DIR 1 = pin 4
//right motor
CytronMD motor2(PWM_DIR, 6, 7); //PWM 2 = pin 6, DIR 2 = pin 7
//left motor

#include <Servo.h>
//servo
Servo servo;
int pos = 0;

#include <Pixy2.h>
//Pixy cam
Pixy2 pixy;
int m_signature = 0;
int x = 0;
int width;
int errorX;
int steerX;
int baseSpeed = 204; //base speed of motors
int line=0;



void setup() {
  Serial.begin(115200); //begin serial monitoring
  pinMode (IR_L, INPUT); //configure sensors as inputs
  pinMode (IR_R, INPUT);
  pinMode (IR_B, INPUT);

  servo.attach(9); //servo on pin 9
  pixy.init(); //initate Pixycam
}

void loop() {
if (line==0) //default state is line = 0, beginning of track
  {
    lineFollow();
  }
else if (line==1) //if it reaches the end of track: scan for can, center, and grab
  {
    scan(); //scan for can
    center();  
  }
}




//functions
void lineFollow() //line following code
{      
int IR_L_val = digitalRead(IR_L); //read IR sensor state
int IR_R_val = digitalRead(IR_R);
int IR_B_val = digitalRead(IR_B);

Serial.print("leftvalue: ");
Serial.println(IR_L_val); //print IR sensor state to see if sensors are working accordingly

Serial.print("rightvalue: ");
Serial.println(IR_R_val);

Serial.print("centervalue: ");
Serial.println(IR_B_val);


if (IR_R_val == 1 && IR_L_val == 0) //if right sensor detects line
    {
    motor2.setSpeed(204); //motor 2 runs forward at base speed
    motor1.setSpeed(0); //motor 1 stops
    }
  else if (IR_L_val == 1 && IR_R_val == 0) //if left sensor detects line
    {
    motor2.setSpeed(0); //motor 2 stops
    motor1.setSpeed(204); //motor 1 runs at base speed 
    }
  else if (IR_B_val == 1 && IR_R_val == 0 && IR_L_val == 0) //if bottom sensor detects line
    {
    motor2.setSpeed(204); //both motors run
    motor1.setSpeed(204);
    }
  else if (IR_B_val == 1 && IR_R_val == 1 && IR_L_val == 1)
    {
    line = 1;
    motor2.setSpeed(0); //both motors stop
    motor1.setSpeed(0);  
    }
}

//motors stop
void stop(){
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  Serial.println("stopping");
  //delay(500);
}


//pixy scan
void scan() {
  int i;
  uint16_t blocks;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks)
    {
    for (i=0; i<pixy.ccc.numBlocks; i++)
      {
        m_signature = pixy.ccc.blocks[i].m_signature;
        x = pixy.ccc.blocks[i].m_x;  
        width = pixy.ccc.blocks[i].m_width;
        Serial.print("width");
        Serial.print(i);
        Serial.print(": ");
        pixy.ccc.blocks[i].print();
        Serial.println("can detected");  //check if can is detected in serial monitor

      }
    }
    
}

//grab can function
void center() {
errorX = (156-x); //how far can is from center x value 156
steerX = errorX*(0.3); //steering adjustment based on error


//bound error between max and minimum motor speed values
if (steerX < -baseSpeed) { //if it's less than -128, it'll set the value to -128
  steerX = -baseSpeed;
}
if (steerX > baseSpeed) { //if it's > 255 it'll set value to 255
  steerX = baseSpeed;
}

//if x value of can is not centered, steer to adjust
if (x<140)
  {
  //if can is to the left of center, steer left
    motor1.setSpeed(steerX + 204);
    motor2.setSpeed(204 - steerX);
  }
else if (x>175)
  {
  //if can is to the right of center, steer right
    motor2.setSpeed(steerX + 204);
    motor1.setSpeed(204 - steerX);
  }

if (width>150)
//if can is close enough to pixy, close claw
  {
    stop(); //stop motors
    delay(100);
    servo.write(45); //servo moves 45 degrees to close claw
    line=2; //line=2 so line following does not run while it's grabbing/centering
  }
}
