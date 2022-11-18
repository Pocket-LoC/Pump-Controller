#include <SoftwareSerial.h>

SoftwareSerial btSerial(5, 4); // RX, TX

char Buffer_Data[50];
uint8_t strPointer=0;
char str[200];

uint8_t nPumpVoltageDisplay[4] = {250,250,250,250};
uint16_t frequencyDisplay=100;
uint8_t liquidDisplay = 0;
uint16_t temp_1=0;

void Serial_init() {
  Serial.begin(9600);
  btSerial.begin(9600);  
  strPointer=0;
  Buffer_Data[strPointer]=0;
}

void Serial_println(const char str[]) {
  Serial.println(str);
  btSerial.println(str);
}
void Serial_print(const char str[]) {
  Serial.print(str);
  btSerial.print(str);
}

void Serial_println(unsigned char val, int base = DEC) {
  Serial.println(val,base);
  btSerial.println(val,base);
}

void Serial_print(unsigned char val, int base = DEC) {
  Serial.print(val,base);
  btSerial.print(val,base);
}

void Serial_println(int val, int base = DEC) {
  Serial.println(val,base);
  btSerial.println(val,base);
}

void Serial_print(int val, int base = DEC) {
  Serial.print(val,base);
  btSerial.print(val,base);
}

/*void Serial_println(float f) {
  Serial.println(f);
  btSerial.println(f);
}*/

