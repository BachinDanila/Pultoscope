#include "Pultoscope.h"

Data::Data(uint8_t left, uint8_t right, uint8_t ok){
    _scroll_left_pin = left;
    _scroll_right_pin = right;
    _ok_pin = ok;
    _Vmax = 0;
    _scale = 0;
    _reference_voltage_flag = 1;
    _Vexternal = 0;
    _time = 0;
    _scale_time_step = 0;
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
    _pause_flag = false;
    _pause_clean_flag = false;
}

uint8_t Data::get_max_voltage(){
    return _Vmax;
}

uint8_t Data::get_scale_time_step(){
    return _scale_time_step;
}

uint8_t Data::get_external_power_source_voltage(){
    return _Vexternal;
}

uint8_t Data::get_adc_buffer_element(uint16_t index){
    if(index < 501)
        return _adc_buffer[index];
    else return NULL;
}

uint8_t Data::get_adc_del_buffer_element(uint16_t index){
    if(index < 501)
        return _adc_buffer_del[index];
    else return NULL;
}

uint8_t Data::get_scale(){
    return _scale;
}

uint8_t Data::get_scroll_right_pin(){
    return _scroll_right_pin;
}

uint8_t Data::get_scroll_left_pin(){
    return _scroll_left_pin;
}

uint8_t Data::get_ok_pin(){
    return _ok_pin;
}

void Data::set_scale(uint8_t value){
    _scale = value;
}

void Data::set_adc_del_buffer_element(uint16_t index, uint8_t value){
    if(index < 501) 
        _adc_buffer_del[index] = value;
}

void Data::set_reference_voltage_flag(bool flag){
    _reference_voltage_flag = flag;
}

void Data::set_external_power_source_voltage(uint8_t external_power_pin){
    _Vexternal = analogRead(external_power_pin)*5.3/1024;
}

void Data::set_reference_voltage_value(){
    if(!_reference_voltage_flag){ADMUX = 0b11100101;} //Internal reference voltage 1,1V
    if(_reference_voltage_flag){ADMUX = 0b01100101;}  //External reference voltage
}

void Data::set_max_voltage(){
    _Vmax=0; 
    for(int i=0;i<500;i++){  
        if(_Vmax < _adc_buffer[i]) _Vmax = _adc_buffer[i];
    }
}

bool Data::get_reference_voltage_flag(){
    return _reference_voltage_flag;
}

