#include <Arduino.h>


/* Camera Slider Touch LCD

 This program uses the UTFT libraries from :-
 Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
 web: http://www.RinkyDinkElectronics.com/

 5 page display :-
 Play Page displays stats and Start, Pause and Stop buttons for unatented shooting
 Manual Page displays controls that will move the slider and or the rotator using the touch display
 Record Page displays controls to allow the camera to be moved through a number of positions which will then be repeated automatiaclly
           using the motorised slider and motorised rotator.
 Camera Page displays controls to set Interval between shots, Exposure (if camera is in bulb mode) Total number of shots for sequence
 Utility Page - future development
*/

//*****************************************************************
#include<stdlib.h>
#include <UTFT.h>                 // basic TFT Library            *
#include <UTouch.h>               // basic touch library          *
#include <UTFT_Geometry.h>        // extended TFT library         *
#include <Encoder.h>              // encoder library              *
#include <avr/pgmspace.h>         //                              *
#include <AccelStepper.h>         // Stepper motor library        *
#include <MultiStepper.h>
#include <TimerThree.h>           // Interupt timer               *
#include <UTFT_CTE.h>             // cold tears //                *
#include "SPI.h"                  //                              *
#include <PinChangeInt.h>         // Pin Change Interupt library  *
//*****************************************************************

//Initialise LCD ColdTears CTE 50 **************************************
UTFT LCD(CTE50, 38, 39, 40, 41); // Arduino Mega pins for display          *
// Initialize Touch Coldtears touchscreen //                           *
UTouch  Touch( 6, 5, 4, 3, 2); // Arduino Mega pins for touch          *
//Initialise UTFT Extension UTFT_Geometry                              *
UTFT_Geometry geo(&LCD);//                                             *
UTFT_CTE CTE_LCD(&LCD); // Coldtears fonts extensions                  *
//**********************************************************************

//Initialise Encoders **************************************************
Encoder RotaryEnc(19, 18);  // Encoder reading Roatry Position         *
Encoder LinearEnc(20, 21);  // Encoder reading Linear Position         *
//**********************************************************************

/*
  I can never remeber this !
  uses stdlib.h
  change float to string with control!
  dtostrf(FLOAT, WIDTH, PRECISION, BUFFER)
  WIDTH is the number of characters to use in the output. For a value in the range 0.000 to 5.000, the width should be the precision +2. More is OK.
  PRECISION is the number of characters after the decimal point. For temperature, 1 would be sufficient.
  BUFFER is where the write the characters to.
  char buffer[10];
  CTE_LCD.Put_Text(dtostrf(FocusTime,5,1,buffer), 110, 95, BVS_22);
*/

// The Linear Stepper pins
#define STEPPER1_DIR_PIN 17
#define STEPPER1_STEP_PIN 16
// The Rotary stepper pins
#define STEPPER2_DIR_PIN 13
#define STEPPER2_STEP_PIN 9

// Define both steppers and the pins to use
AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);
 
// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SmallFont[];
extern uint8_t Retro8x16[];  // this same width as small but taller - more readable
extern unsigned int cooperman[0x25F8];

/* Available coldtears font sizes
  BVS_13,   BVS_15   BVS_19   BVS_22   BVS_28   BVS_34   BVS_43   BVS_52   BVS_74   BVS_112
  Character spacing and use:-
  CTE_LCD.Set_character_spacing(20);
  CTE_LCD.Put_Text("Coldtears electronics",10,5,BVS_13);
*/
word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_SILVER, VGA_GRAY, VGA_MAROON, VGA_OLIVE, VGA_LIME, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA, VGA_TEAL, VGA_NAVY, VGA_PURPLE, VGA_TRANSPARENT };

