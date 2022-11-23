
char Buffer_Data[50];
uint8_t strPointer=0;
char str[200];

uint8_t nPumpVoltageDisplay[4] = {250,250,250,250};
uint16_t frequencyDisplay=100;
uint8_t liquidDisplay = 0;
uint16_t temp_1=0;

void Serial_init() {
  Serial.begin(9600);
  strPointer=0;
  Buffer_Data[strPointer]=0;
}

void Serial_println(const char str[]) {
  Serial.println(str);
}

void Serial_println(String str){
  Serial.println(str);
}
void Serial_print(const char str[]) {
  Serial.print(str);
}

void Serial_input() {
  while ((Serial.available() > 0) && (strPointer<50)) {
    Buffer_Data[strPointer] = Serial.read();
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
      if (Buffer_Data[1]=='1' || Buffer_Data[1]=='2' || Buffer_Data[1]=='3' || Buffer_Data[1]=='4'){ // Einzelne Pumpe
        char raw[] = {Buffer_Data[1]}; //atoi needs this
        uint8_t pump_no = atoi(raw);
        if(Buffer_Data[2]=='V'){                                      // Amplitude
          if(Buffer_Data[3]=='?'){                                    // Abfragen
            Serial_println((bPumpState[pump_no-1] ? nPumpVoltageDisplay[pump_no-1] : 0));
          } else {             //Setzen
            nPumpVoltageDisplay[pump_no-1] = constrain(atoi(&Buffer_Data[3]),0,250);
            Driver_setvoltage(pump_no, nPumpVoltageDisplay[pump_no-1]);
            Serial_println("OK, set pump " + String(pump_no));
          }
        } else if(strcmp(&Buffer_Data[2],"ON\r\n")==0) {              // Pumpe einschalten
          Driver_switchOn(pump_no);
          Serial_println("OK");
        } else if(strcmp(&Buffer_Data[2],"OFF\r\n")==0) {             // Pumpe ausschalten
          Driver_switchOff(pump_no);
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
    } else if (strcmp(Buffer_Data, "ID\r\n")==0){
      Serial_println("PocketLoCPumpController");
    }else { 
      char *p = strstr(Buffer_Data,"\r\n"); *p=0;
      Serial_print("Wrong command (");
      Serial_print(Buffer_Data);
      Serial_println(")"); 
    }
    strPointer = 0;
    Buffer_Data[strPointer]=0;
  }  
}
