#include <Arduino.h>

/*
 * Read, decode and drive head as instructed by recorded sequence
 */



void SequencePlayBack() {
  
  TotalShots = 300;
  taken = 0;
  Timer3.attachInterrupt(PauseOrStopIsr );                               // attach the service routine here
  //LCD.print("Attach ", 20, 220);

  // steps per mm ---- 48.51752021563342 mm per rev of encoder;   1mm = 1440 / 48.51752021563342 = 29.68 counts
  
  //LinMove[9]=30000;
  //LinMove[8]=1000;
  //LinMove[7]=8000;
 // LinMove[6]=5000;

  // steps per degree --- 7200 per rev of encoder;   1 degree = 7200 /360 = 20
  //RotMove[9]=-900;
  //RotMove[8]= 900;
  //RotMove[7]= -0;
  //RotMove[6]= 0;

 // MoveType[9]=4; // end
  //MoveType[8]=1; // way
  //MoveType[7]=2;
  //MoveType[6]=1; //start
  
  drawSequence();
  newRotary = RotaryEnc.read();                                  // read encoders to find out where we are
  newLinear = LinearEnc.read();
  drawSlider(newLinear,newRotary);
  
  // all steps counted as movement - negative values are reverse direction but still steps!!!

  // Set speed and acceleration before movement loop as they are expensive calls
  stepper1.setMaxSpeed(LRSpeed);
  stepper1.setAcceleration(LRSpeed * 1.75);                                  //(LRSpeed*1.75);
  stepper2.setMaxSpeed(RSpeed);
  stepper2.setAcceleration(RSpeed * 1.75);                                    //(RSpeed*1.75);

  // Arrays are in encoder counts
  long Lin;
  long Rot;
  totalsteps = 0;

  // Loop through the sequence array, find the total steps and total time for all segments
  // Arrays are in absolute encoder coordinates and should all be positive - however they may move positive to the current location or negative of it to
  // count total steps and total time we need to count all values so we must work with unsigned integers Lin and Rot

  for (int n = 0; n < 9; n++) {                                          // loop through arrays to find total steps and time
    if ((LinMove[n] > 0) || (RotMove[n] > 0)) {
      Lin = 0;
      Rot = 0;
      Lin = LinMove[n + 1] - LinMove[n];                                 // get the value without sign
      Lin = abs(Lin);
      Rot = RotMove[n + 1] - RotMove[n];                                 // get the value without sign
      Rot = abs(Rot);
      if (Lin >= Rot) {                                                  // compare the magnitude regardless of + or -
        totalsteps = totalsteps + (Lin * StepsPerLinCount);              // count total steps
        TotTim = TotTim + (Lin * StepsPerLinCount) * LRSpeed *.6;
      }
      else if (Rot > Lin) {
        // compare the magnitude regardless of + or -
        totalsteps = totalsteps + (Rot * StepsPerRotCount);             // total steps should be a positive number
        TotTim = TotTim + (Rot * StepsPerRotCount) * RSpeed *.6;            // total time should be a positive number
      }
    }
  }
  stepspershot = totalsteps / TotalShots;

  //LCD.printNumI(totalsteps, 200,200,4);
  //LCD.printNumI(stepspershot, 200,220,4);
  TotTim = TotTim / 10000 / 60;                                        // get into seconds - not sure about this need to check it!!!!
  TotTim = TotTim + (Interval + 0.2) * TotalShots;
  //LCD.printNumI(TotTim, 200,260,4);


  // Find start and check we are there
  for (int n1 = 0;  n1 < 9;  n1++) {
    if (MoveType[n1] == 1)  {                                          // Drive To Start if not already there - movetype 1 is start position in array
      StartPos = n1;
      newRotary = RotaryEnc.read();                                    // read encoders to find out where we are
      newLinear = LinearEnc.read();

      if ((LinMove[n1] != newLinear) || (RotMove[n1] != newRotary)){ // check Lin and Rot positions
        //LCD.printNumI(LinMove[n1]-newLinear, 600,200,4);
        //LCD.printNumI(RotMove[n1]-newRotary, 600,220,4);
        analogWrite(11,0);                                          // Lin using PWM on Sleep line to control motor current - 255 full power to move
        analogWrite(12, 255);                                          // Rot using PWM on Sleep line to control motor current - 255 full power to move
        //stepper1.setCurrentPosition(0);
        long LinStart = LinMove[n1] - newLinear;                       // encoder units
        LinStart = LinStart * StepsPerLinCount;                        // convert to stepper units
        //stepper1.moveTo(LinStart);                                     // Move to start Lin location
        //stepper2.setCurrentPosition(0);
        long RotStart = newRotary - RotMove[n1];                       // encoder units
        //LCD.printNumI(LinStart, 600,200,4);
        //LCD.printNumI(RotStart, 600,220,4);
        RotStart = (RotStart * StepsPerRotCount );                     // convert to stepper units
        //stepper2.moveTo(RotStart);                                     // Move to start Rot location
        //while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {      // run both motors at the same time
          // run steppers
          //stepper1.run();
          //stepper2.run();
        //}
        //  MultiStepper manage both steppers
        MultiStepper steppers; 
        steppers.addStepper(stepper1);
        steppers.addStepper(stepper2);
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        positions[0] = LinStart;
        positions[1] = RotStart;
        steppers.moveTo(positions);
        steppers.runSpeedToPosition(); // Blocks until all are in position
  
        analogWrite(11,0);                                          // use PWM to "hold" motor - needs adjusting dependant on angle of slider. Needs increasing when at a steep angle !!
        analogWrite(12, 140);                                          // use PWM to "hold" motor
        delay(500);
        newRotary = RotaryEnc.read();
        newLinear = LinearEnc.read();
        n1 = 9;
      }
      drawSlider(newRotary, newLinear);
    }
  }

  long LinMovement;  // this will be in counts
  long RotMovement;  // this will be in counts

  for (int n2 = StartPos;  n2 < 9;  n2++)  {
    if (StartSeq) {
      if (StopSeq) {                                                   // if stop is touched set n2 to 9 and reset stop and pause if set break out of loop - sequence ends
        n2 = 9;
        StopSeq = false;
        PauseSeq = false;
        //Timer3.detachInterrupt( );                                   // detach the interupt service routine here
        break;
      }
    } 
      while (PauseSeq) {                                               // if pause is touched then loop until it changes - continue
        if (StopSeq) {                                                 // if stop is touched while paused set n2 to 9 and break out of loop - sequence ends
          n2 = 9;
          StopSeq = false;
          //Timer3.detachInterrupt( );                                 // detach the service routine here
          break;
        }
      }
      switch (MoveType[n2]) {
        case 1:  //Start
          LinMovement = (LinMove[n2 + 1]) - (LinMove[n2]) ;
          RotMovement = (RotMove[n2 + 1]) - (RotMove[n2]) ;
          //LCD.printNumI(LinMovement, 600,200,4);
          //LCD.printNumI(RotMovement, 600,220,4);
          if (StillTimeLapse) {
            Movement(LinMovement, RotMovement);
          }
          else {
            Video(LinMovement, RotMovement);
          }
          break;
  
        case 2: //Way Point
          LinMovement = (LinMove[n2 + 1]) - (LinMove[n2]) ;
          RotMovement = (RotMove[n2 + 1]) - (RotMove[n2]) ;
          //LCD.printNumI(LinMovement, 600,200,4);
          //LCD.printNumI(RotMovement, 600,220,4);
          if (StillTimeLapse) {
            Movement(LinMovement, RotMovement);
          }
          else {
            Video(LinMovement, RotMovement);
          }
  
          break;
        case 3: // Fixed Target
  
          break;
  
        case 4: // End Point
          LinMovement = (LinMove[n2 + 1]) - (LinMove[n2]) ;
          RotMovement = (RotMove[n2 + 1]) - (RotMove[n2]) ;
          //LCD.printNumI(LinMovement, 600,200,4);
          //LCD.printNumI(RotMovement, 600,220,4);
          if (StillTimeLapse) {
            Movement(LinMovement, RotMovement);
          }
          else {
            Video(LinMovement, RotMovement);
          }
  
          break;
      }
    
    }  // for sequence loop
  
  StartSeq = 0;
  Timer3.detachInterrupt( );                                         // detach the interupt service routine here

}  // sequencPlayBack