void Data::read_data(){
    if (_scale >= 6){ADCSRA = 0b11100010;}  //divisor 4
    if (_scale == 5){ADCSRA = 0b11100011;}  //divisor 8
    if (_scale == 4){ADCSRA = 0b11100100;}  //divisor 16
    if (_scale == 3){ADCSRA = 0b11100101;}  //divisor 32
    if (_scale == 2){ADCSRA = 0b11100110;}  //divisor 64
    if (_scale <  2){ADCSRA = 0b11100111;}  //divisor 128
    if (_scale == 0){
        _time=micros(); 
        for(int i=0;i<500;i++){ 
            while ((ADCSRA & 0x10)==0);
            ADCSRA|=0x10;
            delayMicroseconds(100);
            _adc_buffer[i]=ADCH;
        }
        _time = micros() - _time;
        _scale_time_step = _time * horizontal_axis_pixel_step / 501;
    }
    if (_scale > 0){
        _time=micros();
        for(int i=0;i<500;i++){ 
            while ((ADCSRA & 0x10)==0);
            ADCSRA|=0x10;
            _adc_buffer[i]=ADCH;
        }
        _time = micros() - _time;
        _scale_time_step = _time * horizontal_axis_pixel_step / 501;
    }
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
    _tft.fillRect(0, 10, 10,display_width, BLACK); //Erase bottom menu
    _tft.setRotation(1);
    _tft.setCursor(0,display_height - 8);
    _tft.print("t=");

    uint8_t scale = _data->get_scale();
    uint8_t scale_time_step = _data->get_scale_time_step();

    if(scale <  7) _tft.print(scale_time_step);
    if(scale == 7) _tft.print(scale_time_step/2); 
    if(scale == 8) _tft.print(scale_time_step/3);
    if(scale == 9) _tft.print(scale_time_step/4);
    if(scale == 10)_tft.print(scale_time_step/5);
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

void Screen::draw_default_top_menu(){
    _tft.setRotation(1);
    _tft.setTextColor(RED);
    _tft.fillRect(display_width - 54, 0,50, 10, CYAN);
    _tft.setCursor(0,2);
    if(!_data->get_reference_voltage_flag()) _tft.print("op-1.1V ");
    if( _data->get_reference_voltage_flag()) _tft.print("op-5.3V ");

    if(digitalRead(_data->get_scroll_right_pin())==HIGH){//доработать скорость
        _tft.setRotation(0); 
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране графика  
        _tft.fillRect(display_height - 12,display_width - 70, 12 , 10, BLACK);
        _data->set_scale(_data->get_scale() + 1);
        if(_data->get_scale()==11) _data->set_scale(10); 
    }

    if(digitalRead(_data->get_scroll_left_pin())==HIGH){//доработать скорость
        _tft.setRotation(0); 
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране графика  
        _tft.fillRect(display_height - 12,display_width - 70, 12 , 10, BLACK);
        _data->set_scale(_data->get_scale() - 1);
        if(_data->get_scale() == 255) _data->set_scale(0);
    }

    _tft.setRotation(1); 
    _tft.print(" scale*");
    _tft.print(_data->get_scale());
    _tft.print("  PauseOFF");
}

void Screen::draw_pause_menu(){
    _pause_flag = true;
    if(!_pause_clean_flag){
        _pause_clean_flag = true;  
        _tft.setRotation(1);
        _tft.setTextColor(RED);
        _tft.fillRect(display_width - 54, 0,50, 10, CYAN);
        _tft.setCursor(0,2);
        if(!_data->get_reference_voltage_flag()) _tft.print("op-1.1V ");
        if( _data->get_reference_voltage_flag()) _tft.print("op-5.3V ");
        _tft.print(" scale*");
        _tft.print(_data->get_scale());
        if(!_pause_flag) _tft.print("  PauseOFF");
        if( _pause_flag) _tft.print("  PauseON");
    }
}

void Screen::draw_reference_voltage_choose_menu(){
    _tft.setRotation(1);
    _tft.setTextColor(RED);
    _tft.fillRect(0, 0, 45, 10, CYAN);
    _tft.setCursor(0,2);
    if(!_data->get_reference_voltage_flag()) _tft.print("op-1.1V ");
    if( _data->get_reference_voltage_flag()) _tft.print("op-5.3V ");

    if(digitalRead(_data->get_scroll_right_pin())==HIGH){
        _tft.setRotation(0); 
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
        _tft.fillRect(16, 0, 97, 24, BLACK);//стирание напряжения
        _data->set_reference_voltage_flag(!_data->get_reference_voltage_flag());
    }

    if(digitalRead(_data->get_scroll_left_pin())==HIGH){
        _tft.setRotation(0);  
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
        _tft.fillRect(16, 0, 97, 24, BLACK);//стирание напряжения
        _data->set_reference_voltage_flag(!_data->get_reference_voltage_flag());
    }

    _tft.setRotation(1);  
    _tft.print(" scale*");
    _tft.print(_data->get_scale());
    _tft.print("  PauseOFF");
}

void Screen::pause_button_handler(){
    if(digitalRead(_data->get_ok_pin())==HIGH){
        _top_menu++;
        _tft.setRotation(0); 
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
        if(_top_menu == 3) { 
            _top_menu=0; 
            _pause_flag = false; 
            _pause_clean_flag = false;
        } 
        _tft.setRotation(1); 
        _tft.fillRect(0, 0, display_width, 12, BLACK); //Стирание верхнего меню   
    }
}

void Screen::draw_voltage_axis(){
    _tft.setRotation(0);
    _tft.drawFastHLine(20,28,95,BLUE);
    _tft.drawFastHLine(20,27,95,BLUE);

    _tft.drawFastVLine(110,23,5, BLUE);
    _tft.drawFastVLine(92,23,5, BLUE);
    _tft.drawFastVLine(74,23,5,BLUE);
    _tft.drawFastVLine(56,23,5,BLUE);
    _tft.drawFastVLine(38,23,5,BLUE);
}

void Screen::draw_time_axis(){
    _tft.setRotation(0);
    _tft.drawFastVLine(19,27,141,RED);
    _tft.drawFastVLine(20,27,140,RED);

    _tft.drawFastHLine(15,45,5, RED);
    _tft.drawFastHLine(15,63,5, RED);
    _tft.drawFastHLine(15,81,5, RED);
    _tft.drawFastHLine(15,99,5, RED);
    _tft.drawFastHLine(15,117,5,RED);
    _tft.drawFastHLine(15,135,5,RED);
    _tft.drawFastHLine(15,153,5,RED);
}

void Screen::draw_grid(){
    _tft.setRotation(0);
    _tft.drawFastHLine(75,310,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,310,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,310,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,310,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,285,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,285,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,285,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,285,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,260,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,260,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,260,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,260,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,235,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,235,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,235,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,235,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,210,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,210,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,210,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,210,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,185,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,185,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,185,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,185,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,160,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,160,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,160,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,160,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,135,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,135,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,135,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,135,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,110,5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,110,5*_grid_size,YELLOW);
    _tft.drawFastHLine(145,110,5*_grid_size,YELLOW);
    _tft.drawFastHLine(180,110,5*_grid_size,YELLOW);
    _tft.drawFastHLine(75,85, 5*_grid_size, YELLOW);
    _tft.drawFastHLine(110,85,5*_grid_size, YELLOW);
    _tft.drawFastHLine(145,85,5*_grid_size, YELLOW);
    _tft.drawFastHLine(180,85,5*_grid_size, YELLOW);
}

void Screen::draw_voltage_scale(){
    _tft.setRotation(1);  
    _tft.setTextColor(RED);

    if(!_data->get_reference_voltage_flag()){
        _tft.setCursor(0, 105);_tft.println("0.00"); 
        _tft.setCursor(0, 87); _tft.println("0.22"); 
        _tft.setCursor(0, 69); _tft.println("0.44"); 
        _tft.setCursor(0, 51); _tft.println("0.66"); 
        _tft.setCursor(0, 33); _tft.println("0.88"); 
        _tft.setCursor(0, 15); _tft.println("1.1V");   
    }
    if(_data->get_reference_voltage_flag()){
        _tft.setCursor(0, 105);_tft.println("0.00"); 
        _tft.setCursor(0, 87); _tft.println("1.00"); 
        _tft.setCursor(0, 69); _tft.println("2.00"); 
        _tft.setCursor(0, 51); _tft.println("3.00"); 
        _tft.setCursor(0, 33); _tft.println("4.00");
        _tft.setCursor(0, 15); _tft.println("5.0V"); 
    }
}

void Screen::draw_graph_default(){
    _tft.setRotation(0); 
    _x=28;
    uint8_t scale = _data->get_scale();
    for(int y=0;y<260;y++){
        _tft.setRotation(1); 
        if(scale <  7)  _x++;
        if(scale == 7)  _x+=2;
        if(scale == 8)  _x+=3;
        if(scale == 9)  _x+=4;
        if(scale == 10) _x+=5;

        //стираем график
        _tft.drawLine(_x + 1, graph_lower_bound - _data->get_adc_del_buffer_element(y)/
            graph_upper_bound + 1, _x + 2,graph_lower_bound -
            _data->get_adc_del_buffer_element(y+1)/
            graph_upper_bound + 1, BLACK);

        //стираем график
        _tft.drawLine(_x, graph_lower_bound - _data->get_adc_del_buffer_element(y)/ 
            graph_upper_bound, _x + 1, graph_lower_bound - 
            _data->get_adc_del_buffer_element(y+1)/
            graph_upper_bound, BLACK);   

        //график
        _tft.drawLine(_x + 1, graph_lower_bound - _data->get_adc_buffer_element(y)/
            graph_upper_bound + 1, _x + 2,graph_lower_bound - 
            _data->get_adc_buffer_element(y + 1)/
            graph_upper_bound + 1, GREEN);         

        //график
        _tft.drawLine(_x, graph_lower_bound - _data->get_adc_buffer_element(y)/ 
            graph_upper_bound, _x + 1, graph_lower_bound - 
            _data->get_adc_buffer_element(y + 1)/
            graph_upper_bound, GREEN);           
            
    }
    //Write current value of ADC buffer to ADC_DEL buffer
    for(int i=0;i<500;i++) 
        _data->set_adc_del_buffer_element(i, _data->get_adc_buffer_element(i));
}

void Screen::draw_graph_pause(){
    //листаем вправо
    if(digitalRead(_data->get_scroll_right_pin())==HIGH){
        _tft.setRotation(0); 

        //стиране поля графика  
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);

        uint8_t scale = _data->get_scale();
        _x=28;

        for(int y=0;y<260;y++){
            _tft.setRotation(1);

            if(scale  <  7) _x++;
            if(scale ==  7) _x+=2;
            if(scale ==  8) _x+=3;
            if(scale ==  9) _x+=4;
            if(scale == 10) _x+=5;

            //график
            _tft.drawLine(_x+1,graph_lower_bound - _data->get_adc_buffer_element(y)/
                graph_upper_bound + 1, _x + 2, graph_lower_bound -
                _data->get_adc_buffer_element(y+1)/ graph_upper_bound+1, GREEN);

            //график
            _tft.drawLine(_x,graph_lower_bound - _data->get_adc_buffer_element(y)/
            graph_upper_bound, _x + 1, graph_lower_bound -
            _data->get_adc_buffer_element(y+1)/graph_upper_bound, GREEN);
        }
        draw_bottom_menu();
    }
    //листаем влево
    if(digitalRead(_data->get_scroll_left_pin()) == HIGH){ 
        _tft.setRotation(0); 

        //стиране поля графика
        _tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);  
        _x=28;
        uint8_t scale = _data->get_scale();

        for(int y=0;y<260;y++){
            _tft.setRotation(1); 
            if(scale  <  7) _x++;
            if(scale ==  7) _x+=2;
            if(scale ==  8) _x+=3; 
            if(scale ==  9) _x+=4;
            if(scale == 10) _x+=5;

            //график
            _tft.drawLine(_x + 1, graph_lower_bound - _data->get_adc_buffer_element(y)/
            graph_upper_bound + 1, _x + 2, graph_lower_bound -
            _data->get_adc_buffer_element(y+1) / graph_upper_bound + 1, GREEN);

            //график
            _tft.drawLine(_x, graph_lower_bound-_data->get_adc_buffer_element(y)/
            graph_upper_bound, _x + 1, graph_lower_bound-
            _data->get_adc_buffer_element(y+1) / graph_upper_bound, GREEN);
        }
        draw_bottom_menu();
    } 
}

void Screen::top_menu_state_handler(){
    if(_top_menu == 0)
        draw_reference_voltage_choose_menu();
    if(_top_menu == 1)
        draw_default_top_menu();
    if(_top_menu == 2)
        draw_pause_menu();
}

bool Screen::get_pause_flag(){
    return _pause_flag;
}