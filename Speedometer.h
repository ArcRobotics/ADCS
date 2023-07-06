#ifndef Speedometer_H_
#define Speedometer_H_
             
  float alphaL = 0.15;   // Smoothing factor for the low-pass filter
  float samples = 10.0; // Number of samples for averaging
  float accumlated_avg;
  float filteredRpm = 0.0;     // Filtered RPM value
  float rpm=0.0;
  
  // Variables for speed calculation
  volatile int encoderCount = 0;
  unsigned long prevTime = 0;
  volatile float speedRPM = 0;  // Current RPM value
  const unsigned long updateInterval = 5; // Update interval in milliseconds
  
  int lastEncoderState = 0;

//======================Prototypes======================//
//void Get_speed();
// ========================================================
void Get_speed()
{
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= updateInterval) {
    // Calculate speed in RPM
    speedRPM = (encoderCount * 60000.0) / (currentTime - prevTime);

    // Reset encoder count
    encoderCount = 0;
    filteredRpm = alphaL* speedRPM + (1.0 - alphaL) * filteredRpm;
  if (samples < 10) {
      accumlated_avg += filteredRpm;
      samples++;
    }
    else {
      accumlated_avg = accumlated_avg / 10.0;
      samples = 0;   

      rpm=accumlated_avg/1000.0;
       //Serial.println(accumlated_avg/1000.0);
    // Print speed
   /* Serial.print("Speed: ");
    Serial.print(accumlated_avg);
    Serial.println(" RPM");*/
    }

    prevTime = currentTime;
  }  
}
// ========================================================
void handleEncoderInterrupt() {
  // Increment encoder count
  encoderCount++;
}

#endif
