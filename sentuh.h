#ifndef _SENTUH_H_
#define _SENTUH_H_
#include <stdint.h>

#define ESP32_WIFI_TOUCH  
 
#define ADC_MAX 4095  // maximum value for ESP32 ADC (default 11db, 12 bits)
#define aXM 9  // analog input pin connected to LCD_RS 
#define aYP 15  // analog input pin connected to LCD_WR
#define NOISE_LEVEL 4  // Allow small amount of measurement noise
 
typedef volatile uint32_t RwReg;
  
class TSPoint {
 public:
  TSPoint(void);
  TSPoint(int16_t x, int16_t y, int16_t z); 
  bool operator==(TSPoint);
  bool operator!=(TSPoint);
  int16_t x, y, z;
};

class TouchScreen {
 public:
  TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx);

  bool isTouching(void);
  uint16_t pressure(void);
  int readTouchY();
  int readTouchX();
  TSPoint getPoint();
  int16_t pressureThreshhold;

private:
  uint8_t _yp, _ym, _xm, _xp;
  uint16_t _rxplate;

  volatile RwReg *xp_port, *yp_port, *xm_port, *ym_port;
  RwReg xp_pin, xm_pin, yp_pin, ym_pin;

};

#endif
