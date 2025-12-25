#ifndef SENSOR_DISTANCIA_H
    #define SENSOR_DISTANCIA_H
    #include <Arduino.h>
    #include <Wire.h>
    #include "Adafruit_VL6180X.h"


    #define m 5        //pendiente calibración
    #define b 0        //ordenada calibración

    class Sensor_Distancia { 
        private:
            TwoWire* wire;
            Adafruit_VL6180X vl = Adafruit_VL6180X();
            float lux;
            uint8_t rangeBuffer[10];
            uint8_t range;
            uint8_t status;
            
        public:
            Sensor_Distancia(TwoWire* wireExt);
            bool begin();           //como boolean
            bool ask_distance();   //como boolean
            String get_error();
            uint8_t get_range();
            float get_distance();

    };
#endif