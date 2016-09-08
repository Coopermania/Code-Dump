#include <Arduino.h>

/*
 * Toggle / set the LCD dynamic items
 */


void drawStill_Video() {
  // draw Still / Video toggle button and associated text
  LCD.setBackColor(VGA_TEAL);
  LCD.setColor(VGA_YELLOW);
  CTE_LCD.Put_Text("Active Mode ", 15, 105, BVS_22);
  LCD.setColor(VGA_BLUE);
  LCD.fillRoundRect(145, 95, 235, 135);
  LCD.setColor(VGA_YELLOW);
  LCD.drawRoundRect(145, 95, 235, 135);
  
  if (StillTimeLapse) {
      LCD.setColor(VGA_YELLOW);
      LCD.setBackColor(VGA_BLUE);
      CTE_LCD.Put_Text("S t i l l", 158, 102, BVS_28);
      LCD.setBackColor(VGA_TEAL);
      CTE_LCD.Put_Text("Playback Sequence in Intervals Matching Total Shots    ", 242, 105, BVS_22);
    }
    else  {
      LCD.setColor(VGA_YELLOW);
      LCD.setBackColor(VGA_BLUE);
      CTE_LCD.Put_Text("Video", 158, 102, BVS_28);
      LCD.setBackColor(VGA_TEAL);
      CTE_LCD.Put_Text("Continuous Movement Throughout Playback Sequence", 242, 105, BVS_22);
    }
    while (Touch.dataAvailable()) {
    }
}
void drawStart() {                                                      // draw start button on home secreen
  // draw Start Button
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(40, 145, 260, 185);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_LIME);
  LCD.setFont(BigFont);
  if ( StartSeq) {
    CTE_LCD.Put_Text("R  U  N  N  I  N  G", 65, 155, BVS_28);
  }
  else  {
    CTE_LCD.Put_Text("S  T  A  R  T  ", 80, 155, BVS_28);
  }
  while (Touch.dataAvailable()) {
  }

} // end of routine


void drawPause() {

  // draw Pause Button
  LCD.setColor(VGA_YELLOW);
  LCD.fillRoundRect(300, 145, 510, 185);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_YELLOW);
  LCD.setFont(BigFont);
  if ( PauseSeq) {
    CTE_LCD.Put_Text("P A U S E D", 340, 155, BVS_28);
  }
  else  {
    CTE_LCD.Put_Text("P  A  U  S  E  ", 345, 155, BVS_28);
  }
  while (Touch.dataAvailable()) {
  }
}

void drawStop()  {
  // draw Stop Button
  LCD.setColor(VGA_RED);
  LCD.fillRoundRect(540, 145, 760, 185);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_RED);
  LCD.setFont(BigFont);
  if ( StopSeq) {
    CTE_LCD.Put_Text("S T O P P E D", 580, 155, BVS_28);
  }
  else  {
    CTE_LCD.Put_Text("S  T  O  P   ", 600, 155, BVS_28);
  }
  while (Touch.dataAvailable()) {
  }
}

// draw comit sequence boxes and get user to cancel or comit sequence
void drawComitSeq() {
  // draw Comit Button
  LCD.setColor(100, 254, 80);                 // Green
  LCD.fillRoundRect(490, 100, 760, 150);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(100, 254, 80);
  CTE_LCD.Set_character_spacing(5);
  CTE_LCD.Put_Text("Comit Sequence", 510, 115, BVS_28);
  
    // draw Cancel Button
  LCD.setColor(VGA_RED);
  LCD.fillRoundRect(550, 160, 680, 185);
  LCD.setColor(VGA_WHITE);
  LCD.setBackColor(VGA_RED);
  CTE_LCD.Put_Text("CANCEL", 575, 160, BVS_22);
  
  
} // drawComitSeq


void LinSpeedUpdate() {
  LinSpeed = constrain (LinSpeed, 0, 255);
  LRSpeed = map(LinSpeed, 0, 255, 1, 3500);                           //was 3500
  flValueOfAnyBox = 190.0 / 255.0 * LinSpeed;
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(25, 255, 215, 280);                               // remove any green by drawing in white first
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(25, 255, 25 + flValueOfAnyBox, 280);              // complete box is 190 long, starts at 25 - LinSpeed 255 max
  LCD.setColor(VGA_PURPLE);
  LCD.drawRoundRect(25, 255, 215, 280);                               // Put a border around as white doesn't show on the yellow very well when empty
  LCD.setBackColor(238, 190, 250);                                    // Pale purple
  //LCD.printNumI(LinSpeed, 80, 210, 4);
  CTE_LCD.Put_Text(String(LinSpeed) + "     ", 105, 205,BVS_28);
}

