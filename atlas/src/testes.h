#include <Arduino.h>
#include "defines.h"
#include "pwm.h"

#ifndef TESTES_H
#define TESTES_H

void piscar_led()
{
  digitalWrite(LEDPIN, HIGH);
  delay(500);
  digitalWrite(LEDPIN, LOW);
  delay(500);
}

void LerLinhaDigital()
{
  if ((analogRead(SENSOR_LINHA_A)) > 1900)
  {
    linha_esquerda = 1;
  }
  else
  {
    linha_esquerda = 0;
  }
  if ((analogRead(SENSOR_LINHA_B)) > 1900)
  {
    linha_direita = 1;
  }
  else
  {
    linha_direita = 0;
  }
}

void LerDistanciaDigital()
{
  if (digitalRead(SENSOR_LAT_ESQ) == 0)
  {
    distancia_lateral_esquerdo = 0;
  }
  else
  {
    distancia_lateral_esquerdo = 1;
  }

  if (digitalRead(SENSOR_FRONT_ESQ) == 0)
  {
    distancia_frontal_esquerdo = 0;
  }
  else
  {
    distancia_frontal_esquerdo = 1;
  }

  if (digitalRead(SENSOR_FRONT_DIR) == 0)
  {
    distancia_frontal_direita = 0;
  }
  else
  {
    distancia_frontal_direita = 1;
  }

  if (digitalRead(SENSOR_LAT_DIR) == 0)
  {
    distancia_lateral_direita = 0;
  }
  else
  {
    distancia_lateral_direita = 1;
  }
}

