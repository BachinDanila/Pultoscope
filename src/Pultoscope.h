/*
  Pultoscope.h - Library for oscilloscope
  based on Arduino and 1.8" tft display.
  Released into the public domain.
*/

#ifndef Pultoscope_h
#define Pultoscope_h

#include <Arduino.h>
#include <SPI.h>
#include <TFT.h>

//Define some colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

class Data {
    public:
        Data(uint8_t grid_size, uint8_t left, uint8_t right, uint8_t ok);
    private:
        uint8_t _scroll_left_pin;   //Pin for button which is scrolling graph left
        uint8_t _scroll_right_pin;  //Pin for button which is scrolling graph right
        uint8_t _ok_pin;            //Pin for confirm button
        uint8_t _grid_size;         //Graph grid size (set to 0 to turn off)
        uint8_t _Vmax;              //Max voltage
        uint8_t _reference_voltage; //Voltage reference flag
        uint8_t _scan;              //Oscilloscope scan value
};

class Screen { 
    public:
        Screen::Screen(uint8_t cs, uint8_t dc, uint8_t reset, Data data);
        void begin();
        void do_hello_world();
    private:
        uint8_t _cs;                //Chip Select pin
        uint8_t _dc;                //Dc or A0 pin on tft display
        uint8_t _reset;             //Reset pin(Can be connected to Arduino's reset pin)
        TFT _tft;                   //Main TFT Screen object
        Data _data;                //Class with all data 
        void draw_bottom_menu();    //Refresh bootom menu
};

#endif