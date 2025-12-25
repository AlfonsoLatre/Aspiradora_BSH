#include "SoC.h"

SoC::SoC(uint16_t capacity, SoCLevel initLevel){
    this->capacity=capacity;
    this->numElements=0;
    switch (initLevel){
        case SoCLevel::HIGH_LEVEL:
            this->level=1;
            break;
        case SoCLevel::THREE_QUARTER_LEVEL:
            this->level=0.75;
            break;
        case SoCLevel::MIDDLE_LEVEL:
            this->level=0.50;
            break;
        case SoCLevel::QUARTER_LEVEL:
            this->level=0.25;
            break;
        default:
            this->level=0;
            break;
    }
    this->actualCapacity=this->level*this->capacity;
}
int8_t SoC::addElement(int16_t powerCon, TypeElement type){
    int8_t pos=this->findFirstFree();
    int16_t LPowerCon=powerCon;
    if(type==TypeElement::LOAD)
        LPowerCon=-LPowerCon;
    if(pos!=-1){
        this->powerCon[pos]=LPowerCon;
        this->numElements++;
    }
    return pos;
    
}
void SoC::deleteElement(uint8_t charge){
    if(charge<maxElements){
        this->powerCon[charge]=0;
        this->stateElements[charge]=0;
        this->numElements--;
    }
}
void SoC::deleteAllElements(){
    for(uint8_t i=0;i<maxElements;i++){
        this->stateElements[i]=0;
        this->powerCon[i]=0;
        this->numElements=0;
    }
}
void SoC::turnOff(uint8_t charge){
    if(charge<maxElements)
        this->stateElements[charge]=0;
}
void SoC::turnOn(uint8_t charge){
    if(charge<maxElements)
        this->stateElements[charge]=1;
}
void SoC::calculateSoC(uint8_t dt){
    float cost=0;
    float LActualCapacity=0;
    for(int i=0;i<maxElements;i++)
        cost+=this->stateElements[i]*this->powerCon[i];
    cost=cost*dt/3600;
    LActualCapacity=this->actualCapacity+cost;
    if(LActualCapacity<0)
        LActualCapacity=0;
    else if(LActualCapacity>this->capacity){
        LActualCapacity=this->capacity;
    }
    this->actualCapacity=LActualCapacity;
    this->level=LActualCapacity/this->capacity;
}

float SoC::getLevel(){
    return this->level;
}

SoCLevel SoC::getLevelAsEnum(){
    SoCLevel levelLabel=SoCLevel::LOW_LEVEL;
    if(this->level==0)
        levelLabel=SoCLevel::LOW_LEVEL;
    else if(this->level<=0.25)
        levelLabel=SoCLevel::QUARTER_LEVEL;
    else if(this->level<=0.75)
        levelLabel=SoCLevel::MIDDLE_LEVEL;
    else if(this->level<1)
        levelLabel=SoCLevel::THREE_QUARTER_LEVEL;
    else
        levelLabel=SoCLevel::HIGH_LEVEL;
    return levelLabel;
}

String SoC::ToString(){
    return String(this->level*100)+"%";
}

//PRIVATE FUNCTIONS
int16_t SoC::findFirstFree(){
    int16_t pos=0;
    if(this->numElements<maxElements){
        while(this->powerCon[pos]!=0)
            pos++;
    }else{
        pos=-1;
    }
    return pos;
}