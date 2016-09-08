#include <Arduino.h>

/*
 * Draw all perminent LCD screen items for all 5 screens
 * Items which change - buttons, gauges, and graphics are handled elsewhere
 */




// Draw Utility Screen *******************************************************************************************************************************
void drawUtilityScreen() {
  // page border and set to page colour
  LCD.fillScr(VGA_WHITE);
  LCD.setColor(243, 254, 180);                                          // Pale yellow
  LCD.fillRoundRect( 6, 52, 795, 350);
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 52, 795, 350);

// Utility Tab in same color
  LCD.setColor(VGA_GREEN);                                              // Pale purple
  LCD.fillRoundRect(515, 3, 630, 56);
  LCD.setColor(VGA_BLUE);
  LCD.setBackColor(238, 190, 250);                                      // Pale purple
  CTE_LCD.Put_Text("Utility", 160, 18, BVS_28);
  
  
  
  CTE_LCD.Set_character_spacing(1);                                     // Tighten spacing for tab text
  // Sequencer Tab in same color
  LCD.setColor(243, 254, 180);                                          // Pale yellow
  LCD.fillRoundRect(255, 3, 380, 56);
  LCD.setColor(VGA_RED);
  LCD.setBackColor(243, 254, 180);                                      // Pale yellow
  CTE_LCD.Put_Text("Record", 265, 18, BVS_28);

  //Play Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(6, 3, 140, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Play", 15, 18, BVS_28);

  // Manual Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(145, 3, 250, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Manual", 155, 18, BVS_28);
  
  // Camera Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(385, 3, 510, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Camera", 395, 18, BVS_28);
  CTE_LCD.Set_character_spacing(3);                                       // Open spacing for "in page" text
    
  //draw black border around each tab
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 3, 140, 50);
  LCD.drawRoundRect(145, 3, 250, 50);
  //LCD.drawRoundRect(255, 3, 380, 50);
  LCD.drawRoundRect( 385, 3, 510, 50);



}




// Draw all static Sequencer Page Items *******************************************************************************************************************************
void drawSequenceScreen() {
  // page border and set to page colour
  LCD.fillScr(VGA_WHITE);
  LCD.setColor(243, 254, 180);                                          // Pale yellow
  LCD.fillRoundRect( 6, 52, 795, 350);
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 52, 795, 350);

  CTE_LCD.Set_character_spacing(1);                                      // Tighten spacing for tab text
  // Sequencer Tab in same color
  LCD.setColor(243, 254, 180);                                          // Pale yellow
  LCD.fillRoundRect(255, 3, 380, 56);
  LCD.setColor(VGA_RED);
  LCD.setBackColor(243, 254, 180);                                      // Pale yellow
  CTE_LCD.Put_Text("Record", 265, 18, BVS_28);

  //Play Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(6, 3, 140, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Play", 15, 18, BVS_28);

  // Manual Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(145, 3, 250, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Manual", 155, 18, BVS_28);
  
  // Camera Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(385, 3, 510, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Camera", 395, 18, BVS_28);
  CTE_LCD.Set_character_spacing(3);                                       // Open spacing for "in page" text

 
  //draw black border around each tab
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 3, 140, 50);
  LCD.drawRoundRect(145, 3, 250, 50);
  //LCD.drawRoundRect(255, 3, 380, 50);
  LCD.drawRoundRect( 385, 3, 510, 50);


  // draw position information box - values in drawslider routine
  LCD.setColor(VGA_SILVER);
  LCD.fillRoundRect(10, 320, 790, 349);                                 // position information text is written in drawslider routine

  // Record Sequence group box  ******************************************************************************
  LCD.setColor(VGA_PURPLE);
  LCD.drawRoundRect(15, 60, 450, 190);
  LCD.setBackColor(243, 254, 180);                                      // Pale yellow
  CTE_LCD.Put_Text("Record Camera Sequence", 50, 65, BVS_28);

  // Sequence Available actions box
  LCD.setColor(VGA_PURPLE);
  LCD.drawRoundRect(460, 60, 790, 190);
  LCD.setBackColor(243, 254, 180);                                      // Pale yellow
  CTE_LCD.Put_Text("Available Actions :-  ", 470, 70,BVS_22);
  CTE_LCD.Put_Text("Set End Point", 470, 95, BVS_22);
  LCD.drawRoundRect(15, 200, 790, 310);                                 // Sequence diagram box


  // Draw Arrow to Direction "End Point" then either "Way point" or "Fixed Target" fianlly "Start Point"
  for (int n = 93; n < 98; n++) {
    LCD.drawLine(20, n, 425, n);
  }
  geo.fillTriangle(425, 107, 425, 83, 440, 95);

  // draw End Point Button
  LCD.setColor(VGA_GREEN);
  LCD.fillRoundRect(25, 110, 120, 180);
  LCD.setColor(VGA_YELLOW);
  LCD.setBackColor(VGA_GREEN);                                      // Dark Orange
  CTE_LCD.Put_Text(" End", 52, 120, BVS_22);
  CTE_LCD.Put_Text("Point", 50, 150, BVS_22);

  // draw Way Point Button
  LCD.setColor(VGA_YELLOW);
  LCD.fillRoundRect(130, 110, 225, 180);
  LCD.setColor(VGA_BLUE);
  LCD.setBackColor(VGA_YELLOW);                                      // Pale Green
  CTE_LCD.Put_Text("Way", 155, 120, BVS_22);
  CTE_LCD.Put_Text("Point", 152, 150, BVS_22);

  // draw Fixed Target Button
  LCD.setColor(VGA_BLUE);
  LCD.fillRoundRect(235, 110, 342, 180);
  LCD.setColor(VGA_YELLOW);
  LCD.setBackColor(VGA_BLUE);
  CTE_LCD.Put_Text("Fixed", 262, 120, BVS_22);
  CTE_LCD.Put_Text("Target", 255, 150, BVS_22);

  // draw Start Point Button
  LCD.setColor(VGA_RED);
  LCD.fillRoundRect(355, 110, 445, 180);
  LCD.setColor(VGA_YELLOW);
  LCD.setBackColor(VGA_RED);
  CTE_LCD.Put_Text("Start", 375, 120, BVS_22);
  CTE_LCD.Put_Text("Point", 377, 150, BVS_22);
  drawSlider(newRotary, newLinear);

}

