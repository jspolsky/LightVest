#include "VUDisplayClassVest.h"

// Gamma correction from https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void VUDisplayClassVest::setup()
{
    
    strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
	iBrightnessLevel = 2;
	ToggleBrightness();

	iSolidColor = 0;

}

uint16_t VUDisplayClassVest::getRange()
{
    return 10;
}

void VUDisplayClassVest::showMeter(uint16_t level, uint16_t peak)
{

	uint32_t offColor = strip.Color(0,0,0),
		     peakColor = CorrectedColor(30,144,255);

	for (uint16_t x = 0; x < N_PIXELS; x++)
		strip.setPixelColor(x, offColor);

	if (peak > 0)
	{
		strip.setPixelColor(10-peak, peakColor);
		strip.setPixelColor(peak+29, peakColor);	
	}
	
	for (uint16_t x = 0; x < level; x++) 
	{
		strip.setPixelColor(9-x,   Wheel(map(x, 0, getRange(), 0, 255)));
		strip.setPixelColor(x+30,  Wheel(map(x, 0, getRange(), 0, 255)));
	}

    strip.show();
}

void VUDisplayClassVest::Solid()
{
	static uint32_t colorMap[] = 
		{
			CorrectedColor(30,144,255),		// trademark blue

			CorrectedColor(255,0,0),		// red
			CorrectedColor(255,192,64),		// yellow
			CorrectedColor(0,255,0),		// green

			CorrectedColor(255,0,255),		// purple
			CorrectedColor(255,128,0),		// orange

			CorrectedColor(255,255,255),	// white
			CorrectedColor(255,204,153),	// offwhite
		};

	for (uint16_t x = 0; x < N_PIXELS; x++)
		strip.setPixelColor(x, colorMap[iSolidColor]);

	strip.show();
}

void VUDisplayClassVest::ToggleSolidColor()
{
	iSolidColor = (iSolidColor + 1)	% 8;
}

void VUDisplayClassVest::ToggleBrightness()
{
	static byte brightMap[] = {1, 4, 8, 16, 32, 64};

	iBrightnessLevel = (iBrightnessLevel + 1) % 6;
	strip.setBrightness(brightMap[iBrightnessLevel]);
}



uint32_t VUDisplayClassVest::CorrectedColor( byte r, byte g, byte b )
{
	return strip.Color(
		pgm_read_byte(&gamma8[r]),
		pgm_read_byte(&gamma8[g]),
		pgm_read_byte(&gamma8[b])
	);
}



// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t VUDisplayClassVest::Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return CorrectedColor(WheelPos * 3, 255 - WheelPos * 3, 0);
	} 
	else if(WheelPos < 170) {
		WheelPos -= 85;
		return CorrectedColor(255 - WheelPos * 3, 0, WheelPos * 3);
	} 
	else {
		WheelPos -= 170;
		return CorrectedColor(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}
