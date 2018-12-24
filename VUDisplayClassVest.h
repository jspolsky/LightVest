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

        void ShowMeter(byte level, byte peak);
        void ShowSolid();
        void ShowMessage();
        void ToggleSolidColor();
        void ToggleVUMeterMode();
        void ToggleBrightness();

    private:
        Stream& Serial;
        byte iSolidColor;            // color for Solid() mode - index into colorMap
        byte iVUMeterMode;
        byte iBrightnessLevel;       
        uint32_t VUColor(byte level);
        uint32_t CorrectedColor( byte r, byte g, byte b );

        uint32_t colorMap[8];       // eight colors available for solid mode
    
    public:    
        Adafruit_NeoPixel strip;

};