// Draw all static Manual Page Items ****************************************************************************************************************
void drawManualScreen() {
  
  // page border and set to page colour ***********
  LCD.fillScr(VGA_WHITE);
  LCD.setColor(238, 190, 250);                                          // Pale purple
  LCD.fillRoundRect( 6, 52, 795, 350);
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 52, 795, 350);
  CTE_LCD.Set_character_spacing(1);                                      // Tighten spacing for tab text

  
  // Manual Tab in same color
  LCD.setColor(238, 190, 250);                                          // Pale purple
  LCD.fillRoundRect(145, 3, 250, 56);
  LCD.setColor(VGA_BLUE);
  LCD.setBackColor(238, 190, 250);                                      // Pale purple
  CTE_LCD.Put_Text("Manual", 160, 18, BVS_28);


  //Play Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(6, 3, 140, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Play", 15, 18, BVS_28);

  
  // Sequencer Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(255, 3, 380, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Record", 265, 18, BVS_28);

  // Camera Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(385, 3, 510, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Camera", 395, 18, BVS_28);
  
  
  //draw black border around each tab
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 3, 140, 50);
  LCD.drawRoundRect(255, 3, 380, 50);
  LCD.drawRoundRect( 385, 3, 510, 50);

  // draw position information box - values in drawslider routine
  LCD.setColor(VGA_SILVER);
  LCD.fillRoundRect(10, 320, 790, 349);                                 // position information text is written in drawslider routin  e

  CTE_LCD.Set_character_spacing(2);                                       // Open spacing for "in page" text
  // Manual Movement Linear and Rotary group box
  LCD.setColor(VGA_PURPLE);
  LCD.drawRoundRect(15, 60, 450, 300);
  LCD.setBackColor(238, 190, 250);                                        // Pale purple
  CTE_LCD.Put_Text("Linear Movement ", 55, 65, BVS_22);
  CTE_LCD.Put_Text("          & Speed", 55, 85, BVS_22);
  CTE_LCD.Put_Text("Rotary Movement ", 275, 65, BVS_22);
  CTE_LCD.Put_Text("          & Speed", 275, 85, BVS_22);

  // Move Left, Arrow
  LCD.drawRoundRect(25, 115, 105, 185);                                   // Left arrow touch area indicator box
  LCD.fillRoundRect(50, 140, 100, 160);
  geo.fillTriangle(30, 150, 52, 120, 52, 180);

  // Move Right, Arrow
  LCD.drawRoundRect(135, 115, 215, 185);                                  // Right arrow touch area indicator box
  LCD.fillRoundRect(140, 140, 190, 160);
  geo.fillTriangle(210, 150, 188, 120, 188, 180);
  
  // Rotate Left, Arrow - counter-clockwise
  LCD.drawRoundRect(250, 115, 335, 185);                                  // Rotate Right arrow touch area indicator box
  geo.drawArc(275, 190, 45, 0, 75, 19);
  geo.fillTriangle(255, 145, 275, 120, 275, 170);

    // Rotate Right, Arrow - clockwise
  LCD.drawRoundRect(355, 115, 440, 185);                                  // Rotate Right arrow touch area indicator box
  geo.drawArc(415, 190, 45, 285, 360, 19);                                // x,y,r, start angle, end angle, thickness
  geo.fillTriangle(435, 145, 415, 120, 415, 170);

  CTE_LCD.Set_character_spacing(1);                                       // Tighten Text a bit as there is a lot to fit in
   // Linear adjust slow / fast buttons
  LCD.setColor(VGA_PURPLE);
  LCD.setBackColor(238, 190, 250);                                        // Pale purple
  LCD.drawRoundRect(25, 200, 65, 235);                                    // slow button touch area indicator box
  CTE_LCD.Put_Text("Dec", 31, 205, BVS_22);
  LCD.drawRoundRect(175, 200, 215, 235);                                  // slow button touch area indicator box
  CTE_LCD.Put_Text("Inc", 183, 205, BVS_22);

  // Rotary adjust Decrement / Increment buttons
  LCD.setColor(VGA_PURPLE);
  LCD.setBackColor(238, 190, 250);                                        // Pale purple
  LCD.drawRoundRect(250, 200, 290, 235);                                  // Decrement button touch area indicator box
  CTE_LCD.Put_Text("Dec", 255, 205, BVS_22);
  LCD.drawRoundRect(400, 200, 440, 235);                                  // Increment button touch area indicator box
  CTE_LCD.Put_Text("Inc", 408, 205,BVS_22);

  LinSpeedUpdate();                                                       // set green area in Linear indicator
  RotSpeedUpdate();                                                       // set green area in Rotary indicator

  // *********************************************************
  // Focus Shoot Multi Shot box
  LCD.setColor(VGA_PURPLE);
  LCD.drawRoundRect(460, 60, 790, 300);
  LCD.setBackColor(238, 190, 250); // Pale purple
  CTE_LCD.Put_Text("Focus;  Held - touch again to Re-Focus", 470, 65, BVS_22);
  CTE_LCD.Put_Text("Shoot;  One Pulse Sent", 470, 90, BVS_22);
  CTE_LCD.Put_Text("Multi Shot;  Pulsing, Whilst Touching", 470, 115,BVS_22);

  CTE_LCD.Set_character_spacing(3);                                       // open text up a bit
  // draw Focus Button
  LCD.setColor(VGA_LIME);
  LCD.fillRoundRect(470, 140, 600, 220);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_LIME);
  CTE_LCD.Put_Text("Focus", 500, 165, BVS_34);

  // draw Shoot Button
  LCD.setColor(VGA_YELLOW);
  LCD.fillRoundRect(640, 140, 780, 220);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_YELLOW);
  CTE_LCD.Put_Text("Shoot", 670, 165, BVS_34);

  CTE_LCD.Set_character_spacing(5);                                       // open text up a bit more
  // draw Multi - shot Button
  LCD.setColor(VGA_RED);
  LCD.fillRoundRect(470, 240, 780, 290);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_RED);
  CTE_LCD.Put_Text("Multi - Shot", 540, 250, BVS_34);
  CTE_LCD.Set_character_spacing(1);                                       // Tighten text back to normal
  drawSlider(newRotary, newLinear);
}  // end of Draw Manual Screen

