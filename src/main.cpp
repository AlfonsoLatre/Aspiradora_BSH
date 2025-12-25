#include <Arduino.h>
// OWN CLASSES
#include <Casillero.h>
#include <Hour.h>
#include <Sensor_Inercial.h>
#include <SoC.h>
#include <USB_Comunication.h>
#include <Pantalla_TFT.h>

// DEFINITIONS
#define I2C_VELOCITY 100000
#define SPI_VELOCITY 10e6
#define SERIAL_VELOCITY 115200
#define PI 3.14159265359
#define WHEEL_CIRC 2*PI*0.1 //m WITH R=0.1m
#define MAX_IN_STAND_BY 1  //min
#define MIN_SPACE_BETWEEN_PRESS 1 //s
#define MIN_SPACE_BETWEEN_SHOW 3 //s
#define L_SQUARE 0.3  //m

  //CONSUMPTIONS---LOS DATOS ESTAN INVENTADOS
#define INIT_STATE SoCLevel::HIGH_LEVEL
#define LOAD TypeElement::LOAD
#define SOURCE TypeElement::SOURCE
#define CAPACITY 3500
#define CON_uC 50
#define CON_MPU 100

// PIN DEFINITIONS
  //MPU
#define SCL_MPU_PIN 5
#define SDA_MPU_PIN 4
  //OTHERS
#define BUTTON_PIN 15
#define PUMP_PIN 0
#define EN_BOOST_PIN 37
#define IS_CHARGING_PIN 42
#define RED_LED_PIN 2
#define GREEN_LED_PIN 1
  //TFT
#define CLK_TFT_PIN 12
#define MOSI_TFT_PIN 11
#define RST_TFT_PIN 10
#define DC_TFT_PIN 9
#define CS_TFT_PIN 46
/* TFT COLORS
#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00
*/

// ENUM DEFINITIONS
enum class State : uint8_t {
    ON,
    OFF,
    STANDBY,
    SEND_INFO,
};

// VARIABLE AND OBJETCT DECLARATIONS
  //INTERRUPT VARIABLES
volatile bool flagButton=false;
volatile bool flagClock=false;
hw_timer_t* timer=nullptr;
  //OBJECTS
TwoWire wireMPU=TwoWire(1);
Casillero cells=Casillero(L_SQUARE);
Hour actualHour=Hour();
Hour initHour=Hour();
Hour endHour=Hour();
Hour maxStandByHour=Hour();
//Sensor_Inercial MPU=Sensor_Inercial(&wireMPU);
SoC stateBattery=SoC(CAPACITY,INIT_STATE);
//Pantalla_TFT tft=Pantalla_TFT(CS_TFT_PIN,DC_TFT_PIN,RST_TFT_PIN,MOSI_TFT_PIN,CLK_TFT_PIN);
USB_Comunication ser=USB_Comunication(&Serial);
  //OTHER VARIABLES
int16_t SoC_uC,SoC_MPU;
State actualState=State::OFF;
SoCLevel actualSoCLevel=INIT_STATE;
float angle;
int8_t direction=1;
uint8_t initCell[2];
uint8_t endCell[2];
float initUS, endUS;
float dx,dy;
uint8_t contSec;
uint32_t lastPressTime;
int pruebaX, pruebaY;   //BORRAR EN EL FINAL
bool primera=true;
uint8_t error;

// INTERRUPT DECLARATIONS
void IRAM_ATTR buttonInterrupt();
void IRAM_ATTR oneSec();

// FUNCTION DECLARATIOS
uint8_t sendInfoUSB();

void setup() {
  ser.begin(SERIAL_VELOCITY);
  //OTHER PIN
  pinMode(PUMP_PIN,OUTPUT);
  pinMode(EN_BOOST_PIN,OUTPUT);
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(RED_LED_PIN,OUTPUT);

  //COMUNICATIONS  
  wireMPU.begin(SDA_MPU_PIN,SCL_MPU_PIN,I2C_VELOCITY);
  //INIT MPU
  //MPU.begin();
  //MPU.calibrate();
  //INIT SoC
  SoC_uC=stateBattery.addElement(CON_uC,LOAD);
  SoC_MPU=stateBattery.addElement(CON_MPU,LOAD);
  //INIT TFT
  //tft.begin();
  //tft.setTextColor(ST77XX_WHITE);
  //tft.setTextSize(5);
  //tft.setCursor(0,0);
  //tft.println("Hola");

  //INTERRUPTS PIN
  pinMode(BUTTON_PIN,INPUT);
  attachInterrupt(BUTTON_PIN,buttonInterrupt,CHANGE);
  lastPressTime=millis();
  //TIMER INTERRUPT
  timer=timerBegin(0,80,true);
  timerAttachInterrupt(timer,&oneSec,true);
  timerAlarmWrite(timer,1000000,true);
  timerAlarmEnable(timer);
  contSec=0;

}

