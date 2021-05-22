#include <Servo.h>
#include <Ultrasonic.h>

Servo servoflag;

//Pour définir les Pins de Moteur, Led, Servo moteur, et bouton.
#define MOTOR_LEFT_VITESSE 6
#define MOTOR_LEFT_SENS 7
#define MOTOR_RIGHT_SENS 4
#define MOTOR_RIGHT_VITESSE 5

#define LED_PIN 12

#define SERVO_MOTEUR_PIN 8

#define BUTTON_PIN 2

// Pour définir les pins de capteur ultrason.
Ultrasonic sensor_left(A0);
Ultrasonic sensor_front(A2);
Ultrasonic sensor_right(A3);

// Pour définir la vitèsse.
// la valeur de la vitèsse est entre 0, 255
int SPEED_LINE = 180;
int SPEED_TURN_PETIT_LOW = 160;
int SPEED_TURN_PETIT_HIGH = 240;
int SPEED_TURN_GRANDE_LOW = 155;
int SPEED_TURN_GRANDE_HIGH = 240;

int SPEED_ADJUST_LINE = 200;
int SPEED_TRIG = 0;

// Initialisation du temps de delay.
int TIME_TURN = 2000;
int TIME_ADJUST = 1000;

int buttonState = 0;

int angle = 0;

int COUNT = 0;

float VITESSE_CM_PAR_SECOND = 340.00;

bool premiereCliquer = false;
bool deuxiemeCliquer = false;

// Initialisation de la valeur sortie(Output).
int error = 0;

// le tableau de 3 capteurs.
int sensor[3] = {0, 0, 0};

// Lire la première valeur du capteur.
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
    if (buttonState == LOW)  // Pour compter le nombre d'appuie sur le bouton.
    {
      COUNT += 1;
      delay(500);
      break;
    }

    else if (COUNT == 1) //Etat 1 initialisation.
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
    else if (COUNT == 2) //Etat 2 Moteur commence à fonctionner.
    {
      digitalWrite(LED_PIN, HIGH);
      premiereCliquer = false;
      deuxiemeCliquer = true;
      Serial.println("Button Pressed 2");
      break;
    }
    else if (COUNT == 3) retourner au Etat 0.
    {
      COUNT = 0;
      premiereCliquer = false;
      deuxiemeCliquer = false;
      Serial.println("Initializing");
    }
    break;
  }

  Serial.println(COUNT); //Initialisation de moteur et servo moteur.
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

    // Lire la valeur de capteur.
    value = getSensorValue();
    switch (value)
    {
      // Avancer tout droit.
      case 0:
        aller();
        Serial.println("Aller!");
        break;


      // Tourner fortement à droite.
      case 2:
        aGrandeDroite();
        Serial.println("Grande Droite");
        break;

      // Tourner légèrement à droite.
      case 1:
        aPetitDroite();
        Serial.println("Petit Droite");
        break;

      // Tourner légèrement à gauche.
      case -1:
        aPetitGauche();
        Serial.println("Petit Gauche");
        break;

      case -2:
        // Tourner fortement à gauche.
        aGrandeGauche();
        Serial.println("Grande Gauche");
        break;

      // Jeter le drapeau.
      case 4:
        servoMoteurLancerDeapeau();
        _stop();
        Serial.println("Lancer Deapeau");
        COUNT = 3;
        //        exit(1);
        break;

      // Arret
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

// Initialisation de model de capteur.

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

//Initialisation de bouton.

void buttonInitPin()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

// Initialisation de servo moteur.
void servoMoteurPinInit()
{
  servoflag.attach(SERVO_MOTEUR_PIN);
}

// Initialisation model de moteur.
void motorPinInit()
{
  pinMode(MOTOR_LEFT_SENS, OUTPUT);
  pinMode(MOTOR_RIGHT_SENS, OUTPUT);
  pinMode(MOTOR_LEFT_VITESSE, 0);
  pinMode(MOTOR_RIGHT_VITESSE, 0);
  digitalWrite(MOTOR_LEFT_SENS, LOW);
  digitalWrite(MOTOR_RIGHT_SENS, HIGH);
}

// Fonction de servo moteur.
// Principe: Jeter le drapeau.

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

// Fonction d'arret
// Principe: Arreter le véhicule
void _stop()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TRIG);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TRIG);
}

// Fonction d'aller tout droit.
// Principe: Faire le véhicule avancer tout droit.
void aller()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_LINE);
}

// Fonction de tourner légèrement à gauche.
// Principe: Faire le véhicule tourner à gauche, puis ralentir pendant un certain temps.
void aPetitGauche()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_PETIT_LOW);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_PETIT_HIGH);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Fonction de tourner fortement à gauche.
// Principe: Faire le véhicule tourner fortement à gauche, puis ralentir pendant un certain temps.
void aGrandeGauche()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_LOW);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_HIGH);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}



// Fonction de tourner légèrement à droite.
// Principe: Faire le véhicule tourner à droite, puis ralentir pendant un certain temps.
void aPetitDroite()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_PETIT_HIGH);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_PETIT_LOW);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Fonction de tourner fortement à droite.
// Principe: Faire le véhicule tourner fortement à droite, puis ralentir pendant un certain temps.
void aGrandeDroite()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_HIGH);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_LOW);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Fonction de lire les valeur des capteurs.
int getSensorValue()
{
  // Lire le signal de capteur.
  sensor[0] = sensorValueLeft();
  sensor[1] = sensorValueRight();
  sensor[2] = sensorValueFront();

  if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0)
  {
    error = 0; // 110 Aller tout droit
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