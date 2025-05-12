#include "sac21.h"
#include <Arduino.h>
#include <Wire.h>      // Inclut la bibliothèque pour la communication I2C
#include "rgb_lcd.h"   // Inclut la bibliothèque pour l'écran LCD RGB
#include "Servo.h"     // Inclut la bibliothèque pour le servomoteur
#include <math.h>      // Inclut la bibliothèque pour les fonctions mathématiques
// Définition des objets
Servo servo;
rgb_lcd lcd;

Servo servo;                        // Déclare un objet servo pour contrôler le servomoteur
rgb_lcd lcd;                         // Déclare un objet LCD pour contrôler l'écran RGB

// Définition des broches pour les capteurs à ultrasons
const uint8_t TRIGGER_PIN1 = 9;     // Broche TRIGGER pour le capteur 1
const uint8_t ECHO_PIN1 = 10;       // Broche ECHO pour le capteur 1
const uint8_t TRIGGER_PIN2 = 11;    // Broche TRIGGER pour le capteur 2
const uint8_t ECHO_PIN2 = 12;       // Broche ECHO pour le capteur 2
const uint8_t TRIGGER_PIN3 = 3;     // Broche TRIGGER pour le capteur 3
const uint8_t ECHO_PIN3 = 2;        // Broche ECHO pour le capteur 3
const uint8_t servoPin = 6;         // Broche pour le servomoteur

// Coordonnées des trois capteurs à ultrasons (en cm)
const float X0 = 15, Y0 = 10;  // Point de référence (0,0)
const float X1 = 30, Y1 = 60;
const float X2 = 45, Y2 = 40;
const float X3 = 30, Y3 = 10;  // Point d'origine (représente la base de l'algorithme)

const float vitesse = 340.0;    // Vitesse du son en cm/ms
const uint8_t iteration = 100;  // Nombre de mesures pour la moyenne
const float distanceMax = 60.0; // Distance maximale pour valider les mesures
uint32_t delayTime = 500;       // Délai entre les boucles (500ms)
const uint8_t offset = 219;     // Offset pour le calcul de l'angle (ajustable en fonction du montage)

// Variables utilisées pour la trilatération et les matrices
float a12, b12, a13, b13, a23, b23;
float det12, det23, det13;
const uint8_t ultrasons[3][2] = {{TRIGGER_PIN1, ECHO_PIN1}, {TRIGGER_PIN2, ECHO_PIN2}, {TRIGGER_PIN3, ECHO_PIN3}}; // Tableau des capteurs
float Coeff_C[3];                 // Tableau pour stocker les coefficients C
float Tab_DET[3];                 // Tableau pour stocker les déterminants
float Coord_X_Y[3][2];           // Tableau des coordonnées des capteurs
float Coord_US[3][2] = {{X1,Y1},{X2,Y2},{X3,Y3}}; //Tableau stockant les coordonnées des trois capteurs ultrasons.



// Fonction pour mesurer la distance à partir d'un capteur à ultrasons
float MesureUS(uint8_t trigPin, uint8_t echoPin, uint8_t N) {
  uint32_t duration; //Déclaration en un entier de 32bits de la durée émission+réception  de l'onde ultrasonore par le capteur
  float sum = 0; //Initialisation de la somme des distances mesurées par un capteur à 0
  for (uint8_t i = 0; i < N; i++) {
    digitalWrite(trigPin, LOW); //Desactivation de la broche trigPin
    delayMicroseconds(2);//On attend 2 µs
    digitalWrite(trigPin, HIGH);//Activation de la broche trigPin
    delayMicroseconds(10); //On attend 10 µs
    digitalWrite(trigPin, LOW);//Desactivation de la broche trigPin
    duration = pulseIn(echoPin, HIGH); //Determination de la durée nécessaire pour l'activation de la broche echoPin après émission de l'onde ultrasonore
    sum += (duration / 2.0) * vitesse * 0.0001;  // Calcul du cumul des distances exprimées en cm
  }
  return sum / N;  // Retourner la distance moyenne
}

// Fonction pour calculer le terme C dans l'équation de la trilatération
float calcul_C(float d1, float d2, float x1, float y1, float x2, float y2) {
  return d1*d1 - d2*d2 - x1*x1 + x2*x2 - y1*y1 + y2*y2;
}

// Fonction pour calculer le déterminant du matrice 2x2 A12 ou A23 ou A13
float Calcul_det(float a1, float b1, float a2, float b2) {
  return a1*b2 - b1*a2;
}

// Configuration des pins pour les capteurs à ultrasons
void Configure_US(uint8_t ultrasons[3][2]) {
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(ultrasons[i][0], OUTPUT); //Les broches Trigger sont mises en sorties
    pinMode(ultrasons[i][1], INPUT); ////Les broches Echo sont mises en entrées
  }
}

