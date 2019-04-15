// #include <TFT.h>    // Core graphics library
// #include <SPI.h>    // Serial peripheral interface library

// #define LCD_CS 10   // Chip Select goes to D10
// #define LCD_DC 9    //Dc or A0 pin on tft display 
// #define LCD_RESET 8 // Can alternately just connect to Arduino's reset pin
// #define external_power_pin A6

// //Piout user settings
// #define levo 7
// #define ok 5
// #define pravo 6
// #define vertikal 0          //размер вертикальной сетки(если "0" отключениа)
// #define default_text_size 1
// #define display_height 128   
// #define display_width 160
// #define tmp1 105
// #define tmp2 2.75

// //Define some colors
// #define BLACK   0x0000
// #define RED     0x001F
// #define BLUE    0xF800
// #define GREEN   0x07E0
// #define YELLOW  0x07FF
// #define MAGENTA 0xF81F
// #define CYAN    0xFFE0
// #define WHITE   0xFFFF

// //Initialize main TFT Screen object
// TFT tft(LCD_CS,LCD_DC,LCD_RESET);

// byte mass[501];//массив АЦП
//byte massDEL[501];
// byte menu=1;//переменная верхнего меню
// byte razv=0;//значение развертки
// float x=28;//счетчик оси Х
// int Vmax=0;// максимальное напряжение 
// float Vakb=0;//напряженеие АКБ
// unsigned long t=0;//переменная для ращета развертки
// unsigned long pause=0;//переменная для замены Delay
// int t_del=0; //значение цены деления развертки в микросекундах
// bool opornoe=1; //флаг опорного напряжения
// bool paus=0; //флаг режима паузы
// bool clen_p=0;//флаг чистки паузы
// void setup(){  
//     Vakb=analogRead(external_power_pin)*5.3/1024;//проверка напряжения на АКБ
//     if(opornoe==0){ADMUX = 0b11100101;}//выбор внутреннего опорного 1,1В
//     if(opornoe==1){ADMUX = 0b01100101;}//Выбор внешнего опорного
//     tft.begin();
//     delay(50);
//     tft.fillScreen(BLACK);
//     tft.setTextSize(default_text_size);
//     delay(500);
// }
// void Zamer(){//заполнение буфера АЦП
//   if (razv>=6){ADCSRA = 0b11100010;}//delitel 4
//   if (razv==5){ADCSRA = 0b11100011;}//delitel 8
//   if (razv==4){ADCSRA = 0b11100100;}//delitel 16
//   if (razv==3){ADCSRA = 0b11100101;}//delitel 32
//   if (razv==2){ADCSRA = 0b11100110;}//delitel 64
//   if (razv<2){ADCSRA = 0b11100111;}//delitel 128
//   if (razv==0){
//       t=micros(); 
//       for(int i=0;i<500;i++){ 
//           while ((ADCSRA & 0x10)==0);
//           ADCSRA|=0x10;
//           delayMicroseconds(100);
//           mass[i]=ADCH;
//       }
//       t= micros()-t;
//       t_del=t*18/501;
//   }
//   if (razv>0){
//       t=micros();
//       for(int i=0;i<500;i++){ 
//           while ((ADCSRA & 0x10)==0);
//           ADCSRA|=0x10;
//           mass[i]=ADCH;
//       }
//       t= micros()-t;
//       t_del=t*18/501;
//   }
  
// }
// void MenuT(){//перерисовка нижнего меню    
//     tft.setRotation(0);  
//     tft.fillRect(0, 10, 10,display_width, BLACK);//стирание поля меню низ
//     tft.setRotation(1);
//     tft.setCursor(0,display_height - 8);
//     tft.print("t=");
//     if(razv<7){tft.print(t_del);}
//     if(razv==7){tft.print(t_del/2);}
//     if(razv==8){tft.print(t_del/3);}
//     if(razv==9){tft.print(t_del/4);}
//     if(razv==10){tft.print(t_del/5);}
//     tft.print("us  ");
//     tft.print("Vmax=");
//     if(opornoe==0){tft.print(Vmax*1.1/255);}
//     if(opornoe==1){tft.print(Vmax*5.3/255);}
//     tft.print(" ");
//     tft.print("B ");
//     tft.print(Vakb);  
// }

// void loop() { 
//     Zamer(); 
//     //отрисовка и перебор меню###############################
//     if(menu==0){
//         tft.setRotation(1);
//         tft.setTextColor(RED);
//         tft.fillRect(0, 0, 45, 10, CYAN);
//         tft.setCursor(0,2);
//         if(opornoe==0) tft.print("op-1.1V ");
//         if(opornoe==1) tft.print("op-5.3V ");

