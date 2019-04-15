// include Pultoscope libraries
#include "src/Pultoscope.h"

//Handle value of menu state variable
// pin definition for Arduino UNO
#define LCD_CS 10   // Chip Select goes to D10
#define LCD_DC 9    //Dc or A0 pin on tft display 
#define LCD_RESET 8 // Can alternately just connect to Arduino's reset pin
#define external_power_pin A6

#define levo 7
#define ok 5
#define pravo 6
#define vertikal 0  //размер вертикальной сетки(если "0" отключениа)

// create an instance of the library
Data data = Data(levo, pravo, ok);
Screen screen = Screen(LCD_CS, LCD_DC, LCD_RESET, vertikal ,&data);

void setup() {
    data.set_external_power_source_voltage(external_power_pin);
    data.set_reference_voltage_value();
    screen.begin();
}

void loop() {
    data.read_data();
    screen.top_menu_state_handler();
    screen.pause_button_handler();

    data.set_reference_voltage_value();
    delay(5);

    screen.draw_voltage_axis();
    screen.draw_time_axis();
    screen.draw_grid();
    screen.draw_voltage_scale();

    if(!screen.get_pause_flag())
        screen.draw_bottom_menu();

    data.set_max_voltage();

    if(!screen.get_pause_flag())
        screen.draw_graph_default();

    if(screen.get_pause_flag())
        screen.draw_graph_pause();
}