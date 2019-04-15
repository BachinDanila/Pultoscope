/*
  Pultoscope.h - Library for oscilloscope
  based on Arduino and 1.8" tft display.
  Released into the public domain.
*/

#ifndef Pultoscope_h
#define Pultoscope_h
#include <TFT.h>

//Define some colors
#define BLACK   0x0000
#define RED     0x001F
#define BLUE    0xF800
#define GREEN   0x07E0
#define YELLOW  0x07FF
#define MAGENTA 0xF81F
#define CYAN    0xFFE0
#define WHITE   0xFFFF

//Define some default screen settings
#define default_text_size 1             //Default screen text size
#define display_height 128              //Display height(pixels)
#define display_width 160               //Display width(pixels)
#define graph_lower_bound 105           //Constant which defines lower bound of the graph
#define graph_upper_bound 2.75          //Constant which defines upper bound of the graph
#define horizontal_axis_pixel_step 25

class Data {
    public:
        Data(uint8_t left, uint8_t right, uint8_t ok);
        uint8_t get_external_power_source_voltage();
        uint8_t get_adc_buffer_element(uint16_t index);
        uint8_t get_adc_del_buffer_element(uint16_t index);
        uint8_t get_scale();                //Get scale value
        uint8_t get_max_voltage();          //Get max voltage
        uint8_t get_scale_time_step();      //Get trigger division price
        uint8_t get_scroll_right_pin();     //Get button pin
        uint8_t get_scroll_left_pin();      //Get button pin
        uint8_t get_ok_pin();               //Get button pin
        void set_scale(uint8_t value);      //Set scale value
        void read_data();                   //Read data and write it to ADC buffer
        void set_external_power_source_voltage(uint8_t external_power_pin);
        void set_adc_del_buffer_element(uint16_t index, uint8_t value);
        void set_reference_voltage_value();  
        void set_reference_voltage_flag(bool flag);
        void set_max_voltage();
        bool get_reference_voltage_flag();
    private:
        uint8_t  _scroll_left_pin;     //Pin for button which is scrolling graph left
        uint8_t  _scroll_right_pin;    //Pin for button which is scrolling graph right
        uint8_t  _ok_pin;              //Pin for confirm button
        uint8_t  _adc_buffer[501];     //ADC buffer array
        uint8_t  _adc_buffer_del[501]; //ADC buffer array with deleted elements
        uint8_t  _Vmax;                //Max voltage
        uint8_t  _scale;               //Oscilloscope scale value(Значение развертки)
        uint64_t _time;                //Temporary variable used to calculate scale time step
        uint64_t  _scale_time_step;     //Value of the scale price in microseconds
        bool  _reference_voltage_flag; //Oscilloscope reference voltage flag
        float _Vexternal;              //External power source voltage
        
};

class Screen { 
    public:
        Screen::Screen(uint8_t cs, uint8_t dc, uint8_t reset,
        uint8_t grid_size, Data *data);
        void begin();                               //Initialize class in setup()
        void do_hello_world();                      //Test function
        void draw_bottom_menu();                    //Refresh bootom menu
        void draw_reference_voltage_choose_menu();  //Top menu mode 0(choose reference voltage)
        void draw_default_top_menu();               //Top menu mode 1(choose scale)
        void draw_pause_menu();                     //Top menu mode 2(pause)
        void pause_button_handler();                //Handle pause button state
        void top_menu_state_handler();              //Handle top menu state
        void draw_voltage_axis();                   //Draw voltage vertical axis
        void draw_time_axis();                      //Draw time horizontal axis
        void draw_grid();                           //Draw grid
        void draw_voltage_scale();                  //Draw voltage scale
        void draw_graph_default();                  //Draw graph in default mode 
        void draw_graph_pause();                    //Draw graph in pause mode
        bool get_pause_flag();                      //Get pause flag
    private:
        uint8_t _cs;                //Chip Select pin
        uint8_t _dc;                //Dc or A0 pin on tft display
        uint8_t _reset;             //Reset pin(Can be connected to Arduino's reset pin)
        uint8_t _top_menu;          //Top menu state
        uint8_t _grid_size;         //Graph grid size (set to 0 to turn off)
        bool  _pause_flag;          //Pause mode flag
        bool  _pause_clean_flag;    //Clean pause time flag
        float _x;                   //X axis counter
        TFT _tft;                   //Main TFT Screen object
        Data *_data;                //Class with all data 
};

#endif