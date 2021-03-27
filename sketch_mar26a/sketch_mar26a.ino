// 一共两个马达， 两个传感器，前轮两个珠子 xD，后轮各两个马达

#define left_motor_pin 5
#define right_motor_pin 6

#define left_sensor_pin 3
#define right_sensor_pin 4

void motorPinInit();
void go();
void _stop();
void turnLeft();
void turnRight();

int error = 0;
int sensor[2] = {0, 0};
int getSensorValue(void);

void setup()
{
  // put your setup code here, to run once:
  sensorPinInit();
  motorPinInit();
}

void loop()
{
  // put your main code here, to run repeatedly:
  static int value = 0;
  value = getSensorValue();
  switch (value)
  {
  case 0:
    aller();
    break;
  case -1:
    aDroite();
    break;
  case 1:
    aGauche();
    break;
  default:
    _stop();
    break;
  }
}
void sensorPinInit()
{
  pinMode(left_sensor_pin, INPUT);
  pinMode(right_sensor_pin, INPUT);
}
void motorPinInit()
{
  pinMode(left_motor_pin, OUTPUT);
  pinMode(right_motor_pin, OUTPUT);
}
void _stop()
{
  analogWrite(left_motor_pin, 0);
  analogWrite(right_motor_pin, 0);
}
void aller()
{
  analogWrite(left_motor_pin, 50);
  analogWrite(right_motor_pin, 50);
}
void aGauche()
{
  analogWrite(left_motor_pin, 30);
  analogWrite(right_motor_pin, 60);
}
void aDroite()
{
  analogWrite(left_motor_pin, 60);
  analogWrite(right_motor_pin, 30);
}

int getSensorValue()
{
  sensor[0] = digitalRead(left_sensor_pin);
  sensor[1] = digitalRead(right_sensor_pin);
  if (sensor[0] == 0 && sensor[1] == 0)
  {
    error = 0; // Arreter
  }
  else if (sensor[0] == 0 && sensor[1] == 1)
  {
    error = -1; // Droite
  }
  else if (sensor[0] == 1 && sensor[1] == 0)
  {
    error = 1; // Gauche
  }
  else if (sensor[0] == 1 && sensor[1] == 1)
  {
    error = 0; // Aller
  }
  return error;
}