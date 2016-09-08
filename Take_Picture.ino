#include <Arduino.h>


/*
 * Camera controls
 */

void TakePicture() {
  // Take a picture
  // have to sort out the pin allocations etc, for now just delay the interval

  taken++;
  // read encoders
  newRotary = RotaryEnc.read();
  newLinear = LinearEnc.read();
  drawSlider(newRotary, newLinear);
  // update Shots Remaining box
  LCD.setColor(VGA_SILVER);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_SILVER);
  String ShotofShots = "Shot ";
  ShotofShots = ShotofShots + String(taken);                         // sort out new print of Shot of Shots
  ShotofShots = ShotofShots + " of ";
  ShotofShots = ShotofShots + String (TotalShots);
  ShotofShots = ShotofShots + "  ";
  CTE_LCD.Put_Text(ShotofShots, 26, 65, BVS_22);
  
  float TimPerShot = TotTim / float(TotalShots);
  String RemTime = "Remaining Time ";
  RemTime = RemTime + String(TotTim - (TimPerShot * float(taken)));
  RemTime  = RemTime + " of ";
  RemTime  = RemTime + String (TotTim);
  RemTime  = RemTime + "     ";
  
  CTE_LCD.Put_Text(RemTime,350, 65, BVS_22);
  
  oldLin = newLinear;
  digitalWrite(A0, HIGH);                                            // Focus - Pin is Analog 0 used as Digital output
  delay(FocusTime * 1000);
  digitalWrite(A1, HIGH);                                            // Shoot - Pin is Analog 1 used as Digital output
  delay(ShotTime * 1000);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  delay(Interval * 1000);                                         // Interval is displayed  
                                                                    

}  // end take picture

