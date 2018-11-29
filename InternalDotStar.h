#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_DotStar.h>

class InternalDotStar
{
    public:
        InternalDotStar(Stream& s) : Serial(s), strip(1, 3, 4, DOTSTAR_BRG) {};
        virtual ~InternalDotStar() {};

        void setup();
        void flash(byte r, byte g, byte b);

    private:
        Stream& Serial;

    public:
        Adafruit_DotStar strip;
};

