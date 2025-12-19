#include "Casillero.h"

//CONSTRUCTOR
Casillero::Casillero(float L){
  this->size=L*L;
  for(char i=0; i<numFilas; i++){
    for(char j=0; j<numCols; j++){
      this->celdas[i][j]=0;
    }
  }
}

//FUNCIONES PRIVADAS
void Casillero::setCelda(int x, int y, uint8_t cant){
  if((x>=0 && x<numFilas) && (y>=0 && y<numCols)){
    this->celdas[x][y]=cant;
  }
}

//FUNCIONES PUBLICAS
void Casillero::reset(){
  for(char i=0; i<numFilas; i++){
    for(char j=0; j<numCols; j++){
      this->celdas[i][j]=0;
    }
  }
}

void Casillero::addCelda(int x, int y){
  if((x>=0 && x<numFilas) && (y>=0 && y<numCols)){
    char celda=this->getCell(x, y);
    if(celda<255){
      this->setCelda(x, y, celda+1);
    }
  }
}

uint8_t Casillero::getCell(int x, int y){
  if((x>=0 && x<numFilas) && (y>=0 && y<numCols)){
    return this->celdas[x][y];
  }
  return 0;
}

float Casillero::getM2Casa(){
  return this->getNumCeldas()*this->size;
}


float Casillero::getM2Recorridos(){
    uint32_t cont=0;
  for(char i=0; i<numFilas; i++){
    for(char j=0; j<numCols; j++){
      cont=cont+this->getCell(i, j);
    }
  }
  return cont*this->size;
}

uint16_t Casillero::getNumCeldas(){
  uint32_t cont=0;
  for(char i=0; i<numFilas; i++){
    for(char j=0; j<numCols; j++){
      if(this->getCell(i, j)>0){
        cont++;
      }
    }
  }
  return cont;
}

uint8_t Casillero::maxPasadas(){
  int max=0;
  for(char i=0; i<numFilas; i++){
    for(char j=0; j<numCols; j++){
      if(this->getCell(i, j)>max){
        max=this->getCell(i,j);
      }
    }
  }
  return max;
}

uint8_t Casillero::getNumCol(){
  return numCols;
}

uint8_t Casillero::getNumRow(){
  return numFilas;
}