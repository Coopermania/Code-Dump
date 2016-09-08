#include <Arduino.h>

/*
 * Service any touch or button press
 */



void ScreenTouch() {
  int LCDx, LCDy;

  // check if screen is touched
  LCDx = 0;
  LCDy = 0;
  if (Touch.dataAvailable()) {
    Touch.read();
    LCDx = Touch.getX();
    LCDy = Touch.getY();
    x = LCDx;
    y = LCDy;
  }
  //LCD.printNumI(x, 600, 10, 2);
  //LCD.printNumI(y, 700, 10, 2);

  if ((y >= 2) && (y <= 50)) { // Page Tab Area
    PageSelector();
  }

  if (y > 50) { // In page
    InPageSelection();
  }
}



// Service Page / Tab request ****************************
void PageSelector()
{
  // find where is touched and set page number and draw
  if ((x >= 6) && (x <= 140)) {                                      // Home Tab
    pg = 1;
    PlayScreen();
  }
  if ((x >= 145) && (x <= 250)) {                                    // Manual Tab
    pg = 2;
    drawManualScreen();
  }
  if ((x >= 255) && (x <= 380)) {                                    // Sequencing Tab
    pg = 3;
    drawSequenceScreen();
    drawSequence();
  }
  if ((x >= 385) && (x <= 510)) {                                    // Camera
    pg=4;
    drawCameraScreen();
  }
  if ((x >= 593) && (x <= 790))  {                                   // Utility
    pg = 5;
    drawUtilityScreen();
  }
}





