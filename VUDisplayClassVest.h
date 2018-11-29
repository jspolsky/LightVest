#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 1 
#define N_PIXELS 52


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
        void ToggleBrightness();

    private:
        Stream& Serial;
        uint32_t Wheel(byte WheelPos);
        int iSolidColor;            // color for Solid() mode
        int iBrightnessLevel;       
        uint32_t CorrectedColor( byte r, byte g, byte b );
    
    public:    
        Adafruit_NeoPixel strip;

};
