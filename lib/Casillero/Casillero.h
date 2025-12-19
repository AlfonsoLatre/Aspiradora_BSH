#ifndef CASILLERO_h
  #define CASILLERO_h
  #include <Arduino.h>
  #include <vector>
  #define numFilas 64
  #define numCols 64

  
  class Casillero{
    private:
      uint8_t celdas[numFilas][numCols];
      float size; 
      void setCelda(int x, int y, uint8_t celda);
    
    public:
      Casillero(float L=0.3);
      void reset();
      void addCelda(int x, int y);
      float getM2Casa();
      float getM2Recorridos();
      uint16_t getNumCeldas();
      uint8_t maxPasadas();
      uint8_t getCell(int x, int y);
      uint8_t getNumRow();
      uint8_t getNumCol();
  };
#endif