// Variables ******************************************************************************************************
int rotatorCenterX = 100;     // x, centre of rotator symbol - range 100 to 700
int rotatorCenterY = 415;     // y, centre of rotator symbol - fixed on lower section of screen
int RotPos = 0;               // current position in counts of rotary encoder from datum clockwise
int LinPos = 0;               // linear position in counts of rotary encoder
bool RotDir = 0;               // Roatary Direction currently set 0=clockwise -1=counterclockwise
bool LinDir = 0;               // Linear Direction currently set 0= away from motor end of slider -1 towards motor end of slider
long TimRem = 0;              // Time remaining in seconds
int x = 0;                    // touch x read
int y = 0;                    // touch y read
int pg = 1;                   // page counter 1 to 5 home manual sequence camera  - utility not yet implimented
int LinSpeed = 255;           // Linear Speed displayed on screen 0 to 255
int RotSpeed = 175;           // Roation Speed on displayed on screen 0 to 255
float FocusTime = 1.5;        // Focus time in seconds - allowance for focusing before shot
float ShotTime =.5;           // Shot time - pulse length
float Interval = 0;           // Interval time between shots start at 2 sec - there is a 2.5 sec delay to focus and a half second delay to shoot in addition to any interval set
float Exposure = 1.0;         // exposure time normally camera will be in automatic or aperture mode this is only for extended exposure - the camera would be stationary and in "Bulb" mode
int TotalShots = 100;         // Number of shots to divided across the movement of the camera
float flValueOfAnyBox = 0;    // a general floating point number used for displaying green strip on "gauges"
long positionRotary = -999;   // Last known position of Rotary encoder
long positionLinear = -999;   // Last known position of Linear encoder
long newRotary = 0;           // raw count from rotary encoder
long newLinear = 0;           // raw count from linear encoder
long LinMove [10];            // Movement array Linear
long RotMove [10];            // Movement array Rotary
int MoveType [10];            // Type 1=Start, 2=Way point, 3=Fixed point (must be in pairs) 4=End point
int Sequence = 0;             // Sequetially move throught the array ---- When recording positions it will normally be in reverse order so the cameras last position is the start
bool ComitSequence = 0;       // Comit Sequence -1 true when ready to comit and move from sequence page to camera page
int LRSpeed = 1;              // Left / right speed - actually the amount of delay between pules in ms - range 50  to 1024
int RSpeed = 1;               // rotational speed  - actually the amount of delay between pules in ms - range 50  to 1024
int JoyLR;                    // Joystick Left Right value
int JoyRot;                   // Joystick Rotation value
int oldJoyLR;                 // Joystick previous Left Right value
int oldJoyRot;                // Joystick previous Rotation value
int stepspershot;             // number of steps per shot
unsigned long totalsteps;     // totalsteps can be mix of Lin and Rot depending on who has the most in a segement
int taken = 0;                // number of shots taken in total
int Datum = 0;                // Datum will be true if slider has hit near limit switch and roatry datum switch - make -1 manually for now
int StartPos;                 // start position of array[0] min array[8] max  -- array[9] is end point
float ratio;                  // ratio between lin and rot steps
float StepsPerLinCount ;      // set in setup
float StepsPerRotCount ;      // set in setup
float oldLin = 0;             // temporary variable used in "takePicture" procedure to check movement
volatile bool NearBlock;      // Near Limit made inhibit movement in negative direction
volatile bool FarBlock;       // Far Limit made inhibit movement in posative direction
volatile bool PauseSeq = 0;   // PauseSeq is set true only if a sequence is running and the "Pause" button is touched
volatile bool StopSeq = 0;    // StopSeq is set true only if a sequence is running and the "Stop" button is touched
bool StartSeq = 0;            // used to show if sequence is running  or not
unsigned long TotTim;         // total time of sequence
bool StillTimeLapse = true;   // if still mode is active sequence  will play in increments to end at the requiered postion and shot count else it will drive continuously through the sequence (video)
long positions[2];            // Array of stepper positions for MultiStepper


