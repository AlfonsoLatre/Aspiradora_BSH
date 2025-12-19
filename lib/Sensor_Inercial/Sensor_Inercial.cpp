#include "Sensor_Inercial.h"

// CONSTRUCTOR AND BEGIN
Sensor_Inercial::Sensor_Inercial(TwoWire* wireExt){
    this->wire=wireExt;
}
bool Sensor_Inercial::begin(uint8_t addr,ACCEL_FS_SEL a, GYRO_FS_SEL g){
    MPU9250Setting cfg;
    cfg.accel_fs_sel=a;
    cfg.gyro_fs_sel=g;
    cfg.mag_output_bits=MAG_OUTPUT_BITS::M16BITS;
    return this->mpu.setup(addr,cfg, *this->wire);

}
void Sensor_Inercial::calibrate(){
    this->mpu.verbose(true);
    delay(1000);
    this->mpu.calibrateAccelGyro();
    delay(1000);
    this->mpu.calibrateMag();
    this->mpu.verbose(false);
}

// READ AND MATHEMATICS
bool Sensor_Inercial::update(){
    if(!this->mpu.update()) return false;
    // Acelerometro
    this->ax = this->mpu.getAccX();
    this->ay = this->mpu.getAccY();
    this->az = this->mpu.getAccZ();

    // Giroscopo
    this->gx = this->mpu.getGyroX();
    this->gy = this->mpu.getGyroY();
    this->gz = this->mpu.getGyroZ();

    // Magnetometro
    this->mx= this->mpu.getMagX();
    this->my=this->mpu.getMagY();
    this->mz=this->mpu.getMagZ();

    // Cuaternion
    this->q.w=this->mpu.getQuaternionW();
    this->q.x=this->mpu.getQuaternionX();
    this->q.y=this->mpu.getQuaternionY();
    this->q.z=this->mpu.getQuaternionZ();

    // Angulo Yaw
    this->yaw=this->mpu.getYaw();

    return true;
}

// GET
float Sensor_Inercial::getAccX(){return this->ax;}
float Sensor_Inercial::getAccY(){return this->ay;}
float Sensor_Inercial::getAccZ(){return this->az;}
float Sensor_Inercial::getGyroX(){return this->gx;}
float Sensor_Inercial::getGyroY(){return this->gy;}
float Sensor_Inercial::getGyroZ(){return this->gz;}
float Sensor_Inercial::getMagX(){return this->mx;}
float Sensor_Inercial::getMagY(){return this->my;}
float Sensor_Inercial::getMagZ(){return this->mz;}
float Sensor_Inercial::getQr(){return this->q.w;}
float Sensor_Inercial::getQi(){return this->q.x;}
float Sensor_Inercial::getQj(){return this->q.y;}
float Sensor_Inercial::getQk(){return this->q.z;}
float Sensor_Inercial::getYaw(){return this->yaw;}
String Sensor_Inercial::toString(){
    return "\nAcc: "+String(this->ax)+" "+String(this->ay)+" "+String(this->az)
        +"\nGyro: "+String(this->gx)+" "+String(this->gy)+" "+String(this->gz)
           +"\nMag: "+String(this->mx)+" "+String(this->my)+" "+String(this->mz)
            +"\nQ: "+String(this->q.w)+" "+String(this->q.x)+" "+String(this->q.y)+" "+String(this->q.z)
           +"\nYaw: "+String(this->yaw);
}