// Draw all static Home Page Items ********************************************************************************************************************
void PlayScreen() {
  // page border and set to page colour *****************
  LCD.fillScr(VGA_WHITE);
  LCD.setColor(VGA_TEAL);
  LCD.fillRoundRect( 6, 52, 795, 350);
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 52, 795, 350);
  LCD.setColor(243, 254, 180);                                           // Pale yellow  180
  LCD.fillRoundRect(10, 200, 790, 310);                                  // Remove previously drawn sequence diagram
  CTE_LCD.Set_character_spacing(1);                                      // Tighten spacing for tab text
  
  //Play Tab in same color
  LCD.setColor(VGA_TEAL);
  LCD.fillRoundRect(6, 3, 140, 56);
  LCD.setColor(VGA_YELLOW);
  LCD.setBackColor(VGA_TEAL);
  CTE_LCD.Put_Text("Play", 15, 18, BVS_28);
                   
  // Manual Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(145, 3, 250, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Manual", 155, 18, BVS_28);

  // Sequencer Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(255, 3, 380, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Record", 265, 18, BVS_28);

  // Camera Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(385, 3, 510, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Camera", 395, 18, BVS_28);
  CTE_LCD.Set_character_spacing(3);                                       // Open spacing for "in page" text
  
  //draw black border around each tab
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(145, 3, 250, 50);
  LCD.drawRoundRect(255, 3, 380, 50);
  LCD.drawRoundRect( 385, 3, 510, 50);

  // draw position information box - values in drawslider routine
  LCD.setColor(VGA_SILVER);
  LCD.fillRoundRect(10, 320, 790, 349);                                 // position information text is written in drawslider routine

  // Draw status boxes ***************************************************
  // Shot of Shots & remaining time
  LCD.setColor(VGA_SILVER);
  LCD.fillRoundRect(10, 60, 780, 90);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_SILVER);
  CTE_LCD.Put_Text("Shot  0  of  0", 26, 65, BVS_22);
  CTE_LCD.Put_Text("Remaining Time =  0    ", 350, 65, BVS_22);

  
  // draw Still / Video toggle button and associated text
  LCD.setBackColor(VGA_TEAL);
  LCD.setColor(VGA_YELLOW);
  CTE_LCD.Put_Text("Active Mode ", 15, 105, BVS_22);
  LCD.setColor(VGA_BLUE);
  LCD.fillRoundRect(145, 95, 235, 135);
  LCD.setColor(VGA_YELLOW);
  LCD.drawRoundRect(145, 95, 235, 135);
  
  drawStill_Video();
  drawStart();
  drawPause();
  drawStop();

  drawSequence();
  drawSlider(newRotary, newLinear);
}

