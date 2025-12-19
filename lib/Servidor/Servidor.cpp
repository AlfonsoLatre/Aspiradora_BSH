#include "Servidor.h"

Servidor::Servidor(const char* ssid,const char* password,const char* serverURL){
    this->ssid=ssid;
    this->password=password;
    this->serverURL=serverURL;
}

bool Servidor::connect(){
    char i=0;
    WiFi.begin(this->ssid, this->password);
    while(WiFi.status()!=WL_CONNECTED && i<10){
        delay(500);
        i++;
    }
    return WiFi.status()==WL_CONNECTED;
}
bool Servidor::disconnect(){
    char i=0;
    bool dis=WiFi.disconnect();
    while(dis==false && i<10){
        delay(500);
        dis=WiFi.disconnect();
        i++;
    }
    return dis;
}
bool Servidor::createSession(){
    int8_t send=1;
    if(WiFi.status()==WL_CONNECTED){
        // HACER CONEXION CON EL SERVIDOR
        HTTPClient http;
        http.begin(this->serverURL);
        http.addHeader("Content-Type", "application/octet-stream");
        //ENVIAR PAQUETE
        uint8_t payload[]={ID_INSTRUCTION};
        // NO SE PORQUE DA FALLO.
        int httpResponseCode=http.POST(payload,sizeof(payload));
        if(httpResponseCode > 0){
            // no deja hacer el cast. Pensar como hacerlo
            this->session_id=http.getString();
        } else {
            Serial.println("Error en envío: " + String(httpResponseCode));
            send=httpResponseCode;
        }
        // CERRAR CONEXIÓN
        http.end();
    }else{
        send=-13;
    }
    return send;
}
int8_t Servidor::sendUS(float init, float end){
    /*
    OUTPUT VALUES
    ALL CORRECT                     (1)     -->PROPIA
    HTTPC_ERROR_CONNECTION_REFUSED  (-1)
    HTTPC_ERROR_SEND_HEADER_FAILED  (-2)
    HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
    HTTPC_ERROR_NOT_CONNECTED       (-4)
    HTTPC_ERROR_CONNECTION_LOST     (-5)
    HTTPC_ERROR_NO_STREAM           (-6)
    HTTPC_ERROR_NO_HTTP_SERVER      (-7)
    HTTPC_ERROR_TOO_LESS_RAM        (-8)
    HTTPC_ERROR_ENCODING            (-9)
    HTTPC_ERROR_STREAM_WRITE        (-10)
    HTTPC_ERROR_READ_TIMEOUT        (-11)
    HTTP_ERROR_IN_PROCESSED_INFO    (-12)   -->PROPIA
    WIFI_NOT_CONNECTED              (-13)   -->PROPIA
    */
    int8_t send=1;
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
    if(WiFi.status()==WL_CONNECTED){
        // HACER CONEXION CON EL SERVIDOR
        HTTPClient http;
        http.begin(this->serverURL);
        http.addHeader("Content-Type", "application/octet-stream");
        // GENERAR PAQUETE
        uint8_t len=this->session_id.length();
        uint8_t payload[1+1+len+8];     //1 de instruction, otro de size session_id, len de session_id, 8 de los dos us
        uint8_t offset=1+1+len;
        payload[0]=US_INSTRUCTION;
        payload[1]=len;
        for (int i =0; i < len; i++) {
            payload[i+2] = this->session_id[i];   // copiar ASCII directamente
        }
        
        payload[offset+1]=(enteroInit>>16 & 0xFF);
        payload[offset+2]=(enteroInit>>8 & 0xFF);
        payload[offset+3]=(enteroInit>>0 & 0xFF);
        payload[offset+4]=decimalInit;
        payload[offset+5]=(enteroEnd>>16 & 0xFF);
        payload[offset+6]=(enteroEnd>>8 & 0xFF);
        payload[offset+7]=(enteroEnd>>0 & 0xFF);
        payload[offset+8]=decimalEnd;
        //ENVIAR PAQUETE
        int httpResponseCode=http.POST(payload,sizeof(payload));
        if(httpResponseCode > 0){
            String response = http.getString();
            if(response[0] != char(0xFF)){
                send=-12;
            }
        } else {
            Serial.println("Error en envío: " + String(httpResponseCode));
            send=httpResponseCode;
        }
        // CERRAR CONEXIÓN
        http.end();
    }else{
        send=-13;
    }
    return send;
}

int8_t Servidor::sendHours(Hour init, Hour end){
    int8_t send=1;
    if(WiFi.status()==WL_CONNECTED){
        // HACER CONEXION CON EL SERVIDOR
        HTTPClient http;
        http.begin(this->serverURL);
        http.addHeader("Content-Type", "application/octet-stream");
        // GENERAR PAQUETE
        uint8_t len=this->session_id.length();
        uint8_t payload[1+1+len+6];     //1 de instruction, otro de size session_id, len de session_id, 6 de dos hour
        uint8_t offset=1+1+len;
        payload[0]=HOUR_INSTRUCTION;
        payload[1]=len;
        for (int i =0; i < len; i++) {
            payload[i+2] = this->session_id[i];   // copiar ASCII directamente
        }
        payload[offset+1]=init.getHours();
        payload[offset+2]=init.getMinutes();
        payload[offset+3]=init.getSeconds();
        payload[offset+4]=end.getHours();
        payload[offset+5]=end.getMinutes();
        payload[offset+6]=end.getSeconds();
        //ENVIAR PAQUETE
        int httpResponseCode=http.POST(payload,sizeof(payload));

        if(httpResponseCode > 0){
            String response = http.getString();
            if(response[0] != char(0xFF)){
                send=-12;
            }
        } else {
            Serial.println("Error en envío: " + String(httpResponseCode));
            send=httpResponseCode;
        }
        // CERRAR CONEXIÓN
        http.end();
    }else{
        send=-13;
    }
    return send;
}

int8_t Servidor::sendMatrix(Casillero* matrix){
    int8_t send=1;
    if(WiFi.status()==WL_CONNECTED){
        // HACER CONEXION CON EL SERVIDOR
        HTTPClient http;
        http.begin(this->serverURL);
        http.addHeader("Content-Type", "application/octet-stream");
        // GENERAR PAQUETE
        uint8_t len=this->session_id.length();
        uint8_t offset=1+1+len+2;
        uint8_t* payload=(uint8_t*) malloc(matrix->getNumRow()*matrix->getNumCol()+offset); //MIRAR QUE HACE EXACTAMENTE PARA EXPLICARLO    
        if(payload==NULL)
            return -15;     //NO HAY ESPACIO EN MEMORIA DEL ESP32
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
        int httpResponseCode=http.POST(payload,matrix->getNumRow()*matrix->getNumCol()+offset);
        free(payload);
        if(httpResponseCode > 0){
            String response = http.getString();
            if(response[0] != (char)0xFF){
                send=-12;
            }
        } else {
            Serial.println("Error en envío: " + String(httpResponseCode));
            send=httpResponseCode;
        }
        // CERRAR CONEXIÓN
        http.end();
    }else{
        send=-13;
    }
    return send;
}
