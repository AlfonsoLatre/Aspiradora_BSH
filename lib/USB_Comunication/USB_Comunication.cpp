#include "USB_Comunication.h"

USB_Comunication::USB_Comunication(HardwareSerial* serialPort){
    this->serialPort = serialPort;
}
void USB_Comunication::begin(uint32_t baudrate){
    this->serialPort->begin(baudrate);
}
bool USB_Comunication::read(){
    bool read=false;
    unsigned long timeout;
    uint8_t b=0;
    uint8_t checksumCalc=0; 
    uint8_t checksumRead=0;
    ReadState state=ReadState::HEADER_STATE;
    while(state!=ReadState::IDLE_STATE){
        timeout=millis()+3000;
        switch (state){
            case ReadState::HEADER_STATE:
                if(this->serialPort->available()>=1 && this->serialPort->read()==HEADER)
                    state=ReadState::LENGTH_STATE;
                else
                    state=ReadState::IDLE_STATE;
                break;
            case ReadState::LENGTH_STATE:
                while(this->serialPort->available()<2 && millis()<timeout);
                if(this->serialPort->available()<2){
                    state=ReadState::IDLE_STATE;
                    break;
                }
                b=this->serialPort->read();
                b=(b<<8) | this->serialPort->read();
                if(b>MAX_LENGTH_MESSAGE){
                    state=ReadState::IDLE_STATE;
                }else{
                    this->lengthRead=b;
                    state=ReadState::DATA_STATE;
                }
                break;
            case ReadState::DATA_STATE:
                while(this->serialPort->available()<this->lengthRead && millis()<timeout);
                if(this->serialPort->available()<this->lengthRead){
                    state=ReadState::IDLE_STATE;
                }else{
                    this->serialPort->readBytes(this->bufferRead,this->lengthRead);
                    state=ReadState::CHECKSUM_STATE;
                }
                break;
            case ReadState::CHECKSUM_STATE:
                while(this->serialPort->available()<1 && millis()<timeout);
                if(this->serialPort->available()<1){
                    state=ReadState::IDLE_STATE;
                }else{
                    checksumRead=this->serialPort->read();
                    state=ReadState::FOOTER_STATE;
                }
                break;
            case ReadState::FOOTER_STATE:
                while(this->serialPort->available()<1 && millis()<timeout);
                if(this->serialPort->available()>=1 && this->serialPort->read()==FOOTER){
                    for(uint8_t i=0;i<this->lengthRead;i++)
                        checksumCalc^=this->bufferRead[i];
                    if(checksumCalc==checksumRead)
                        read=true;  
                }
                state=ReadState::IDLE_STATE;
                break;    
            default:
                state=ReadState::IDLE_STATE;
                break;
        }
    }
    return read;           
}
uint8_t USB_Comunication::getMessageLength(){
    return this->lengthRead;
}
uint8_t* USB_Comunication::getMessage(){
    return this->bufferRead;
}
void USB_Comunication::write(uint8_t* data, uint16_t length){
    uint8_t checksum=0;
    for(uint8_t i=0;i<length;i++){
        checksum^=data[i];
    }
    this->serialPort->write(HEADER);
    this->serialPort->write((length>>8) & 0xFF);
    this->serialPort->write((length>>0) & 0xFF);
    this->serialPort->write(data,length);
    this->serialPort->write(checksum);
    this->serialPort->write(FOOTER);
}

