#ifndef RTC_H_
#define RTC_H_

#define RTC_ADDR 0x68

//======================Prototypes======================//
void RTC_init();
void Time_init();
byte bcdToDec(byte val);
byte decToBcd(byte val);
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
void getDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);
//========================================================
void RTC_init()
{

 delay(500);
  }
//========================================================
void Time_init()
{
  String timeString = __TIME__;
  String dateString = __DATE__;


  // Extract the individual components from the time string
  int hour = timeString.substring(0, 2).toInt();
  int minute = timeString.substring(3, 5).toInt();
  int second = timeString.substring(6, 8).toInt();
  second+=10;//add ten seconds for compilation
  if(second>60)
  {
    minute++;
    second-=60;
  }

  if(hour>12)hour-=12;
  else if(hour==0)hour=12;

  // Extract the individual components from the date string
  int day = dateString.substring(4, 6).toInt();
  int month =dateString.substring(0, 3).toInt();
  int year = dateString.substring(7).toInt() - 2000; // Assuming 21st century


  setDS3231time(second, minute, hour, 1, day, 5, year);
  }
byte decToBcd(byte val) {
  return (val / 10 * 16) + (val % 10);
}

byte bcdToDec(byte val) {
  return (val / 16 * 10) + (val % 16);
}
//=======================================================================================
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}
//=======================================================================================
void getDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDR, 7);
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

#endif