// Movement is the amount of counts between any two array elements then scales counts to steps and returns to sequencePlayBack
// This is very similar to Movement (long LinMovement, long RotMovement) routine but without the intervals i.e. smooth running and start pulse and end pulse only

void Video(long LinMovement, long RotMovement) {
  LinMovement = (LinMovement * StepsPerLinCount);                  // up until now arrays, LinMovement and RotMovement are in encoder counts now have to change to steps for the actual...
  RotMovement = (RotMovement * StepsPerRotCount);
  if (StopSeq) {                                               // if stop is touched set n2 to 9 and reset stop and pause if set break out of loop - sequence ends
    StopSeq = false;
    PauseSeq = false;
    Timer3.detachInterrupt( );                                 // detach the service routine here
    //break;
  }
  while (PauseSeq) {                                           // if pause is touched then loop until it changes - continue
    if (StopSeq) {                                             // if stop is touched while paused set n2 to 9 and break out of loop - sequence ends
      StopSeq = false;
      Timer3.detachInterrupt();                                // detach the service routine here
      break;
    }
  }
  analogWrite(12, 255);                                       // Rot using PWM on Sleep line to control motor current
  analogWrite(11,0);                                       // Lin using PWM on Sleep line to control motor current

  //  MultiStepper manage both steppers
  MultiStepper steppers; 
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  positions[0] = LinMovement;
  positions[1] = -RotMovement;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  analogWrite(11,0);                                       // use PWM to "hold" motor
  analogWrite(12, 128);
} // end of video movement routine


