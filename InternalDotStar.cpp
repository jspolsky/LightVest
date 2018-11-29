#include "InternalDotStar.h"

void InternalDotStar::setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    strip.begin();
    strip.clear();
    strip.setBrightness(0);
    strip.show();
}

void InternalDotStar::flash( byte r, byte g, byte b )
{
    strip.setPixelColor(0, strip.Color(r,g,b));
    strip.setBrightness(96);
    strip.show();
    delay(75);
    strip.setBrightness(0);
    strip.show();
}

