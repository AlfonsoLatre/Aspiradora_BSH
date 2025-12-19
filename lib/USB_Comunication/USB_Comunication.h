#ifndef USB_COMUNICATION_H
  #define USB_COMUNICATION_H
  #include <Arduino.h>
  #include "Casillero.h"
  #include "Hour.h"
  
  #define HEADER 0xAA
  #define FOOTER 0x55 
  #define MAX_LENGTH_MESSAGE 255

  enum class ReadState : uint8_t {
   IDLE_STATE,
   HEADER_STATE,
   LENGTH_STATE,
   DATA_STATE,
   CHECKSUM_STATE,
   FOOTER_STATE,
  };

  #define ID_INSTRUCTION 0x3A
  #define US_INSTRUCTION 0x3B
  #define HOUR_INSTRUCTION 0X3C
  #define MATRIX_INSTRUCTION 0x3D

  class USB_Comunication{
    private:
      uint8_t bufferRead[MAX_LENGTH_MESSAGE];
      uint8_t lengthRead;
      HardwareSerial* serialPort;
      String session_id;
      void write(uint8_t* data, uint16_t length);

    public:
      USB_Comunication(HardwareSerial* serialPort);
      void begin(uint32_t baudrate=115200);
      bool read();
      uint8_t getMessageLength();
      uint8_t* getMessage();
      bool createSession();
      bool sendUS(float initUS, float endUS);
      bool sendHours(Hour initHour, Hour endHour);
      bool sendMatrix(Casillero* matrix);
      
  };
#endif