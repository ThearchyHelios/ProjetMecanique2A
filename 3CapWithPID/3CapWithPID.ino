// 一共两个马达， 两个传感器，前轮两个珠子 xD，后轮各两个马达

// Define Motor Pin
#define MOTOR_LEFT_PIN 5
#define MOTOR_RIGHT_PIN 6

// Define Sensor Pin
#define SENSOR_LEFT_PIN_INPUT 3
#define SENSOR_LEFT_PIN_OUTPUT 4
#define SENSOR_RIGHT_PIN_INPUT 5
#define SENSOR_RIGHT_PIN_OUTPUT 6
#define SENSOR_FRONT_PIN_INPUT 7
#define SENSOR_FRONT_PIN_OUTPUT 8

// Define Speed
#define SPEED_LINE 60
#define SPEED_TURN_PETIT_LOW 30
#define SPEED_TURN_PETIT_HIGH 60
#define SPEED_TURN_GRANDE_LOW 20
#define SPEED_TURN_GRANDE_HIGH 80
#define SPEED_TURN_SHARP_LOW 0
#define SPEED_TURN_SHARP_HIGH 100

#define SPEED_ADJUST_LINE 20
#define SPEED_TRIG 0

// Init delay temp
#define TIME_TURN 2000
#define TIME_ADJUST 1000

float VITESSE_CM_PAR_SECOND = 340.00;

float Kp = 10, Ki = 0.5, Kd = 0;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;

void motorPinInit();

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

    // Init sensor
    sensorPinInit();

    // Init motor
    motorPinInit();
}

void loop()
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

        // Stop
    default:
        _stop();
        break;
    }
}

// Init sensor model
void sensorPinInit()
{
    pinMode(SENSOR_LEFT_PIN_INPUT, INPUT);
    pinMode(SENSOR_LEFT_PIN_OUTPUT, OUTPUT);
    pinMode(SENSOR_RIGHT_PIN_INPUT, INPUT);
    pinMode(SENSOR_RIGHT_PIN_OUTPUT, OUTPUT);
    pinMode(SENSOR_FRONT_PIN_INPUT, INPUT);
    pinMode(SENSOR_FRONT_PIN_OUTPUT, OUTPUT);
}

int sensorValueLeft()
{
    digitalWrite(SENSOR_LEFT_PIN_OUTPUT, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_LEFT_PIN_OUTPUT, LOW);

    long tmp;
    tmp = pulseIn(SENSOR_LEFT_PIN_INPUT, HIGH);
    tmp = tmp / 2;

    // cm
    float distance;
    distance = tmp * VITESSE_CM_PAR_SECOND / 10000;

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
    digitalWrite(SENSOR_RIGHT_PIN_OUTPUT, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_RIGHT_PIN_OUTPUT, LOW);

    long tmp;
    tmp = pulseIn(SENSOR_RIGHT_PIN_INPUT, HIGH);
    tmp = tmp / 2;

    float distance;
    distance = tmp * VITESSE_CM_PAR_SECOND / 10000;

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
    digitalWrite(SENSOR_FRONT_PIN_OUTPUT, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_FRONT_PIN_OUTPUT, LOW);

    long tmp;
    tmp = pulseIn(SENSOR_FRONT_PIN_INPUT, HIGH);
    tmp = tmp / 2;

    float distance;
    distance = tmp * VITESSE_CM_PAR_SECOND / 10000;

    if (distance > 15)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
// Init motor model
void motorPinInit()
{
    pinMode(MOTOR_LEFT_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_PIN, OUTPUT);
}

// Function stop
// Feature: Stop the vehicle
void _stop()
{
    analogWrite(MOTOR_LEFT_PIN, SPEED_TRIG);
    analogWrite(MOTOR_RIGHT_PIN, SPEED_TRIG);
}

void calc_pid()
{
  P = error;
  I = I + error;
  D = error - previous_error;
 
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
 
  previous_error = error;

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
    return error;
}