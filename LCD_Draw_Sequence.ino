#include <Arduino.h>

/*
 * Draws sequence as recoreded by user
 */


// Draw a graphic reprsentation of the sequence showing angle and position  **************************************************************
void drawSequence() {
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int m = 0, RotCenterY = 250, LineCenterY = 300, RotCenterX = 0, RotCenterX2 = 0;

  LCD.setColor(243, 254, 180);                                            // Pale yellow  180
  LCD.fillRoundRect(10, 200, 790, 310);                                   // Remove previously drawn sequence diagram

  // draw the slider box
  LCD.setColor(VGA_MAROON);
  LCD.drawRoundRect( 65, 225, 735, 275);

  for (int n = 0; n < 10; n++) {                                            // Sequence loop

    if (LinMove[9] == 0) {                                                // If LinMove 9 (end point) is empty then there is no sequence - return
      return  ;
    }
    else {                                                                // LinMove 9 is non zero then endpoint has been set (end point is never zero - at least 1)
      while (LinMove[n] == 0) {                                           // don't know how many points have been set, so find first non zero array position
        n++;
      }
    }

    // Get and Scale Sequenced Rotary data
    m = RotMove [n] / 4 / 5;                                              // Rot is in 0.25 degree increments / 4 to get degrees and then gear ratio head to encoder 5:1
    m = m % 360;                                                          // Could have roated more than one rev so mod 360 to get orientation for screen

    // Get and Scale Sequenced Linear data
    RotCenterX = (LinMove [n] / 1.76 / 29.68) + 65;                       // divide by 1.76 to scale to screen (about 670 pixels to work with) and by 29.68 counts per mm and add 65 pixels from left edge
    RotCenterX2 = (LinMove [n + 1] / 1.762 / 29.68) + 65;

    if (LinMove [n] == 0)  {
      RotCenterX2 = RotCenterX  ;
    }

    // Draw pointer in new position in red
    m = m + 270;                                                         // get 0 deg at top - it would start at 3 oclock otherwise  as zero dir is right
    x1 = 47 * cos(m * 0.0175);                                           // 0.0175 = pi/180
    y1 = 47 * sin(m * 0.0175);
    x2 = 5 * cos(m * 0.0175);
    y2 = 5 * sin(m * 0.0175);
    x3 = 40 * cos((m + 8) * 0.0175);
    y3 = 40 * sin((m + 8) * 0.0175);
    x4 = 40 * cos((m - 8) * 0.0175);
    y4 = 40 * sin((m - 8) * 0.0175);

    // Diamond shaped pointer to show camera direction
    LCD.setColor(VGA_RED);
    LCD.setBackColor(243, 254, 180);                                     // Pale yellow
    geo.fillTriangle(x1 + RotCenterX, y1 + RotCenterY, x3 + RotCenterX, y3 + RotCenterY, x4 + RotCenterX, y4 + RotCenterY);
    geo.fillTriangle(x3 + RotCenterX, y3 + RotCenterY, x2 + RotCenterX, y2 + RotCenterY, x4 + RotCenterX, y4 + RotCenterY);
    LCD.drawCircle( RotCenterX, RotCenterY, 13);
    LCD.drawCircle( RotCenterX, RotCenterY, 12);

    switch (MoveType[n]) {
      case 1:
        CTE_LCD.Put_Text("S", RotCenterX - 4, RotCenterY - 8, BVS_19);
        break;
      case 2:
        CTE_LCD.Put_Text("W", RotCenterX - 7, RotCenterY - 8, BVS_19);
        break;
      case 3:
        CTE_LCD.Put_Text("F", RotCenterX - 4, RotCenterY - 8, BVS_19);
        break;
      case 4:
        CTE_LCD.Put_Text("E", RotCenterX - 4, RotCenterY - 8, BVS_19);
        break;
    }

    //draw Direction line with arrow head to show direction of travel

    // Get the direction arrow the right way round
    if ((LinMove[n] < LinMove[n + 1]) && (n != 9)) {
      LCD.setColor(VGA_BLUE);
      LCD.drawLine(RotCenterX, LineCenterY, RotCenterX2, LineCenterY);
      LCD.drawLine(RotCenterX, LineCenterY + 1, RotCenterX2, LineCenterY + 1);
      LCD.drawLine(RotCenterX, LineCenterY + 2, RotCenterX2, LineCenterY + 2);
      geo.fillTriangle(RotCenterX2 + 5, LineCenterY, RotCenterX2 - 15, LineCenterY + 10, RotCenterX2 - 15, LineCenterY - 9);
    }

    else if ((LinMove[n] > LinMove[n + 1]) && (n != 9)) {
      LCD.setColor(VGA_GREEN);
      LCD.drawLine(RotCenterX, LineCenterY - 90, RotCenterX2, LineCenterY - 90);
      LCD.drawLine(RotCenterX, LineCenterY - 91, RotCenterX2, LineCenterY - 91);
      LCD.drawLine(RotCenterX, LineCenterY - 92, RotCenterX2, LineCenterY - 92);
      geo.fillTriangle(RotCenterX2 - 5, LineCenterY - 91, RotCenterX2 + 15, LineCenterY - 81, RotCenterX2 + 15, LineCenterY - 101);
    }
  }  // for sequence loop
}  // Drawsequence

