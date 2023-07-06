#ifndef Battery_H_
#define Battery_H_

//======================Prototypes======================//
float Get_batteryV();   //Function to get the voltage of the battery
void Moitor_battery();  //Function to monitor the battery level and buzz below a certain value
//Motor Description A2212/10T 1400KV 
//========================================================
float Get_batteryV()
{
  float battery_level=0;
  float Vsupply = 3.3; //power supply voltage (3.3 V rail) 
  float Analog_res=1023.0;
  battery_level=3.782*analogRead(Battery)*(Vsupply/Analog_res);
 return(battery_level);
  }
//=============================================
void Moitor_battery()
{
  if(Get_batteryV()<=11.0){digitalWrite(Buzzer,HIGH);}
  else {digitalWrite(Buzzer,LOW);}
}
#endif