// Calcul des coefficients des matrices A12 A23 et A13 et déduction de leurs déterminants
void coef_Matrix(float Coord_US[3][2]) {
  a12 = 2*(Coord_US [1][0] - Coord_US [0][0]);  b12 = 2*(Coord_US [1][1] - Coord_US [0][1]);
  a13 = 2*(Coord_US [2][0] - Coord_US [0][0]);  b13 = 2*(Coord_US [2][1] - Coord_US [0][1]);
  a23 = 2*(Coord_US [2][0] - Coord_US [1][0]);  b23 = 2*(Coord_US [2][1] - Coord_US [1][1]);
  det12 = Calcul_det(a12, b12, a23, b23);
  det23 = Calcul_det(a23, b23, a13, b13);
  det13 = Calcul_det(a12, b12, a13, b13);
  Tab_DET[0] = det12;  Tab_DET[1] = det23;  Tab_DET[2] = det13;
}

// Fonction pour déterminer la position en utilisant la trilatération
bool DeterminerPosition(float distance[3], float* x, float* y) {
  //Calcul des coefficients C de la matrice colonne
  Coeff_C[0] = calcul_C(distance[0], distance[1], X1, Y1, X2, Y2);
  Coeff_C[1] = calcul_C(distance[1], distance[2], X2, Y2, X3, Y3);
  Coeff_C[2] = calcul_C(distance[0], distance[2], X1, Y1, X3, Y3);
  //Calcul des déterminants relatifs aux inconues X12,Y12,X23,Y23,X13 et Y13
  float detX[3] = {
    -Calcul_det(b12, Coeff_C[0], b23, Coeff_C[1]), //det(X12)
    -Calcul_det(b23, Coeff_C[1], b13, Coeff_C[2]), //det(X23)
    -Calcul_det(b12, Coeff_C[0], b13, Coeff_C[2]) //det(X13)
  };
  float detY[3] = {
     Calcul_det(a12, Coeff_C[0], a23, Coeff_C[1]), //det(Y12)
     Calcul_det(a23, Coeff_C[1], a13, Coeff_C[2]), //det(Y23)
     Calcul_det(a12, Coeff_C[0], a13, Coeff_C[2]) //det(Y13)
  };
  //Déclarations des variables nécessaires aux calculs des moyennes pondérées de l'abscisse X et de l'ordonnée Y
  float sumX = 0.0, sumY = 0.0, xi = 0.0, yi = 0.0;
  int count = 0;
  for (uint8_t i = 0; i < 3; i++) {
    //Si la distance mesurée par un capteur est "correcte" i.e elle est inférieur à la distance maximale de tolérance
    if (distance[i] > 0 && distance[i] < distanceMax && Tab_DET[i] != 0) {
      //Calcul des coordonnées xi et yi grâce à la méthode de Cramer
      xi = (detX[i] / Tab_DET[i]); //xi pondérée par 1
      yi = (detY[i] / Tab_DET[i]); //yi pondérée par 1 
      sumX += xi; //Incrémenter des xi où chaque xi est pondéré par 1
      sumY += yi; //Incrémenter des yi où chaque yi est pondéré par 1
      count++; //Somme total des coeffecients de pondérations
    }
  }
  if (count == 0) return false; //Si aucune distance n'est valide, il n y a pas de pondération donc on sort de la fonction en retournant false
  *x = sumX / count; //Calcul de la moyenne pondérée des xi
  *y = sumY / count; //Calcul de la moyenne pondérée des yi
  return true; //On sort de la fonction en retournant true
}

// Fonction pour déterminer l'angle basé sur la position (x, y)
int DeterminerAngle(float x, float y) {
  int angle = offset - (int)atan2(y - Y0, x - X0) * 180.0 / PI;
  return angle;
}

// Fonction pour mettre à jour le servo moteur
void updateServo(int angle) {
  servo.write(angle);
}

// Fonction pour afficher la position sur l'écran LCD
void afficherPosition(float x, float y) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("X:"); lcd.print(x);
  lcd.setCursor(0, 1);
  lcd.print("Y:"); lcd.print(y);
}

// Fonction pour afficher une erreur en cas de problème
void afficherErreur() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Erreur position");
}

// Fonction principale pour effectuer les mesures et ajuster le servo
void Init() {
  float distance[3] = {0.0, 0.0, 0.0};
  float x = 0.0, y = 0.0;
  int angle = 0;

  // Mesure des distances des trois capteurs
  for (uint8_t i = 0; i < 3; i++) {
    distance[i] = MesureUS(ultrasons[i][0], ultrasons[i][1], iteration);
  }  

  // Déterminer la position de l'objet
  //Si la fonction DeterminerPosition renvoit true après avoir évaluée les distances et les positions alors la ible est détecté
  if (DeterminerPosition(distance, &x, &y)) {
    angle = DeterminerAngle(x, y); //On détermine l'angle en fonction de la position statistique (x,y)
    updateServo(angle);
    afficherPosition(x, y);  // Affichage des coordonnées statistiques sur l'écran LCD
  } else {
    afficherErreur();  // Affichage de l'erreur en cas de problème
  }
}
