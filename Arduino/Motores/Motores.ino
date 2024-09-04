// Importar librerias
#include <Arduino.h>

// Definir los pines de control del motor
const int motor1Pin1 = 26;
const int motor1Pin2 = 27;
const int enable1Pin = 14;

const int motor2Pin1 = 32;
const int motor2Pin2 = 33;
const int enable2Pin = 25;

// Configurar la frecuencia del PWM
const int freq = 2100;
const int resolution = 8;

// Definir las direcciones posibles
enum Direction { STOP, FORWARD, BACKWARD, RIGHT, LEFT };

void setup() {
  // Configurar los pines de control del motor como salidas
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Configurar el PWM
  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);

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
 
  controlMotor(255, LEFT);
  delay(2000); // Esperar 2 segundos
}

void controlMotor(int speed, Direction direction) {
  // Establecer la dirección del motor izquierdo
  switch (direction) {
    case FORWARD:
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      break;
    case BACKWARD:
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      break;
    case RIGHT:
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      break;
    case LEFT:
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      break;
  }

  // Establecer la dirección del motor derecho
  switch (direction) {
    case FORWARD:
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
      break;
    case BACKWARD:
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
      break;
    case RIGHT:
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
      break;
    case LEFT:
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
      break;
    case STOP:
    default:
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
      break;
  }

  // Establecer la velocidad de ambos motores
  ledcWrite(enable1Pin, speed); // Canal PWM para motor izquierdo
  ledcWrite(enable2Pin, speed); // Canal PWM para motor derecho
}
