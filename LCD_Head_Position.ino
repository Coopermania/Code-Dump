#include <Arduino.h>

/*
 * Draws Position of head, both linear and roatry position based on output from rotary encoders
 */



// Draw the graphic displaying the current positions and directions **************************************************************
void drawSlider(long Rotcounts, long Lincounts) {

  float x1, y1, x2, y2, x3, y3, x4, y4;
  int m = 0;
  float deg = 0.0; // for printing current position in degrees - the encoder only has a resalution of .25 deg.
  float mm = 0.0; // for printing current position in mm - encoder has a resalution of 0.02mm

  if (positionRotary > Rotcounts )  {
    RotDir = 1;
  }
  else if (positionRotary < Rotcounts ) {
    RotDir = 0;
  }
  if (positionLinear > Lincounts) {
    LinDir = 1;
  }
  else if (positionLinear < Lincounts) {
    LinDir = 0;
  }

  if (Lincounts < 0) {
    Lincounts = 0;  // Ignor negative movement - should be zeroed by limit switch
  }
  if (Lincounts > 57500) {
    Lincounts = 57485;  // Ignor movement above max - something has gone wrong stop and switch should stop this happening
  }

  // Scale Raw Rotary data
  deg = (float(Rotcounts) / 4 / 5);                                       // floating point for printing to screen in fractional steps

  m = Rotcounts / 4 / 5;                                                  // Rot is in 0.25 degree increments / 4 to get degrees and then gear ratio head to encoder 5:1
  m = m % 360;                                                            // Could have roated more than one rev so mod 360 to get orientation for screen

  // Scale Raw Linear data
  mm = Lincounts / 29.68;                                                 // floating point for printing
  LinPos = Lincounts / 29.68;                                             // turn rev into mm  about 29.68 counts per mm  (max 40 revs)
  rotatorCenterX = 65 + (LinPos / 1.76);                                  // scale to screen 670 pixels available for 40 revs - 1185mm

  LCD.printNumI(Rotcounts,600,270,5);
  LCD.printNumI(Lincounts,600,290,5);

  LCD.setColor(VGA_WHITE);
  LCD.fillRoundRect( 0, 351, 800, 480);

  // draw the slider and drive touch guides
  LCD.setColor(VGA_MAROON);
  LCD.drawRoundRect( 65, 400, 735, 430);
  if (!StartSeq) {                                                            // only draw slider touch guides if not in a sequence
                                        // slider rail rectangle
      LCD.setColor(221,221,221);                                              // light Gray for the blocks and arrows so they don't overpower the slider position graphic 
      LCD.fillRoundRect(390, 351, 410, 390);                                  // box to indicate half way - drives to datum
      geo.fillTriangle(232 , 365, 252, 355, 252, 375);                        // arrow head left in centre of arc
      geo.fillTriangle(70 , 380, 88, 365, 92, 385);                           // arrow head left at left end of arc
      geo.fillTriangle(567 , 365, 547, 355, 547, 375);                        // arrow head right in centre of arc
      geo.fillTriangle(730 , 380, 712, 365, 708, 385);                        // arrow head right at right end of arc
      geo.fillTriangle(70 , 455, 90, 465, 90, 445);                           // arrow head left
      geo.fillTriangle(730 , 455, 710, 465, 710, 445);                        // arrow head right
      LCD.fillRoundRect(40, 435, 60, 475);                                    // box to indicate near limit - drives to limit
      LCD.fillRoundRect(740, 435, 760, 475);                                  // box to indicate near limit - drives to limit
      
      LCD.setColor(VGA_GRAY);                                                 //darker gray for the lines
      geo.drawArc(232, 1300, 935,- 9, 9.5, 1);                                // x,y rad,start deg, end deg, thickness
      geo.drawArc(567, 1300, 935,- 9.5, 9, 1);                                // x,y rad,start deg, end deg, thickness
      LCD.drawLine(90, 455, 710, 455);                                        // line to indicate left right travel
  }

  
  // Draw pointer in new position in red
  m = m + 270; // get 0 deg at top - it would start at 3 oclock otherwise
  x1 = 47 * cos(m * 0.0175); //0.0175 = pi/180
  y1 = 47 * sin(m * 0.0175);
  x2 = 5 * cos(m * 0.0175);
  y2 = 5 * sin(m * 0.0175);
  x3 = 40 * cos((m + 8) * 0.0175);
  y3 = 40 * sin((m + 8) * 0.0175);
  x4 = 40 * cos((m - 8) * 0.0175);
  y4 = 40 * sin((m - 8) * 0.0175);

  // Diamond shaped pointer to show camera direction
  LCD.setColor(VGA_RED);
  geo.fillTriangle(x1 + rotatorCenterX, y1 + rotatorCenterY, x3 + rotatorCenterX, y3 + rotatorCenterY, x4 + rotatorCenterX, y4 + rotatorCenterY);
  LCD.setColor(VGA_RED);
  geo.fillTriangle(x3 + rotatorCenterX, y3 + rotatorCenterY, x2 + rotatorCenterX, y2 + rotatorCenterY, x4 + rotatorCenterX, y4 + rotatorCenterY);
  LCD.drawCircle( rotatorCenterX, rotatorCenterY, 11);
  LCD.drawCircle( rotatorCenterX, rotatorCenterY, 10);

  // position information  text
  LCD.setBackColor(VGA_SILVER);
  LCD.setColor(VGA_BLACK);
  LCD.setFont(BigFont);

  String Degrees = "Rotary Position   ";
  Degrees = Degrees + String(deg);
  Degrees = Degrees + String("deg.    ");
  CTE_LCD.Put_Text(Degrees, 400 , 320, BVS_28);               // update screen with current degrees

  String Milimeters = "Linear Position   ";
  Milimeters = Milimeters + String(mm);
  Milimeters = Milimeters + String("mm    ");
  CTE_LCD.Put_Text(Milimeters, 15 , 320, BVS_28);                  // update screen with current mm


  // draw position of rotator in new position ******************************************
  LCD.setColor(VGA_BLUE);
  // draw circle to represent rotating head
  LCD.drawCircle( rotatorCenterX, rotatorCenterY, 50);
  LCD.drawCircle( rotatorCenterX, rotatorCenterY, 49);
  LCD.setColor(VGA_RED);
  geo.fillTriangle(rotatorCenterX, 365, rotatorCenterX - 5, 355, rotatorCenterX + 5, 355); // show zero position
  LCD.setBackColor(VGA_WHITE);
  CTE_LCD.Put_Text("C", rotatorCenterX - 5, rotatorCenterY - 9, BVS_19);

  // draw arc and arrow head to show direction of current rotation
  LCD.setColor(VGA_BLUE);
  if (RotDir == 1) {
    geo.drawArc(rotatorCenterX, 415, 61, 10, 80, 3);
    geo.fillTriangle(rotatorCenterX + 60, 415, rotatorCenterX + 55, 405, rotatorCenterX + 65, 405);
  }
  else {
    geo.drawArc(rotatorCenterX, 415, 61, 280, 350, 3);
    geo.fillTriangle(rotatorCenterX - 60, 415, rotatorCenterX - 65, 405, rotatorCenterX - 55, 405);
  }

  // draw Direction line with arrow head to show direction of travel
  LCD.setColor(VGA_BLUE);
  if (LinDir == 1) {
    LCD.drawLine(rotatorCenterX + 50, 468, rotatorCenterX - 10, 468);
    LCD.drawLine(rotatorCenterX + 50, 469, rotatorCenterX - 10, 469);
    LCD.drawLine(rotatorCenterX + 50, 470, rotatorCenterX - 10, 470);
    geo.fillTriangle(rotatorCenterX + 50, 469, rotatorCenterX + 40, 475, rotatorCenterX + 40, 464);
  }
  else {
    LCD.drawLine(rotatorCenterX + 10, 468, rotatorCenterX - 50, 468);
    LCD.drawLine(rotatorCenterX + 10, 469, rotatorCenterX - 50, 469);
    LCD.drawLine(rotatorCenterX + 10, 470, rotatorCenterX - 50, 470);
    geo.fillTriangle(rotatorCenterX - 50, 469, rotatorCenterX - 40, 475, rotatorCenterX - 40, 464);
  }
  positionRotary = Rotcounts;
  positionLinear = Lincounts;
}