//         if(digitalRead(pravo)==HIGH){
//             tft.setRotation(0); 
//             tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//             tft.fillRect(16, 0, 97, 24, BLACK);//стирание напряжения
//             opornoe=!opornoe;
//         }

//         if(digitalRead(levo)==HIGH){
//             tft.setRotation(0);  
//             tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//             tft.fillRect(16, 0, 97, 24, BLACK);//стирание напряжения
//             opornoe=!opornoe;
//         }

//         tft.setRotation(1);  
//         tft.print("  Razv*");
//         tft.print(razv);
//         tft.print("  PauseOFF");
//     }
//     if(menu==1){
//         tft.setRotation(1);
//         tft.setTextColor(RED);
//         tft.fillRect(display_width - 54, 0,50, 10, CYAN);
//         tft.setCursor(0,2);
//         if(opornoe==0) tft.print("op-1.1V ");
//         if(opornoe==1) tft.print("op-5.3V ");

//         if(digitalRead(pravo)==HIGH){//доработать скорость
//             tft.setRotation(0); 
//             tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//             tft.fillRect(display_height - 12,display_width - 70, 12 , 10, BLACK);
//             razv++;
//             if(razv==11) razv=10; 
//         }

//         if(digitalRead(levo)==HIGH){//доработать скорость
//             tft.setRotation(0); 
//             tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//             tft.fillRect(display_height - 12,display_width - 70, 12 , 10, BLACK);
//             razv--;
//             if(razv==255) razv=0;
//         }

//         tft.setRotation(1); 
//         tft.print("  Razv*");
//         tft.print(razv);
//         tft.print("  PauseOFF");
//     }
//     if(menu==2){//пауза
//         paus=1;
//         if(clen_p==0){
//             clen_p=1;  
//             tft.setRotation(1);
//             tft.setTextColor(RED);
//             tft.fillRect(display_width - 54, 0,50, 10, CYAN);
//             tft.setCursor(0,2);
//             if(opornoe==0) tft.print("op-1.1V ");
//             if(opornoe==1) tft.print("op-5.3V ");
//             tft.print("  Razv*");
//             tft.print(razv);
//             if(paus==0) tft.print("  PauseOFF");
//             if(paus==1) tft.print("  PauseON");
//         }
//     }
//     if(digitalRead(ok)==HIGH){
//         menu++;
//         tft.setRotation(0); 
//         tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//         if(menu==3) { menu=0; paus=0; clen_p=0;} 
//         tft.setRotation(1); 
//         tft.fillRect(0, 0, display_width, 12, BLACK); //Стирание верхнего меню   
//     }
//     //отрисовка и перебор меню###############################

//     //Выбор опорного#########################################  
//     if(opornoe==0){ADMUX = 0b11100101;}//выбор внутреннего опорного 1,1В
//     if(opornoe==1){ADMUX = 0b01100101;}//Выбор внешнего опорного
//     delay(5);
//     //Выбор опорного#########################################  

//     tft.setRotation(0); 

//     //ось напряжения##########################################
//     tft.drawFastHLine(20,28,95,BLUE);
//     tft.drawFastHLine(20,27,95,BLUE);

//     tft.drawFastVLine(110,23,5, BLUE);
//     tft.drawFastVLine(92,23,5, BLUE);
//     tft.drawFastVLine(74,23,5,BLUE);
//     tft.drawFastVLine(56,23,5,BLUE);
//     tft.drawFastVLine(38,23,5,BLUE);
//     //tft.drawFastVLine(20,23,5,BLUE);
//     //ось напряжения##########################################

//     //ось времени#############################################
//     tft.drawFastVLine(19,27,141,RED);
//     tft.drawFastVLine(20,27,140,RED);

//     tft.drawFastHLine(15,45,5, RED);
//     tft.drawFastHLine(15,63,5, RED);
//     tft.drawFastHLine(15,81,5, RED);
//     tft.drawFastHLine(15,99,5, RED);
//     tft.drawFastHLine(15,117,5,RED);
//     tft.drawFastHLine(15,135,5,RED);
//     tft.drawFastHLine(15,153,5,RED);
//     //ось времени#############################################

//     //сетка вертикальная #####################################
//     tft.drawFastHLine(75,310,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,310,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,310,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,310,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,285,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,285,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,285,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,285,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,260,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,260,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,260,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,260,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,235,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,235,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,235,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,235,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,210,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,210,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,210,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,210,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,185,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,185,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,185,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,185,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,160,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,160,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,160,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,160,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,135,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,135,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,135,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,135,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,110,5*vertikal, YELLOW);
//     tft.drawFastHLine(110,110,5*vertikal,YELLOW);
//     tft.drawFastHLine(145,110,5*vertikal,YELLOW);
//     tft.drawFastHLine(180,110,5*vertikal,YELLOW);
//     tft.drawFastHLine(75,85, 5*vertikal, YELLOW);
//     tft.drawFastHLine(110,85,5*vertikal, YELLOW);
//     tft.drawFastHLine(145,85,5*vertikal, YELLOW);
//     tft.drawFastHLine(180,85,5*vertikal, YELLOW);
//     //сетка вертикальная #####################################