void loop() {
  /**/
  //INTERRUPT TREATMENT 
  if(flagClock==true){
    actualHour.addSeconds(1);
    stateBattery.calculateSoC(1);
    flagClock=false;
    contSec=0;
  }
  if(flagButton==true){
    if(millis()-lastPressTime>MIN_SPACE_BETWEEN_PRESS*1000){
      lastPressTime=millis();  
      if(actualState==State::ON){
        //detachInterrupt(HALL1_PIN);
        //detachInterrupt(HALL2_PIN);
        digitalWrite(PUMP_PIN,LOW);
        maxStandByHour.copy(actualHour);
        maxStandByHour.addMinutes(MAX_IN_STAND_BY);
        actualState=State::STANDBY;
      }else{
        //attachInterrupt(HALL1_PIN,hall1Interrupt,RISING);
        //attachInterrupt(HALL2_PIN,hall2Interrupt,RISING);
        digitalWrite(PUMP_PIN,HIGH);
        
        //while(VL.ask_distance()==false){
        //  delay(100);
        //}
        //initUS=VL.get_distance();
        
       initUS=13.5;
        actualState=State::ON;
      }  
    }
    flagButton=false;
  }
  /* PRUEBA MPU. FALTA CALIBRAR
  MPU.update();
  delay(10);
  if(millis()-lastPressTime>MIN_SPACE_BETWEEN_SHOW*1000){
    Serial.println(MPU.toString());
    pruebaX=random(5);
    pruebaY=random(5);
    Serial.println("Celda original en"+String(pruebaX)+","+String(pruebaY)+":");
    Serial.println(cells.getCell(pruebaX,pruebaY));
    cells.addCelda(pruebaX,pruebaY);
    Serial.println("Celda nueva en"+String(pruebaX)+","+String(pruebaY)+":");
    Serial.println(cells.getCell(pruebaX,pruebaY));
    lastPressTime=millis();
  }
  */
  /* PRUEBA VL. FALTA CALIBRAR
  //while(VL.ask_distance()==false){
  //  delay(100);
  //}
  //initUS=VL.get_range();
  //initUS=VL.get_distance();
  */
 /* PRUEBA COMUNICACIÓN SERIE*/
 if (primera){
  endHour.setHours(3);
  endHour.setMinutes(13);
  endHour.setSeconds(5);
  initHour.setHours(1);
  initHour.setMinutes(20);
  initHour.setSeconds(10);
  initUS=100.3;
  endUS=150.3;
  cells.addCelda(10,5);
  cells.addCelda(5,1);
  sendInfoUSB();
  primera=false;
 }else{
  delay(1000);
 }

  /*
  //STATE MACHINE;
  switch (actualState){
    case State::ON:
      if(direction!=0){
        while(!MPU.update()){
          delay(100);
        }
        angle=MPU.getYaw()*PI/180;
        dx=direction*WHEEL_CIRC*cos(angle);
        endCell[0]=initCell[0]+(uint8_t)floor(dx/L_SQUARE);
        dy=direction*WHEEL_CIRC*sin(angle);
        endCell[1]=initCell[1]+(uint8_t)floor(dy/L_SQUARE);
        cells.addCelda(endCell[0],endCell[1]);
        direction=0;
      }
      break;
    case State::STANDBY:
      if(actualHour.isAfter(maxStandByHour))
        actualState=State::SEND_INFO;
      break;
    case State::SEND_INFO:
      endUS=30.4;
      while(VL.ask_distance()==false){
        delay(100);
      }
      endUS=VL.get_distance();
      endHour.copy(actualHour);
      tft.println("MENSAJE A ENVIAR");    //FALTA PONER EL MENSAJE
      digitalWrite(EN_BOOST_PIN,LOW);
      stateBattery.turnOff(SoC_MPU);
      stateBattery.turnOff(SoC_VL);
      stateBattery.turnOff(SoC_TFT);
      stateBattery.turnOff(SoC_BOOST);
      if(sendInfoUSB()!=1)

      actualState=State::OFF;
      break;
    default:
      break;
  }
  */
}

// FUNCTIONES DEFINITIOS
uint8_t sendInfoUSB(){
  //SI SE QUEDA ATASCADO ES PUESTO QUE HA ENTRADA EN UN BUCLE INFINITO DE ESPERA DENTRO DE 
  //ALGUNO DE LOS ENVIOS
  int cont=0;
  ser.createSession();
  ser.sendUS(initUS,endUS);
  ser.sendHours(initHour,endHour);
  ser.sendMatrix(&cells);
  ser.deleteSession();
  return 1;
}
// INTERRUPT DEFINITIONS
void IRAM_ATTR buttonInterrupt(){
  flagButton=true;
}
void IRAM_ATTR oneSec(){
  flagClock=true;
  contSec+=1;
}