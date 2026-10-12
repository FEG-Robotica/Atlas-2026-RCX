#include <Arduino.h>
#include "pwm.h"
#include "defines.h"
#include "testes.h"

#ifndef ESTRATEGIAS_H
#define ESTRATEGIAS_H

void STRAT1() // Estratégia(1) de Busca e Linha
{
  if (digitalRead(MODULO_START) == 1)
  {
    levantar();
    int sensorLatEsq = digitalRead(SENSOR_LAT_ESQ);
    int sensorDiaEsq = digitalRead(SENSOR_FRONT_ESQ);
    int sensorFrontEsq = digitalRead(SENSOR_MEIO_ESQ);
    int sensorFrontDir = digitalRead(SENSOR_MEIO_DIR);
    int sensorDiaDir = digitalRead(SENSOR_FRONT_DIR);
    int sensorLatDir = digitalRead(SENSOR_LAT_DIR);

    TestarLinha();

    if ((sensorFrontEsq == 1) || (sensorFrontDir == 1))
    {
      frente();
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      girar_eixo();
    }
    else if (((sensorLatEsq == 1) || (sensorDiaEsq == 1)) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      mover_motor('e', 't', 300);
      mover_motor('d', 'f', 1023);
      delay(50);
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 1) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      mover_motor('e', 'f', 300);
      mover_motor('d', 'f', 1023);
      delay(50);
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && ((sensorLatDir == 1) || (sensorDiaDir == 1)))
    {
      mover_motor('d', 'f', 1023);
      mover_motor('e', 't', 300);
      delay(50);
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 1) && (sensorLatDir == 0))
    {
      mover_motor('e', 'f', 1023);
      mover_motor('d', 'f', 300);
      delay(50);
    }
  }
  else{
    parar();
  }
}

void STRAT2() // Estratégia(2) Órbita
{
  if (digitalRead(MODULO_START) == 1)
  {
    if (!abaixada)
    {
      abaixar();
    }

    //vTaskResume(taskEvitar);

    if ((digitalRead(SENSOR_MEIO_DIR) == 1) && (digitalRead(SENSOR_MEIO_ESQ) == 1))
    {
      mover_motor('e','f',200);
      mover_motor('d','f',200);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else if (digitalRead(SENSOR_LAT_DIR) == 1) //inimigo à direita
    {
      mover_motor('e','f',250);
      mover_motor('d','t',250);
      vTaskDelay(pdMS_TO_TICKS(250));
    }
    else if (digitalRead(SENSOR_LAT_ESQ) == 1) //inimigo à esquerda
    {
      mover_motor('e','t',250);
      mover_motor('d','f',250);
      vTaskDelay(pdMS_TO_TICKS(250));
    }
    else if (digitalRead(SENSOR_FRONT_DIR) == 1) //inimigo na diagonal direita
    {
      mover_motor('e','f',180);
      mover_motor('d','t',180);
      vTaskDelay(pdMS_TO_TICKS(124));
    }
    else if (digitalRead(SENSOR_FRONT_ESQ) == 1) //inimigo na diagonal esquerda
    { 
      mover_motor('e','t',180);
      mover_motor('d','f',180);
      vTaskDelay(pdMS_TO_TICKS(124));
    }
    else if (digitalRead(SENSOR_MEIO_DIR) == 1) //inimigo à frente na direita
    {
      mover_motor('e','f',250);
      mover_motor('d','f',210);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else if (digitalRead(SENSOR_MEIO_ESQ) == 1) //inimigo à frente na esquerda
    {
      mover_motor('e','f',210);
      mover_motor('d','f',250);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else
    {
      orbitar();
    }
  }
  else{
    parar();
  }
}

void STRAT3()
{
  if (digitalRead(MODULO_START) != 1)
  {
    parar();
    return;
  }

  int latE  = digitalRead(SENSOR_LAT_ESQ);
  int diaE  = digitalRead(SENSOR_FRONT_ESQ);
  int meioE = digitalRead(SENSOR_MEIO_ESQ);
  int meioD = digitalRead(SENSOR_MEIO_DIR);
  int diaD  = digitalRead(SENSOR_FRONT_DIR);
  int latD  = digitalRead(SENSOR_LAT_DIR);

  TestarLinha();

  if (meioE && meioD)            // inimigo de frente: ataca
  {
    frente();
  }
  else if (meioE)                // frente-esquerda
  {
    mover_motor('e', 'f', 200);
    mover_motor('d', 'f', 500);
  }
  else if (meioD)                // frente-direita
  {
    mover_motor('e', 'f', 500);
    mover_motor('d', 'f', 200);
  }
  else if (latE || diaE)         // esquerda
  {
    mover_motor('e', 't', 200);
    mover_motor('d', 'f', 500);
  }
  else if (latD || diaD)         // direita
  {
    mover_motor('e', 'f', 500);
    mover_motor('d', 't', 200);
  }
  else                           // não vê ninguém: procura
  {
    girar_eixo();
  }
}


void truquezin() // Estratégia anti-míssil
{
  if (digitalRead(MODULO_START) != 1)
  {
    parar();
    return;
  }

  // 1) Esquiva primeiro, sem bloquear a checagem de linha
  unsigned long inicio = millis();
  while (millis() - inicio < 400)          // tempo
  {
    TestarLinha();                         
    mover_motor('e', 'f', 1023);
    mover_motor('d', 't', 400);            
  }
  comando = "s1";
}




#endif