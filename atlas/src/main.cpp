#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ESP32Servo.h>
#include "defines.h"
#include "testes.h"


std::string comando = "p";

#define bleRobo "Atlas"

bool deviceConnected = false;
bool oldDeviceConnected = false;
bool bleBool = false;

BLEServer *pServer = NULL;

BLECharacteristic bleComCharacteristics("9295411b-0868-4655-9b84-ec3a4c74e258", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bleComDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic bleLinhaCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bleLinhaDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic bleDistanciaCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bleDistanciaDescriptor(BLEUUID((uint16_t)0x2902));

// Função no Core 0
TaskHandle_t taskEvitar = NULL;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("CONECTADO");
  };
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    pServer->startAdvertising();
    Serial.println("DESCONECTADO");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    comando = value;
    Serial.println(value.c_str());

    if (value.substr(0, 3) == "vel")
    {
      velmotor = std::stoi(value.substr(3, 7));
    }

    if (value.substr(0, 3) == "par")
    {
      parametro = std::stoi(value.substr(3, 7));
    }

    // Controlar o Servo
    if (value.substr(0,5) == "servo")
    {
        int angulo = atoi(value.substr(5).c_str());
        if (angulo >= 0 && angulo <= 180)
        {
            band1.write(angulo);
            band2.write(angulo);
            Serial.print("Servos -> ");
            Serial.println(angulo);
        }
    }
  }
};

void envia_sensores_distancia()
{
  leituraSensoresDistancia();
  std::string sensores = " E:";
  sensores.append(std::to_string(distancia_lateral_esquerdo));
  sensores.append(" FE:");
  sensores.append(std::to_string(distancia_frontal_esquerdo));
  sensores.append(" FD:");
  sensores.append(std::to_string(distancia_frontal_direita));
  sensores.append(" D:");
  sensores.append(std::to_string(distancia_lateral_direita));
  bleDistanciaCharacteristics.setValue(sensores);
  bleDistanciaCharacteristics.notify();
  delay(100);
}

void envia_sensores_linha()
{
  leituraSensoresLinha();
  std::string sensores = " SLE:";
  sensores.append(std::to_string(linha_esquerda));
  sensores.append(" SLD:");
  sensores.append(std::to_string(linha_direita));
  bleLinhaCharacteristics.setValue(sensores);
  bleLinhaCharacteristics.notify();
  delay(100);
}

void setup()
{
  pinMode(SENSOR_LAT_DIR, INPUT_PULLDOWN);   // sensor de distancia A
  pinMode(SENSOR_FRONT_DIR, INPUT_PULLDOWN); // sensor de distancia B
  pinMode(SENSOR_MEIO_DIR, INPUT_PULLDOWN); // sensor de distancia C
  pinMode(SENSOR_MEIO_ESQ, INPUT_PULLDOWN);   // sensor de distancia D
  pinMode(SENSOR_FRONT_ESQ, INPUT_PULLDOWN); // sensor de distancia E
  pinMode(SENSOR_LAT_ESQ, INPUT_PULLDOWN); // sensor de distancie F
  pinMode(SENSOR_LINHA_A, INPUT);            // sensor de linha A
  pinMode(SENSOR_LINHA_B, INPUT);            // sensor de linha B
  pinMode(MODULO_START, INPUT_PULLDOWN);     // modulo start
  pinMode(PWMA, OUTPUT);                     // entrada motor A
  pinMode(PWMB, OUTPUT);                     // entrada motor A
  pinMode(PWMC, OUTPUT);                     // entrada motor B
  pinMode(PWMD, OUTPUT);                     // entrada motor B
  pinMode(LEDPIN, OUTPUT);

  // led
  /* ledcSetup(0, 5000, 10);
  ledcAttachPin(PWMA, 0);
  ledcSetup(1, 5000, 10);
  ledcAttachPin(PWMB, 1);
  ledcSetup(2, 5000, 10);
  ledcAttachPin(PWMC, 2);
  ledcSetup(3, 5000, 10);
  ledcAttachPin(PWMD, 3); */

  // Servo
  band1.attach(39);
  band1.write(40);
  band2.attach(40);
  band2.write(40);

  Serial.begin(9600);

  BLEDevice::init(bleRobo);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pService->addCharacteristic(&bleDistanciaCharacteristics);
  bleDistanciaDescriptor.setValue("bleDist");
  bleDistanciaCharacteristics.addDescriptor(&bleDistanciaDescriptor);

  pService->addCharacteristic(&bleLinhaCharacteristics);
  bleLinhaDescriptor.setValue("Linha");
  bleLinhaCharacteristics.addDescriptor(&bleLinhaDescriptor);

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("ENVIAR CMD");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  pAdvertising->start();

  // Função de evitar linha no core 0 do processador
  xTaskCreatePinnedToCore(
    EvitaLinha,       //função q vai ser executada
    "Evitar Linha",   //nome da função
    4096,             //tamanho do stack em bytes
    NULL,             //argumento da tarefa
    2,                //prioridade
    &taskEvitar,      //handle da tarefa
    0);               //núcleo onde a tarefa vai ser usada

  //vTaskSuspend(taskEvitar);
}

