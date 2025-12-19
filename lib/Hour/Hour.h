#ifndef HOUR_H
  #define HOUR_H
  #include <Arduino.h>

  class Hour {
    private:
       uint8_t hours;
       uint8_t minutes;
       uint8_t seconds;

    public:
      Hour();
      Hour(int hours, int minutes, int seconds);
      void copy(Hour otherHour);
      void setHours(int hours);
      void setMinutes(int minutes);
      void setSeconds(int seconds);
      void addHours(int hours);
      void addMinutes(int minutes);
      void addSeconds(int seconds);
      uint8_t getHours();
      uint8_t getMinutes();
      uint8_t getSeconds();
      bool isBefore(Hour otherHour);
      bool isAfter(Hour otherHour);
      bool isBetween(Hour firstHour, Hour secondHour);
      String ToString();
  };
#endif