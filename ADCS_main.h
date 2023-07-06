#ifndef ADCS_main_H_
#define ADCS_main_H_

Servo ESC;     // create servo object to control the ESC
class main 
{
 public:
  void Pins_init();
  void SD_cardModule_init();
  void Led_on();
  }ADCS;
//======================================================================================
void main::Pins_init()
{
  ESC.attach(M3, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds)
  ESC.write(90);
  
  pinMode(Buzzer,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoder2),handleEncoderInterrupt, FALLING);
  }
//======================================================================================
void main::SD_cardModule_init()
{
    SD_init(); 
  //Start the file
  dataFile = SD.open("Log.txt", FILE_WRITE);
  dataFile.println(F("Time(Sec),Speed(Rpm),Battery(Volt)")); 
  }
//======================================================================================
void main::Led_on()
{
  digitalWrite(LED_BUILTIN,HIGH);
  }
//======================================================================================
void handleEncoderInterrupt() {
  // Increment encoder count
  encoderCount++;
}
#endif
