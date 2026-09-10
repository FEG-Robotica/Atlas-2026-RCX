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
    int sensorLatEsq = digitalRead(SENSOR_LAT_ESQ);
    int sensorFrontEsq = digitalRead(SENSOR_FRONT_ESQ);
    int sensorFrontDir = digitalRead(SENSOR_FRONT_DIR);
    int sensorLatDir = digitalRead(SENSOR_LAT_DIR);

    TestarLinha();

    if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      mover_motor('e', 'f', 200);
      mover_motor('d', 'f', 200);
      delay(50);
    }
    else if ((sensorLatEsq == 1) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      mover_motor('e', 't', 100);
      mover_motor('d', 'f', 150);
      delay(50);
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 1) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      mover_motor('e', 'f', 100);
      mover_motor('d', 'f', 150);
      delay(50);
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && (sensorLatDir == 1))
    {
      mover_motor('e', 'f', 150);
      mover_motor('d', 't', 100);
      delay(50);
    }
    else if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 1) && (sensorLatDir == 0))
    {
      mover_motor('e', 'f', 150);
      mover_motor('d', 'f', 100);
      delay(50);
    }
    else if ((sensorFrontEsq == 1) && (sensorFrontDir == 1))
    {
      frente();
    }
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

    // vTaskResume(taskEvitar);

    if (digitalRead(SENSOR_LAT_DIR) == 1) //inimigo à direita
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
    else if ((digitalRead(SENSOR_MEIO_DIR) == 1) && (digitalRead(SENSOR_MEIO_ESQ) == 1))
    {
      mover_motor('e','f',200);
      mover_motor('d','f',200);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else
    {
      orbitar();
    }
  }
}
#endif