bool USB_Comunication::createSession(){
    uint8_t payload[]={ID_INSTRUCTION};
    this->write(payload,sizeof(payload));
    while(this->read()==false)
        delay(750);
    uint8_t* data=this->getMessage();
    uint8_t len=this->getMessageLength();
    if(len<=1)
        return false;
    this->lengthSessionId=len;
    this->session_id=data;
    return true;
}
void USB_Comunication::deleteSession(){
    this->lengthSessionId=0;
    this->session_id=0;
}
bool USB_Comunication::sendUS(float init, float end){
    uint32_t enteroInit=(uint32_t) init;
    uint8_t decimalInit;
    if(init>=(float)enteroInit)
        decimalInit=(uint8_t) ((init-(float)enteroInit)*256);
    else
        decimalInit=(uint8_t) (((float)enteroInit-init)*256);
    uint32_t enteroEnd=(uint32_t) end;
    uint8_t decimalEnd;
    if(end>=(float)enteroEnd)
        decimalEnd=(uint8_t) ((end-(float)enteroEnd)*256);
    else
        decimalEnd=(uint8_t) (((float)enteroEnd-end)*256);
    
    
    uint8_t len=this->lengthSessionId;
    uint8_t payload[1+1+len+8];     //1 de instruction, otro de size session_id, len de session_id, 8 de los dos us
    uint8_t offset=1+1+len;
    payload[0]=US_INSTRUCTION;
    payload[1]=len;
    for (int i =0; i < len; i++) {
        payload[i+2] = this->session_id[i];   // copiar ASCII directamente
    }
        
    payload[offset]=(enteroInit>>16 & 0xFF);
    payload[offset+1]=(enteroInit>>8 & 0xFF);
    payload[offset+2]=(enteroInit>>0 & 0xFF);
    payload[offset+3]=decimalInit;
    payload[offset+4]=(enteroEnd>>16 & 0xFF);
    payload[offset+5]=(enteroEnd>>8 & 0xFF);
    payload[offset+6]=(enteroEnd>>0 & 0xFF);
    payload[offset+7]=decimalEnd;
    //ENVIAR PAQUETE
    this->write(payload,sizeof(payload));
    while(this->read()==false)
        delay(750);
    uint8_t* data=this->getMessage();
    len=this->getMessageLength();
    if(len<1 or data[1]==0x00)
        return false;
    return true;
}

bool USB_Comunication::sendHours(Hour init, Hour end){
    uint8_t len=this->lengthSessionId;
    uint8_t payload[1+1+len+6];     //1 de instruction, otro de size session_id, len de session_id, 6 de dos hour
    uint8_t offset=1+1+len;
    payload[0]=HOUR_INSTRUCTION;
    payload[1]=len;
    for (int i =0; i < len; i++) {
        payload[i+2] = this->session_id[i];   // copiar ASCII directamente
    }
    payload[offset]=init.getHours();
    payload[offset+1]=init.getMinutes();
    payload[offset+2]=init.getSeconds();
    payload[offset+3]=end.getHours();
    payload[offset+4]=end.getMinutes();
    payload[offset+5]=end.getSeconds();
    //ENVIAR PAQUETE
    this->write(payload,sizeof(payload));
    while(this->read()==false)
        delay(750);
    
    len=this->getMessageLength();
    uint8_t* data=this->getMessage();
    if(len<1 or data[0]==0x00)
        return false;
    return true;
}

bool USB_Comunication::sendMatrix(Casillero* matrix){
    uint8_t len=this->lengthSessionId;
    uint8_t offset=1+1+len+2;
    uint8_t* payload=(uint8_t*) malloc(matrix->getNumRow()*matrix->getNumCol()+offset); //MIRAR QUE HACE EXACTAMENTE PARA EXPLICARLO    
    if(payload==NULL)
        return false;     //NO HAY ESPACIO EN MEMORIA DEL ESP32
    int pos=0;
    payload[0]=MATRIX_INSTRUCTION;
    payload[1]=len;
    for (int i =0; i<len; i++) {
        payload[i+2]=this->session_id[i];   // copiar ASCII directamente
    }
    payload[offset-2]=matrix->getNumRow();
    payload[offset-1]=matrix->getNumCol();
    for(char i=0; i<matrix->getNumRow(); i++){
        for(char j=0; j<matrix->getNumCol(); j++){
            payload[offset+pos]=matrix->getCell(i,j);
            pos++;
        }
    }
    //ENVIAR PAQUETE
    this->write(payload,offset+matrix->getNumRow()*matrix->getNumCol());
    while(this->read()==false)
        delay(750);
    uint8_t* data=this->getMessage();
    len=this->getMessageLength();
    if(len<1 or data[0]==0x00)
        return false;
    return true;
}

void USB_Comunication::println(String txt){
    this->serialPort->println(txt);
}