//     tft.setRotation(1);  
//     tft.setTextColor(RED);

//     //шкала напряжения########################################
//     if(opornoe==0){
//         tft.setCursor(0, 105);tft.println("0.00"); 
//         tft.setCursor(0, 87); tft.println("0.22"); 
//         tft.setCursor(0, 69); tft.println("0.44"); 
//         tft.setCursor(0, 51); tft.println("0.66"); 
//         tft.setCursor(0, 33); tft.println("0.88"); 
//         tft.setCursor(0, 15); tft.println("1.1V");   
//     }
//     if(opornoe==1){
//         tft.setCursor(0, 105);tft.println("0.00"); 
//         tft.setCursor(0, 87); tft.println("1.00"); 
//         tft.setCursor(0, 69); tft.println("2.00"); 
//         tft.setCursor(0, 51); tft.println("3.00"); 
//         tft.setCursor(0, 33); tft.println("4.00");
//         tft.setCursor(0, 15); tft.println("5.0V"); 
//     }
//     //шкала напряжения########################################

//     if(paus==0){MenuT();}

//     //максимальное значение сигнала##########################
//     Vmax=0; 
//     for(int i=0;i<500;i++){  
//             if(Vmax<mass[i]){ Vmax=mass[i];} 
//     }
//     //максимальное значение сигнала##########################

//     //отрисовка графика
//     if(paus==0){
//         tft.setRotation(0); 
//         x=28;
//         for(int y=0;y<260;y++){
//             tft.setRotation(1); 
//             if(razv <  7)  x++;
//             if(razv == 7)  x+=2;
//             if(razv == 8)  x+=3;
//             if(razv == 9)  x+=4;
//             if(razv == 10) x+=5;

//             tft.drawLine(x + 1, tmp1 - massDEL[y]/tmp2 + 1, 
//                 x+2,tmp1-massDEL[y+1]/tmp2+1, BLACK);       //стираем график

//             tft.drawLine(x, tmp1 - massDEL[y] / tmp2, 
//                 x+1,tmp1-massDEL[y+1]/tmp2, BLACK);         //стираем график

//             tft.drawLine(x + 1, tmp1 - mass[y]/tmp2 + 1, 
//                 x+2,tmp1-mass[y+1]/tmp2+1, GREEN);          //график

//             tft.drawLine(x, tmp1 - mass[y] / tmp2,
//                 x+1,tmp1-mass[y+1]/tmp2, GREEN);            //график
                
//         }
//         for(int i=0;i<500;i++) massDEL[i]=mass[i];
//     }
//     if(paus==1){                                        //режим паузы
//         if(digitalRead(pravo)==HIGH){                   //листаем вправо
//             tft.setRotation(0); 
//             tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//             x=28;
//             for(int y=0;y<260;y++){
//                 tft.setRotation(1);
//                 if(razv  <  7) x++;
//                 if(razv ==  7) x+=2;
//                 if(razv ==  8) x+=3;
//                 if(razv ==  9) x+=4;
//                 if(razv == 10) x+=5;

//                 tft.drawLine(x+1,tmp1-mass[y]/tmp2+1, 
//                     x+2,tmp1-mass[y+1]/tmp2+1, GREEN);      //график

//                 tft.drawLine(x,tmp1-mass[y]/tmp2, 
//                     x+1,tmp1-mass[y+1]/tmp2, GREEN);        //график
//             }
//             MenuT();
//         }
//         if(digitalRead(levo)==HIGH){                    //листаем влево
//             tft.setRotation(0); 
//             tft.fillRect(21,29, display_height - 32 , display_width - 29, BLACK);//стиране поля графика  
//             x=28;
//             for(int y=0;y<260;y++){
//                 tft.setRotation(1); 
//                 if(razv  <  7) x++;
//                 if(razv ==  7) x=x+2;
//                 if(razv ==  8) x=x+3; 
//                 if(razv ==  9) x=x+4;
//                 if(razv == 10) x=x+5;

//                 tft.drawLine(x+1,tmp1-mass[y]/tmp2+1, 
//                     x+2,tmp1-mass[y+1]/tmp2+1, GREEN);      //график

//                 tft.drawLine(x,tmp1-mass[y]/tmp2, 
//                     x+1,tmp1-mass[y+1]/tmp2, GREEN);        //график
//             }
//             MenuT();
//         }     
//     }
//     //отрисовка графика
// }