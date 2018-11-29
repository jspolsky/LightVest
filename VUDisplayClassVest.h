#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 1 
#define N_PIXELS 40


class VUDisplayClassVest
{

    public:
        VUDisplayClassVest(Stream& s) : Serial(s) {};
        virtual ~VUDisplayClassVest() {};

        void setup();
        uint16_t getRange();

        void showMeter(uint16_t level, uint16_t peak);
        void Solid();
        void ToggleSolidColor();

    private:
        Stream& Serial;
        uint32_t Wheel(byte WheelPos);
        int iSolidColor; // color for Solid() mode
        uint32_t CorrectedColor( byte r, byte g, byte b );
    
    public:    
        Adafruit_NeoPixel strip;

};
