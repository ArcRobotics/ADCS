#include "Main.h"

float M_speed=0.0;
int seconds;

//=======================================================

void setup() {
  
  Serial.begin(115200); 
  
  ADCS.Pins_init();
  
  //SD card Initialization
  ADCS.SD_cardModule_init();  
  
  Motor1_Controller.kp=55;//2.75
  Motor1_Controller.ki=0.06;//0.4

  Motor1_Controller.SetPoint=0;

  Get_speed(); 
  delay(5000);

  ADCS.Led_on();
  
}

//=========================================================

void loop() {
 Get_speed(); 
 //Motor1_Controller.Calculate_pid(rpm,Motor1_Controller.SetPoint,180.0,90.0);
 ESC.write(Motor1_Controller.Calculate_pid(rpm,Motor1_Controller.SetPoint,180.0,90.0));
 //ESC.write(Motor1_Controller.SetPoint);
 get_data();
 delay(1);
} 

//==========================================================
void get_data()
{
  
  currT2= millis();
  if(currT2-prevT2>=500){
  
  //Serial.print("Battery:"+String(Get_batteryV()));
  //Serial.print("PID:"+String(Motor1_Controller.pid_value));
  Serial.print("SetPoint:"+String(Motor1_Controller.SetPoint));
  Serial.println(",MeaSpeed:"+String(rpm)); 
    // Calculate RPM
   if (Serial.available() > 0)
    {
      M_speed = Serial.readStringUntil('\n').toFloat();
      /*if(M_speed==0||M_speed<90)M_speed=90;
      if(M_speed==500)
      {
        save_data();
        delay(100);
        Serial.println("File saved");
        M_speed=90;
        while(1);
      }*/
     Motor1_Controller.SetPoint=M_speed;
    }
  prevT2=currT2;
  }
}
//==========================================================
void Get_Data_Record()
{
  if(currT2-prevT3>=1000){
  seconds+=1.0;
  log_data(seconds,rpm,Get_batteryV());
  prevT3=currT2;
  Serial.println("Saving...");
  if(seconds>180)
  {        save_data();
        delay(100);
        Serial.println("File saved");
        M_speed=90;
        while(1);
    
    }
  }
}
//=======================================================
//=====================Second Core=======================
//=======================================================
/*
//This is the Second core
void setup1() {
  Serial.println("Core 2 activated");
  Server_init();
  delay(1000);
}

void loop1() {
  run_client();
}*/
