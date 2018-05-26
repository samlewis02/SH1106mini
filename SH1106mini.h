#pragma once

#include <Arduino.h>
#include <TinyWireM.h>

#define CHARGEPUMP 0x8D
#define COLUMNADDR 0x21
#define COMSCANDEC 0xC8
#define COMSCANINC 0xC0
#define DISPLAYALLON 0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define EXTERNALVCC 0x1
#define INVERTDISPLAY 0xA7
#define MEMORYMODE 0x20
#define NORMALDISPLAY 0xA6
#define PAGEADDR 0x22
#define PAGESTARTADDRESS 0xB0
#define SEGREMAP 0xA1
#define SETCOMPINS 0xDA
#define SETCONTRAST 0x81
#define SETDISPLAYCLOCKDIV 0xD5
#define SETDISPLAYOFFSET 0xD3
#define SETHIGHCOLUMN 0x10
#define SETLOWCOLUMN 0x00
#define SETMULTIPLEX 0xA8
#define SETPRECHARGE 0xD9
#define SETSEGMENTREMAP 0xA1
#define SETSTARTLINE 0x40
#define SETVCOMDETECT 0xDB
#define SWITCHCAPVCC 0x2
#define READMODIFYWRITE 0xe0
#define END 0xee
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
enum { 
	LEFT, 
	CENTRE, 
	RIGHT 
	};

class SH1106mini {

private:
    // I2C
    int myI2cAddress;
   	char charflipped[8];
	// flip font  (not visible publicly)
	void flipChar(const char * bitmap);

 public:
   // For I2C display: create the display object 
   SH1106mini(int i2cAddress);

   // Initialize the display
   void init();

   // Cycle through the initialization
   void resetDisplay(void);

   // Connect again to the display through I2C
   void reconnect(void);

   // Turn the display on
   void displayOn(void);

   // Turn the display offs
   void displayOff(void);

   // Clear the display buffer
   void clear(void);

      // Set display contrast
   void setContrast(char contrast);

   // Turn the display upside down
   void flipScreenVertically();

   // Send a command to the display (low level function)
   void sendCommand(unsigned char com);

   // Send all the init commands
   void sendInitCommands(void);

   // Write a  8 x 8 bitmap direct to display
   void drawBitmap(int x, int y, uint8_t *bitmap);
   
   // Write a string to display using alignment
   void printAt (int y, String s, int alignment);

   };