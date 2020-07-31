#include <Arduino.h>
#include "DRV8825.h"
#include "MultiDriver.h"
#include "SyncDriver.h"


#define DIR_R 12
#define STEP_R 5
#define MS1_R 4
#define MS2_R 13
#define MS3_R 8
#define DIR_L 7
#define STEP_L 6
#define MS1_L 9
#define MS2_L 10
#define MS3_L 11
#define RPM 3000
#define SPR 200
#define MOTOR_ACCEL 1000
#define MOTOR_DECEL 1000
#define MICROSTEPS 16
#define Rbutton A0
#define Lbutton A1
#define distancepin 3
int turndegree = 410;
int snaketime = 0;

bool accelFlag = false;
DRV8825 stepperR(SPR, DIR_R, STEP_R, MS1_R, MS2_R, MS3_R);
DRV8825 stepperL(SPR, DIR_L, STEP_L, MS1_L, MS2_L, MS3_L);

SyncDriver controller(stepperR, stepperL);

int speeddelay = 10;
int directionFlag = 0;
int reverseFlag = 0;
 

void setup() {
   
  pinMode(Rbutton, INPUT);
  pinMode(Lbutton, INPUT);
  stepperR.begin(RPM, MICROSTEPS);
  stepperL.begin(RPM, MICROSTEPS);
  stepperR.setSpeedProfile(stepperR.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  stepperL.setSpeedProfile(stepperL.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
   
}
void accelforward(){
  int acceldelay = 550;
  for (int i=0; i<=549; ++i){
  digitalWrite(DIR_R, HIGH);
  digitalWrite(DIR_L, HIGH);
  digitalWrite(STEP_L, HIGH);
  digitalWrite(STEP_R, HIGH);
  delayMicroseconds(acceldelay);
  digitalWrite(STEP_L, LOW);
  digitalWrite(STEP_R, LOW); 
  delayMicroseconds(acceldelay);
  acceldelay--;
  }
}
void decelforward(){
  int acceldelay = 10;
  for (int i=0; i<=540; ++i){
  digitalWrite(DIR_R, HIGH);
  digitalWrite(DIR_L, HIGH);
  digitalWrite(STEP_L, HIGH);
  digitalWrite(STEP_R, HIGH);
  delayMicroseconds(acceldelay);
  digitalWrite(STEP_L, LOW);
  digitalWrite(STEP_R, LOW); 
  delayMicroseconds(acceldelay);
  acceldelay++;
  }
}
void turnleft(){
controller.rotate(turndegree, -turndegree);
}
void turnright(){
 controller.rotate(-turndegree, turndegree);
}
 void turnaroundR(){
 controller.rotate(-turndegree*2, turndegree*2);
}
void turnaroundL(){
 controller.rotate(turndegree*2, -turndegree*2);
}
void moveforward(){  
  digitalWrite(DIR_R, HIGH);
  digitalWrite(DIR_L, HIGH);
  digitalWrite(STEP_L, HIGH);
  digitalWrite(STEP_R, HIGH);
  delayMicroseconds(speeddelay);
  digitalWrite(STEP_L, LOW);
  digitalWrite(STEP_R, LOW); 
  delayMicroseconds(speeddelay); 
}
void movebackward(){
  controller.rotate(-90, -90);
}

   void snake1(){
   //delay(900);
   digitalRead(distancepin);
       if(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700 && accelFlag == false){
                  accelforward();
                  accelFlag = true;
       }
       while(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700){                  
                  moveforward();                                   
       }
       
      if (digitalRead(distancepin)){ decelforward(); turnleft(); directionFlag=1; return directionFlag;}
        movebackward();
        turnleft();
        directionFlag=1;
        return directionFlag;  
   }

    void snake2(){
    //delay(900);
    digitalRead(distancepin);
     if(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700 && accelFlag == false){
                  accelforward();
                  accelFlag = true;
     }
      while(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700 && snaketime <= 2500){
                  moveforward();                 
                  snaketime++;                
      }  
      if (snaketime >= 2500) {reverseFlag--;}
      if ( analogRead(Lbutton) >= 700 && analogRead(Rbutton) >= 700){movebackward();}
      if (!digitalRead(distancepin)){reverseFlag++; decelforward(); turnleft(); directionFlag=2; return directionFlag;}  
      if (digitalRead(distancepin)){reverseFlag++;  decelforward(); turnleft(); directionFlag=2; return directionFlag;}
       
    }
      void snake3(){
    //delay(900);
    digitalRead(distancepin);
       if(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700 && accelFlag == false){
                  accelforward();
                  accelFlag = true;
       }
       while(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700){
                  moveforward();                  
       }
      
      if (digitalRead(distancepin)){ decelforward(); turnright(); directionFlag=3; return directionFlag;}
       movebackward();
        turnright();
        directionFlag=3;
        return directionFlag;  
   }  
  
    void snake4(){
      //delay(900);
      digitalRead(distancepin);
      if(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700 && accelFlag == false){
                  accelforward();
                  accelFlag = true;
      }
      while(!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700 && snaketime <= 2500){
                  moveforward();                 
                  snaketime++; 
                   
      }
      if (snaketime >= 2500) {reverseFlag--;}
      if ( analogRead(Lbutton) >= 700 && analogRead(Rbutton) >= 700){movebackward();}
      if (!digitalRead(distancepin)){reverseFlag++; decelforward(); turnright(); directionFlag=0; return directionFlag;}  
      if (digitalRead(distancepin)){reverseFlag++; decelforward(); turnright(); directionFlag=0; return directionFlag;}
           
  } 
  void gotstuckL(){
    delay(500);
    if (!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700){return;}
      turnaroundL();
      reverseFlag = 0;
    
   }
   void gotstuckR(){
    delay(500);
    if (!digitalRead(distancepin) && analogRead(Lbutton) <= 700 && analogRead(Rbutton) <= 700){return;}
      turnaroundR();
      reverseFlag = 0;
    
   }
void loop() {
   
  
   if (directionFlag == 0){snake1();}
   accelFlag = false;                                                                 //preparing for acceleration in the next cycle
   snaketime = 0;                                                                     //preparing to count for the next cycle of the "snake" pattern
   
   if(directionFlag == 1){snake2();}
   if (reverseFlag >= 3){gotstuckL();}
   accelFlag = false;
   
   if (directionFlag == 2){snake3();}   
   accelFlag = false;
    snaketime = 0;
    
    if(directionFlag == 3){snake4();}
    if (reverseFlag >= 3){gotstuckR();} 
    accelFlag = false;
}
    

   

    
    
  
  
  
  
      
    
