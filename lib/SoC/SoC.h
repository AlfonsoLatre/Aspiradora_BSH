#ifndef SOC_H
  #define SOC_H
  #include <Arduino.h>
  
  #define maxElements 64    // IT MUST BE LOWER THAN 255
  
  enum class SoCLevel : uint8_t {
    LOW_LEVEL,
    QUARTER_LEVEL,
    MIDDLE_LEVEL,
    THREE_QUARTER_LEVEL,
    HIGH_LEVEL,
  };
  enum class TypeElement : uint8_t {
    LOAD,
    SOURCE,
  };

  class SoC {
    private:
      uint16_t capacity;
      float actualCapacity;
      float level; // In %
      uint8_t numElements;
      uint8_t stateElements[maxElements];
      int16_t powerCon[maxElements];
      int16_t findFirstFree();

    public:
      SoC(uint16_t capacity, SoCLevel initLevel=SoCLevel::LOW_LEVEL);
      int8_t addElement(int16_t powerCon, TypeElement type=TypeElement::LOAD);
      void deleteElement(uint8_t charge);
      void deleteAllElements();
      void turnOff(uint8_t charge);
      void turnOn(uint8_t charge);
      void calculateSoC(uint8_t dt);  //dt en s
      float getLevel();
      SoCLevel getLevelAsEnum();
      String ToString();
  };
#endif