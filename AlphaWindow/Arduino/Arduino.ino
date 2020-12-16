#include<Stepper.h>
#include <SoftwareSerial.h>
SoftwareSerial ESP_Serial(2,3); //软串口定义，2为Tx,3为Rx


const int STEPS_PER_ROTOR_REV = 32;

const int GEAR_REDUCTION = 64;

const int STEPS_PER_OUT_REV = STEPS_PER_ROTOR_REV * GEAR_REDUCTION;

int StepsRequired;

Stepper steppermotor(STEPS_PER_ROTOR_REV, 8, 10, 9, 11);

void setup()
{
  Serial.begin(9600);  //定义串口波特率
  ESP_Serial.begin(115200);  //定义软串口波特率
}

int p = 0, lasp = 0;

void loop()
{
  while (ESP_Serial.available() > 0)  //判断是否获得数据
  {
    p = ESP_Serial.parseInt();
    Serial.println(p);
  }
  if(p != lasp) {
    if(!p) {
      //关窗
      StepsRequired = STEPS_PER_OUT_REV*3/2;
      steppermotor.setSpeed(900);
      steppermotor.step(StepsRequired);
      delay(2000);
   }
   else {
      //开窗
      StepsRequired = - STEPS_PER_OUT_REV*3/2;
      steppermotor.setSpeed(900);
      steppermotor.step(StepsRequired);
      delay(2000);
    }
    lasp = p;
  }
}
