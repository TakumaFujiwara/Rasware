#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/adc.h>
#include <unistd.h>
// Blink the LED to show we're on
tBoolean blink_on = true;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}
void blinky(void) {
    SetPin(PIN_F1, blink_on);
    blink_on = !blink_on;
}
void blinkys(void) {
    SetPin(PIN_F2, blink_on);
    blink_on = !blink_on;
}
float linefollowfindravg(float linevals[8])
{
  float val[8]={-4,-3,-2,-1,1,2,3,4};
  float mult[8];
  float value;
  float avg;
  for(int i=0;i<8;i++)
  {if(linevals[i]>1)
    linevals[i]=1;
    mult[i]=linevals[i]*val[i];
    value+=mult[i];
  }
  avg=value;
  return avg;
}
float linefollowfindavg(float lintervals[8])
{
  float val[8] = {-4.3,-3.2,-1.8,-.7,.7,1.8,3.2,4.3};
  float trig[8];
  float mult[8];
  float value;
  float avg;
  for(int i=0; i<8;i++)
  {
    if(1<lintervals[i])
     trig[i]=1;
     else
     trig[i]=0;
  }
  for(int i=0;i<8;i++)
  {
    mult[i]=trig[i]*val[i];
    value+=mult[i];
  }
  avg=value;
  return avg;
}
void linefollow(tMotor *left, tMotor *right, float avg){
Printf("%f",avg);
float leftMotorVal=0;
float rightMotorVal=0;
  if (-.5<avg&&avg<.5)
  {
    leftMotorVal=-1;
    rightMotorVal=-1;
    //Printf("left -.25\t");
    //Printf("right -.25\t");
  }

  // else if(avg<=-5)
  // {
  //   leftMotorVal =-.5;
  //   rightMotorVal =0.5;
  //   //Printf("left -.02\t");
  //   //Printf("right -.5\t");
  // }
  //
  // else if(avg>=5)
  // {
  //   leftMotorVal=0.5;
  //     rightMotorVal=-.5;
  //   //Printf("left -.02\t");
  //   //Printf("right -.5\t");
  // }
  else
  {

    // float absAvg = avg;
    // if(avg <0){
    //   avg = avg*-1;
    // }
    // float baseSpeed = -.5 +(absAvg*(.3/4.5));
    leftMotorVal = -.3+avg*.07;
    rightMotorVal = -.3-avg*.07;

    // if(avg<-4){
    //   leftMotorVal=-0.38+(-4)*.08;
    //   rightMotorVal=-0.38-(-4)*.08;
    // }
    // else if(avg>4){
    //   leftMotorVal=-0.38+4*.08;
    //   rightMotorVal=-0.38-4*.08;
    // }



    if(leftMotorVal<-0.5){
      leftMotorVal=-.5;
    }
    else if(leftMotorVal>.2){
      leftMotorVal=.2;
    }
    if(rightMotorVal<-.5){
      rightMotorVal=-.5;
    }
    else if (rightMotorVal>.2){
      rightMotorVal=.2;
    }


  //  leftMotorVal= -0.38+avg*.1;
  //  rightMotorVal=0.38-avg*.1;
    //Printf("left %f\t",-0.25-avg*.125);
    //Printf("right %f\t",-0.25+avg*.125);
  }


  SetMotor(left,leftMotorVal);
  SetMotor(right,rightMotorVal);
//Printf("%f",avg);

}
void linerfollow(tMotor *left, tMotor *right, float prop,float derivative,float integral){
  float output=.085*prop+1.2*derivative;//.000005*integral;
  Printf("%f\t%f\t%f\t%f\n",prop,derivative,integral,.075*prop+.015*derivative+.005*integral);
  if(output>.65)
  output=.65;
  else if (output<-.65)
  output=-.65;
  SetMotor(left,-0.35+output*.95);
  SetMotor(right,-0.35-output*.95);
  /*if(cavg==0&&avg-inital>0.1)
  {
    SetMotor(left,-0.57+.1*(avg-pravg));
    SetMotor(right,-0.57-.1*(avg-pravg));
    return pravg;
  }
  else if(cavg==0 &&avg-initial<-0.1)
  {
    SetMotor(left,-0.57+.1*(avg-pravg));
    SetMotor(right,-0.57-.1*(avg-pravg));
    return pravg;
  }
  else
  {
    SetMotor(left,-0.5);
    SetMotor(right,-0.5);
    return 0;
  }*/
}


void clockwisemode(tMotor *left, tMotor *right, float distvalcw){
if(distvalcw>0.7)
distvalcw=0.7;
else if(distvalcw<-0.7)
distvalcw=-0.7;
  SetMotor(right,-0.3+distvalcw);
  SetMotor(left,-0.3-distvalcw);
}

