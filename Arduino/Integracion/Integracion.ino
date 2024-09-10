#include <Arduino.h>
// #include <ESP32BleGamepad.h>

// Definición de pines

// !!!! MOVIMIENTO !!!!!
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
// VELOCIDAD
enum VELOCIDAD { SLOW, MEDIUM, TOP };

// !!!! DISTANCIA !!!!
// ULTRASONIDO 1
#define SOUND1ECHO 4
#define SOUND1TRIGGER 18
// ULTRASONIDO 2
#define SOUND2ECHO 19
#define SOUND2TRIGGER 21

// !!!! ANTIDESBORDE !!!!
// INFRA ROJO 1
#define IR1PIN 23
// INFRA ROJO 2
#define IR2PIN 22

// Variables para el control manual y el modo autónomo
bool manualMode = false;
// bool autonomousMode = false;

// Crear una instancia del controlador Bluetooth
// ESP32BleGamepad gamepad;
//
String direction = "";
int velocidad = 200;

void setup() {
  // Configuración de pines
  setupMotors();
  setupUltrasonicSensors();
  setupInfraredSensors();

  // Inicializar el controlador Bluetooth
  // gamepad.begin();

  // !!!! SERIAL !!!!
  Serial.begin(115200);
}

void loop() {
  // test();
  primary();
}

void test() {
  Serial.println("BACKWARD");
  controlEngine(TOP, BACKWARD);
  delay(2000);
  Serial.println("FORWARD");
  controlEngine(TOP, FORWARD);
  delay(2000);
}

void primary() {
  // Leer los datos del controlador Bluetooth
  // gamepad.read();

  // Leer distancias desde los sensores ultrasónicos
  float distanceFront = readDistance(SOUND1TRIGGER, SOUND1ECHO);
  float distanceSide = readDistance(SOUND2TRIGGER, SOUND2ECHO);

  // Leer los valores de los sensores infrarrojos
  bool irSensor1 = digitalRead(IR1PIN);
  bool irSensor2 = digitalRead(IR2PIN);

  // Modo manual
  if (manualMode) {
    handleManualMode();
  }
  // Modo autónomo
  else {
    handleAutonomousMode(distanceFront, distanceSide, irSensor1, irSensor2);
  }

  // Verificar si se presionó el botón para cambiar de modo
  // if (gamepad.isPressed(BUTTON_START)) {
    // manualMode = !manualMode;
    // autonomousMode = !autonomousMode;
    // delay(500); // Evitar múltiples cambios de modo rápidos
  // }

  // delay(100); // Esperar un poco antes de la siguiente lectura
  printVariables(direction, velocidad);
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

// Función para configurar los sensores ultrasónicos
void setupUltrasonicSensors() {
  pinMode(SOUND1ECHO, INPUT);
  pinMode(SOUND1TRIGGER, OUTPUT);
  pinMode(SOUND2ECHO, INPUT);
  pinMode(SOUND2TRIGGER, OUTPUT);
}

// Función para configurar los sensores infrarrojos
void setupInfraredSensors() {
  pinMode(IR1PIN, INPUT);
  pinMode(IR2PIN, INPUT);
}

// Función para controlar los motores
void controlEngine(VELOCIDAD speed, DIRECTION dir) {
  // Detener motores para evitar picos de voltaje
  digitalWrite(ENG1PIN1, LOW);
  digitalWrite(ENG1PIN2, LOW);
  digitalWrite(ENG2PIN1, LOW);
  digitalWrite(ENG2PIN2, LOW);
  ledcWrite(ENABLE1PIN, 0);
  ledcWrite(ENABLE2PIN, 0);
  
  delayMicroseconds(25);  // Pequeña pausa

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
  delayMicroseconds(25);  // Pequeña pausa
  controlSpeed(speed);
}

// Función para controlar velocidad
void controlSpeed(VELOCIDAD speed) {
  int value = 0;
  if (speed == SLOW) {
    value = 220;
  } else if (speed == MEDIUM) {
    value = 235;
  } else {
    value = 255;
  }
  switch (speed) {
    case SLOW:
      ledcWrite(ENABLE1PIN, value); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, value); // Canal PWM para motor derecho
    case MEDIUM:
    default:
      ledcWrite(ENABLE1PIN, value); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, value); // Canal PWM para motor derecho
    case TOP:
      ledcWrite(ENABLE1PIN, value); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, value); // Canal PWM para motor derecho
  }
  velocidad = value;
}

