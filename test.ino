// include Pultoscope libraries
#include "src/Pultoscope.h"

//Handle value of menu state variable
// pin definition for Arduino UNO
#define LCD_CS 10   // Chip Select goes to D10
#define LCD_DC 9    //Dc or A0 pin on tft display 
#define LCD_RESET 8 // Can alternately just connect to Arduino's reset pin
#define external_power_pin A6

#define levo 7
#define ok 6
#define pravo 5
#define vertikal 0  //размер вертикальной сетки(если "0" отключениа)

// create an instance of the library
Data data = Data(levo, pravo, ok);
Screen screen = Screen(LCD_CS, LCD_DC, LCD_RESET, vertikal ,&data);

void setup() {
    screen.begin();
}

void loop() {
    screen.do_hello_world();
}