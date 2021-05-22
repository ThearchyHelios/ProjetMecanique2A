#include <Servo.h>
#include <Ultrasonic.h>

Servo servoflag;

// 一共两个马达， 两个传感器，前轮两个珠子 xD，后轮各两个马达

// Define Motor Pin
#define MOTOR_LEFT_VITESSE 6
#define MOTOR_LEFT_SENS 7
#define MOTOR_RIGHT_SENS 4
#define MOTOR_RIGHT_VITESSE 5

#define LED_PIN 12

#define SERVO_MOTEUR_PIN 8

#define BUTTON_PIN 2

// Define Sensor Pin
Ultrasonic sensor_left(A0);
Ultrasonic sensor_front(A2);
Ultrasonic sensor_right(A3);

// Define Speed
// PWN sont entre 0, 255
int SPEED_LINE = 180;
int SPEED_TURN_PETIT_LOW = 160;
int SPEED_TURN_PETIT_HIGH = 240;
int SPEED_TURN_GRANDE_LOW = 155;
int SPEED_TURN_GRANDE_HIGH = 240;

int SPEED_ADJUST_LINE = 200;
int SPEED_TRIG = 0;

// Init delay temp
int TIME_TURN = 2000;
int TIME_ADJUST = 1000;

int buttonState = 0;

int angle = 0;

int COUNT = 0;

float VITESSE_CM_PAR_SECOND = 340.00;

bool premiereCliquer = false;
bool deuxiemeCliquer = false;

// Init output value
int error = 0;

// A tableau with 2 sensors
int sensor[3] = {0, 0, 0};

// Read Sensors' first value
int getSensorValue(void);

void setup()
{
  // put your setup code here, to run once:
  // Set port 9600
  Serial.begin(9600);
  buttonInitPin();
  Serial.println("Setup successful");

}

void loop()
{
  Serial.println("IN Loop");
  buttonState = digitalRead(BUTTON_PIN);


  while (true)
  {
    if (buttonState == LOW)
    {
      COUNT += 1;
      delay(500);
      break;
    }

    else if (COUNT == 1)
    {
      premiereCliquer = true;
      deuxiemeCliquer = false;
      Serial.println("Button Pressed 1");
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
      delay(50);
      break;
    }
    else if (COUNT == 2)
    {
      digitalWrite(LED_PIN, HIGH);
      premiereCliquer = false;
      deuxiemeCliquer = true;
      Serial.println("Button Pressed 2");
      break;
    }
    else if (COUNT == 3)
    {
      COUNT = 0;
      premiereCliquer = false;
      deuxiemeCliquer = false;
      Serial.println("Initializing");
    }
    break;
  }

  Serial.println(COUNT);
  while (premiereCliquer)
  {
    // Init moteur
    motorPinInit();
    servoMoteurPinInit();
    break;
  }
  while (deuxiemeCliquer)
  {
    // put your main code here, to run repeatedly:
    static int value = 0;
    error = 0;

    // Read sensor value
    value = getSensorValue();
    switch (value)
    {
      // Move forward
      case 0:
        aller();
        Serial.println("Aller!");
        break;


      // Big Right
      case 2:
        aGrandeDroite();
        Serial.println("Grande Droite");
        break;

      // Move right
      case 1:
        aPetitDroite();
        Serial.println("Petit Droite");
        break;

      // Move left
      case -1:
        aPetitGauche();
        Serial.println("Petit Gauche");
        break;

      case -2:
        // Big left
        aGrandeGauche();
        Serial.println("Grande Gauche");
        break;

      // Lancer Deapeau
      case 4:
        servoMoteurLancerDeapeau();
        _stop();
        Serial.println("Lancer Deapeau");
        COUNT = 3;
        //        exit(1);
        break;

      // Stop
      default:
        aller();
        Serial.println("Aller! Default");
        break;
    }
    if (COUNT == 3) {
      break;
    }

  }
}

// Init sensor model

