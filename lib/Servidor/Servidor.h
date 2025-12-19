#ifndef SERVIDOR_H
  #define SERVIDOR_H
  #include <Arduino.h>
  #include <WiFi.h>
  #include <HTTPClient.h>
  #include "Casillero.h"
  #include "Hour.h"

  #define ID_INSTRUCTION 0x3A
  #define US_INSTRUCTION 0x3B
  #define HOUR_INSTRUCTION 0X3C
  #define MATRIX_INSTRUCTION 0x3D

  class Servidor {
    private:
       const char* ssid;
       const char* password;
       const char* serverURL;
       String session_id;

    public:
        Servidor(const char* ssid,const char* password,const char* serverURL);
        bool connect();
        bool disconnect();
        bool createSession();
        int8_t sendUS(float init, float end);
        int8_t sendHours(Hour init, Hour end);
        int8_t sendMatrix(Casillero* matrix);
  };
#endif