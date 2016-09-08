#include <Arduino.h>

/*
 * Drive motors at direction and speed based on position of joystick
 */


void JoyStick() {
  int JoyRotSpeed;
  int JoyLRSpeed;
  int LRMultiplier = 7;
  int RotMultiplier= 5;
  
  // joystick deadzones
  int const JoyLRMin=500;
  int const JoyLRMax=610;
  int const JoyRotMin=500;
  int const JoyRotMax=610;

  // turn both motors to full power  
  analogWrite(11,0);                                                 // Lin using PWM on enable 0
  analogWrite(12, 255);
while ((JoyLR < JoyLRMin) || (JoyLR > JoyLRMax) || (JoyRot <JoyRotMin) || (JoyRot >JoyRotMax))  {       // keep in loop as long as the joystick is out of the deadzone

      JoyLR = analogRead(A14);                                        // read linear joystick value
      
      if (JoyLR < JoyLRMin)  {            // vector right
        
        JoyLRSpeed = (JoyLRMin-JoyLR)*LRMultiplier;                                   // use the readings to get speed value
        if (!FarBlock) {
          stepper1.setSpeed(JoyLRSpeed);
          stepper1.runSpeed();                                            // run motor
        }
      }
      if (JoyLR > JoyLRMax)  {             // vector left 
       
       JoyLRSpeed = (JoyLR-JoyLRMax)*LRMultiplier;                                    // use the readings to get speed value
       if (!NearBlock) {
        stepper1.setSpeed(-JoyLRSpeed);
         stepper1.runSpeed();                                            // run motor
        }
      }
      

      JoyRot = analogRead(A15);                                       // read Rotary joystick values
      if (JoyRot < JoyRotMin)  {            // vector right
        JoyRotSpeed = (JoyRotMin-JoyRot)*RotMultiplier;                                 // use the readings to get speed value
        stepper2.setSpeed(-JoyRotSpeed);
      }
      if (JoyRot > JoyRotMax)  {             // vector left 
       JoyRotSpeed = (JoyRot-JoyRotMax)*RotMultiplier;                                  // use the readings to get speed value
      stepper2.setSpeed(JoyRotSpeed);
      }
      stepper2.runSpeed();                                            // run motor

}

  
//analogWrite(11, 110);                                                 // Lin using PWM on Sleep line to control motor current - 255 full power to move
analogWrite(12, 10);


} // end of joystick
