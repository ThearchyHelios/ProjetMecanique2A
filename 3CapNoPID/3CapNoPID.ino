// 一共两个马达， 两个传感器，前轮两个珠子 xD，后轮各两个马达

// Define Motor Pin
#define left_motor_pin 5
#define right_motor_pin 6

// Define Sensor Pin
#define left_sensor_pin 3
#define right_sensor_pin 4
#define front_sensor_pin 2

void motorPinInit();

// Create movement function
void go();
void _stop();
void turnLeft();
void turnRight();

// Init output value
int error = 0;

// Init delay temp
float tmp = 2;

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

        // Move right
    case -1:
        aDroite();
        break;

        // Move left
    case 1:
        aGauche();
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
    pinMode(left_sensor_pin, INPUT);
    pinMode(right_sensor_pin, INPUT);
    pinMode(front_sensor_pin, INPUT);
}

// Init motor model
void motorPinInit()
{
    pinMode(left_motor_pin, OUTPUT);
    pinMode(right_motor_pin, OUTPUT);
}

// Function stop
// Feature: Stop the vehicle
void _stop()
{
    analogWrite(left_motor_pin, 0);
    analogWrite(right_motor_pin, 0);
}

// Function aller
// Feature: Make vehicle move forward
void aller()
{
    analogWrite(left_motor_pin, 50);
    analogWrite(right_motor_pin, 50);
}

// Function aGauche
// Feature: Make vehicle turn left, then slow down for a moment
void aGauche()
{
    analogWrite(left_motor_pin, 30);
    analogWrite(right_motor_pin, 60);

    delay(tmp);
    analogWrite(left_motor_pin, 20);
    analogWrite(right_motor_pin, 20);
    delay(tmp);
}

// Function aDroite
// Feature: Make vehicle turn right
void aDroite()
{
    analogWrite(left_motor_pin, 60);
    analogWrite(right_motor_pin, 30);

    delay(tmp);
    analogWrite(left_motor_pin, 20);
    analogWrite(right_motor_pin, 20);
    delay(tmp);
}

// Function Read Sensors' Values
int getSensorValue()
{
    // Read semsors' signals
    sensor[0] = digitalRead(left_sensor_pin);
    sensor[1] = digitalRead(right_sensor_pin);
    if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0)
    {
        error = 0; // 110 Aller
    }
    else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 1)
    {
        error = -1; // 011 Droite
    }
    else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 1)
    {
        error = 1; // 101 Gauche
    }
    else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 1)
    {
        error = 0; // 001 Arreter 
    }
    else{
        error = 0;
    }
    return error;
}