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

// Pour définir les pins des capteurs ultrasons.
Ultrasonic sensor_left(A0);
Ultrasonic sensor_front(A2);
Ultrasonic sensor_right(A3);

// Pour définir la vitesse.
// La valeur de la vitesse se trouve entre 0, 255.
int SPEED_LINE = 255;
int SPEED_TURN_PETIT_LOW = 115;
int SPEED_TURN_PETIT_HIGH = 245;
int SPEED_TURN_GRANDE_LOW = 30;
int SPEED_TURN_GRANDE_HIGH = 255;

int SPEED_ADJUST_LINE = 220;
int SPEED_TRIG = 0;

// Initialisation du temps de "delay".
int TIME_TURN = 2150;
int TIME_ADJUST = 850;

int buttonState = 0;

int angle = 0;

int COUNT = 0;

float VITESSE_CM_PAR_SECOND = 340.00; //On définit la vitesse des ondes sonores dans l'air.

bool premiereCliquer = false; // On définit par défaut les deux pressions boutons.
bool deuxiemeCliquer = false;

// Initialisation de la valeur de sortie(Output).
int error = 0;

// Le tableau de 3 capteurs.
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
    if (buttonState == LOW) // Pour compter le nombre d'appuie sur le bouton.
    {
      COUNT += 1;
      delay(500);
      break;
    }

    else if (COUNT == 1) //Etat 1 : initialisation.
    {
      premiereCliquer = true;
      deuxiemeCliquer = false;
      Serial.println("Button Pressed 1");
      digitalWrite(LED_PIN, HIGH); //La LED verte de la carte gestion de puissance s'allume pendant 50ms,
      delay(50);
      digitalWrite(LED_PIN, LOW); //Puis s'éteint.
      delay(50);
      break;
    }
    else if (COUNT == 2) //Etat 2 : Les moteurs commencent à fonctionner.
    {
      digitalWrite(LED_PIN, HIGH); //Allumage de la LED verte de la carte gestion de puissance.
      premiereCliquer = false;
      deuxiemeCliquer = true;
      Serial.println("Button Pressed 2");
      break;
    }
    else if (COUNT == 3) // On retourne à l'Etat 0 ici.
    {
      COUNT = 0;
      premiereCliquer = false;
      deuxiemeCliquer = false;
      Serial.println("Initializing");
    }
    break;
  }

  Serial.println(COUNT); //Initialisation des moteurs et du servomoteur.
  while (premiereCliquer)
  {
    motorPinInit();       //Initilisation moteur.
    servoMoteurPinInit(); //Initialisation servomoteur.
    break;
  }
  while (deuxiemeCliquer)
  {

    static int value = 0;
    error = 0;

    // On lit ici la valeur du capteur.
    value = getSensorValue();
    switch (value) //On a donc plusieurs possibilités de "direction" du robot en fonction de cette valeur.
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

    // Déployer le drapeau.
    case 4:
      servoMoteurLancerDrapeau();
      _stop();
      Serial.println("Lancer Drapeau");
      COUNT = 3;
      //        exit(1);
      break;

    // Arret
    default:
      aller();
      Serial.println("Aller! Default");
      break;
    }
    if (COUNT == 3)
    {
      break;
    }
  }
}

// Initialisation des 3 capteurs.

int sensorValueLeft() //Capteur de gauche.
{
  long distance;
  distance = sensor_left.MeasureInCentimeters();
  delay(10);
  //A partir d'un certaine disatnce mesurée, on va retourner soit la valeur 0 ou 1 pour ce capteur.
  if (distance > 35)
  {
    return 0;
  }
  else
  {
    Serial.println("Sensor Left has value 1");
    return 1;
  }
}

int sensorValueRight() //Capteur de droite.
{
  long distance;
  distance = sensor_right.MeasureInCentimeters();
  delay(10);
  //A partir d'un certaine disatnce mesurée, on va retourner soit la valeur 0 ou 1 pour ce capteur.
  if (distance > 35)
  {
    return 0;
  }
  else
  {
    Serial.println("Sensor Right has value 1");
    return 1;
  }
}

int sensorValueFront() //Capteur Avant.
{
  long distance;
  distance = sensor_front.MeasureInCentimeters();
  delay(10);
  //A partir d'un certaine disatnce mesurée, on va retourner soit la valeur 0 ou 1 pour ce capteur.
  if (distance > 45)
  {
    return 0;
  }
  else
  {
    Serial.println("Sensor Front has value 1");
    return 1;
  }
}

//Fonction initialisation du bouton.
void buttonInitPin()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

