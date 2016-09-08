#include <Arduino.h>

/*
 * On power-up a linear and rotary datum is required. Although sequences can be recorded from relative movement, screen update would not be scaled and represent the actual head position 
 * therefore the system drives the head to left limit - viewed with cable entry on left (near limit) and rotary to a single limit switch, the camera should be attached and aligned to be perpendicular to the linear slide toward the oposite side to the cable entry
 * to ensure the LCD graphics are representative
 */

void EstablishDatum () {
  // display some info to state moving to datum
  LCD.fillScr(221,221,221);
  LCD.setBackColor(221,221,221);                                    // light Gray                                     
  LCD.setColor(VGA_BLUE);
  CTE_LCD.Put_Text("Running to Linear Datum ...", 50, 100, BVS_28);


  // run linear motor to stop
  stepper1.setMaxSpeed(LRSpeed);                                   // speed is set in manual screen
  stepper1.setAcceleration(LRSpeed * 1.75);                        // acceleration - change to be function of speed ????
  analogWrite(11,0);                                               // Lin this was PWM on old driver now 0 to enable motor driver

  if ( digitalRead(65) == LOW) {                                   // (65=A12) if we are already on limit switch then move off of it to ensure we are in a repeatable position
    stepper1.setCurrentPosition(0);
    stepper1.moveTo(1000);                                         // Just move a few hundred steps away from limit
    while ( digitalRead(65) == LOW) {                              // keep moving as long until off limit
      // run stepper
      stepper1.run();                                              // run motor
    }
    stepper1.stop();                                               // stop motor this zeros count to go - dead stop no deceleration
  }
  
  stepper1.setCurrentPosition(0);
  stepper1.moveTo(-150000);
  
  while (digitalRead(65) ) {                                       // keep moving as long as near limit is high
    // run stepper
    stepper1.run();                                                // run motor
    
  }
  stepper1.stop();                                                 // stop motor this zeros count to go - dead stop no deceleration
  stepper1.setCurrentPosition(0);

  if (digitalRead(65) == LOW) {                                                // if we just drove to the near limit switch then it should be LOW if it is then Far Limit must be HIGH
    NearBlock = true;                                              // .. so if that is the case set a block on driving further left (negative direction)
    FarBlock = false;                                              // .. and set FarBlock as false
    LCD.setColor(VGA_GREEN);
    CTE_LCD.Put_Text("Linear Datum Established", 400, 100, BVS_28);
  }
  analogWrite(11,0);                                            // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
  
  LCD.setColor(VGA_BLUE);
  CTE_LCD.Put_Text("Running to Rotary Datum ...", 50, 150, BVS_28);

  // run rotary motor to datum
  stepper2.setMaxSpeed(RSpeed);                                    // speed is set in manual screen
  stepper2.setAcceleration(RSpeed * 1.75);                         // acceleration - change to be function of speed ????
  analogWrite(12, 255);                                            // Lin using PWM on Sleep line to control motor current - 255 full power to move

  if ( digitalRead(67) == HIGH ) {                                 // if we are already on datum switch then move off of it to ensure we are in a repeatable position
    stepper2.setCurrentPosition(0);
    stepper2.moveTo(500);                                          // Just move a few hundred steps away from limit
    while ( digitalRead(67) == HIGH ) {                            // keep moving as until off datum
      // run stepper
      stepper2.run();                                              // run motor
    }
    stepper2.stop();                                               // stop motor this zeros count to go - dead stop no deceleration
  }

  stepper2.setCurrentPosition(0);
  stepper2.moveTo(-45000);                                         // propose a long distance to travel - stopped in next section
  
  while ( digitalRead(67) == LOW ) {                    // keep moving as long as rotary datum is low or 45000 pulses have been sent
    // run stepper
    stepper2.run();                                                // run motor
    
  }
  // stop stepper
  stepper2.stop();                                                 // stop motor this zeros count to go - dead stop no deceleration
  stepper2.setCurrentPosition(0);
  analogWrite(12, 112);
  if (digitalRead(67) == HIGH) {
    LCD.setColor(VGA_GREEN);
    CTE_LCD.Put_Text("Rotary Datum Established  ", 380, 150, BVS_28);
  }

  if (digitalRead(65)==LOW && digitalRead(67)==HIGH) {
    LCD.setColor(VGA_GREEN);
    CTE_LCD.Put_Text("Both Datum Successfully Established", 50, 200, BVS_28);
    CTE_LCD.Put_Text("Touch To Continue", 50, 250, BVS_28);
   }
   else {
    LCD.setColor(VGA_RED);
    CTE_LCD.Put_Text("One or Both Datum Not Established  ", 50, 200, BVS_28);
    CTE_LCD.Put_Text("Check System and Re-Start", 50, 250, BVS_28);
   }



  //int V20=analogRead(A2);                                                   // +20 volt monitor 19.8 volt through 50K and 10K potential divider = 3 volt on input 33 counts per volt  495 counts = 15 volts
  //V5=analogRead(A6);                                                   // +5 volt monitor
  //VFullA=analogRead(A7);                                                   // Battery A full
  //VMidA=analogRead(A8);                                                   // Battery A mid
  //VFullB=analogRead(A9);                                                   // Battery B full
  //VMidB=analogRead(A10);                                                  // Battery B mid



  
  while  (! Touch.dataAvailable()) {                              // wait for touch to continue
    // wait for touch
    int V20=analogRead(A2);                                                   // +20 volt monitor 19.8 volt through 50K and 10K potential divider = 3 volt on input
    LCD.printNumI(V20,600,290,5);
    delay(500);
  }
  
  //Zero encoder counts
  RotaryEnc.write(0);
  LinearEnc.write(0);
  Datum = -1;

}
