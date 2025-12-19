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
    this->lux=this->vl.readLux(VL6180X_ALS_GAIN_5);
    this->range=this->vl.readRange();
    this->status=this->vl.readRangeStatus();
    if(this->status!=VL6180X_ERROR_NONE)
        return false;
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