void Serial_input() {
  while ((Serial.available() > 0) && (strPointer<50)) {
    Buffer_Data[strPointer] = Serial.read();
    if ((Buffer_Data[strPointer]>='a') && (Buffer_Data[strPointer]<='z')) Buffer_Data[strPointer]+='A'-'a';  //Großbuchstaben
    strPointer++;
    Buffer_Data[strPointer]=0;
  }
  while ((btSerial.available() > 0) && (strPointer<50)) {
    Buffer_Data[strPointer] = btSerial.read();
    if ((Buffer_Data[strPointer]>='a') && (Buffer_Data[strPointer]<='z')) Buffer_Data[strPointer]+='A'-'a';  //Großbuchstaben
    strPointer++;
    Buffer_Data[strPointer]=0;
  }
      
  if (strcmp(Buffer_Data,"\r\n")==0) {    //Kein Befehlt eingegeben, nur Zeilenumbruch -> Aktuelle Einstellungen ausgeben
    sprintf(str,"Multiboard:%s\r\nFrequency: %iHz\r\nPump1: %iV (%s)\r\nPump2: %iV (%s)\r\nPump3: %iV (%s)\r\nPump4: %iV (%s)", current_driver==DRIVER_HIGHDRIVER4?"Highdriver4":"No driver",
      frequencyDisplay,
      nPumpVoltageDisplay[0],(bPumpState[0] ? "ON" : "OFF"),
      nPumpVoltageDisplay[1],(bPumpState[1] ? "ON" : "OFF"),
      nPumpVoltageDisplay[2],(bPumpState[2] ? "ON" : "OFF"),
      nPumpVoltageDisplay[3],(bPumpState[3] ? "ON" : "OFF"));
    Serial_println(str);
    strPointer=0;
    Buffer_Data[strPointer]=0;
  } else if (strstr(Buffer_Data,"\r\n")>0)   {                        //Befehl wurde mit Return beendet
    if (Buffer_Data[0]=='P'){
      if (Buffer_Data[1]=='1'){                                       // Erste Pumpe
        if(Buffer_Data[2]=='V'){                                      // Amplitude
          if(Buffer_Data[3]=='?'){                                    // Abfragen
            Serial_println((bPumpState[0] ? nPumpVoltageDisplay[0] : 0));
          } else {             
            nPumpVoltageDisplay[0] = constrain(atoi(&Buffer_Data[3]),0,250);
            Driver_setvoltage(1,nPumpVoltageDisplay[0]);
            Serial_println("OK");
          }
        } else if(strcmp(&Buffer_Data[2],"ON\r\n")==0) {              // Erste Pumpe einschalten
          Driver_switchOn(1);
          Serial_println("OK");
        } else if(strcmp(&Buffer_Data[2],"OFF\r\n")==0) {             // Erste Pumpe ausschalten
          Driver_switchOff(1);
          Serial_println("OK");
        } 
      } else if (Buffer_Data[1]=='2'){                               // Zweite Pumpe
        if(Buffer_Data[2]=='V'){                                     // Amplitude
          if(Buffer_Data[3]=='?'){                                   // Abfragen
            Serial_println((bPumpState[1] ? nPumpVoltageDisplay[1] : 0));
          } else {                                                   // Setzen
            nPumpVoltageDisplay[1] = constrain(atoi(&Buffer_Data[3]),0,250);
            Driver_setvoltage(2,nPumpVoltageDisplay[1]);
            Serial_println("OK");
          }
        } else if (strcmp(&Buffer_Data[2],"ON\r\n")==0) {              // Zweite Pumpe einschalten
          Driver_switchOn(2);
          Serial_println("OK");
        } else if(strcmp(&Buffer_Data[2],"OFF\r\n")==0) {              // Zweite Pumpe ausschalten
          Driver_switchOff(2);
          Serial_println("OK");
        }
      } else if (Buffer_Data[1]=='3') {                                // Dritte Pumpe
        if(Buffer_Data[2]=='V'){                                       // Amplitude
          if(Buffer_Data[3]=='?'){                                     // Abfragen
            Serial_println((bPumpState[2] ? nPumpVoltageDisplay[2] : 0));
          } else {                                                     // Setzen
            nPumpVoltageDisplay[2] = constrain(atoi(&Buffer_Data[3]),0,250);
            Driver_setvoltage(3,nPumpVoltageDisplay[2]);
            Serial_println("OK");
          }
        } else if(strcmp(&Buffer_Data[2],"ON\r\n")==0){                // Dritte Pumpe einschalten
          Driver_switchOn(3);
          Serial_println("OK");
        } else if(strcmp(&Buffer_Data[2],"OFF\r\n")==0){               // Dritte Pumpe ausschalten
          Driver_switchOff(3);
          Serial_println("OK");
        }
      } else if (Buffer_Data[1]=='4'){                                 // Vierte Pumpe
        if(Buffer_Data[2]=='V'){                                       // Amplitude
          if(Buffer_Data[3]=='?'){                                     // Abfragen
            Serial_println((bPumpState[3] ? nPumpVoltageDisplay[3] : 0));
          } else {                                                     // Setzen
            nPumpVoltageDisplay[3] = constrain(atoi(&Buffer_Data[3]),0,250);
            Driver_setvoltage(4,nPumpVoltageDisplay[3]);
            Serial_println("OK");
          }
        } else if(strcmp(&Buffer_Data[2],"ON\r\n")==0){                // Dritte Pumpe einschalten
          Driver_switchOn(4);
          Serial_println("OK");
        } else if(strcmp(&Buffer_Data[2],"OFF\r\n")==0){               // Dritte Pumpe ausschalten
          Driver_switchOff(4);
          Serial_println("OK");
        }
      } else if(Buffer_Data[1]=='A'){                            //Spannung für alle Pumpen Setzen (PA000#000#000#000)
        nPumpVoltageDisplay[0] = constrain(atoi(&Buffer_Data[2]),0,250);
        nPumpVoltageDisplay[1] = constrain(atoi(&Buffer_Data[6]),0,250);
        nPumpVoltageDisplay[2] = constrain(atoi(&Buffer_Data[10]),0,250);
        nPumpVoltageDisplay[3] = constrain(atoi(&Buffer_Data[14]),0,250);
        Driver_setvoltage(nPumpVoltageDisplay[0], nPumpVoltageDisplay[1], nPumpVoltageDisplay[2], nPumpVoltageDisplay[3]);
        Serial_println("OK");
      } else if(strcmp(&Buffer_Data[1],"ON\r\n")==0){                  // Alle Pumpen einschalten
        Driver_switchOn();
        Serial_println("OK");
      } else if(strcmp(&Buffer_Data[1],"OFF\r\n")==0){                  // Alle Pumpen ausschalten
        Driver_switchOff();
        Serial_println("OK");
      }        
    } else if (Buffer_Data[0]=='F') {                                   // Frequenz
      if (Buffer_Data[1]=='?'){                                         // Abfrage
        Serial_println((int)frequencyDisplay);
      } else {
        frequencyDisplay = atoi(&Buffer_Data[1]);                              // Setzen
        Driver_setfrequency(frequencyDisplay);
        Serial_println("OK");
      }

    } else if (strcmp(Buffer_Data,"V\r\n")==0) { 
      sprintf(str,"%s#%d",SOFTWARE_VERSION,bBoardversion);
      Serial_println(str);
    } else if (strcmp(Buffer_Data,"#\r\n")==0) { 
      Serial_print(nFrequencyByte,HEX);
      Serial_print(nPumpVoltageByte[0],HEX);
      Serial_print(nPumpVoltageByte[1],HEX);
      Serial_print(nPumpVoltageByte[2],HEX);
      Serial_print(nPumpVoltageByte[3],HEX);
      Serial_println((bPumpState[0]?0x01:0)|(bPumpState[1]?0x02:0)|(bPumpState[2]?0x04:0)|(bPumpState[3]?0x08:0),HEX);
    } else if (strcmp(Buffer_Data,"SELECTQUADDRIVER\r\n")==0) {
      if (Driver_select(DRIVER_HIGHDRIVER4))
        Serial_println("OK");
      else
        Serial_println("FAIL");    
    } else if (strcmp(Buffer_Data,"SELECTNONE\r\n")==0) {
      if (Driver_select(DRIVER_NONE))
        Serial_println("OK");
      else
        Serial_println("FAIL");    
    } else { 
      char *p = strstr(Buffer_Data,"\r\n"); *p=0;
      Serial_print("Wrong command (");
      Serial_print(Buffer_Data);
      Serial_println(")"); 
    }
    strPointer = 0;
    Buffer_Data[strPointer]=0;
  }  
}