// Movement is the amount of counts between any two array elements then scales counts to steps and returns to sequencePlayBack
void Movement(long LinMovement, long RotMovement) {
  
  // The one with most counts has to be the controling element with the other lower count interleaved as necessary
  // i.e. if linear has 6000 steps to make and roatary 300 then linear will be the "master" and the roatry steps made at
  // intervals 6000/300 = 20,  one roatary step will be injected for every 20 linear

  int modifier = totalsteps % stepspershot; // the total number of steps divided by the number of shots is not always a whole number
  modifier = 1 / modifier ;                 // so if we work out what the "odd" part is in terms of the number of steps made to lose 1 step
  // we can inject the "missing" pulse - if need be - not implimented yet

  if (abs(LinMovement) > abs(RotMovement)) {

    //LCD.printNumI(LinMovement, 400,200,4);
    //LCD.printNumI(RotMovement, 400,220,4);
    LinMovement = (LinMovement * StepsPerLinCount);                  // up until now arrays, LinMovement and RotMovement are in encoder counts now have to change to steps for the actual...

    RotMovement = (RotMovement * StepsPerRotCount);
    //LCD.printNumI(LinMovement, 400,240,4);
    //LCD.printNumI(RotMovement, 400,260,4);
    //LCD.printNumI(RotMovement / LinMovement, 400,280,4);

    // ratio is ratio between Lin and Rot steps
    if ((LinMovement != 0) && (RotMovement != 0)) {
      ratio = (float(RotMovement) / float(LinMovement));
      ratio = abs(ratio);                                              //  ratio is just a magnitude - should be unsigned
    }
    else  {
      ratio = 0;
    }
    LCD.print("R     ", 20, 290);
    LCD.printNumF(ratio, 4, 50, 290);
    //LCD.printNumF(stepspershot*ratio,4, 550,280);

    for (int L = 0; L < abs(LinMovement); L = L + stepspershot) {    // Drive Motors for this segment in step per shot increments
      TakePicture();
      if (taken > TotalShots) {
        break;
      }
      if (StopSeq) {                             // if stop is touched set n2 to 9 and reset stop and pause if set break out of loop - sequence ends
        L = abs(LinMovement);
        StopSeq = false;
        PauseSeq = false;
        Timer3.detachInterrupt( );            // detach the service routine here
        break;
      }
      while (PauseSeq) {                        // if pause is touched then loop until it changes - continue
        if (StopSeq) {                            // if stop is touched while paused set n2 to 9 and break out of loop - sequence ends
          L = abs(LinMovement);
          StopSeq = false;
          Timer3.detachInterrupt();             // detach the service routine here
          break;
        }
      }
      analogWrite(12, 255);                      // Rot using PWM on Sleep line to control motor current - full to move
      analogWrite(11,0);                      // Lin using PWM on Sleep line to control motor current - full to move

      stepper1.setCurrentPosition(0);
      if (LinMovement < 0) {
        stepper1.moveTo(-stepspershot);
      }
      else {
        stepper1.moveTo(stepspershot);
      }
      stepper2.setCurrentPosition(0);

      if (ratio != 0) {                          // if ratio is not zero then roatary has movment
        if (RotMovement < 0) {
          stepper2.moveTo (stepspershot * ratio );
        }
        else {
          stepper2.moveTo (-stepspershot * ratio );
        }

        while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0)) {
          //LCD.printNumI(stepper1.distanceToGo(), 600,240,4);
          //LCD.printNumI(stepper2.distanceToGo(), 600,260,4);
          // run steppers
          stepper1.run();
          stepper2.run();
        }
      }
      else {                                     // ratio must be zero - do not run motor2, roatary
        while (stepper1.distanceToGo() != 0) {
          // run stepper
          stepper1.run();
        }
      }
      analogWrite(11,0);                      // use PWM to "hold" motor - half to hold
      analogWrite(12, 128);

      //newRotary = RotaryEnc.read();
      //newLinear = LinearEnc.read();
      //delay(250);
      //LCD.printNumI(newLinear, 500,240,4);
      //LCD.printNumI(newRotary, 500,260,4);
      //LCD.print("Lin Master", 320,200);
      //LCD.printNumI(L, 340,280,4);
    }  // end of for loop


  } // end of (abs(LinMovement) > abs(RotMovement)

  else if (abs(RotMovement) > abs(LinMovement)) {
    //LCD.printNumI(LinMovement, 400,200,4);
    // LCD.printNumI(RotMovement, 400,220,4);
    LinMovement = (LinMovement * StepsPerLinCount);                  // up until now arrays, LinMovement and RotMovement are in encoder counts now have to change to steps for the actual...

    RotMovement = (RotMovement * StepsPerRotCount);
    //LCD.printNumI(LinMovement, 400,240,4);
    //LCD.printNumI(RotMovement, 400,260,4);
    //LCD.printNumI(RotMovement / LinMovement, 400,280,4);

    // ratio is ratio between Lin and Rot steps
    if ((LinMovement != 0) && (RotMovement != 0)) {
      ratio = (float(LinMovement) / float(RotMovement));
      ratio = abs(ratio);                                              //  ratio is just a magnitude - should be unsigned
    }
    else  {
      ratio = 0;
    }

    LCD.print("r     ", 20, 290);
    LCD.printNumF(ratio, 4, 50, 290);
    //LCD.printNumF(stepspershot*ratio,4, 550,280);

    for (int L = 0; L < abs(RotMovement); L = L + stepspershot) {    // Drive Motors for this segment in step per shot increments
      TakePicture();
      if (taken > TotalShots) {
        break;
      }
      if (StopSeq) {                             // if stop is touched set n2 to 9 and reset stop and pause if set break out of loop - sequence ends
        L = abs(RotMovement);
        StopSeq = false;
        PauseSeq = false;
        Timer3.detachInterrupt( );            // detach the service routine here
        break;
      }
      while (PauseSeq) {                        // if pause is touched then loop until it changes - continue
        if (StopSeq) {                            // if stop is touched while paused set n2 to 9 and break out of loop - sequence ends
          L = abs(RotMovement);
          StopSeq = false;
          Timer3.detachInterrupt();             // detach the service routine here
          break;
        }
      }
      analogWrite(12, 255);                      // Rot using PWM on Sleep line to control motor current
      analogWrite(11,0);                      // Lin using PWM on Sleep line to control motor current - full to hold

      stepper2.setCurrentPosition(0);
      if (RotMovement < 0) {
        stepper2.moveTo(stepspershot);
      }
      else {
        stepper2.moveTo(-stepspershot);
      }
      stepper1.setCurrentPosition(0);

      if (ratio != 0) {                          // if ratio is not zero then roatary has movment
        if (LinMovement < 0) {
          stepper1.moveTo (stepspershot * ratio );
        }
        else {
          stepper1.moveTo (-stepspershot * ratio );
        }

        while ((stepper2.distanceToGo() != 0) || (stepper1.distanceToGo() != 0)) {
          //LCD.printNumI(stepper1.distanceToGo(), 600,240,4);
          //LCD.printNumI(stepper2.distanceToGo(), 600,260,4);
          // run steppers
          stepper1.run();
          stepper2.run();
        }
      }
      else {                                     // ratio must be zero - do not run motor1, linear
        while (stepper2.distanceToGo() != 0) {
          // run stepper
          stepper2.run();
        }
      }
      analogWrite(11,0);                      // use PWM to "hold" motor - half power to hold
      analogWrite(12, 128);

      //newRotary = RotaryEnc.read();
      //newLinear = LinearEnc.read();
      //delay(250);
      //LCD.printNumI(newLinear, 500,240,4);
      //LCD.printNumI(newRotary, 500,260,4);
      //LCD.print("Rot Master", 320,200);
      //LCD.printNumI(L, 340,280,4);
    }  // end of for loop
  } // end of (abs(RotMovement) > abs(LinMovement))
  analogWrite(11,0);                                                 // use PWM to "hold" motor
  analogWrite(12, 96);

} // end of Movement routine

