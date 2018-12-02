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

        void showMeter(byte level, byte peak);
        void Solid();
        void ToggleSolidColor();
        void ToggleVUMeterMode();
        void ToggleBrightness();

    private:
        Stream& Serial;
        byte iSolidColor;            // color for Solid() mode
        byte iVUMeterMode;
        byte iBrightnessLevel;       
        uint32_t VUColor(byte level);
        uint32_t CorrectedColor( byte r, byte g, byte b );
    
    public:    
        Adafruit_NeoPixel strip;

};
