#include "Pultoscope.h"

Data::Data(uint8_t left, uint8_t right, uint8_t ok){
    _scroll_left_pin = left;
    _scroll_right_pin = right;
    _ok_pin = ok;
    _Vmax = 0;
    _trigger_value = 0;
    _reference_voltage_flag = 1;
    _Vexternal = 0;
    _t = 0;
    _t_division_value = 0;
}

Screen::Screen(uint8_t cs, uint8_t dc, uint8_t reset,
    uint8_t grid_size, Data *data): 
    _tft(TFT(cs, dc, reset)),_data(data){

    _cs = cs;
    _dc = dc;
    _reset = reset;
    _tft = TFT(_cs, _dc, _reset);
    _data = data;
    _grid_size = grid_size;
    _top_menu = 1;
    _x = 60;
    _pause_time = 0;
    _pause_flag = 0;
    _pause_clean_flag = 0;
}

uint8_t Data::get_max_voltage(){
    return _Vmax;
}

uint8_t Data::get_trigger_value(){
    return _trigger_value;
}

uint8_t Data::get_t_division_value(){
    return _t_division_value;
}

bool Data::get_reference_voltage_flag(){
    return _reference_voltage_flag;
}

void Data::read_data(){
    if (_trigger_value >= 6){ADCSRA = 0b11100010;}  //delitel 4
    if (_trigger_value == 5){ADCSRA = 0b11100011;}  //delitel 8
    if (_trigger_value == 4){ADCSRA = 0b11100100;}  //delitel 16
    if (_trigger_value == 3){ADCSRA = 0b11100101;}  //delitel 32
    if (_trigger_value == 2){ADCSRA = 0b11100110;}  //delitel 64
    if (_trigger_value <  2){ADCSRA = 0b11100111;}   //delitel 128
    if (_trigger_value == 0){
        _t=micros(); 
        for(int i=0;i<500;i++){ 
            while ((ADCSRA & 0x10)==0);
            ADCSRA|=0x10;
            delayMicroseconds(100);
            _adc_buffer[i]=ADCH;
        }
        _t = micros() - _t;
        _t_division_value = _t * 25/501;
    }
    if (_trigger_value > 0){
        _t=micros();
        for(int i=0;i<500;i++){ 
            while ((ADCSRA & 0x10)==0);
            ADCSRA|=0x10;
            _adc_buffer[i]=ADCH;
        }
        _t = micros() - _t;
        _t_division_value = _t * 25/501;
    }
}

uint8_t Data::get_external_power_source_voltage(){
    return _Vexternal;
}

void Data::set_external_power_source_voltage(uint8_t external_power_pin){
    _Vexternal = analogRead(external_power_pin)*5.3/1024;
}

void Data::set_reference_voltage_value(){
    if(!_reference_voltage_flag){ADMUX = 0b11100101;} //Internal reference voltage 1,1V
    if(_reference_voltage_flag){ADMUX = 0b01100101;}  //External reference voltage
}

void Screen::begin(){
    _tft.begin();
    delay(50);
    _tft.fillScreen(BLACK);
    _tft.textSize(default_text_size);
    delay(500);
}

void Screen::do_hello_world(){
    //generate a random color
    uint8_t redRandom = random(0, 255);
    uint8_t greenRandom = random (0, 255);
    uint8_t blueRandom = random (0, 255);
    
    // set a random font color
    _tft.stroke(redRandom, greenRandom, blueRandom);
    
    // print Hello, World! in the middle of the screen
    _tft.text("Hello, World!", 6, 57);
}

void Screen::draw_bottom_menu(){                
    _tft.setRotation(0);  
    _tft.fillRect(0, 20, 20,320, BLACK);    //Erase the bottom menu
    _tft.setRotation(1);
    _tft.setCursor(0,220);
    _tft.print("t=");

    uint8_t trigger_value = _data->get_trigger_value();
    uint8_t t_division_value = _data->get_t_division_value();

    if(trigger_value <  7) _tft.print(t_division_value);
    if(trigger_value == 7) _tft.print(t_division_value/2); 
    if(trigger_value == 8) _tft.print(t_division_value/3);
    if(trigger_value == 9) _tft.print(t_division_value/4);
    if(trigger_value == 10)_tft.print(t_division_value/5);
    _tft.print("us  ");
    _tft.print("Vmax=");
    if(!(_data->get_reference_voltage_flag())){
        _tft.print(_data->get_max_voltage()*1.1/255);
    }
    if(_data->get_reference_voltage_flag()){
        _tft.print(_data->get_max_voltage()*5.3/255);
    }
    _tft.print(" ");
    _tft.print("B");
    _tft.print(_data->get_external_power_source_voltage());
}

void Screen::top_menu_state_handler(){
    if(_top_menu == 0){}
}