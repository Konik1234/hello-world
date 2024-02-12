#include <Wire.h>
#include <Stepper.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <AccelStepper.h>
// Pinout
#define diEncA 2
#define diEncB 3
#define doMotorStep 5
#define doMotorDir 4
#define doStatusLED 13
// pos 0, enc negative
// Encoder variables
double encRotation;
Encoder Encoder0(diEncA , diEncB);

// PID regulator variables
float Kp, Ki, Kd; //parameters


float PastValueDeviation; //ostatnie odchylenie wyniku
float ValueErrorSum; //suma błedów

double OutputSignal; //sygnał wyjściowy
double Setpoint =0;
unsigned long pidActTime, pidPrevTime;
const int pidSamplingTime = 100; //ms 10Hz

// Stepper motor controll variables
#define STEPS_PER_REV             200
int smVelocity; //predkość
const int smMaxSpeed = 1000;
AccelStepper stepper1 (1, 5, 4); //declaration of yours stepper motor steps,
#define MAX_DISTANCE 640
#define RAMP_MIN 200
#define RAMP_MAX (MAX_DISTANCE - RAMP_MIN)
float KP  = 60;
float KI = 500;
float KD = 1;

int16_t angleZeroOffset = 0;
float Kd_cart = 10;

PID pendulumPID(&encRotation,&OutputSignal,&Setpoint,KP,KI,KD,DIRECT);

void setup() {
  pinMode(doStatusLED, OUTPUT);
  Serial.begin(9600);
  stepper1.setAcceleration(7500);//set acceleration per second^2
  //Serial.println("Reverse pendulum V0.1");

  SetPIDPar(KP, KI, KD);
  Serial.println("angle speed");
  pidPrevTime = millis();

  pendulumPID.SetMode(AUTOMATIC);
  pendulumPID.SetTunings(KP,KI,KD);
}

void loop() {

  pidActTime = millis();
  //encRotation = Encoder0.read()%1600;

  if ((pidActTime - pidPrevTime) >= pidSamplingTime)
  {
    pidPrevTime = pidActTime;
    SerialMenu();

    encRotation = Encoder0.read() + angleZeroOffset;

    // cart p
    int16_t positionCentered = (int16_t)stepper1.currentPosition() - MAX_DISTANCE / 2;
    int16_t cartP = positionCentered / (MAX_DISTANCE / 20);
    // cart d use velocity directly instead of calculating from position delta
    float cartSpeed = smVelocity / smMaxSpeed * Kd_cart;
    float cartD = constrain(cartSpeed, -10, 10);
    encRotation += cartP + cartD;// add PD terms to angle reading to cascade pids
    Serial.print(encRotation * 10);

    
    pendulumPID.Compute();
    //PIDRegulator();
    pendulumPID.SetOutputLimits(-smMaxSpeed,smMaxSpeed);
    smVelocity = OutputSignal; //SignalToVelocity(OutputSignal, smMaxSpeed, -smMaxSpeed);
    // disable pid if angle too high
    if (abs(encRotation) > 200) { //je
      smVelocity = 0;
    }
    Serial.print(' ');
    Serial.print(smVelocity);
    Serial.println();
    MoveStepperMotor2(smVelocity);
  }
  stepper1.run();
}

void SetPIDPar(float KP, float KI, float KD)
{
  Kp = KP;
  Ki = KI;
  Kd = KD;
}

void PIDRegulator()
{
  float p, i, d;    //calculation variables
  float ValueDeviation;
  float ValueError;
  
  PastValueDeviation = ValueDeviation;
  ValueDeviation = encRotation;

  //p
  p = Kp * ValueDeviation;

  //i
  if (OutputSignal > smMaxSpeed || OutputSignal < -smMaxSpeed) ValueErrorSum = ValueErrorSum * 0.99f;
  else ValueErrorSum += ValueDeviation;
  if (abs(ValueErrorSum) > 10000 ) ValueErrorSum -= ValueDeviation;
  i = Ki * pidSamplingTime * ValueErrorSum / 1000.0f;

  //d
  ValueError = ValueDeviation - PastValueDeviation;
  d = (Kd * 1000.0f / pidSamplingTime) * ValueError;

  //signal
  OutputSignal = p + i + d;
}

int SignalToVelocity(int Signal, int MaxVel, int MinVel)
{
  if (Signal > MaxVel) Signal = MaxVel;
  if (Signal < MinVel) Signal = MinVel;
  return Signal;
}


void MoveStepperMotor2(int Velocity) {
  int velAbs = abs(Velocity);
  stepper1.setMaxSpeed(velAbs); //set speed in step/second
  if (Velocity > 0) {
    stepper1.moveTo(MAX_DISTANCE);
  } else {
    stepper1.moveTo(0);
  }
}

void SerialMenu() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    bool set = false;
    switch (cmd) {
      case 'p':
        KP = Serial.parseFloat();
        set = true;
        break;
      case 'i':
        KI = Serial.parseFloat();
        set = true;
        break;
      case 'd':
        KD = Serial.parseFloat();
        set = true;
        break;
      case 'D':
        Kd_cart = Serial.parseFloat();
        break;
      case '+':
        angleZeroOffset++;
        break;
      case '-':
        angleZeroOffset--; //  to motor side
        break;
      default:
        break;
    }
    if (set) {
      SetPIDPar(KP, KI, KD);
      //        Serial.print("kp: ");
      //        Serial.print(KP);
      //        Serial.print(" ki: ");
      //        Serial.print(KI);
      //        Serial.print(" kd: ");
      //        Serial.print(KD);
      //        Serial.println();
      //        delay(1000);
    }
  }
}
