#ifndef SENSOR_INERCIAL_H
    #define SENSOR_INERCIAL_H
    #include <Arduino.h>
    #include <Wire.h>
    #include <math.h>
    #include "MPU9250.h"

    //REGISTROS
    #define    MPU9250_ADDRESS            0x68
    #define    MAG_ADDRESS                0x0C
    
    class Sensor_Inercial{
        private:
            struct Quaternion{
                float w,x,y,z;
            };
            TwoWire* wire;
            MPU9250 mpu;
            float ax,ay,az;
            float gx,gy,gz;
            float mx,my,mz;
            Quaternion q;
            float yaw;

        public:
            Sensor_Inercial(TwoWire* wireExt);
            bool begin(uint8_t addr=MPU9250_ADDRESS,ACCEL_FS_SEL=ACCEL_FS_SEL::A16G, GYRO_FS_SEL=GYRO_FS_SEL::G2000DPS);
            void calibrate();
            bool update();
            
            //Get
            float getAccX();
            float getAccY();
            float getAccZ();
            float getGyroX();
            float getGyroY();
            float getGyroZ();
            float getMagX();
            float getMagY();
            float getMagZ();
            float getQr();
            float getQi();
            float getQj();
            float getQk();
            float getYaw();
            String toString();
    };
#endif
