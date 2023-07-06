#ifndef SDcard_H_
#define SDcard_H_

#if !defined(ARDUINO_ARCH_RP2040)
  #error For RP2040 only
#endif

#include <RP2040_SD.h>

File dataFile;
//======================Prototypes======================//
void SD_init();
void save_data();
void log_data(float value1,float value2,float value3);
//========================================================
void SD_init() 
{
  SPI.setRX(PIN_SD_MOSI);
  SPI.setTX(PIN_SD_MISO);
  SPI.setSCK(PIN_SD_SCK);
  
  delay(100);
  if (!SD.begin(PIN_SD_SS)) 
  {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");
}
//========================================================
void log_data(float value1,float value2,float value3) {
  char data[10];
  dtostrf(value1, 3, 2, data);
  dataFile.print(data);
  dataFile.print(',');
  dtostrf(value2, 3, 2, data);
  dataFile.print(data);
  dataFile.print(',');
  dtostrf(value3, 3, 2, data);
  dataFile.println(data);
}
//========================================================
void save_data()
{
  dataFile.close();
}
#endif
