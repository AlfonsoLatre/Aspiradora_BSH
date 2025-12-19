#include "Pantalla_TFT.h"

//CONSTRUCTOR
Pantalla_TFT::Pantalla_TFT(int CS_PIN,int DC_PIN,int RST_PIN,int MOSI_PIN,int SCLK_PIN){
    this->tft=new Adafruit_ST7789(CS_PIN,DC_PIN,MOSI_PIN,SCLK_PIN,RST_PIN);  
}
void Pantalla_TFT::begin(int width, int height, int color){
    this->tft->init(width,height);
    this->tft->setRotation(3);
    this->color=color;
    this->tft->fillScreen(ST77XX_RED); // Pinta ROJO al inicio para ver si vive
    delay(1000);
    this->tft->fillScreen(color);
}
void Pantalla_TFT::setCursor(int x, int y){
    this->tft->setCursor(x,y);
}                
void Pantalla_TFT::setTextSize(int size){
    this->tft->setTextSize(size);
}
void Pantalla_TFT::setTextColor(int color){
    this->tft->setTextColor(color);
}
void Pantalla_TFT::print(String txt){
    this->tft->print(txt);
}
void Pantalla_TFT::println(String txt){
    this->tft->println(txt);
}
void Pantalla_TFT::clearScreen(){
    this->tft->fillScreen(this->color);
}