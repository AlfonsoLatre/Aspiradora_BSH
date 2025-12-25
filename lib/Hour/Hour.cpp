#include "hour.h"

//CONSTRUCTOR
Hour::Hour(){
    this->hours=0;
    this->minutes=0;
    this->seconds=0;
};
Hour::Hour(int hours,int minutes, int seconds){
    this->hours=hours;
    this->minutes=minutes;
    this->seconds=seconds;
};
void Hour::copy(Hour otherHour){
    this->hours=otherHour.hours;
    this->minutes=otherHour.minutes;
    this->seconds=otherHour.seconds;
};
//SET 
void Hour::setHours(int hours){
    this->hours=hours%24;
};
void Hour::setMinutes(int minutes){
    this->minutes=minutes%60;
};
void Hour::setSeconds(int seconds){
    this->seconds=seconds%60;
};

//ADD
void Hour::addHours(int hours){
    int aux=this->hours+hours;
    if(aux>=24){
        this->hours=aux%24;
    }else{
        this->hours=aux;
    }
};
void Hour::addMinutes(int minutes){
    int aux=this->minutes+minutes;
    if(aux>=60){
        this->minutes=aux%60;
        addHours(aux/60);
    }else{
        this->minutes=aux;
    }
};
void Hour::addSeconds(int seconds){
    int aux=this->seconds+seconds;
    if(aux>=60){
        this->seconds=aux%60;
        addMinutes(aux/60);
    }else{
        this->seconds=aux;
    }
};
//GET
uint8_t Hour::getHours(){
    return this->hours;
};
uint8_t Hour::getMinutes(){
    return this->minutes;
};
uint8_t Hour::getSeconds(){
    return this->seconds;
};
String Hour::ToString(){
    String delimiter=":";
    return String(this->hours)+delimiter+String(this->minutes)+delimiter+String(this->seconds);
};
    
//COMPARE HOURS
bool Hour::isBefore(Hour otherHour){
    return this->hours<otherHour.hours || 
        (this->hours==otherHour.hours && (this->minutes<otherHour.minutes ||
        (this->minutes==otherHour.minutes && this->seconds<otherHour.seconds)));
};
bool Hour::isAfter(Hour otherHour){
    return this->hours>otherHour.hours || 
        (this->hours==otherHour.hours && (this->minutes>otherHour.minutes ||
        (this->minutes==otherHour.minutes && this->seconds>otherHour.seconds)));
};
bool Hour::isBetween(Hour firstHour, Hour secondHour){
    bool value=false;
    if(firstHour.isAfter(secondHour)){
        value=this->isAfter(firstHour) || this->isBefore(secondHour);
    }else{
        value=this->isAfter(firstHour) && this->isBefore(secondHour);
    }
    return value;
};