#include "VUDisplayClassVest.h"

void VUDisplayClassVest::setup()
{
    
    strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.setBrightness(30);

}

uint16_t VUDisplayClassVest::getRange()
{
    return 10;
}

void VUDisplayClassVest::showMeter(uint16_t level, uint16_t peak)
{

	uint32_t offColor = strip.Color(0,0,0),
		     peakColor = strip.Color(255,255,255);

	for (uint16_t x = 0; x < 40; x++)
		strip.setPixelColor(x, offColor);

	for (uint16_t x = 0; x < level; x++) {
		strip.setPixelColor(9-x,   Wheel(map(x, 0, getRange(), 0, 255)));
		strip.setPixelColor(x+30,  Wheel(map(x, 0, getRange(), 0, 255)));
	}

	if (peak > 2)
	{
		strip.setPixelColor(9-peak, peakColor);
		strip.setPixelColor(peak+30, peakColor);	
	}

    strip.show();
}



// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t VUDisplayClassVest::Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} 
	else if(WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} 
	else {
		WheelPos -= 170;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}