// Draw all static Camera Page Items *****************************************************
void drawCameraScreen() {
  // page border and set to page colour *****************
  LCD.fillScr(VGA_WHITE);
  LCD.setColor(150, 255, 150);                                            // Pale Green
  LCD.fillRoundRect( 6, 52, 795, 350);
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 52, 795, 350);

  CTE_LCD.Set_character_spacing(1);                                      // Tighten spacing for tab text
  
  // Camera Tab
  LCD.setColor(150, 255, 150);                                           // Pale Green
  LCD.fillRoundRect(385, 3, 510, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(150, 255, 150);
  CTE_LCD.Put_Text("Camera", 395, 18, BVS_28);
   
  //Play Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(6, 3, 140, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Play", 15, 18, BVS_28);

  // Manual Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(145, 3, 250, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Manual", 155, 18, BVS_28);

  // Sequence Tab
  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect(255, 3, 380, 50);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("Record", 265, 18, BVS_28);
  
  //draw black border around each tab
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(6, 3, 140, 50);
  LCD.drawRoundRect(145, 3, 250, 50);
  LCD.drawRoundRect(255, 3, 380, 50);
  //LCD.drawRoundRect( 385, 3, 510, 50);

  // draw position information box - values in drawslider routine
  LCD.setColor(VGA_SILVER);
  LCD.fillRoundRect(10, 320, 790, 349);                                 // position information text is written in drawslider routin  e

  CTE_LCD.Set_character_spacing(3);                                       // Open spacing for "in page" text
 
  // Focus in Seconds box
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(15, 60, 265, 170);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  CTE_LCD.Put_Text("Focus Time (5)", 70, 65, BVS_22);
  FocusTimeUpdate();
  
  // Interval in Seconds box
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(275, 60, 525, 170);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  CTE_LCD.Put_Text("Shot Pulse Time (2)", 305, 65, BVS_22);
  ShotTimeUpdate();  

  // Interval in Seconds box
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(535, 60, 785, 170);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  CTE_LCD.Put_Text("Interval Time (100)", 565, 65, BVS_22);
  IntervalUpdate();                                                     // set green area in Interval indicator

  //  Focus adjust Increment and Decrement buttons
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  LCD.drawRoundRect(25, 90, 70, 125);                                  // Decrement Interval
  CTE_LCD.Put_Text("Dec", 30, 95, BVS_22);
  LCD.drawRoundRect(210, 90, 255, 125);                                // Increment Interval
  CTE_LCD.Put_Text("Inc", 220, 95, BVS_22);

  //  Shot Time adjust Increment and Decrement buttons
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  LCD.drawRoundRect(285, 90, 330, 125);                                 // Decrement Interval
  CTE_LCD.Put_Text("Dec", 290, 95, BVS_22);
  LCD.drawRoundRect(470, 90, 515, 125);                                 // Increment Interval
  CTE_LCD.Put_Text("Inc", 480, 95, BVS_22);

  //  interval adjust Increment and Decrement buttons
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  LCD.drawRoundRect(545, 90, 590, 125);                                 // Decrement Interval
  CTE_LCD.Put_Text("Dec", 550, 95, BVS_22);
  LCD.drawRoundRect(730, 90, 775, 125);                                 // Increment Interval
  CTE_LCD.Put_Text("Inc", 740, 95, BVS_22);

 // indications lines and arrows that show that the minimum Interval time is dependant on Focus and shot times
  LCD.drawLine(150, 170,150,185);                                       // vertical line - Focus
  LCD.drawLine(151, 170,151,185);
  LCD.drawLine(150, 184,350,184);                                       // horizontal line focus to shot
  LCD.drawLine(150, 185,350,185);
  geo.fillTriangle(350 + 5, 185, 350 - 10, 185 + 5, 350 - 10, 185 - 5); // Horiz arrow head for above  
  CTE_LCD.Put_Text("+", 370, 172, BVS_28);    
  LCD.drawLine(400, 170,400,185);                                       // vertical line - shot
  LCD.drawLine(401, 170,401,185);
  LCD.drawLine(400, 184,510,184);                                       // horizontal line - shot to Interval
  LCD.drawLine(400, 185,510,185);
  geo.fillTriangle(510 + 5, 185, 510 - 10, 185 + 5, 510 - 10, 185 - 5); // Horiz arrow head for above
  CTE_LCD.Put_Text("=", 530, 172, BVS_28);
  CTE_LCD.Put_Text("Min", 560, 175, BVS_22);
  LCD.drawLine(600, 184,660,184);                                       // horizontal line - min to Interval
  LCD.drawLine(600, 185,660,185);
  LCD.drawLine(660, 170,660,185);                                       // vertical line - Interval
  LCD.drawLine(661, 170,661,185);
  geo.fillTriangle(661, 170, 661 - 5, 180 , 661 + 5, 180);              // Vert arrow head for above
  
  // Total Shots Box
  LCD.setColor(VGA_BLACK);
  LCD.drawRoundRect(15, 200, 265, 310);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  CTE_LCD.Put_Text("Total Shots", 70, 205, BVS_22);
  
  // Total Shots Increment and Decrement buttons
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(150, 255, 150);                                      // Pale Green
  LCD.drawRoundRect(25, 235, 70, 270);                                // Decrement Interval
  CTE_LCD.Put_Text("Dec", 30, 243, BVS_22);
  LCD.drawRoundRect(210, 235, 255, 270);                                // Increment Interval
  CTE_LCD.Put_Text("Inc", 220, 243, BVS_22);
  TotalShotsUpdate();                                                   // set green area in Total Shots indicator

  drawSlider(newRotary, newLinear);
}

