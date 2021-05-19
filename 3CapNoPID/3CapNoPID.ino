#include <Servo.h>
#include <Ultrasonic.h>

Servo servoflag;

// 一共两个马达， 两个传感器，前轮两个珠子 xD，后轮各两个马达

// Define Motor Pin
#define MOTOR_LEFT_VITESSE 6
#define MOTOR_LEFT_SENS 7
#define MOTOR_RIGHT_SENS 4
#define MOTOR_RIGHT_VITESSE 5

#define SERVO_MOTEUR_PIN 5

#define BUTTON_PIN 5

// Define Sensor Pin
Ultrasonic sensor_left(A0);
Ultrasonic sensor_front(A1);
Ultrasonic sensor_right(A3);

// Define Speed
// PWN sont entre 0, 255
int SPEED_LINE = 60;
int SPEED_TURN_PETIT_LOW = 30;
int SPEED_TURN_PETIT_HIGH = 60;
int SPEED_TURN_GRANDE_LOW = 20;
int SPEED_TURN_GRANDE_HIGH = 80;
int SPEED_TURN_SHARP_LOW = 0;
int SPEED_TURN_SHARP_HIGH = 100;

int SPEED_ADJUST_LINE = 20;
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
}

void loop()
{
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH)
    {
        COUNT += 1;
    }

    while (true)
    {
        if (COUNT == 1)
        {
            premiereCliquer = true;
            deuxiemeCliquer = false;
            return;
        }
        if (COUNT == 2)
        {
            premiereCliquer = false;
            deuxiemeCliquer = true;
            return;
        }
    }
    while (premiereCliquer)
    {
        // Init moteur
        motorPinInit();
        servoMoteurPinInit();
    }
    while (deuxiemeCliquer)
    {
        // put your main code here, to run repeatedly:
        static int value = 0;

        // Read sensor value
        value = getSensorValue();
        switch (value)
        {
            // Move forward
        case 0:
            aller();
            break;

            //Sharp Right
        case -3:
            aSharpDroite();
            break;

            // Big Right
        case -2:
            aGrandeDroite();
            break;

            // Move right
        case -1:
            aPetitDroite();
            break;

            // Move left
        case 1:
            aPetitGauche();
            break;

        case 2:
            // Big left
            aGrandeGauche();
            break;

            // Sharp left
        case 3:
            aSharpGauche();
            break;
            // Lancer Deapeau
        case 4:
            servoMoteurLancerDeapeau();
            break;

            // Stop
        default:
            _stop();
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
        return 1;
    }
}

//Init Button moteur

void buttonInitPin()
{
    pinMode(BUTTON_PIN, INPUT);
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
}

// Function servo moteur
// Feature: Lancer le drapeau.

void servoMoteurLancerDeapeau()
{
    for (angle = 0; angle < 60; angle += 3)
    {
        servoflag.write(angle);
        delay(15);
    }
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
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_LINE);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_LINE);
}

// Function aPetitGauche
// Feature: Make vehicle turn left, then slow down for a moment
void aPetitGauche()
{
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
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
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_LOW);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_HIGH);

    delay(TIME_TURN);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
    delay(TIME_ADJUST);
}

// Function aSharpGauche
// Feature: Make vehicle turn big left, then slow down for a moment
void aSharpGauche()
{
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_SHARP_LOW);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_SHARP_HIGH);

    delay(TIME_TURN);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
    delay(TIME_ADJUST);
}

// Function aPetitDroite
// Feature: Make vehicle turn right, then slow down for a moment
void aPetitDroite()
{
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
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
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_HIGH);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_LOW);

    delay(TIME_TURN);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
    delay(TIME_ADJUST);
}

// Function aSharpDroite
// Feature: Make vehicle turn sharp right, then slow down for a moment
void aSharpDroite()
{
    digitalWrite(MOTOR_LEFT_SENS, HIGH);
    digitalWrite(MOTOR_RIGHT_SENS, HIGH);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_SHARP_HIGH);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_SHARP_LOW);

    delay(TIME_TURN);
    analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
    analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
    delay(TIME_ADJUST);
}

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
    else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
    {
        error = 0; // 111 Arreter (Error mais Aller)
    }
    else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 0)
    {
        error = 1; //100 Petit Droite
    }
    else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 0)
    {
        error = -1; //010 Petit Gauche
    }
    else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0)
    {
        if (error == -2)
        {
            error = -3; // Sharp Gauche
        }
        else
        {
            error = 3; // Sharp Droite
        }
    }
    else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
    {
        delay(100);
        if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
        {
            error = 4;
        }
    }
    return error;
}
