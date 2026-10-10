#include <ESP32Servo.h>
//** Definição dos Pinos **

#ifndef DEFINES_H
#define DEFINES_H

// Sensores de Distancia
#define SENSOR_LAT_DIR 4   //_SDA
#define SENSOR_FRONT_DIR 5 //_SDB
#define SENSOR_MEIO_DIR 6 //_SDC
#define SENSOR_MEIO_ESQ 7 //_SDD
#define SENSOR_FRONT_ESQ 15  //_SDE
#define SENSOR_LAT_ESQ 16   //_SDF

// Pwm
#define PWMA 41 // entrada motor A IN1_A
#define PWMB 42 // entrada motor A IN2_A
#define PWMC 48 // entrada motor B IN3_B
#define PWMD 47 // entrada motor B IN4_B

// Sensores de Linha
#define SENSOR_LINHA_A 13 // linha esquerda
#define SENSOR_LINHA_B 14 // linha direita

// Modulo Start
#define MODULO_START 21 // SIN8

// Led
#define LEDPIN 2

// Bluetooth
#define SERVICE_UUID "09e15beb-7685-4c74-babc-c1be7c182b9e"
#define CHARACTERISTIC_UUID "4e9badb4-3f7b-475c-928d-67ad57c5147b"

// Variaveis Tipo Inteiro e Iguais á Zero
volatile int linha_direita = 0;
volatile int linha_esquerda = 0;
int distancia_lateral_esquerdo = 0;
int distancia_frontal_esquerdo = 0;
int distancia_frontal_direita = 0;
int distancia_lateral_direita = 0;
int velmotor = 0;
int parametro = 0;
int pos;

// Servo
Servo band1;
Servo band2;
int bnd1 = 39;
int bnd2 = 40;


// Variaveis Tipo Booleana
volatile bool fora = false;
bool abaixada = false;

// Limite da Arena branco < 1000, preto > 1000
int limite = 1000;

// Chamada de funções
void leituraSensoresDistancia();
void leituraSensoresLinha();

#endif
