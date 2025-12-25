#include "Sensor_Distancia.h"

//CONSTRUCTOR
Sensor_Distancia::Sensor_Distancia(TwoWire* wireExt){
    this->wire=wireExt;
}

//BEGIN
bool Sensor_Distancia::begin(){
    if(!this->vl.begin(this->wire))
        return false;
    return true;
}
//ASK
bool Sensor_Distancia::ask_distance(){
    bool cant=0;
    uint8_t aux2Sort=0;
    while(cant<10){
        this->lux=this->vl.readLux(VL6180X_ALS_GAIN_5);
        this->rangeBuffer[cant]=this->vl.readRange();
        this->status=this->vl.readRangeStatus();
        if(this->status!=VL6180X_ERROR_NONE)
            return false;
        cant++;
        delay(10);      //SE PUEDE PONER EL DELAY DEBIDO A QUE NO SE ESTÁ EJECUTANDO A LA VEZ EL MPU (NO NECESITA LEER CADA POCOS MS PARA DAR BIEN LOS DATOS AL NO DARLOS)
    }
    for(uint8_t i=0; i<10-1; i++){
        for(uint8_t j=0; j<10-i-1; j++){
            if(this->rangeBuffer[j]>this->rangeBuffer[j+1]){
                aux2Sort=this->rangeBuffer[j];
                this->rangeBuffer[j]=this->rangeBuffer[j+1];
                this->rangeBuffer[j+1]=aux2Sort;
            }
        }
    }
    this->range=this->rangeBuffer[4];   //LA MEDIANA
    return true;    
}

//GET
uint8_t Sensor_Distancia::get_range(){
    return this->range;
}

float Sensor_Distancia::get_distance(){
    return m*this->range+b;
}

String Sensor_Distancia::get_error(){
    String error="None error";
    if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
        error="System error";
    }
    else if (status == VL6180X_ERROR_ECEFAIL) {
        error="ECE failure";
    }
    else if (status == VL6180X_ERROR_NOCONVERGE) {
        error="No convergence";
    }
    else if (status == VL6180X_ERROR_RANGEIGNORE) {
        error="Ignoring range";
    }
    else if (status == VL6180X_ERROR_SNR) {
        error="Signal/Noise error";
    }
    else if (status == VL6180X_ERROR_RAWUFLOW) {
        error="Raw reading underflow";
    }
    else if (status == VL6180X_ERROR_RAWOFLOW) {
        error="Raw reading overflow";
    }
    else if (status == VL6180X_ERROR_RANGEUFLOW) {
        error="Range reading underflow";
    }
    else if (status == VL6180X_ERROR_RANGEOFLOW) {
        error="Range reading overflow";
    }
    return error;
}