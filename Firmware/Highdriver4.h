#define I2C_HIGHDRIVER_ADRESS  (0x7b)   //Defaultadress for QuadOEM 11110XX

#define I2C_DEVICEID       0x00
#define I2C_POWERMODE      0x01
#define I2C_FREQUENCY      0x02
#define I2C_SHAPE          0x03
#define I2C_BOOST          0x04
#define I2C_PVOLTAGE       0x06
#define I2C_P1VOLTAGE      0x06
#define I2C_P2VOLTAGE      0x07
#define I2C_P3VOLTAGE      0x08
#define I2C_P4VOLTAGE      0x09
#define I2C_UPDATEVOLTAGE  0x0A
#define I2C_AUDIO          0x05

extern boolean bPumpState[4];
extern uint8_t nPumpVoltageByte[4];
extern uint8_t nFrequencyByte;

bool Highdriver4_check(void) {
  Wire.beginTransmission(I2C_HIGHDRIVER_ADRESS);
  Wire.write(I2C_DEVICEID);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(I2C_HIGHDRIVER_ADRESS,1);
  uint8_t data = Wire.read();
  return ((data&0xF0)==0xB0);
}

void Highdriver4_init(void) {                                               // QuadOEM initialisieren                       
  Wire.beginTransmission(I2C_HIGHDRIVER_ADRESS);
  Wire.write(I2C_POWERMODE);                                          // Start Register 0x01
  Wire.write(0x01);                                                   // Register 0x01 = 0x01 (enable)
  Wire.write(nFrequencyByte);                                         // Register 0x02 = 0x40 (100Hz)
  Wire.write(0x00);                                                   // Register 0x03 = 0x00 (sine wave)
  Wire.write(0x00);                                                   // Register 0x04 = 0x00 (800KHz)
  Wire.write(0x00);                                                   // Register 0x05 = 0x00 (audio off)
  Wire.write(0);              // Register 0x06 = Amplitude1
  Wire.write(0);              // Register 0x07 = Amplitude2
  Wire.write(0);              // Register 0x08 = Amplitude3
  Wire.write(0);              // Register 0x09 = Amplitude4
  Wire.write(0x01);                                                   // Register 0x0A = 0x01 (update)
  Wire.endTransmission();
  bPumpState[0] = false;
  bPumpState[1] = false;
  bPumpState[2] = false;
  bPumpState[3] = false;
  nPumpVoltageByte[0] = 0x1F;
  nPumpVoltageByte[1] = 0x1F;
  nPumpVoltageByte[2] = 0x1F;
  nPumpVoltageByte[3] = 0x1F;
}

void save_pump_voltage(uint8_t _pump, uint8_t _voltage){
  float temp = _voltage; temp*=31.0f; temp/=250.0f;   //250Vpp = 0x1F
  if (_pump>=1 && _pump<=4) nPumpVoltageByte[_pump-1]=constrain(temp,0,31);
}

void Highdriver4_setvoltage(uint8_t _pump, uint8_t _voltage) {                                      // Amplituden uebernehmen  
  save_pump_voltage(_pump, _voltage);
  Wire.beginTransmission(I2C_HIGHDRIVER_ADRESS);
  Wire.write(I2C_PVOLTAGE);  
  Wire.write((bPumpState[0] ? nPumpVoltageByte[0] : 0));
  Wire.write((bPumpState[1] ? nPumpVoltageByte[1] : 0));
  Wire.write((bPumpState[2] ? nPumpVoltageByte[2] : 0));
  Wire.write((bPumpState[3] ? nPumpVoltageByte[3] : 0));
  Wire.write(0x01);                                                     // Register Werte uebernehmen
  Wire.endTransmission(); 
}

void Highdriver4_setvoltage(uint8_t _v1, uint8_t _v2, uint8_t _v3, uint8_t _v4){
  save_pump_voltage(1, _v1);
  save_pump_voltage(2, _v2);
  save_pump_voltage(3, _v3);
  save_pump_voltage(4, _v4);

  Wire.beginTransmission(I2C_HIGHDRIVER_ADRESS);
  Wire.write(I2C_PVOLTAGE);  
  Wire.write((bPumpState[0] ? nPumpVoltageByte[0] : 0));
  Wire.write((bPumpState[1] ? nPumpVoltageByte[1] : 0));
  Wire.write((bPumpState[2] ? nPumpVoltageByte[2] : 0));
  Wire.write((bPumpState[3] ? nPumpVoltageByte[3] : 0));
  Wire.write(0x01);                                                     // Register Werte uebernehmen
  Wire.endTransmission(); 
  
  
}

void Highdriver4_setvoltage(void) {                                      // Amplituden uebernehmen  
  Wire.beginTransmission(I2C_HIGHDRIVER_ADRESS);
  Wire.write(I2C_PVOLTAGE);  
  Wire.write((bPumpState[0] ? nPumpVoltageByte[0] : 0));
  Wire.write((bPumpState[1] ? nPumpVoltageByte[1] : 0));
  Wire.write((bPumpState[2] ? nPumpVoltageByte[2] : 0));
  Wire.write((bPumpState[3] ? nPumpVoltageByte[3] : 0));
  Wire.write(0x01);                                                     // Register Werte uebernehmen
  Wire.endTransmission(); 
}

void Highdriver4_setfrequency(uint16_t _frequency) {
  if (_frequency>=800) {
    nFrequencyByte=0xFF;
  } else if (_frequency>=400) {                                     // Bereich 400-800
    _frequency-=400;
    _frequency*=64;
    _frequency/=400;
    nFrequencyByte = _frequency|0xC0;
  } else if (_frequency>=200) {                                     // Bereich 200-400
    _frequency-=200;
    _frequency*=64;
    _frequency/=200;
    nFrequencyByte = _frequency|0x80;
  } else if (_frequency>=100) {                                     // Bereich 100-200
    _frequency-=100;
    _frequency*=64;
    _frequency/=100;
    nFrequencyByte = _frequency|0x40;
  } else if (_frequency>=50) {                                      // Bereich 50-100
    _frequency-=50;
    _frequency*=64;
    _frequency/=50;
    nFrequencyByte = _frequency|0x00;
  } else {                                                         // Ausserhalb gueltigen Bereich
    nFrequencyByte=0x00;
  }
  Wire.beginTransmission(I2C_HIGHDRIVER_ADRESS);
  Wire.write(I2C_FREQUENCY);
  Wire.write(nFrequencyByte);
  Wire.endTransmission();  
}
