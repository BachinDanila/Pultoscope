#include "Pultoscope.h"

Data::Data(uint8_t grid_size, uint8_t left, uint8_t right, uint8_t ok){
    _scroll_left_pin = left;
    _scroll_right_pin = right;
    _ok_pin = ok;
    _grid_size = grid_size;
}

Screen::Screen(uint8_t cs, uint8_t dc, 
    uint8_t reset, Data data): 
    _tft(TFT(cs, dc, reset)),_data(data){

    _cs = cs;
    _dc = dc;
    _reset = reset;
    _tft = TFT(_cs, _dc, _reset);
    _data = data;
}

void Screen::begin(){
    _tft.begin();
    delay(50);
    _tft.fillScreen(BLACK);
    _tft.textSize(2);
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
    // _tft.setRotation(0);  
    // _tft.fillRect(0, 20, 20,320, BLACK);    //Erase the bottom menu
    // _tft.setRotation(1);
    // _tft.setCursor(0,220);
    // _tft.print("t=");
    // switch ()
    // {
    //     case /* constant-expression */:
    //         /* code */
    //         break;
    
    //     default:
    //         break;
    // }
    // if(razv<7){_tft.print(t_del);}
    // if(razv==7){_tft.print(t_del/2);}
    // if(razv==8){_tft.print(t_del/3);}
    // if(razv==9){_tft.print(t_del/4);}
    // if(razv==10){_tft.print(t_del/5);}
    // _tft.print("us  ");
    // _tft.print("Vmax=");
    // if(opornoe==0){_tft.print(Vmax*1.1/255);}
    // if(opornoe==1){_tft.print(Vmax*5.3/255);}
    // _tft.print(" ");
    // _tft.print("B");
    // _tft.print(Vakb);  
}