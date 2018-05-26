/*

*/

#include "SH1106mini.h"
#include "SH1106fonts.h"

// constructor for I2C - we indicate i2cAddress
SH1106mini::SH1106mini(int i2cAddress) {
  myI2cAddress = i2cAddress;
}

void SH1106mini::init() {
  TinyWireM.begin();
  sendInitCommands();
  resetDisplay();
}

void SH1106mini::resetDisplay(void) {
  displayOff();
  clear();
  displayOn();
}

void SH1106mini::reconnect() {
   TinyWireM.begin();
}

void SH1106mini::displayOn(void) {
    sendCommand(0xaf);        //display on
}

void SH1106mini::displayOff(void) {
  sendCommand(0xae);          //display off
}

void SH1106mini::setContrast(char contrast) {
  sendCommand(0x81);
  sendCommand(contrast);
}

void SH1106mini::flipScreenVertically() {
  sendCommand(0xA0);              //SEGREMAP   //Rotate screen 180 deg
  sendCommand(SETCOMPINS);
  sendCommand(0x22);
  sendCommand(COMSCANINC);            //COMSCANDEC  Rotate screen 180 Deg
}

void SH1106mini::clear(void) {
    for (uint8_t page = 0; page<8; page++){
		sendCommand(PAGESTARTADDRESS | page);
		sendCommand(SETLOWCOLUMN | 2);
		sendCommand(SETHIGHCOLUMN | 0);
			
		// send as 16 bunches of 8 bytes data in one xmission
		for(uint8_t y=0; y<16; y++){
			TinyWireM.beginTransmission(myI2cAddress);
			TinyWireM.write(0x40);
			for (uint8_t w=0; w<8; w++) {
				TinyWireM.write(0x0);
			}
			yield();
			TinyWireM.endTransmission();
		}
	}
}

void SH1106mini::drawBitmap(int x, int y, uint8_t *bitmap) {
	uint8_t  width = 8;
	
	if ( x < 0) x = 0;
	if (x > 119) x = 119;
	x += 2;
	if (y < 0) y = 0;
    if (y > 55) y = 55;	// constrain x,y to stop running over edge
	uint8_t page = y >> 3; // quicker divide by eight
	uint8_t posn = y & 7; // might be 0
	// Use union to pre-shift data
	union shifty
	{
	uint16_t intVar;
    uint8_t Bytes[2];
	};
	
	shifty newval[8]; // array of unions to hold shifted data
	
	for (uint16_t n = 0; n<8; n++){ // initialise array and shift by posn
		newval[n].intVar = 0; // don't assume it's clear
		newval[n].Bytes[0] = pgm_read_byte_near(bitmap[n]);
		newval[n].intVar = newval[n].intVar << posn;
	}
	// write first page (might be the only one, depending on posn)
	sendCommand(PAGESTARTADDRESS | page++); // increment in case we need another page
	sendCommand(SETLOWCOLUMN | ( x & 0xf));
	sendCommand(SETHIGHCOLUMN | ( (x>>4)& 0xf));

	// send 8 bytes of data in one go
	TinyWireM.beginTransmission(myI2cAddress);
	TinyWireM.write(0x40);
	for (uint8_t w=0; w<width; w++) {
		TinyWireM.write(newval[w].Bytes[0]);
	}
	yield();
	TinyWireM.endTransmission();

	if (posn) { // true (non zero) so do another page
		sendCommand(PAGESTARTADDRESS | page);
		sendCommand(SETLOWCOLUMN | ( x & 0xf));
		sendCommand(SETHIGHCOLUMN | ( (x>>4)& 0xf));
		// send 8 bytes of data in one go
		TinyWireM.beginTransmission(myI2cAddress);
		TinyWireM.write(0x40);
		for (uint8_t w=0; w<width; w++) {
			TinyWireM.write(newval[w].Bytes[1]);
		}
		yield();
		TinyWireM.endTransmission();
	}
}

void SH1106mini::flipChar (  const char * bitmap) {
	for (int i = 0; i < 8; i++) {
    charflipped[i] = 0; // clear to start
    for (int j = 0; j < 8; j++) {
      charflipped[i] |= (((pgm_read_byte(bitmap + j)) >> i) & 1) << j;
    }
  }
}

void SH1106mini::printAt (int y, String s, int alignment){
	int x;
	// string length must be less or equal to screen width
	// otherwise abandon
	if (s.length()<= 16) { 
		switch (alignment) {
			case LEFT:
				x = 0;
				break;
			case CENTRE:
				x = (128 - (s.length() << 3)) >> 1; // x = (128 - string width) / 2
				break;
			case RIGHT:
				x = (128 - (s.length() << 3)); // x = (128 - string width)
				break;
			default:
				break;
		}

		for (int k = 0; k < s.length(); k++) {
			flipChar (font8x8[s.charAt(k) - 0x20]);
			drawBitmap(x, y, charflipped);
			x += 8;
		}
	}
}
	
void SH1106mini::sendCommand(unsigned char com) {
   TinyWireM.beginTransmission(myI2cAddress);      //begin transmitting
   TinyWireM.write(0x80);                          //command mode
   TinyWireM.write(com);
   TinyWireM.endTransmission();                    // stop transmitting
  
}

void SH1106mini::sendInitCommands(void) {
  sendCommand(DISPLAYOFF);
  sendCommand(NORMALDISPLAY);
  sendCommand(SETDISPLAYCLOCKDIV);
  sendCommand(0x80);
  sendCommand(SETMULTIPLEX);
  sendCommand(0x3F);
  sendCommand(SETDISPLAYOFFSET);
  sendCommand(0x00);
  sendCommand(SETSTARTLINE | 0x00);
  sendCommand(CHARGEPUMP);
  sendCommand(0x14);
  sendCommand(MEMORYMODE);
  sendCommand(0x00);
  sendCommand(SEGREMAP);
  sendCommand(COMSCANDEC);
  sendCommand(SETCOMPINS);
  sendCommand(0x12);
  sendCommand(SETCONTRAST);
  sendCommand(0xCF);
  sendCommand(SETPRECHARGE);
  sendCommand(0xF1);
  sendCommand(SETVCOMDETECT);
  sendCommand(0x40);
  sendCommand(DISPLAYALLON_RESUME);
  sendCommand(NORMALDISPLAY);
  sendCommand(0x2e);            // stop scroll
  sendCommand(DISPLAYON);
}