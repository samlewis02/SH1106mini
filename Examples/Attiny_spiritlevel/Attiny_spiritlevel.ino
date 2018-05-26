
//
// Spirit level program
// Uses X and Y accel from MPU-6050 to move circle
// on 128 x 64 oled display
//
// Uses SH1106mini unbuffered SH1106 library
// Uses MPU-6050 to determine orientation
// A bubble shape is defined as a bit map and placed in the display
// dependent on orientation. It is blanked out and
// redrawn each time it moves
//
// 21/2/18
//

#include<TinyWireM.h>
#include <SH1106mini.h>

SH1106mini display(0x3c);

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX = 64;
int16_t AcY = 32;
int16_t oldAcX, oldAcY;
char  bmap[] =
{ B00111100,
  B01110110,
  B11110011,
  B11111011,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};

char  blank[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,                                  0,
  B00000000
};

void setup() {
  display.init();
  display.setContrast(255);
  display.flipScreenVertically();
  display.clear();

  TinyWireM.begin();
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x6B);  // PWR_MGMT_1 register
  TinyWireM.write(0);     // set to zero (wakes up the MPU-6050)
  TinyWireM.endTransmission(true);
}

void loop() {
  oldAcX = AcX;
  oldAcY = AcY;

  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(MPU_addr, 4); // request a total of 4 registers
  AcX = TinyWireM.read() << 8 | TinyWireM.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = TinyWireM.read() << 8 | TinyWireM.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

  AcY = map(AcY, -16000, 16000, 0, 119);
  AcX = map(AcX, -16000, 16000, 0, 55);
  if ((AcX != oldAcX) || (AcY != oldAcY)) {
    display.drawBitmap(oldAcY, oldAcX, blank );
    display.drawBitmap(AcY, AcX, bmap );
  }
  delay(120);
}