void Busca()
{
  if (digitalRead(MODULO_START) == 1)
  {
    int sensorLatEsq = digitalRead(SENSOR_LAT_ESQ);
    int sensorFrontEsq = digitalRead(SENSOR_FRONT_ESQ);
    int sensorFrontDir = digitalRead(SENSOR_FRONT_DIR);
    int sensorLatDir = digitalRead(SENSOR_LAT_DIR);

    if ((sensorLatEsq == 0) && (sensorFrontEsq == 0) && (sensorFrontDir == 0) && (sensorLatDir == 0))
    {
      girar_eixo();
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

void Missil()
{
  mover_motor('e', 'f', 1023);
  mover_motor('d', 'f', 1023);
}

void Linha()
{
  if (digitalRead(MODULO_START) == 1)
  {
    leituraSensoresLinha();

    int sensorA = analogRead(SENSOR_LINHA_A);
    int sensorB = analogRead(SENSOR_LINHA_B);

    int SLE = sensorA;
    int SLD = sensorB;

    if ((SLE < 1900) && (SLD < 1900))
    {
      mover_motor('e', 't', 700);
      mover_motor('d', 't', 700);
      delay(300);
      mover_motor('e', 'f', 300);
      mover_motor('d', 't', 300);
    }
    else if ((SLE < 1900) && (SLD > 1900)) // Alto = 3.3 e Baixo = 0
    {
      mover_motor('e', 't', 700);
      mover_motor('d', 't', 700);
      delay(300);
      mover_motor('e', 't', 400);
      mover_motor('d', 't', 0);
    }
    else if ((SLE > 1900) && (SLD < 1900))
    {
      mover_motor('e', 't', 700);
      mover_motor('d', 't', 700);
      delay(300);
      mover_motor('e', 't', 0);
      mover_motor('d', 't', 400);
    }
    else
    {
      // parar();
      mover_motor('e', 'f', 330);
      mover_motor('d', 'f', 330);
    }
  }
}

void ataque()
{
  if (digitalRead(MODULO_START) == 1)
  {
    if ((digitalRead(SENSOR_FRONT_DIR) == 0) && (digitalRead(SENSOR_FRONT_ESQ) == 0))
    {
      parar();
    }
    else
    {
      mover_motor('e', 'f', 600);
      mover_motor('d', 'f', 600);
    }
  }
  else
  {
    parar();
  }
}

void Arco()
{
  if (digitalRead(MODULO_START) == 1)
  {
    if ((digitalRead(SENSOR_LAT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_DIR) == 0) && (digitalRead(SENSOR_LAT_DIR) == 0)) // sem ver nada
    {
      Serial.println("Condicao1");
      digitalWrite(PWMA, HIGH);
      digitalWrite(PWMB, HIGH);
      ledcWrite(0, 0);
      digitalWrite(PWMC, HIGH);
      digitalWrite(PWMD, HIGH);
      ledcWrite(1, 0);
    }

    if ((digitalRead(SENSOR_LAT_ESQ) == 1) && (digitalRead(SENSOR_FRONT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_DIR) == 0) && (digitalRead(SENSOR_LAT_DIR) == 0)) // apenas sensor esquerda
    {
      Serial.println("Condicao2");
      mover_motor('d', 'f', 450);
      mover_motor('e', 'f', 400);
    }

    if ((digitalRead(SENSOR_LAT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_ESQ) == 1) && (digitalRead(SENSOR_FRONT_DIR) == 0) && (digitalRead(SENSOR_LAT_DIR) == 0)) // apenas sensor central esquerdo
    {
      Serial.println("Condicao3");
      mover_motor('d', 'f', 450);
      mover_motor('e', 'f', 400);
    }

    if ((digitalRead(SENSOR_LAT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_DIR) == 1) && (digitalRead(SENSOR_LAT_DIR) == 0)) // apenas sensor direito/central
    {
      Serial.println("Condicao4");
      mover_motor('e', 'f', 450);
      mover_motor('d', 'f', 400);
    }

    if ((digitalRead(SENSOR_LAT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_DIR) == 0) && (digitalRead(SENSOR_LAT_DIR) == 1)) // apenas sensor direito
    {
      Serial.println("Condicao5");
      mover_motor('e', 'f', 450);
      mover_motor('d', 'f', 400);
    }

    if ((digitalRead(SENSOR_LAT_ESQ) == 0) && (digitalRead(SENSOR_FRONT_ESQ) == 1) && (digitalRead(SENSOR_FRONT_DIR) == 1) && (digitalRead(SENSOR_LAT_DIR) == 0)) // todos sensores
    {
      mover_motor('d', 'f', 550);
      mover_motor('e', 'f', 550);
    }

    if ((digitalRead(SENSOR_LAT_ESQ) == 1) && (digitalRead(SENSOR_FRONT_ESQ) == 1) && (digitalRead(SENSOR_FRONT_DIR) == 1) && (digitalRead(SENSOR_LAT_DIR) == 1)) // todos sensores
    {
      parar();
    }
  }
  else
  {
    parar();
  }
}

void TestarLinha()
{
  if (digitalRead(MODULO_START) == 1)
  {
    leituraSensoresLinha();
    /*if ((linha_direita == 1) && (linha_esquerda == 1))
    {
      mover_motor('e', 'f', 300);
      mover_motor('d', 'f', 300);
    }*/
    if ((linha_direita == 0) && (linha_esquerda == 1))
    {
      mover_motor('d', 't', 400);
      mover_motor('e', 't', 400);
      delay(300);
      mover_motor('d', 't', 400);
      mover_motor('e', 'f', 400);
      delay(300);
    }
    else if ((linha_direita == 1) && (linha_esquerda == 0))
    {
      mover_motor('d', 't', 400);
      mover_motor('e', 't', 400);
      delay(300);
      mover_motor('d', 'f', 500);
      mover_motor('e', 't', 400);
      delay(300);
    }
    else if ((linha_direita == 0) && (linha_esquerda == 0))
    {
      mover_motor('d', 't', 400);
      mover_motor('e', 't', 400);
      delay(300);
      mover_motor('d', 'f', 500);
      mover_motor('e', 't', 400);
      delay(300);
    }
  }
  else
  {
    parar();
  }
}


void orbitar()
{
  mover_motor('e','f',250);
  mover_motor('d','f',400);
}

void levantar() // (definir ângulo da bandeira levantada)
{
  band1.write(40);
  band2.write(40);
  Serial.println("Bandeira Levantada");
  abaixada = false;
}

void abaixar() // (definir ângulo da bandeira abaixada)
{
  band1.write(90); 
  band2.write(90);
  Serial.println("Bandeira Abaixada");
  abaixada = true;
}

extern TaskHandle_t taskEvitar;

void EvitaLinha(void * parameter)
{
  while (true)
  {
    Serial.println("task EvitaLinha rodando normal");
    fora = false;
    linha_esquerda = analogRead(SENSOR_LINHA_A);
    linha_direita = analogRead(SENSOR_LINHA_B);

    if ((linha_esquerda > limite) || (linha_direita > limite))
    {
      fora = true;
      //tparar() só testando pra saber se precisa
      mover_motor('e','t',400);
      mover_motor('d','t',400);
      vTaskDelay(300 / portTICK_PERIOD_MS);

      mover_motor('e','t',350);
      mover_motor('d','f',250);
      vTaskDelay(250 / portTICK_PERIOD_MS);
      tparar();
    }
    else
    {
      fora = false;
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

#endif
