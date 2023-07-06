#ifndef PID_Controller_H_
#define PID_Controller_H_


unsigned long currT2=0;
unsigned long prevT2=0;
unsigned long prevT3=0;

class PID_controller
{
  public:
  float kp=0;
  float ki=0;
  float kd=0;
  
  float error;
  float prev_error;
  
  float Delta_T;
  unsigned long prevT;
  unsigned long currT;
  
  float Pp=0;
  float Pi=0;
  float Pd=0;

  float pid_value; //return the control signal 0-255

  float SetPoint;

  int  Calculate_pid(float sensor_readings,float setPoint,float pid_clamp_max,float pid_clamp_min);
  };

PID_controller Motor1_Controller,Motor2_Controller,Motor3_Controller,ADCS_Controller;

// ========================================================
int PID_controller::Calculate_pid(float sensor_readings,float setPoint,float pid_clamp_max,float pid_clamp_min)
{

     currT = millis();
     
     //Get the error
     error=setPoint-sensor_readings;
         
     Pp = kp*error;
   
    /*The integral part should only act if we are close to the
    desired position but we want to fine tune the error. That's
    why I've made a if operation for an error between -2 and 2 degree.
    To integrate we just sum the previous integral value with the
    error multiplied by  the integral constant. This will integrate (increase)
    the value each loop till we reach the 0 point*/
    if(error < 1.5)
    {
      Pi = Pi+(ki*error);  
    }
    else Pi=0;
    
    /*The last part is the derivate. The derivate acts upon the speed of the error.
    As we know the speed is the amount of error that produced in a certain amount of
    time divided by that time. For taht we will use a variable called previous_error.
    We substract that value from the actual error and divide all by the elapsed time. 
    Finnaly we multiply the result by the derivate constant*/
    if (prevT != currT) {
      Delta_T = double((currT - prevT)) / 1000.0; 
      
      Pd = kd*((error - prev_error)/Delta_T);
      if (error == prev_error||error==0) {
        // Constant value to add to the derivative term
        Pd = 0.1;
      }  
    }
    
    /*The final PID values is the sum of each of this 3 parts*/
    pid_value = Pp + Pi;
    pid_value =pid_value + pid_clamp_min;
   
    
    //Clamp the pid signal between max signal and min signal 
    if(pid_value<pid_clamp_min||pid_value==0.0){pid_value=pid_clamp_min;}
    else if(pid_value>pid_clamp_max){pid_value=pid_clamp_max;}

    
    //Update Time
    prevT=currT;
    
    //Update Error
    prev_error=error;
    return(pid_value);
}
#endif
