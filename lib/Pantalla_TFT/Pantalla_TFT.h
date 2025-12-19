#ifndef PANTALLA_TFT_H
    #define PANTALLA_TFT_H
    #include <Arduino.h>
    #include <SPI.h>
    #include <Adafruit_GFX.h>
    #include "Adafruit_ST7789.h"


    class Pantalla_TFT { 
        private:
            Adafruit_ST7789* tft;
            int color;
        public:
            Pantalla_TFT(int CS_PIN,int DC_PIN,int RST_PIN,int MOSI_PIN,int SCLK_PIN);
            void begin(int width=240, int height=320, int color=ST77XX_BLACK);           
            void setCursor(int x, int y);   
            void setTextSize(int size);
            void setTextColor(int color);
            void print(String txt);
            void println(String txt);
            void clearScreen();
    };
#endif