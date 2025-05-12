
#ifndef SAC21_H
#define SAC21_H
#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Servo.h"

// Variable et objets externes  qui seront utilisées dans les fonctions
extern Servo servo;
extern rgb_lcd lcd;
extern const uint8_t TRIGGER_PIN1, ECHO_PIN1;
extern const uint8_t TRIGGER_PIN2, ECHO_PIN2;
extern const uint8_t TRIGGER_PIN3, ECHO_PIN3;
extern const uint8_t servoPin;
extern const float X0, Y0, X1, Y1, X2, Y2, X3, Y3;
extern const uint8_t Coord_US[3][2];
extern const float vitesse;
extern const uint8_t iteration;
extern const float distanceMax;
extern const uint32_t delayTime;
extern const uint8_t offset;
extern float a12, b12, a13, b13, a23, b23;
extern float det12, det23, det13;
extern const uint8_t ultrasons[3][2];
extern float Coeff_C[3];
extern float Tab_DET[3];
extern float Coord_X_Y[3][2];

//Prototype des fonctions
float MesureUS(uint8_t trigPin, uint8_t echoPin, uint8_t N);
float calcul_C(float d1, float d2, float x1, float y1, float x2, float y2);
float Calcul_det(float a1, float b1, float a2, float b2);
void Configure_US(uint8_t ultrasons[3][2]);
void coef_Matrix(float Coord_US[3][2]);
bool DeterminerPosition(float distance[3], float &x, float &y);
int DeterminerAngle(float x, float y);
void updateServo(int angle);
void afficherPosition(float x, float y);
void afficherErreur();
void Init();

#endif