void setup() { // S E T U P ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  LCD.InitLCD(LANDSCAPE);
  LCD.clrScr();
  LCD.fillScr(VGA_WHITE);
  LCD.setFont(BigFont);

  Touch.InitTouch(LANDSCAPE);
  Touch.setPrecision(PREC_HI);
  LCD.setFont(BigFont);
  LCD.setColor(VGA_BLACK);
  LCD.setBackColor(VGA_WHITE);

  // buttons
  pinMode(10, INPUT);                                                   // Green
  pinMode(8, INPUT);                                                   // Yellow
  pinMode(14, INPUT);                                                   // Blue
  pinMode(15, INPUT);                                                   // Red
  
  // set pull-ups on buttons
  digitalWrite (10, HIGH);
  digitalWrite (8, HIGH);
  digitalWrite (14, HIGH);
  digitalWrite (15, HIGH);

 // attachPinChangeInterrupt(10, Green, FALLING);                           // Calls Green Button interupt handler
 // attachPinChangeInterrupt(8, Yellow, FALLING);                          // Calls Yellow Button interupt handler
 // attachPinChangeInterrupt(14, Blue, FALLING);                            // Calls Blue Button interupt handler
 // attachPinChangeInterrupt(15, Red, FALLING);                             // Calls Red Button interupt handler
  
  pinMode(11, OUTPUT);                                                   // sleep on easydriver - linear
  analogWrite(11,255);                                                    // use PWM to control current but hold motor position - saves power when motors are stopped
  
  pinMode(12, OUTPUT);                                                   // sleep on easydriver - rotary
  analogWrite(12, 0);                                                    // use PWM to control current but hold motor position - saves power when motors are stopped
  delay(250);
  
  //Camera
  pinMode(A0, OUTPUT);                                                   // focus pin
  pinMode(A1, OUTPUT);                                                   // Shoot pin

  
  
  // Linear enc = 1440 pulses per rev ---- motor 200 steps per rev at full step -- 400 @ half step - 800 @ quarter step - 1600 at eighth step so ratio from enc to motor is:-
  //  7.2 at full - 3.6 at half - 1.8 at quarter and 0.9 at eighth

  // new motor has 5 and 2/11 ths gearbox or 5.18181818:1 ratio so we have to multiply the steps per count by this ratio so 1600 steps per rev of motor * 5.18181818 = 8290.90908  // so 1440 / 8290.909088 = 0.1736842105872576
  // However driving the motor for 100,000 pulses = 24386 Encoder pulses so ratio =24,386/100,000 = 4.100713524153203


  // Rotary enc = 7200 (5:1 ratio on belt drive) pulses per camera rev ---- motor 1000 steps per rev at full step -- 2000 @ half step - 4000 @ quarter step - 8000 at eighth step
  //  7.2 at full - 3.6 at half - 1.8 at quarter and 0.9 at eighth

  // new motor has 5 and 2/11 ths gearbox or 5.18181818:1 ratio so we have to multiply the steps per count by this ratio so 1600 steps per rev of motor * 5.18181818 = 8290.909088 then ...
  //  x 5 for belt ratio - so 7200 counts per rev / (8290.909088 * 5) = 0.1736842105872576     ---- 8290.90909 * 5 = 41454.54, 5454
  // however in testing 416000 pulses gives 10 revolutions of the camera, so we will work with the actual steps per count of 7200 /41600 = 5.777777777777778


  StepsPerLinCount = 4.1 ;                              // adjust multiplier to match step setting above
  StepsPerRotCount = 5.757575756944444 ;                // adjust multiplier to match step setting above

  //Supply monitors
  pinMode(A2, INPUT);                                                   // +20 volt monitor
  pinMode(A6, INPUT);                                                   // +5 volt monitor
  pinMode(A7, INPUT);                                                   // Battery A full
  pinMode(A8, INPUT);                                                   // Battery A mid
  pinMode(A9, INPUT);                                                   // Battery B full
  pinMode(A10, INPUT);                                                  // Battery B mid
  
  // Limit Switches  
  pinMode(A11, INPUT);                                                    // Lin Far End Switch digital pin 65
  pinMode(A12, INPUT);                                                    // Lin Near End Switch digital pin 66
  pinMode(A13, INPUT);                                                    // Rot Datum Switch digital pin 67  
    
  // use interupt for limit switches as when driving motors ...
  //  ... process blocks main execution too much to react quickly enough
  
  //---------------------------------------------- Set PWM frequency for pins D11 & D12 -----------------------------
  TCCR1B = TCCR1B & B11111000 | B00000001;      // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
  
  attachPinChangeInterrupt(A12, FarLimit, CHANGE);                        // Calls FarLimit routine on chnage of value of the pin - Toggle "blocking" of positive travel if on far limit
  attachPinChangeInterrupt(A11, NearLimit, CHANGE);                       // Calls NearLimit routine on chnage of value of the pin - Toggle "blocking" of negative travel if on near limit

  pinMode(A14, INPUT);                                                    // Lin Joystick Analoge input 
  pinMode(A15, INPUT);                                                    // Rot Joystick Analoge input
  
  // draw splash screen - waits for a touch
   LCD.drawBitmap (280, 80, 120, 81, cooperman, 2);//

  CTE_LCD.SPI_Flash_init(FLASH_CS_PIN);
  CTE_LCD.Set_character_spacing(20);
  CTE_LCD.Put_Text("COOPER  SOLUTIONS", 140, 250, BVS_34);
  CTE_LCD.Put_Text("CAMERA  SLIDER   ", 190, 300, BVS_34);
  CTE_LCD.Set_character_spacing(3);
  while  (! Touch.dataAvailable()) {                                     // wait for touch to continue
    // wait for touch
  }
   
  Timer3.initialize(500000);                                             // set a timer of length 500000 microseconds   (or 0.5 sec ) timer used to check for limit switches ...
                                                                         // .. as cpu can be too busy on other processes, especially motor drive
    
  LinSpeed = constrain (LinSpeed, 0, 255);                               // constrain LinSpeed to 0 to 255 for display on Manual screen
  LRSpeed = map(LinSpeed, 0, 255, 1, 3500);                              // map LinSpeed to motor speed requirements in AccelStepper library
  RotSpeed = constrain (RotSpeed, 0, 255);                               // constrain RotSpeed to 0 to 255 for display on Manual screen
  RSpeed = map(RotSpeed, 0, 255, 1, 3000);                               // map RotSpeed to motor speed requirements in AccelStepper library

  LCD.clrScr();
  LCD.fillScr(VGA_WHITE);
 
  EstablishDatum ();
  PlayScreen();
  drawSequence();
  

} // END OF SETUP *************************************************************



void loop() {// M A I N  L O O P ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // read encoders
  newRotary = RotaryEnc.read();
  newLinear = LinearEnc.read();
  LCD.printNumI(newLinear, 540,300,6);
  //LCD.printNumI(newRotary, 540,320,6);

  // If one has changed, process them both
  if (newRotary != positionRotary || newLinear != positionLinear)  {
    drawSlider(newRotary, newLinear);
  }

  if (Touch.dataAvailable()) {
    ScreenTouch();
  }
  
  // read joystick values
  JoyLR = analogRead(A14);
  JoyRot = analogRead(A15);
  //LCD.printNumI(JoyLR, 460,300,4);
  //LCD.printNumI(JoyRot, 360,300,4);
  //LCD.printNumI(pg,260,300,2);

  if (!StartSeq){
    if ((JoyRot < 500) || (JoyRot > 610) || (JoyLR < 500) || (JoyLR > 610)) {
      
      JoyStick();
      
    }
  }
 
}











