/*   User_setup.h in tft_espi library
#define ILI9488_DRIVER //pararel
#define TFT_PARALLEL_8_BIT

// ESP32 pins used for the parallel interface TFT
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   9   // RS Data Command control pin - must use a pin in the range 0-31
#define TFT_RST  46  // Reset pin
#define TFT_WR   16  // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD   10
#define TFT_D0   11  // Must use pins in the range 0-31 for the data bus
#define TFT_D1   17  // so a single register write sets/clears all bits
#define TFT_D2   12
#define TFT_D3   18
#define TFT_D4   13
#define TFT_D5   8
#define TFT_D6   4 //14
#define TFT_D7   3 
*/

#include <TFT_eSPI.h>
#include "sentuh.h"

TFT_eSPI tft = TFT_eSPI();

#define MINPRESSURE 300
#define MAXPRESSURE 1000

int CALB_LEFT=3610, CALB_RIGHT=475, CALB_TOP=3560, CALB_BOT=140;
const int rotation = 1; //  in rotation order - portrait, landscape, etc
const int XP =11, XM = 9, YP = 15, YM = 17;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
bool showTouch= false;
TSPoint p;

void setup() {
    Serial.begin(115200);
    tft.begin();   
    
    String orientation;
    switch (rotation) {
      case 0: 
        orientation = "Portrait";
      break;
      case 1: 
        orientation = "Landscape";
      break;
      case 2: 
        orientation = "Portrait Inverted";
      break;
      case 3: 
        orientation = "Landscape Inverted";
      break;
    }
    Serial.println(orientation);
    tft.setRotation(rotation);  
    tft.fillScreen(TFT_BLUE); 
    calibrates();
}

void loop() {  
    Touch_getXY();
}

bool Touch_getXY() {    
    int x,y;
    p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
      switch (rotation) {
        case 0: // portrait
          x = map(p.x, CALB_LEFT, CALB_RIGHT, 0, tft.width()); 
          y = map(p.y, CALB_TOP, CALB_BOT, 0, tft.height());
        break;
        case 1: // landscape
          x = map(p.y, CALB_TOP,       CALB_BOT, 0, tft.width()); //Serial.print(p.y);Serial.print("|");Serial.println(x); 
          y = map(p.x, CALB_LEFT,    CALB_RIGHT, 0, tft.height());//Serial.print(p.x);Serial.print("|");Serial.println(y);
        break;
        case 2: // portrait inverted
          x = map(p.x, CALB_RIGHT, CALB_LEFT, 0, tft.width()); 
          y = map(p.y, CALB_TOP, CALB_BOT, 0, tft.height());
        break;
        case 3: // landscape inverted
          x = map(p.y, CALB_LEFT, CALB_RIGHT, 0, tft.width()); 
          y = map(p.x, CALB_TOP, CALB_BOT, 0, tft.height());
        break;
        
      }      
      if (showTouch) tft.fillCircle(x, y, 2, TFT_YELLOW);
    }
    return pressed;
}

void calibrates(){
  showTouch= false;
  tft.setTextSize(2); tft.setTextColor(TFT_WHITE, TFT_BLUE);
  
  tft.fillScreen(TFT_BLUE);
  tft.drawString("< TOUCH", 0 , 320/2, 1); 
  while (Touch_getXY()==false){delay(100);} CALB_TOP = p.y;
  
  tft.fillScreen(TFT_BLUE);
  tft.drawString(".", 480/2 , -10, 1);tft.drawString("TOUCH THE DOT", 180 ,30, 1);
  delay(2000);
  while (Touch_getXY()==false){delay(100);} CALB_LEFT = p.x;
  
  tft.fillScreen(TFT_BLUE);  
  tft.drawString("TOUCH >", 395 , 320/2, 1);
  delay(2000);
  while (Touch_getXY()==false){delay(100);} CALB_BOT = p.y;
  
  tft.fillScreen(TFT_BLUE);  
  tft.drawString(".", 480/2 , 305, 1);tft.drawString("TOUCH THE DOT", 180 ,260, 1);
  delay(2000);
  while (Touch_getXY()==false){delay(100);} CALB_RIGHT = p.x;
  
  tft.fillScreen(TFT_BLUE); showTouch= true; 
  //Serial.println(CALB_LEFT);  Serial.println(CALB_RIGHT);  Serial.println(CALB_TOP);  Serial.println(CALB_BOT);
}

 