void RotSpeedUpdate() {
  // Adjust Rotary Speed and update with current value
  RotSpeed = constrain (RotSpeed, 0, 255);
  RSpeed = map(RotSpeed, 0, 255, 1, 2200);                            // was 2200
  flValueOfAnyBox = 190.0 / 255.0 * RotSpeed;                         // length of box divided by the max value * the current value - must use xx.0 with float !
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(250, 255, 440, 280);                              // draw white background
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(250, 255, 250 + flValueOfAnyBox, 280);            // complete box is 190 long, starts at 250 - RotSpeed 255 max
  LCD.setColor(VGA_PURPLE);
  LCD.drawRoundRect(250, 255, 440, 280);                              // Put a border around as white doesn't show on the yellow very well when empty
  LCD.setBackColor(238, 190, 250);
  //LCD.printNumI(RotSpeed, 305, 210, 4);
  CTE_LCD.Put_Text(String(RotSpeed) + "    ", 340, 200,BVS_28);
}

void FocusTimeUpdate() {
  // Adjust Focus and update with current value
  flValueOfAnyBox = 230.0 / 5 * FocusTime;                            // length of box divided by the max value * the current value - must use xx.0 with float !
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(25, 135, 255, 160);                               // draw white background
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(25, 135, 25 + flValueOfAnyBox, 160);              // complete box is 230 long, starts at 25 - Focus 5  max
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(25, 135, 255, 160);                               // Put a border around as white doesn't show very well when empty
  LCD.setBackColor(150, 255, 150);                                    // Pale Green
  if (Interval < FocusTime+ShotTime) {
    Interval = FocusTime+ShotTime;
    IntervalUpdate();
  }
  char buffer[10];                                                    // create buffer for converted string
  String temp = dtostrf(FocusTime,5,1,buffer);                        // make temporary string for result 
  CTE_LCD.Put_Text(temp + " ", 110, 95, BVS_22);                      // print string with space following - doesn't like combining " " space and string conversion in Put_Text instruction
}

/*
 * LCD Items which are updated on touch
 */



void ShotTimeUpdate() {
  // Adjust Shot Time and update with current value
  flValueOfAnyBox = 230.0 / 2 * ShotTime;                             // length of box divided by the max value * the current value - must use xx.0 with float !
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(285, 135, 515, 160);                              // draw white background
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(285, 135, 285 + flValueOfAnyBox, 160);              // complete box is 230 long, starts at 285 - Shot time 2 sec  max
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(285, 135, 515, 160);                               // Put a border around as white doesn't show very well when empty
  LCD.setBackColor(150, 255, 150);                                    // Pale Green
  if (Interval < FocusTime+ShotTime) {
    Interval = FocusTime+ShotTime;
    IntervalUpdate();
  }
  char buffer[10];                                                    // create buffer for converted string
  String temp = dtostrf(ShotTime,5,1,buffer);                         // make temporary string for result 
  CTE_LCD.Put_Text(temp + " ", 380, 95, BVS_22);                      // print string with space following - doesn't like combining " " space and string conversion in Put_Text instruction
}

void IntervalUpdate() {
  // Adjust Interval and update with current value
  flValueOfAnyBox = 230.0 / 100.0 * Interval;                          // length of box divided by the max value * the current value - must use xx.0 with float !
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(545, 135, 775, 160);                               // draw white background
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(545, 135, 545 + flValueOfAnyBox, 160);             // complete box is 230 long, starts at 285 - Interval 100  max
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(545, 135, 775, 160);                               // Put a border around as white doesn't show very well when empty
  LCD.setBackColor(150, 255, 150);                                     // Pale Green
  char buffer[10];                                                     // create buffer for converted string
  String temp = dtostrf(Interval,5,1,buffer);                          // make temporary string for result 
  CTE_LCD.Put_Text(temp + " ", 635, 95, BVS_22);                       // print string with space following - doesn't like combining " " space and string conversion in Put_Text instruction
  
}

void TotalShotsUpdate() {
  // Adjust Total Shots indicator and update with current value
  flValueOfAnyBox = 230.0 / 2000.0 * TotalShots;                       // length of box divided by the max value * the current value - must use xx.0 with float !
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(25, 280, 255, 305);                                // draw white background
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(25, 280, 25 + flValueOfAnyBox, 305);               // complete box is 230 long, starts at 545 - TotalShots 2000 max
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(25, 280, 255, 305);                                // Put a border around as white doesn't show on the yellow very well when empty
  LCD.setBackColor(150, 255, 150);                                     // Pale Green
  CTE_LCD.Put_Text(String(TotalShots) + " ", 100, 245, BVS_22);
}
