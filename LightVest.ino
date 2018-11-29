/*

LED VU meter for Arduino and Adafruit NeoPixel LEDs.

Requires:
	* 8x32 NeoPixel matrix (https://www.adafruit.com/product/2294)
	* Arduino Uno
	* Adafruit electret microphone (https://www.adafruit.com/product/1063)

Features:
	* Automatic gain control


 Hardware requirements:
 - Most Arduino or Arduino-compatible boards (ATmega 328P or better).
 - Adafruit Electret Microphone Amplifier (ID: 1063)
 - Adafruit Flora RGB Smart Pixels (ID: 1260)
 OR
 - Adafruit NeoPixel Digital LED strip (ID: 1138)
 - Optional: battery for portable use (else power through USB or adapter)
 Software requirements:
 - Adafruit NeoPixel library
 
 Connections:
 - 3.3V to mic amp +
 - GND to mic amp -
 - Analog pin to microphone output (configurable below)
 - Digital pin to LED data input (configurable below)

 See notes in setup() regarding 5V vs. 3.3V boards - there may be an
 extra connection to make and one line of code to enable or disable.
 
 */

#include <SPI.h> 
#include <math.h>


#include "VUDisplayClassVest.h"

#define MIC_PIN   		A1  	// Microphone is attached to this analog pin
#define BUTTONS_PIN   A2

typedef enum { BUTTONS_NONE, BUTTONS_MODE, BUTTONS_COLOR, BUTTONS_BRIGHTNESS } btn_t;
typedef enum { VMODE_VU, 
               VMODE_SOLID, 
               VMODE_UNUSED } vmode_t;

#define SAMPLE_WINDOW   10  	// Sample window for average level, in milliseconds -- try 10.


uint16_t peak = 0;      // Peak level of column; used for falling dots
byte peakCycle = 0;     // Toggles between 0 and 1. To prevent peak falling too fast, it only falls when this is 0
vmode_t vmode = VMODE_VU;

// LOOK YOU GUYS I HAVE AUTOMATIC GAIN CONTROL!
unsigned long millisAdjusted;   // the last time we adjusted the gain. Every second we'll reevaluate
float dbMax;					// max db recorded in the last second
float dbRange;					// current range of dbs which will be scaled to (0..WIDTH)


VUDisplayClassVest display(Serial);

void setup() 
{

	peak = peakCycle = 0;
	millisAdjusted = millis();
	dbMax = 0.0; dbRange = 2.0;

	Serial.begin(250000);
	display.setup();

//	analogReference(EXTERNAL);  // for 5V Arduinos like Uno, connect the mic to the AREF pin, and leave this on.
								// for 3.3v Arduinos like Flora, Gemma, comment this out.

}


void loop()
{
  btn_t b = DebouncedButtonPress();

  if (b == BUTTONS_MODE)
  {
    vmode = static_cast<vmode_t> ((int(vmode) + 1) % (int(VMODE_UNUSED)));
  }

  if (b == BUTTONS_COLOR)
  {
    if (vmode == VMODE_SOLID)
      display.ToggleSolidColor();
  }

  if (b == BUTTONS_BRIGHTNESS)
  {
    display.ToggleBrightness();
  }

  switch (vmode)
  {
    case VMODE_VU:
      loopVU(); 
      break;

    case VMODE_SOLID:
      loopSolid();
      break;
  }
}


void loopSolid()
{
  display.Solid();
}


void loopVU() 
{

	peakCycle = (peakCycle + 1) % 10;

	unsigned long startMillis = millis();  // Start of sample window

	uint16_t signalMax = 0;
	uint16_t signalMin = 1023;
	uint16_t scaledLevel = 0;

	float db = 0.0;

	// collect data for length of sample window (in mS)
	while (millis() - startMillis < SAMPLE_WINDOW)
	{
		uint16_t sample = analogRead(MIC_PIN);
		if (sample < 1024)  // toss out spurious readings
		{
			if (sample > signalMax)
			{
				signalMax = sample;  // save just the max levels
			}
			else if (sample < signalMin)
			{
				signalMin = sample;  // save just the min levels
			}
		}
	}

	db = dbScale(signalMax, signalMin, 0.5);

//  Serial.print(db); Serial.print(","); Serial.println(dbRange);

	dbMax = max(dbMax, db);
	scaledLevel = min(display.getRange(), (floor((db / dbRange * (float) display.getRange()))));

	// has 1 second elapsed since we last calculated scaling factor?
	if (millisAdjusted + 1000L < millis()) 
	{
		millisAdjusted = millis();
		dbRange = max(dbMax, 2.0);
		dbMax = 0.0;
	}

	if (scaledLevel > peak)
		peak = scaledLevel;
	else if (peak > 0 && peakCycle == 0)
		peak--;

	display.showMeter(scaledLevel, peak);

}

/*
 *		Calculates decibels based on the peak-to-peak voltage 
 *		Subtracts dbFloor so as to return 0 when it's basically quiet
 */
float dbScale( float signalMax, float signalMin, float dbFloor )
{
	float db = 20.0 * log10( signalMax / signalMin ); 
	float result = db - dbFloor;
	if (result < 0.0)
		result = 0.0;

	return result;
}



// Button processing.
//
// Call DebouncedButtonPress to find out if a button has been pressed.
// If it returns BUTTONS_NONE, nothing has been pressed.
// Otherwise treat the result as a press.
// Note that it will only return each press once no matter how long
// the user holds down the button.

btn_t DebouncedButtonPress()
{
    static btn_t switchState = BUTTONS_NONE;

    btn_t f = BUTTONS_NONE;
    btn_t newSwitchState = WhichButton();

    if (newSwitchState != BUTTONS_NONE && switchState == BUTTONS_NONE)
    {
        // have I been here lately?
        if (Debounce())
        {
            f = newSwitchState;
        }
    }

    switchState = newSwitchState;
    return f;
}

bool Debounce()
{
    static unsigned long debounceTime = 0;

    if (millis() - debounceTime > 100UL)
    {
        debounceTime = millis();
        return true;
    }

    return false;
}



btn_t WhichButton()
{
  uint32_t raw = analogRead(BUTTONS_PIN);

  if (raw < 250) return BUTTONS_NONE;
  if (raw < 500) return BUTTONS_COLOR;
  if (raw < 825) return BUTTONS_BRIGHTNESS;
  return BUTTONS_MODE;
}

