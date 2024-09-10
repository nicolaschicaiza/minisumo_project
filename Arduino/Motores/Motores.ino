// Importar librerias
#include <Arduino.h>

// Definir los pines de control del motor
// MOTOR 1
#define ENG1PIN1 25
#define ENG1PIN2 5
#define ENABLE1PIN 32
// MOTOR 2
#define ENG2PIN1 26
#define ENG2PIN2 27
#define ENABLE2PIN 33
// CONFIGURACION PWM
#define FREQ 2100
#define RESOLUTION 8
// DIRECCIONES
enum DIRECTION { STOP, FORWARD, BACKWARD, RIGHT, LEFT };

String direction = "";

void setup() {
  // Configurar los pines de control del motor como salidas
  setupMotors();

  // Inicializar el puerto serie para depuración
  Serial.begin(115200);
}

void loop() {
  // Velocidad : Rango ~ Valor a utilizar
  // Velocidad máxima: 180-255~255,
  // Velocidad media: 115-180~128,
  // Velocidad baja: 75-115~75,
  // Velocidad ignorada: <70~0

  // Ejemplo de uso 
  
  // controlMotor(255, FORWARD); // Velocidad máxima, dirección adelante
  // Serial.println("Velocidad máxima, dirección adelante");
  // delay(2000); // Esperar 2 segundos

  // controlMotor(128, BACKWARD); // Velocidad media, dirección atrás
  // Serial.println("Velocidad media, dirección atrás");
  // delay(2000); // Esperar 2 segundos

  // controlMotor(128, RIGHT); // Velocidad media, dirección derecha
  // Serial.println("Velocidad media, dirección derecha");
  // delay(2000); // Esperar 2 segundos

  // controlMotor(128, LEFT); // Velocidad baja, dirección izquierda
  // Serial.println("Velocidad baja, dirección izquierda");
  // delay(2000); // Esperar 2 segundos

  // controlMotor(0, STOP); // Velocidad ignorada, detener
  // Serial.println("Velocidad ignorada, detener");
  // delay(2000); // Esperar 2 segundos
 
  Serial.println("BACKWARD");
  controlEngine(255, BACKWARD);
  delay(2000);
  Serial.println("FORWARD");
  controlEngine(255, FORWARD);
  delay(2000);
}

void test() {
  Serial.println("BACKWARD");
  controlEngine(255, BACKWARD);
  delay(2000);
  Serial.println("FORWARD");
  controlEngine(255, FORWARD);
  delay(2000);
}

// Función para configurar los motores
void setupMotors() {
  pinMode(ENG1PIN1, OUTPUT);
  pinMode(ENG1PIN2, OUTPUT);
  pinMode(ENABLE1PIN, OUTPUT);
  pinMode(ENG2PIN1, OUTPUT);
  pinMode(ENG2PIN2, OUTPUT);
  pinMode(ENABLE2PIN, OUTPUT);
  ledcAttach(ENABLE1PIN, FREQ, RESOLUTION);
  ledcAttach(ENABLE2PIN, FREQ, RESOLUTION);
}

// Función para controlar los motores
void controlEngine(int speed, DIRECTION dir) {
  // Primero, detén los motores
  digitalWrite(ENG1PIN1, LOW);
  digitalWrite(ENG1PIN2, LOW);
  digitalWrite(ENG2PIN1, LOW);
  digitalWrite(ENG2PIN2, LOW);
  ledcWrite(ENABLE1PIN, 0);
  ledcWrite(ENABLE2PIN, 0);
  
  delay(50);  // Pequeña pausa

  switch (dir) {
    case FORWARD:
      digitalWrite(ENG1PIN1, HIGH);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, HIGH);
      digitalWrite(ENG2PIN2, LOW);
      direction = "FORWARD";
      break;
    case BACKWARD:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, HIGH);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, HIGH);
      direction = "BACKWARD";
      break;
    case RIGHT:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, HIGH);
      digitalWrite(ENG2PIN1, HIGH);
      digitalWrite(ENG2PIN2, LOW);
      direction = "RIGHT";
      break;
    case LEFT:
      digitalWrite(ENG1PIN1, HIGH);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, HIGH);
      direction = "LEFT";
      break;
    case STOP:
    default:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, LOW);
      direction = "STOP";
      break;
  }

  delay(50);  // Pequeña pausa

  ledcWrite(ENABLE1PIN, speed); // Canal PWM para motor izquierdo
  ledcWrite(ENABLE2PIN, speed); // Canal PWM para motor derecho
}