int sensorValueLeft()
{
  long distance;
  distance = sensor_left.MeasureInCentimeters();
  delay(10);

  if (distance > 15)
  {
    return 0;
  }
  else
  {
    Serial.println("Sensor Left has value 1");
    return 1;
  }
}

int sensorValueRight()
{
  long distance;
  distance = sensor_right.MeasureInCentimeters();
  delay(10);

  if (distance > 15)
  {
    return 0;
  }
  else
  {
    Serial.println("Sensor Right has value 1");
    return 1;
  }
}

int sensorValueFront()
{
  long distance;
  distance = sensor_front.MeasureInCentimeters();
  delay(10);

  if (distance > 25)
  {
    return 0;
  }
  else
  {
    Serial.println("Sensor Front has value 1");
    return 1;
  }
}

//Init Button moteur

void buttonInitPin()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

// Init servo moteur
void servoMoteurPinInit()
{
  servoflag.attach(SERVO_MOTEUR_PIN);
}

// Init motor model
void motorPinInit()
{
  pinMode(MOTOR_LEFT_SENS, OUTPUT);
  pinMode(MOTOR_RIGHT_SENS, OUTPUT);
  pinMode(MOTOR_LEFT_VITESSE, 0);
  pinMode(MOTOR_RIGHT_VITESSE, 0);
  digitalWrite(MOTOR_LEFT_SENS, LOW);
  digitalWrite(MOTOR_RIGHT_SENS, HIGH);
}

// Function servo moteur
// Feature: Lancer le drapeau.

void servoMoteurLancerDeapeau()
{

  analogWrite(MOTOR_LEFT_VITESSE, 100);
  analogWrite(MOTOR_RIGHT_VITESSE, 100);

  for (angle = 0; angle <= 20; angle += 1) {
    servoflag.write(angle);
    delay(15);
  }
  delay(1000);


}

// Function stop
// Feature: Stop the vehicle
void _stop()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TRIG);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TRIG);
}

// Function aller
// Feature: Make vehicle move forward
void aller()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_LINE);
}

// Function aPetitGauche
// Feature: Make vehicle turn left, then slow down for a moment
void aPetitGauche()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_PETIT_LOW);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_PETIT_HIGH);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Function aGrandeGauche
// Feature: Make vehicle turn sharp left, then slow down for a moment
void aGrandeGauche()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_LOW);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_HIGH);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}



// Function aPetitDroite
// Feature: Make vehicle turn right, then slow down for a moment
void aPetitDroite()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_PETIT_HIGH);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_PETIT_LOW);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Function aGrandeDroite
// Feature: Make vehicle turn big right, then slow down for a moment
void aGrandeDroite()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_HIGH);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_LOW);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Function aSharpDroite
// Feature: Make vehicle turn sharp right, then slow down for a moment

// Function Read Sensors' Values
int getSensorValue()
{
  // Read semsors' signals
  sensor[0] = sensorValueLeft();
  sensor[1] = sensorValueRight();
  sensor[2] = sensorValueFront();

  if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0)
  {
    error = 0; // 110 Aller
  }
  else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 1)
  {
    error = -2; // 011 Grande Gauche
  }
  else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 1)
  {
    error = 2; // 101 Grande Droite
  }
  else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 1)
  {
    error = 0; // 001 Arreter (Error mais Aller)
  }
  else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 0)
  {
    error = 1; //100 Petit Droite
  }
  else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 0)
  {
    error = -1; //010 Petit Gauche
  }
  //    else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0)
  //    {
  //        if (error == -2)
  //        {
  //            error = -3; // Sharp Gauche
  //        }
  //        else
  //        {
  //            error = 3; // Sharp Droite
  //        }
  //    }

  else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
  {
    delay(1000);
    sensor[0] = sensorValueLeft();
    sensor[1] = sensorValueRight();
    sensor[2] = sensorValueFront();

    if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
    {
      error = 4;
    }
  }


  return error;
}
