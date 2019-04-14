// include Pultoscope libraries
#include "src/Pultoscope.h"

// pin definition for Arduino UNO
#define cs 10
#define dc 9
#define rst 8

#define levo 7
#define ok 6
#define pravo 5
#define vertikal 2  //размер вертикальной сетки(если "0" отключениа)

// create an instance of the library
Data data = Data(vertikal, levo, pravo, ok);
Screen screen = Screen(cs, dc, rst, &data);

void setup() {
    data.set_reference_voltage_value(NULL);
    screen.begin();
}

void loop() {
    screen.do_hello_world();
    delay(500);
}