//Fonction initialisation du servomoteur.
void servoMoteurPinInit()
{
  servoflag.attach(SERVO_MOTEUR_PIN);
}

//Fonction initialisation des deux moteurs.
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

void servoMoteurLancerDrapeau()
{

  analogWrite(MOTOR_LEFT_VITESSE, 100);
  analogWrite(MOTOR_RIGHT_VITESSE, 100);

  for (angle = 0; angle <= 20; angle += 1) //Avec une inclinaison de 20° de la branche du servomoteur, on peut déployer la drapeau.
  {
    servoflag.write(angle);
    delay(15);
  }
  delay(1000);
}

// Fonction d'arret
// Principe : Arrêter le véhicule
void _stop()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TRIG);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TRIG);
}

// Fonction pour aller tout droit.
// Principe : Faire avancer le véhicule tout droit.
void aller()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_LINE);
}

// Fonction pour tourner légèrement à gauche.
// Principe : Faire tourner le véhicule à gauche, puis ralentir pendant un certain temps.
void aPetitGauche()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_PETIT_LOW);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_PETIT_HIGH);

  delay(TIME_TURN);
  // analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  // analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  // delay(TIME_ADJUST);
}

// Fonction pour tourner fortement à gauche.
// Principe : Faire tourner le véhicule fortement à gauche, puis ralentir pendant un certain temps.
void aGrandeGauche()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_LOW);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_HIGH);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Fonction pour tourner légèrement à droite.
// Principe : Faire tourner le véhicule à droite, puis ralentir pendant un certain temps.
void aPetitDroite()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_PETIT_HIGH);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_PETIT_LOW);

  delay(TIME_TURN);
  // analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  // analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  // delay(TIME_ADJUST);
}

// Fonction pour tourner fortement à droite.
// Principe : Faire tourner le véhicule fortement à droite, puis ralentir pendant un certain temps.
void aGrandeDroite()
{
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_TURN_GRANDE_HIGH);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_TURN_GRANDE_LOW);

  delay(TIME_TURN);
  analogWrite(MOTOR_LEFT_VITESSE, SPEED_ADJUST_LINE);
  analogWrite(MOTOR_RIGHT_VITESSE, SPEED_ADJUST_LINE);
  delay(TIME_ADJUST);
}

// Fonction qui permet de lire les valeurs des capteurs.
int getSensorValue()
{
  // On lit le signal des capteurs.
  sensor[0] = sensorValueLeft();  //Valeur capteur gauche.
  sensor[1] = sensorValueRight(); //Valeur capteur droite.
  sensor[2] = sensorValueFront(); //Valeur capteur avant.

  if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 0)
  {
    error = 0; // 110 Aller tout droit..
  }
  else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 1)
  {
    delay(100);
    sensor[0] = sensorValueLeft();
    sensor[1] = sensorValueRight();
    sensor[2] = sensorValueFront();
    //On recommence ici à retourner les valeurs détectées par les capteur 1 seconde après, pour voir si au moins un des capteurs retourne 0 (donc pas d'arrêt) ou s'il retourne tous 1, en l'occurence le robot s'arrêtera.
    if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 1)
    {
      error = -2;
    } // 011 Grande Gauche.
  }
  else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 1)
  {
    delay(100);
    sensor[0] = sensorValueLeft();
    sensor[1] = sensorValueRight();
    sensor[2] = sensorValueFront();
    //On recommence ici à retourner les valeurs détectées par les capteur 1 seconde après, pour voir si au moins un des capteurs retourne 0 (donc pas d'arrêt) ou s'il retourne tous 1, en l'occurence le robot s'arrêtera.
    if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 1)
    {
      error = 2;
    } // 101 Grande Droite.
  }
  else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 1)
  {
    error = 0; // 001 Arrêter (Error mais Aller).
  }
  else if (sensor[0] == 1 && sensor[1] == 0 && sensor[2] == 0)
  {
    error = 1; //100 Petite Droite.
  }
  else if (sensor[0] == 0 && sensor[1] == 1 && sensor[2] == 0)
  {
    error = -1; //010 Petite Gauche.
  }
  //Fonction pour l'arrêt.
  else if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
  {
    delay(1000);
    sensor[0] = sensorValueLeft();
    sensor[1] = sensorValueRight();
    sensor[2] = sensorValueFront();
    //On recommence ici à retourner les valeurs détectées par les capteur 1 seconde après, pour voir si au moins un des capteurs retourne 0 (donc pas d'arrêt) ou s'il retourne tous 1, en l'occurence le robot s'arrêtera.
    if (sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1)
    {
      error = 4;
    }
  }

  return error;
}