// Atualizar leitura dos sensores de linha e distancia
void leituraSensoresLinha()
{
  linha_direita = analogRead(SENSOR_LINHA_A);
  linha_esquerda = analogRead(SENSOR_LINHA_B);
}
void leituraSensoresDistancia()
{
  distancia_lateral_esquerdo = digitalRead(SENSOR_LAT_ESQ);
  distancia_frontal_esquerdo = digitalRead(SENSOR_FRONT_ESQ);
  distancia_frontal_direita = digitalRead(SENSOR_FRONT_DIR);
  distancia_lateral_direita = digitalRead(SENSOR_LAT_DIR);
}

#include "pwm.h"
#include "estrategias.h"

void loop()
{

  Serial.println(digitalRead(MODULO_START));

  if (deviceConnected)
  {
    envia_sensores_distancia();
    envia_sensores_linha();
    delay(5);
  }

  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);
    pServer->startAdvertising();
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected)
  {
    oldDeviceConnected = deviceConnected;
  }

  /*
  while (digitalRead(MODULO_START) == 0)
  {
    parar();
    Serial.println(digitalRead(MODULO_START));
  }
  */

  if (comando == "led")
  {
    piscar_led();
  }
  if (comando == "p")
  {
    parar();
  }
  if (comando == "f")
  {
    frente();
  }

  if (comando == "t")
  {
    tras();
  }

  if (comando == "eixo")
  {
    girar_eixo();
  }

  if (comando == "circulo")
  {
    girar_circulo();
  }

  if (comando == "180")
  {
    girar_180();
  }

  if (comando == "vel")
  {
    if (digitalRead(MODULO_START))
    {
      int velocidadePWM = 300;
      for (int i = 0; i < 14; i++)
      {
        velocidadePWM = velocidadePWM + 50;
        mover_motor('d', 'f', velocidadePWM);
        mover_motor('e', 'f', velocidadePWM);
        Serial.println(velocidadePWM);
        delay(1000);
      }
    }
    else
    {
      parar();
    }
  }

  if (comando == "printdistancia")
  {
    Serial.print("E: ");
    Serial.print(digitalRead(SENSOR_LAT_ESQ));
    Serial.print("FE: ");
    Serial.print(digitalRead(SENSOR_FRONT_ESQ));
    Serial.print("FD: ");
    Serial.print(digitalRead(SENSOR_FRONT_DIR));
    Serial.print("D: ");
    Serial.println(digitalRead(SENSOR_LAT_DIR));
    delay(1000);
  }

  if (comando == "printlinha")
  {
    Serial.print("SLE: ");
    Serial.println(analogRead(SENSOR_LINHA_A));
    delay(1000);
    Serial.print("SLD: ");
    Serial.println(analogRead(SENSOR_LINHA_B));
    delay(1000);
  }

  // * ESTRATÉGIAS *

  /*analogWrite(PWMA, 1023);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 1023);
  analogWrite(PWMD, 0);
  vTaskDelay(pdMS_TO_TICKS(1000));
  analogWrite(PWMA, 900);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 900);
  analogWrite(PWMD, 0);
  vTaskDelay(pdMS_TO_TICKS(1000));           //é pra teste!
  analogWrite(PWMA, 800);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 800);
  analogWrite(PWMD, 0);
  vTaskDelay(pdMS_TO_TICKS(1000));
  analogWrite(PWMA, 700);
  analogWrite(PWMB, 0);
  analogWrite(PWMC, 700);
  analogWrite(PWMD, 0);
  vTaskDelay(pdMS_TO_TICKS(1000));*/

  /*abaixar();
  vTaskDelay(pdMS_TO_TICKS(200));
  levantar();
  vTaskDelay(pdMS_TO_TICKS(200));*/

  // STRAT 1
  if (comando == "m")
  {
    Missil();
  }

  // STRAT 2
  if (comando == "l")
  {
    Linha();
  }

  // STRAT 3
  if (comando == "b")
  {
    Busca();
  }

  // STRAT 6
  if (comando == "a")
  {
    Arco();
  }

  // STRAT 4
  if (comando == "s1")
  {
    STRAT1();
  }

  // STRAT 5
  if (comando == "s2")
  {
    if (!fora)
    {
    STRAT2();
    }
  }

}
