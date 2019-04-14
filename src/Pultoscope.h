/*
  Pultoscope.h - Library for oscilloscope
  based on Arduino and 1.8" tft display.
  Released into the public domain.
*/

#ifndef Pultoscope_h
#define Pultoscope_h
#include <TFT.h>

//Define some colors
#define BLACK   0x000000
#define BLUE    0x001FFF
#define RED     0xF80000
#define GREEN   0x07E000
#define CYAN    0x07FFFF
#define MAGENTA 0xF81FFF
#define YELLOW  0xFFE000
#define WHITE   0xFFFFFF

//Define some default screen settings
#define default_text_size 1
#define screen_height 160
#define screen_width 128

class Data {
    public:
        Data(uint8_t left, uint8_t right, uint8_t ok);
        uint8_t get_external_power_source_voltage();
        uint8_t get_trigger_value();        //Get trigger value
        uint8_t get_max_voltage();          //Get max voltage
        uint8_t get_t_division_value();     //Get trigger division price
        void read_data();                   //Read data and write it to ADC buffer
        void set_external_power_source_voltage(uint8_t external_power_pin);
        void set_reference_voltage_value();  
        bool get_reference_voltage_flag();
    private:
        uint8_t  _scroll_left_pin;     //Pin for button which is scrolling graph left
        uint8_t  _scroll_right_pin;    //Pin for button which is scrolling graph right
        uint8_t  _ok_pin;              //Pin for confirm button
        uint8_t  _adc_buffer[501];     //ADC buffer array
        uint8_t  _adc_buffer_div[501]; //ADC divisors buffer array
        uint8_t  _Vmax;                //Max voltage
        uint8_t  _trigger_value;       //Oscilloscope trigger value(Значение развертки)
        uint64_t _t;                   //Variable used to calculate oscilloscope trigger
        uint8_t  _t_division_value;    //Oscilloscope trigger division price(microseconds)
        bool  _reference_voltage_flag; //Oscilloscope trigger flag(Флаг опорного напряжения)
        float _Vexternal;              //External power source voltage
        
};

class Screen { 
    public:
        Screen::Screen(uint8_t cs, uint8_t dc, uint8_t reset,
        uint8_t grid_size, Data *data);
        void begin();                               //Initialize class in setup()
        void do_hello_world();                      //Test function
        void draw_bottom_menu();                    //Refresh bootom menu
        void top_menu_state_handler();              //Handle value of menu state variable
    private:
        uint8_t _cs;                //Chip Select pin
        uint8_t _dc;                //Dc or A0 pin on tft display
        uint8_t _reset;             //Reset pin(Can be connected to Arduino's reset pin)
        uint8_t _top_menu;          //Top menu state
        uint8_t _grid_size;         //Graph grid size (set to 0 to turn off)
        uint64_t _pause_time;       //Variable to replace Delay(Pause time)
        bool  _pause_flag;          //Pause mode flag
        bool  _pause_clean_flag;    //Clean pause time flag
        float _x;                   //X axis counter
        TFT _tft;                   //Main TFT Screen object
        Data *_data;                //Class with all data 
};

#endif