/*
 * Rotary Encoder Controlled 4-digit Passcode Lock.
 * 
 * Made by KushagraK7: https://www.instructables.com/member/KushagraK7/
 * 
 * The digits are entered by rotating the encoder in on direction and the digit's place is selected by rotating 
 * it in the other direction. 
 * 
 * You can watch this video to understand its operation: https://youtu.be/31elyQBwYBg
 * 
 * Feel free to tinker with the code and try out new things, but make sure to give proper attribution while 
 * publishing it.
 * 
*/

#include <SevSeg.h>

SevSeg sevseg; //Instantiate a seven segment controller object

#define inA 0
#define inB 1

#define LED 15

int num[4] = {0, 0, 0, 0};//For storing entered passcode.
int pass[] = {2, 3, 4, 5};//This is the stored passcode, you can change it from here.
int i;
int n = 0;
int p = 0;
int c = 0;
int cc = 0;
int nc = 0;

int aState;
int aLastState;

void setup()
{
  pinMode(inA, INPUT);
  pinMode(inB, INPUT);

  pinMode(LED, OUTPUT);
  
  byte numDigits = 4;   
  byte digitPins[] = {18, 19, 20, 21};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};

  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10); //Note: 100 brightness simply corresponds to a delay of 2000us after lighting each segment. A brightness of 0 
                            //is a delay of 1us; it doesn't really affect brightness as much as it affects update rate (frequency).
                            //Therefore, for a 4-digit 7-segment + pd, COMMON_ANODE display, the max update rate for a "brightness" of 100 is 1/(2000us*8) = 62.5Hz.
                            //I am choosing a "brightness" of 10 because it increases the max update rate to approx. 1/(200us*8) = 625Hz.
                            //This is preferable, as it decreases aliasing when recording the display with a video camera....I think.
Serial.begin(9600);

aLastState = digitalRead(inA);   

  p += pass[0]*1000;//Passcode digit at 1000's place.
  p += pass[1]*100;//Passcode digit at 100's place.
  p += pass[2]*10;//Passcode digit at 10's place.
  p += pass[3];//Passcode digit at 1's place.

}

void loop()
{

  aState = digitalRead(inA);

  if(aState != aLastState)
  {
    if(digitalRead(inB) != aState)
    {
      if(cc == 150) cc = 0;
      cc++;
    }
    else
    {
      if(nc == 220) nc = 0;
      nc++;
    }

    aLastState = aState;
  }

  if(cc <= 120) c = map(cc, 0, 120, 0, 4);
  if(nc <= 200) num[3-c] = map(nc, 0, 200, 0, 9);

  

  n = 0;

  n += num[0]*1000;//Set the 1000's digit.
  n += num[1]*100;//Set the 100's digit.
  n += num[2]*10;//Set the 10's digit.
  n += num[3];//Set the 1's digit.
  
  //Serial.println(c);
  if(c <= 4) sevseg.setNumber(n, c);

  if(c == 4)//Checking if the entered passcode matches the stored one when the decimal point dissapears from the display.
  {
    if(n == p)  digitalWrite(LED, HIGH);//Set the pin to HIGH if it matches. You can put any task to be done here.
  }
  else digitalWrite(LED, LOW);//Keep the pin LOW or set it LOW.
  
  
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
}
