#include <Arduino.h>
#include "defines.h"

#ifndef PWM_H
#define PWM_H

float tempo = 200.0;

float corrd = 1.0;
float corre = 1.0;

void mover_motor(char lado, char direcao, float velocidade)
{
  if (lado == 'd')
  {
    if (direcao == 'f')
    { // ir para frente
      analogWrite(PWMC,velocidade*corrd);
      analogWrite(PWMD,0);
    }

    if (direcao == 't')
    { // ir para trás
      analogWrite(PWMC,0);
      analogWrite(PWMD,velocidade*corrd);
    }
  }

  if (lado == 'e')
  {
    if (direcao == 'f')
    { // ir para frente
      analogWrite(PWMA,velocidade*corre);
      analogWrite(PWMB,0);
    }

    if (direcao == 't')
    { // ir para trás
      analogWrite(PWMA,0);
      analogWrite(PWMB,velocidade*corre);
    }
  }
}

void controlar_motores(float velocidade, char sentido)
{
  mover_motor('e', sentido, 0.5);
  mover_motor('d', sentido, 0.5);
}

void testar_PWM()
{
  int cicloDeTrabalho = 400;

  ledcWrite(0, cicloDeTrabalho);
  ledcWrite(1, cicloDeTrabalho);
}

void frente()
{
  mover_motor('e', 'f', 500);
  mover_motor('d', 'f', 500);
}
void tras()
{
  mover_motor('e', 't', 400);
  mover_motor('d', 't', 400);
}

void direita()
{
  mover_motor('e', 'f', 400);
  mover_motor('d', 'f', 200);
}

void esquerda()
{
  mover_motor('e', 'f', 200);
  mover_motor('d', 'f', 400);
}

void parar()
{
  analogWrite(PWMA, 1023);
  analogWrite(PWMB, 1023);
  analogWrite(PWMC, 1023);
  analogWrite(PWMD, 1023);
  delay(50);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 0);
  analogWrite(PWMD, 0);
}

void tparar()
{
  analogWrite(PWMA, 1023);
  analogWrite(PWMB, 1023);
  analogWrite(PWMC, 1023);
  analogWrite(PWMD, 1023);
  vTaskDelay(pdMS_TO_TICKS(50));
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 0);
  analogWrite(PWMD, 0);
}

void girar_eixo()
{
  mover_motor('e', 'f', 300);
  mover_motor('d', 't', 300);
}

void girar_circulo()
{
  mover_motor('e', 'f', 200);
  mover_motor('d', 'f', 100);
}

void girar_180()
{
  mover_motor('e', 'f', 0.5);
  mover_motor('d', 't', 0.5);
  delay(200);
  parar();
}

#endif
