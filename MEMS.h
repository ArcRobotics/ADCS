#ifndef MEMS_H_
#define MEMS_H_
/*
 *  0.0780162176 rad
 */
void Mems_init()
{
    Wire.setSDA(16);
    Wire.setSCL(17);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x69)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
  }
#endif