// Función para leer la distancia de un sensor ultrasónico
float readDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.0344 / 2;
  return distance;
}

// Función para manejar el modo manual
void handleManualMode() {
//   int speed = 255;
//   direction dir = STOP;
// 
//   if (gamepad.isPressed(BUTTON_UP)) {
//     dir = FORWARD;
//   } else if (gamepad.isPressed(BUTTON_DOWN)) {
//     dir = BACKWARD;
//   } else if (gamepad.isPressed(BUTTON_RIGHT)) {
//     dir = RIGHT;
//   } else if (gamepad.isPressed(BUTTON_LEFT)) {
//     dir = LEFT;
//   }
// 
//   controlEngine(speed, dir);
}

// Función para manejar el modo autónomo
void handleAutonomousMode(float distanceFront, float distanceSide, bool irSensor1, bool irSensor2) {
  if (distanceFront < 10) {
    handleFrontObstacle(distanceFront);
    Serial.println("Acción de ataque");
  } else if (distanceSide < 15) {
    handleSideObstacle(distanceSide);
  } else {
    handleNoObstacle(distanceFront, distanceSide);
  }

  // handleEdgeDetection(irSensor1, irSensor2);
}

// Función para manejar un obstáculo frontal
void handleFrontObstacle(float distanceFront) {
  Serial.println("handleFrontObstacle");
  controlEngine(TOP, FORWARD);
}

// Función para manejar un obstáculo lateral
void handleSideObstacle(float distanceSide) {
  Serial.println("handleSideObstacle");
  controlEngine(MEDIUM, RIGHT);
  delay(500);
  distanceSide = readDistance(SOUND2TRIGGER, SOUND2ECHO);
  if (distanceSide >= 15) {
    controlEngine(MEDIUM, LEFT);
    delay(500);
    controlEngine(MEDIUM, STOP);
    delay(500);
  }
}

// Función para manejar la ausencia de obstáculos
void handleNoObstacle(float distanceFront, float distanceSide) {
  Serial.println("handleNoObstacle");
  controlEngine(SLOW, RIGHT);
  delay(500);
  distanceFront = readDistance(SOUND1TRIGGER, SOUND1ECHO);
  if (distanceFront >= 10) {
    controlEngine(MEDIUM, LEFT);
    delay(500);
    controlEngine(MEDIUM, STOP);
    delay(500);
  }
}

// Función para manejar la detección de bordes
void handleEdgeDetection(bool irSensor1, bool irSensor2) {
  Serial.println("handleEdgeDetection");
  if (irSensor1 == LOW || irSensor2 == LOW) {
    if (irSensor1 == LOW) {
      controlEngine(SLOW, RIGHT);
      delay(500);
    } else if (irSensor2 == LOW) {
      controlEngine(SLOW, LEFT);
      delay(500);
    }
  }
}

void printVariables(String dir, int speed) {
  Serial.println("---- Loop ---- Start ----");
  Serial.print("Modo: ");
  if (manualMode) {
    Serial.println("Manual");
  } else {
    Serial.println("Autónomo");
  }
  Serial.println("---- ---- ----");
  Serial.print("Distancia frontal: ");
  Serial.print(readDistance(SOUND1TRIGGER, SOUND1ECHO));
  Serial.println(" cm");
  Serial.print("Distancia lateral: ");
  Serial.print(readDistance(SOUND2TRIGGER, SOUND2ECHO));
  Serial.println(" cm");
  Serial.println("---- ---- ----");
  Serial.print("Dirección: ");
  Serial.println(dir);
  Serial.println("---- ---- ----");
  Serial.print("Velocidad: ");
  Serial.println(speed);
  Serial.println("---- Loop ---- End ----");
}