// Service "in page" touch (not page tab area)
void InPageSelection() {
  
  LCD.setFont(BigFont);
  //LCD.printNumI(pg, 160,300,2);
  switch (pg) {
    case 1:                                                         // Home page
      if ((y >= 95) && (y <= 135)) {                                // Still / Video button  
        if ((x > 145) && (x < 235)) {                               // Start
          StillTimeLapse = !StillTimeLapse;                         // toggle Still / Video  variable
          drawStill_Video();
        }
      }  
      if ((y >= 145) && (y <= 185)) {
        if ((x > 100) && (x < 280)) {                               //  Start
           StartSeq = !StartSeq;                                     // toggle StartSeq variable
           drawStart();                                              // refresh the start button "Start" is displayed when StartSeq is false and "Running" when true
           StopSeq = 0;                                              // if STOP has been pressed previously then reset it - leaving it at STOPPED (if it is) up until now shows it has been pressed
           drawStop();                                               // refresh stop button
           // Play Sequence
           SequencePlayBack();                                       // play Still TimeLapse  sequence
        }
        if (StartSeq){
          if ((x > 320) && (x < 500))   {                             //  Pause
            PauseSeq = !PauseSeq;                                     // toggle the PauseSeq variable, false displays PAUSE, true displays PAUSED
            drawPause();                                              // refresh the pause button
          }
        }
        if (StartSeq){
          if ((x > 540) && (x < 685)) {                                 //  Stop
            StopSeq = !StopSeq;                                         // toggle the StopSeq variable
            drawStop();                                                 // refresh button false = STOP true = STOPPED
            StartSeq = 0;                                               // if we have stopped the sequence then reset to START from RUNNING
            drawStart();                                                // refresh start button
            PauseSeq = 0;                                               // if paused then reset it to PAUSE from PAUSED
            drawPause();
          }
        }
               
      }
      break;
    case 2:    // Manual Page
      if (x < 450) {                                                  // left side of page linear and rotatry movement
        if ((y >= 105) && (y <= 185)) {                               // top row

          if ((x > 25) && (x < 105))  {                               //  Move Left
            if (!NearBlock) {                                         // If NearBlock is true then we are on limit switch do not drive
              stepper1.setMaxSpeed(LRSpeed);                          // speed is set in manual screen
              stepper1.setAcceleration(LRSpeed * 1.75);               // acceleration - change to be function of speed ????
              analogWrite(11,0);                                   // Lin using PWM on Sleep line to control motor current - 255 full power to move
              stepper1.setCurrentPosition(0);
              stepper1.moveTo(-150000);                               // propose a long distance to travel - stopped in next section
              while (Touch.dataAvailable() && NearBlock == false) {   // keep moving as long as screen is touched
                // run stepper
                stepper1.run();                                       // run motor
              }
              //newLinear = LinearEnc.read();
              //LCD.printNumI(newLinear, 540,300,6);
              // stop stepper
              stepper1.stop();                                        // stop motor this zeros count to go - see next
              if (!NearBlock) {                                       // don't decelerate if we hit the limit switch
                stepper1.runToPosition();                             // this adds some extra counts to allow deceleration
              }
              stepper1.setCurrentPosition(0);                         // zeros current position
              analogWrite(11,0);                                   // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
            }
            else {
              stepper1.stop();
            }
          }

          if ((x > 115) && (x < 185)) {                               //  Move Right
            if (!FarBlock) {                                          // If FarBlock is true then we are on limit switch do not drive
              stepper1.setMaxSpeed(LRSpeed);                          // speed is set in manual screen
              stepper1.setAcceleration(LRSpeed * 1.75);               // acceleration - change to be function of speed ????
              analogWrite(11,0);                                   // Lin using PWM on Sleep line to control motor current - 255 full power to move
              stepper1.setCurrentPosition(0);
              stepper1.moveTo(100000);                                // propose a long distance to travel - stopped in next section

              while (Touch.dataAvailable() && FarBlock == false) {    // keep moving as long as screen is touched
                // run stepper
                stepper1.run();                                       // run motor
              }
              newLinear = LinearEnc.read();
              LCD.printNumI(newLinear, 540, 300, 6);
              // stop stepper
              stepper1.stop();                                        // stop motor this zeros count to go - see next
              if (!FarBlock) {                                        // don't decelerate if we hit the limit switch
                stepper1.runToPosition();                             // this adds some extra counts to allow deceleration
              }
              stepper1.setCurrentPosition(0);                         // zeros current position
              analogWrite(11,0);                                   // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
            }
            else {
              stepper1.stop();
            }
          }

          if ((x > 355) && (x < 440)) {                               //  Rotate Clockwise -right
            stepper2.setMaxSpeed(RSpeed);                             // speed is set in manual screen
            stepper2.setAcceleration(RSpeed * 1.75);                  // acceleration - change to be function of speed ????
            analogWrite(12, 255);                                     // Lin using PWM on Sleep line to control motor current - 255 full power to move
            stepper2.setCurrentPosition(0);
            stepper2.moveTo(-41600);                                  // propose a long distance to travel 1 rev - stopped in next section
            while (Touch.dataAvailable()) {                           // keep moving as long as screen is touched
              // run stepper
              stepper2.run();                                         // run motor
            }
            // stop stepper

            stepper2.stop();                                         // stop motor this zeros count to go - see next
            stepper2.runToPosition();                                // this adds some extra counts to allow deceleration
            stepper2.setCurrentPosition(0);                          // zeros current position
            analogWrite(12, 10);                                     // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
          }
          if ((x > 250) && (x < 335)) {                              //  Rotate Counter Clockwise - left
            stepper2.setMaxSpeed(RSpeed);                            // speed is set in manual screen
            stepper2.setAcceleration(RSpeed * 1.75);                 // acceleration - change to be function of speed ????
            analogWrite(12, 255);                                    // Lin using PWM on Sleep line to control motor current - 255 full power to move
            stepper2.setCurrentPosition(0);
            stepper2.moveTo(41600);                                  // propose a long distance to travel = 1 rev - stopped in next section
            while (Touch.dataAvailable()) {                          // keep moving as long as screen is touched
              // run stepper
              stepper2.run();                                        // run motor
            }
            // stop stepper
            newRotary = RotaryEnc.read();
            LCD.printNumI(newRotary, 540, 300, 6);
            stepper2.stop();                                         // stop motor this zeros count to go - see next
            stepper2.runToPosition();                                // this adds some extra counts to allow deceleration
            stepper2.setCurrentPosition(0);                          // zeros current position
            analogWrite(12, 10);                                     // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
          }
        }
        if ((y >= 200) && (y <= 235)) {                              // Bottom row
          if ((x > 25) && (x < 75)) {                                //  Linear Decrease Speed
            LinSpeed--;
            LinSpeedUpdate();
          }
          if ((x > 175) && (x < 215)) {                             //  Linear Increase Speed
            if (LinSpeed < 255) {
              LinSpeed++;
              LinSpeedUpdate();
            }
          }
          if ((x > 240) && (x < 290))  {                            //  Rotary Decrease Speed
            if (RotSpeed > 0)  {
              RotSpeed--;
              RotSpeedUpdate();
            }
          }
          if ((x > 380) && (x < 430))  {                           //  Rotary Increase Speed
            if (RotSpeed < 255)  {
              RotSpeed++;
              RotSpeedUpdate();
            }
          }
        }

        // adjusters - Linear and Rotary Movement Speed - Direct Drag
        if ((y >= 255) && (y <= 280)) {
          if ((x > 25) && (x < 215)) {                              // Linear Speed area
            flValueOfAnyBox = (255.0 / 190.0) * (x - 25);           // max value / length of box in pixels * touched position (x) minus offset from left to start of box
            LinSpeed = flValueOfAnyBox;
            LinSpeedUpdate();
          }
          if ((x > 250) && (x < 440))  {                            // Rotary Speed area
            flValueOfAnyBox = (255.0 / 190.0) * (x - 250);
            RotSpeed = flValueOfAnyBox;
            RotSpeedUpdate();
          }
        }
      } // end of x< 450 Manual In Page case

      else if (x > 450) {                                               // right side of page focus and shoot
        if ((y >= 140) && (y <= 220)) {
          if ((x > 470) && (x < 600)) {                               //  Focus
            LCD.print("Focus              ", 460, 300);
            // Focus - Pin is Analog 0 used as Digital output
            digitalWrite(A0, HIGH);


          }
          if ((x > 640) && (x < 780)) {                             //  Shoot
            LCD.print("Shoot              ", 460, 300);
            // Shoot - Pin is Analog 1 used as Digital output
            digitalWrite(A1, HIGH);
            delay(500);

            digitalWrite(A0, LOW);
            digitalWrite(A1, LOW);
          }
        }
        if ((y >= 240) && (y <= 290)) {
          if ((x > 470) && (x < 780)) {                            //  Multi Shot
            LCD.print("Multi Shot              ", 460, 300);

            // Focus - Pin is Analog 0 used as Digital output
            //digitalWrite(A0,HIGH);
            // Shoot - Pin is Analog 1 used as Digital output
            //digitalWrite(A1,HIGH);
            //delay(500);
            //digitalWrite(A0,LOW);
            //digitalWrite(A1,LOW);

          }
        }
      } // end of X>450


      break; // Case 2 manual page

    case 3:    // Sequencing Page
      if ((y >= 110) && (y <= 180)) {                               // Sequencing buttons - linear is never zero if a point has been set, zero means no point set so we add 1 to any linear value
        LCD.setColor(VGA_PURPLE);
        LCD.setFont(BigFont);
        // The units for LinMove and RotMove are as received from encoder
        // The linear movment is 29.68 counts per mm of travel
        // The rotary movement is 7200 for 1 rev of the camera head  - 1440 counts per rev of enc and 5:1 gear ratio

        if ((x > 25) && (x < 120)) {                            //  End Point
          Sequence = 9;                                         // End Point resets counter to 9 and clears the array
          memset (LinMove, 0, sizeof(LinMove));                 // zero arrays
          memset (RotMove, 0, sizeof(RotMove));
          memset (MoveType, 0, sizeof(MoveType));
          //memset (SeqDir,0,sizeof(SeqDir));
          LinMove [Sequence] = newLinear + 1;                   // assign linear position to the array - once set, never zero
          RotMove [Sequence] = newRotary;                       // assign rotary position in the array
          MoveType [Sequence] = 4;                              // record sequence type
          LCD.setColor(243, 254, 180);                          // Pale yellow
          LCD.fillRoundRect(460, 90, 790, 190);                 // Clears any sequence text
          LCD.setColor(VGA_PURPLE);
          LCD.setBackColor(243, 254, 180);                      // Pale yellow
          LCD.drawRoundRect(460, 60, 790, 190);                 // Draw border
          CTE_LCD.Put_Text("Available Actions :-  ", 470, 70,BVS_22);
          CTE_LCD.Put_Text("Way Point", 480, 95, BVS_22);
          CTE_LCD.Put_Text("Fixed Point", 480, 115, BVS_22);
          CTE_LCD.Put_Text("Start Point", 480, 135, BVS_22);
          CTE_LCD.Put_Text("Steps Remaining", 470, 160, BVS_22);
          CTE_LCD.Put_Text(String(Sequence), 720, 160, BVS_22);
        } // End point

        if ((x > 130) && (x < 225)) {                           // Way Point
          bool WayPointOK ;
          if (Sequence > 1) {                                   // way point must not be last of ten possible, have to leave room for Start
            LCD.setColor(243, 254, 180);                        // Pale yellow
            LCD.fillRoundRect(460, 90, 790, 190);               // Clears any sequence text
            LCD.setColor(VGA_PURPLE);
            LCD.setBackColor(243, 254, 180);                    // Pale yellow
            LCD.drawRoundRect(460, 60, 790, 190);               // Draw border
            if (MoveType [Sequence] != 3) {                     // we haven't decremented Sequence yet as it may not be  valid step so the last step is still active  ..
               WayPointOK =true;                                // .. if last is not fixed then continue as normal increment Sequence and add way point
            }
            else if (MoveType [Sequence] == 3) {                // else if the last was a way point then check the one before - must be in pairs
               if (MoveType [Sequence+1] != 3) {                // if there are not two Fixed Points in a row then don't action way point 
                  WayPointOK = false;
               }
               else {                                           // must be two Fixed -  ok to add Way Point
                WayPointOK = true ;
               } 
            } // type is fixed
          } // sequence >1
        if (WayPointOK) {
            Sequence--;
            LinMove [Sequence] = newLinear + 1;
            RotMove [Sequence] = newRotary;
            MoveType [Sequence] = 2;
            CTE_LCD.Put_Text("Available Actions :-  ", 470, 70,BVS_22);
            CTE_LCD.Put_Text("Way Point", 480, 95, BVS_22);
            CTE_LCD.Put_Text("Fixed Point", 480, 115, BVS_22);
            CTE_LCD.Put_Text("Start Point", 480, 135, BVS_22);
            CTE_LCD.Put_Text("Steps Remaining", 470, 160, BVS_22);
            CTE_LCD.Put_Text(String(Sequence), 720, 160, BVS_22);
        }
        else  {    
            LCD.setColor(VGA_RED);
            CTE_LCD.Put_Text("Second Fixed Point Required", 480, 95, BVS_22);
            LCD.setColor(VGA_PURPLE);
        }
       } // way point

        if ((x > 235) && (x < 342)) {                           //  Fixed Target
          if (Sequence > 1)  {                                  // two fixed points are required so must be >1 to get another Fixed and Start within 10
            LCD.setColor(243, 254, 180);                        // Pale yellow
            LCD.fillRoundRect(460, 90, 790, 190);               // Clears any sequence text
            LCD.setColor(VGA_PURPLE);
            LCD.setBackColor(243, 254, 180);                    // Pale yellow
            LCD.drawRoundRect(460, 60, 790, 190);               // Draw border

            // doesnt matter what was last drawn fixed is valid if sequence >1
            Sequence--;
            LinMove [Sequence] = newLinear + 1;
            //if (LinMove [Sequence]>LinMove[Sequence-1])  { SeqDir[Sequence]=1; }  else   { SeqDir[Sequence]=0; }
            RotMove [Sequence] = newRotary;
            MoveType [Sequence] = 3;
            CTE_LCD.Put_Text("Available Actions :-  ", 470, 70,BVS_22);
            CTE_LCD.Put_Text("Steps Remaining", 470, 160, BVS_22);
            CTE_LCD.Put_Text(String(Sequence), 720, 160, BVS_22);
            if (MoveType [Sequence + 1] == 3) {                 // If last was fixed this makes fixed valid so next can be anything
              CTE_LCD.Put_Text("Way Point", 480, 95, BVS_22);
              CTE_LCD.Put_Text("Fixed Point", 480, 115, BVS_22);
              CTE_LCD.Put_Text("Start Point", 480, 135, BVS_22);
            }
            else   {
              CTE_LCD.Put_Text("Fixed Point", 480, 100, BVS_22);               // This first Fixed so need another
            }
          }
        } // fixed point

        if ((x > 355) && (x < 445)) {                           //  Start Point
          if (Sequence >= 0) {
            LCD.setColor(243, 254, 180);                        // Pale yellow
            LCD.fillRoundRect(460, 90, 790, 190);               // Clears any sequence text
            LCD.setColor(VGA_PURPLE);
            LCD.setBackColor(243, 254, 180);                    // Pale yellow
            LCD.drawRoundRect(460, 60, 790, 190);               // Draw border
            Sequence--;
            LinMove [Sequence] = newLinear + 1;
            //if (LinMove [Sequence]>LinMove[Sequence-1])  { SeqDir[Sequence]=1; }  else   { SeqDir[Sequence]=0; }
            RotMove [Sequence] = newRotary;
            MoveType [Sequence] = 1;
            CTE_LCD.Put_Text("Sequence Complete", 470, 70, BVS_22);
          }
          drawComitSeq ();

        }
        if ((x > 490) && (x < 768)) {
          if ((y > 100) && (y < 150)) {                         // Comit Button
            pg=4;                                               // make pg 4 or drawsequence will appear on Camera screen as we are changing pages mid procedure
            drawCameraScreen();
          }
          if ((y > 160) && (y < 185)) {                         // Cancel Button
            Sequence = 0;                                       // Cancel Button resets counter to one and clears the arrays
            memset (LinMove, 0, sizeof(LinMove));               // zero arrays
            memset (RotMove, 0, sizeof(RotMove));
            memset (MoveType, 0, sizeof(MoveType));
            //memset (SeqDir,0,sizeof(SeqDir));
            LCD.setColor(243, 254, 180);                        // Pale yellow
            LCD.fillRoundRect(460, 90, 790, 190);               // Clears any sequence text
            LCD.setColor(VGA_PURPLE);
            LCD.setBackColor(243, 254, 180);                    // Pale yellow
            LCD.drawRoundRect(460, 60, 790, 190);               // Draw border
          }
        }
        
        LCD.setColor(VGA_PURPLE);
        LCD.drawRoundRect(15, 200, 790, 310);

        if (pg != 4 )  {
          drawSequence();
        }
      }
      break;  // case 3 Sequence


    case 4:    // Camera Page

      // adjusters - Focus / Shot / Interval Buttons
      if ((y >= 90) && (y <= 125)){
        //  Decrement Focus
        if ((x > 25) && (x < 70)) {
          if (FocusTime > 0.5) {
            if ((FocusTime > 0.0) && (FocusTime <=5)) {
            FocusTime=FocusTime - 0.1;
            }
          if (FocusTime < 0.5) {
             FocusTime = 0.5;
          }
          FocusTimeUpdate();
         }
        }

        //  Increment Focus
        if ((x > 210) && (x < 255)) {
            if ((FocusTime >=0.0) && (FocusTime < 5)) {
              FocusTime = FocusTime + 0.1;
            }
            if (FocusTime > 5) {
              FocusTime = 5;
            }
            FocusTimeUpdate();
          }
        
        //  Decrement Shot Time
        if ((x > 285) && (x < 330)) {
          if (ShotTime > 0.5) {
            if ((ShotTime > 0.5) && (ShotTime <= 2)) {
            ShotTime=ShotTime - 0.1;
            }
          if (ShotTime < 0) {
             ShotTime = 0.5;
          }
          ShotTimeUpdate();
         }
        }

        //  Increment Shot Time
        if ((x > 470) && (x < 515)) {
            if ((ShotTime >=0.0) && (ShotTime <=2)) {
              ShotTime = ShotTime + 0.1;
            }
            if (ShotTime > 2) {
              ShotTime = 2;
            }
           
            ShotTimeUpdate();
          }

        //  Decrement Interval Time
        if ((x > 545) && (x < 590)) {
          if (Interval > FocusTime+ShotTime) {
            if ((Interval > 0.5) && (Interval <= 5)) {
            Interval=Interval - 0.1;
            }
          else if ((Interval > 5) && (Interval < 20)) {
          Interval = Interval - 1;
          }
          else if ((Interval >= 20) && (Interval <= 100)) {
            Interval = Interval - 2;
          }
          if (Interval < FocusTime+ShotTime) {
            Interval = FocusTime+ShotTime;
          }
          IntervalUpdate();
         }
        }

        //  Increment Interval Time
        if ((x > 730) && (x < 775)) {
          if ((Interval >=0.0) && (Interval <= 5)) {
            Interval = Interval + 0.1;
          }
          else if ((Interval > 5) && (Interval < 20)) {
            Interval = Interval + 1;
          }
          else if ((Interval >= 20) && (Interval < 100)) {
            Interval = Interval + 2;
          }
          if (Interval > 100) {
            Interval = 100;
          }
            IntervalUpdate();
          }
      }

      // adjusters - Focus Time / Shot Time /Interval / Shots - Direct Drag
      if ((y >= 135) && (y <= 160)) {
        if ((x > 25) && (x < 255)) {                              // Focus area
          flValueOfAnyBox = (5.0 / 230.0) * (x - 25);             // max value / length of box in pixels * touched position (x) minus offset from left to start of box
          FocusTime = flValueOfAnyBox;
          if (FocusTime<0.5) { FocusTime=0.5;}
          FocusTimeUpdate();
        }
        if ((x > 285) && (x < 515))  {                            // Shot Time area
          flValueOfAnyBox = (2.0 / 230.0) * (x - 285);
          ShotTime = flValueOfAnyBox;
          if (ShotTime<0.5) { ShotTime=0.5;}
          ShotTimeUpdate();
        }
        if ((x > 545) && (x < 775)) {                             // Total Shots area
          flValueOfAnyBox = (100.0 / 230.0) * (x - 545);
          Interval = flValueOfAnyBox;
          IntervalUpdate();
        }
      }
      if ((y >= 235) && (y <= 270)) {
         // Decrement Total Shots Button
        if ((x > 25) && (x < 70)) {
          if (TotalShots > 0) {
            TotalShots--;
            TotalShotsUpdate();
          }
        }
        //  Increment Total Shots Button
        if ((x > 210) && (x < 255)) {
          if (TotalShots < 2000) {
            TotalShots++;
            TotalShotsUpdate();
          }
        }
      }

      if ((y >= 280) && (y <= 305)) {
        //  Decrement Total Shots   Direct Drag
        if ((x > 25) && (x < 255)) {                              // Focus area
          flValueOfAnyBox = (2000 / .0) * (x - 25);           // max value / length of box in pixels * touched position (x) minus offset from left to start of box
          TotalShots = flValueOfAnyBox;
          if (TotalShots<0.5) { TotalShots=0.5;}
            TotalShotsUpdate();
        }
      }
 
      break;
  } // end of in-page case
      
      if ((y >= 435) && (y <= 475)) { // Touch Area for  "drive to" next sequence point - motor with gearbox is now too hard to move eaisly so drive to approx position then use joystick to fine tune
        if ((x > 65) && (x < 735)) {
          newRotary = RotaryEnc.read();                                // read encoders to find out where we are
          newLinear = LinearEnc.read();
          analogWrite(11,0);                                        // Lin using PWM on Sleep line to control motor current - 255 full power to move
          analogWrite(12, 255);                                        // Rot using PWM on Sleep line to control motor current - 255 full power to move
          stepper1.setCurrentPosition(0);
          long LinStart = (x - 65) * 1.76 * 29.68 - newLinear;         // get screen units and offset into encoder units
          LinStart = LinStart * StepsPerLinCount;                      // convert to stepper units
          stepper1.moveTo(LinStart);                                   // Move to start Lin location
          LCD.printNumI(LinStart, 400, 400, 4);
          while (stepper1.distanceToGo() != 0)  {                      // run motor
            // run steppers
            stepper1.run();
          }
          analogWrite(11,0);                                        // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
          analogWrite(12, 140);                                        // use PWM to "hold" motor
          delay(500);
          newRotary = RotaryEnc.read();
          newLinear = LinearEnc.read();
        }

        if ((x > 40) && (x < 60))  {
          // drive to near limit
          // run linear motor to stop
          stepper1.setMaxSpeed(LRSpeed);                                   // speed is set in manual screen
          stepper1.setAcceleration(LRSpeed * 1.75);                        // acceleration - change to be function of speed ????
          analogWrite(11,0);                                            // Lin using PWM on Sleep line to control motor current - 255 full power to move
        
          if ( digitalRead(65) == LOW) {                                   // if we are already on limit switch then move off of it to ensure we are in a repeatable position
            stepper1.setCurrentPosition(0);
            stepper1.moveTo(700);                                          // Just move a few hundred steps away from limit
            while ( digitalRead(65) == LOW) {                              // keep moving as long until off limit
              // run stepper
              stepper1.run();                                              // run motor
            }
            stepper1.stop();                                               // stop motor this zeros count to go - dead stop no deceleration
          }
          stepper1.setCurrentPosition(0);
          stepper1.moveTo(-150000);
          while ( digitalRead(65)) {                                       // keep moving as long as near limit is high
            // run stepper
            stepper1.run();                                                // run motor
          }
          stepper1.stop();                                                 // stop motor this zeros count to go - dead stop no deceleration
          stepper1.setCurrentPosition(0);
        
          if (A12 == LOW) {                                                // if we just drove to the near limit switch then it should be LOW if it is then Far Limit must be HIGH
            NearBlock = true;                                              // .. so if that is the case set a block on driving further left (negative direction)
            FarBlock = false;                                              // .. and set FarBlock as false
          }
          analogWrite(11,0);                                            // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
        }
        if ((x > 740) && (x < 760))  {
          // drive to far limit
          // run linear motor to stop
          stepper1.setMaxSpeed(LRSpeed);                                   // speed is set in manual screen
          stepper1.setAcceleration(LRSpeed * 1.75);                        // acceleration - change to be function of speed ????
          analogWrite(11,0);                                            // Lin using PWM on Sleep line to control motor current - 255 full power to move
        
          if ( digitalRead(66) == LOW) {                                   // if we are already on limit switch then move off of it to ensure we are in a repeatable position
            stepper1.setCurrentPosition(0);
            stepper1.moveTo(-700);                                         // Just move a few hundred steps away from limit
            while ( digitalRead(66) == LOW) {                              // keep moving as long until off limit
              // run stepper
              stepper1.run();                                              // run motor
            }
            stepper1.stop();                                               // stop motor this zeros count to go - dead stop no deceleration
          }
          stepper1.setCurrentPosition(0);
          stepper1.moveTo(+150000);
          while ( digitalRead(66)) {                                       // keep moving as long as near limit is high
            // run stepper
            stepper1.run();                                                // run motor
          }
          stepper1.stop();                                                 // stop motor this zeros count to go - dead stop no deceleration
          stepper1.setCurrentPosition(0);
        
          if (A13 == LOW) {                                                // if we just drove to the far limit switch then it should be LOW if it is then Near Limit must be HIGH
            NearBlock = false;                                             // .. so if that is the case set a block on driving further left (posative direction)
            FarBlock = true;                                               // .. and set NearBlock as false
          }
          analogWrite(11,0);                                            // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
        }
        
        drawSlider(newRotary, newLinear);
      }
      
      if ((y >= 355) && (y <= 385)) { // Touch Area for  "drive to" next sequence point 
        if ((x > 65) && (x < 735)) {
          newRotary = RotaryEnc.read();                                // read encoders to find out where we are
          //newLinear = LinearEnc.read();
          analogWrite(11,0);                                        // Lin using PWM on Sleep line to control motor current - 255 full power to move
          analogWrite(12, 255);                                        // Rot using PWM on Sleep line to control motor current - 255 full power to move
          stepper2.setCurrentPosition(0);
          long RotStart;
          
          if ((x > 65) && (x < 390)){                                  // Get direction
            RotStart = map(x, 65, 390,  7200,0);
            LCD.printNumI(RotStart, 500, 240, 5);
            RotStart=RotStart - (0-newRotary);
          }
          else if ((x > 410) && (x < 735)) {
            RotStart = map(x, 410, 735, 0,-7200);
            LCD.printNumI(RotStart, 500, 240, 5);
            RotStart=RotStart - (0-newRotary) ;
          }
          else if ((x > 390) && (x < 410)) {
            // drive to datum
            // run rotary motor to datum
            stepper2.setMaxSpeed(RSpeed);                                    // speed is set in manual screen
            stepper2.setAcceleration(RSpeed * 1.75);                         // acceleration - change to be function of speed ????
            analogWrite(12, 255);                                            // Lin using PWM on Sleep line to control motor current - 255 full power to move
          
            if ( digitalRead(67) == HIGH ) {                                  // if we are already on datum switch then move off of it to ensure we are in a repeatable position
              stepper2.setCurrentPosition(0);
              stepper2.moveTo(500);                                            // Just move a few hundred steps away from limit
              while ( digitalRead(67) == HIGH ) {                               // keep moving as until off datum
                // run stepper
                stepper2.run();                                                // run motor
              }
              stepper2.stop();                                              // stop motor this zeros count to go - dead stop no deceleration
            }
          
            stepper2.setCurrentPosition(0);
            stepper2.moveTo(-45000);                                         // propose a long distance to travel - stopped in next section
          
            while ( digitalRead(67) == LOW ) {                               // keep moving as long as rotary datum is low
              // run stepper
              stepper2.run();                                               // run motor
            }
            // stop stepper
            stepper2.stop();                                                 // stop motor this zeros count to go - dead stop no deceleration
            stepper2.setCurrentPosition(0);
            analogWrite(12, 112);

          }
          LCD.printNumI(RotStart, 300, 240, 5);
          LCD.printNumI(newRotary, 300, 260, 5);
          
          RotStart = RotStart * StepsPerRotCount;                      // convert to stepper units
          stepper2.moveTo(RotStart);                                   // Move to start Lin location
          
          while (stepper2.distanceToGo() != 0)  {                      // run motor
            // run stepper
            stepper2.run();
          }
          analogWrite(11,0);                                        // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
          analogWrite(12, 140);                                        // use PWM to "hold" motor
          delay(500);
          newRotary = RotaryEnc.read();
          newLinear = LinearEnc.read();
        }

        drawSlider(newRotary, newLinear);
      }
}  // end of in-page routine