void counterclockwisemode(tMotor *left, tMotor *right, float distvalcw){
  if(distvalcw<=0.30)
  {
    SetMotor(right, -0.25);
    SetMotor(left, -0.55);
  }

  else
  {
    SetMotor(right, 0.1);
    SetMotor(left, -0.1);
  }
}

void testcounterclockwisemode(tMotor *left, tMotor *right, float distvalccw){
  if(distvalccw<=0.35)
  {
    SetMotor(left, -0.55);
    SetMotor(right, 1.0);
  }
  else if(0.35<distvalccw && distvalccw<=0.4)
  {
    SetMotor(left, -0.75);
    SetMotor(right, 1.0);
  }
  else if(0.4<distvalccw && distvalccw<=0.45)
  {
    SetMotor(left, -0.85);
    SetMotor(right, 1.0);
  }
  else if(0.6>distvalccw && distvalccw>=0.55)
  {
    SetMotor(left, 1.0);
    SetMotor(right, -0.85);
  }
  else if(0.65>distvalccw && distvalccw>=0.6)
  {
    SetMotor(left, 1.0);
    SetMotor(right, -0.75);
  }
  else if(distvalccw>=0.65)
  {
    SetMotor(left, 1.0);
    SetMotor(right, -0.55);
  }
  else
  {
    SetMotor(left, 1.0);
    SetMotor(right, 1.0);
  }
}
//void linesensormode(){}
// The 'main' function is the entry point of the program
int main(void){
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
    tMotor *left = InitializeServoMotor(PIN_D0, true);
    tMotor *right = InitializeServoMotor(PIN_D1, false);

    tADC *dist = InitializeADC(PIN_D2);
    float distvalcw;
    tADC *dist2 = InitializeADC(PIN_E1);
    float distvalccw;
    distvalcw = ADCRead(dist);
    //Printf("IR sensor value is %f\n", distvalcw);
    distvalccw = ADCRead(dist2);
    //Printf("hi");
    tLineSensor *line = InitializeGPIOLineSensor(PIN_B0, PIN_B1, PIN_E4, PIN_E5, PIN_B4, PIN_A5, PIN_A6, PIN_A7);
    float linevals[8];
    float tavg=100;
    int counter=0;
    float avga;

    float initval;
    float error;
    float errorp;
    float integ;
    float deriv;

    int clockwisem;
    float goalw=0.6;
    float werror;
    float werrorp;
    float integw;
    float derivw;
    float output;
    if(distvalcw>distvalccw)
    { CallEvery(blinky, 0, 0.5);
      //determine if robot is in right spot??
      clockwisem=1;
    }
    else
    {CallEvery(blinkys, 0, 0.5);
      clockwisem=0;
    }
    while (1) {

        // Runtime code can go here
        //SetMotor(left, 1.0);
        //SetMotor(right, -1.0);
        LineSensorReadArray(line, linevals);
        //Printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t\n", linevals[0], linevals[1], linevals[2], linevals[3], linevals[4], linevals[5], linevals[6], linevals[7]);
        distvalcw = ADCRead(dist);

        distvalccw = ADCRead(dist2);
        Printf("%f\t%f,\n", distvalcw,distvalccw);
        float ravg=linefollowfindravg(linevals);//ravg gets real value of line sensors with multiplier
        //float avg=linefollowfindavg(linevals);
        if(tavg>=100)
         {tavg=ravg;
           initval=ravg;
         }
        //tavg=previous input
        //ravg=this input
        /*avgs[counter]=avg;
        counter++;
        if (counter>=5)
         counter=0;
        avga=0;
        for(int i=0;i<5;i++)
        {
          avga+=avgs[i];
        }
        avga=avga/5;
*/
      //  linefollow(left,right,avga);
      error=ravg-initval;
      integ=integ+error;
      deriv=error-errorp;
        linerfollow(left,right,error,deriv,integ);

        tavg=ravg;
        errorp=error;
        //Printf("IR sensor value is %f\n", distvalccw);

        if(clockwisem==1)
        {   werror=distvalcw-goalw;
          integw=integw+error;
          derivw=werrorp-werror;
          werrorp=werror;
          output=1.0*werror+1.8*derivw;
          //clockwisemode(left,right,output);
        }
        else
        {
          //  counterclockwisemode(left,right,distvalccw);
        }
        /*if(0.8>distval>0.5)
        {SetMotor(left, 1.0);
        SetMotor(right, -0.8);}
        else if (0.95<distval)
        {SetMotor(left, 0.5);
        SetMotor(right, -1.0);}
        else if (0.5>distval>0)
        {SetMotor(left,1.0);
        SetMotor(right,-0.5);}*/



    }
}
