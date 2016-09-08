#include <Arduino.h>

/*
 * routines called by interupts
 */



// this called when timer 3 is active and time period has elapsed. The timer interupt is started when a sequence is intiated and detach when sequence is complete or stop is touched.
// this is on a timer as the motor control takes a lot of time and the LCD would not otherwise be read. It is mainly for servicing the buttons Stop and Pause

void PauseOrStopIsr()  
{

  if ((Touch.dataAvailable()) && (pg = 1)) {                            // only do something if screen has been touched and we are on page 1. If screen info waiting then process it in the normal way .....
    ScreenTouch();                                                      // .... in screentouch
  }
}

// Green Button
void Green()  {
  if (pg = 1) {  // Start Button
    x=101;
    y=146;
    InPageSelection();
  }
} // End of Green

void Yellow()  {
  if (pg = 1) { // Pause Button
    x=321;
    y=146;
    InPageSelection();
  }
} // End of Yellow

void Red()  {
  if (pg = 1) {  // Stop Button
    x=541;
    y=146;
    InPageSelection();
  }
} // End of Red

void Blue()  {
  if (pg = 1) {  // Still / Video mode toggle
    x=146;
    y=96;
    InPageSelection();
  }
} // End of Blue





// prepared in setup attachPinChangeInterrupt(A11, NearLimit, CHANGE);
void NearLimit()  {   // Toggle NearBlock - called by pin-change interupt - used in movment routine (motor drive)
  NearBlock = !NearBlock;
  //LCD.print("Near Limit",170,270);
  delay(50);
  LCD.printNumI(NearBlock,170,270,3);
}


// prepared in setup attachPinChangeInterrupt(A12, FarLimit, CHANGE);
void FarLimit()  {  // Toggle Far Block - called by pin-change interupt - used in movment routine (motor drive)
  FarBlock = !FarBlock;
  //LCD.print("Far Limit",170,250);
  delay(50);
  //LCD.printNumI(FarBlock,170,250,3);
}

