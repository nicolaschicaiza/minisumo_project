#include <Arduino.h>
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

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
enum DIRECTION { 
  STOP,
  FORWARD,
  BACKWARD,
  RIGHT,
  LEFT,
  FORWARD_RIGHT,
  FORWARD_LEFT,
  BACKWARD_RIGHT,
  BACKWARD_LEFT
};
// VELOCIDAD
enum VELOCIDAD { SLOW, MEDIUM, TOP, BRAKER };

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
bool manualMode = true;
// bool autonomousMode = false;

// Crear una instancia del controlador Bluetooth
// ESP32BleGamepad gamepad;
//
String direction = "";
int velocidad = 0;

XboxSeriesXControllerESP32_asukiaaa::Core
xboxController("68:6C:E6:96:99:7E");  //Nicolas

const int braker = 0;
const int slow = 30;
const int medium = 90;
const int top = 160;

void setup() {
  // Configuración de pines
  setupMotors();
  setupUltrasonicSensors();
  setupInfraredSensors();

  // Inicializar el controlador Bluetooth
  xboxController.begin();

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
  readControl();

  delay(5); // Esperar un poco antes de la siguiente lectura
  // printVariables(direction, velocidad);
}

void readSensores() {

}

// Funcione para leer el control
void readControl() {
  xboxController.onLoop();

  // Leer boton Start
  bool btnStart = xboxController.xboxNotif.btnStart;

  // Print de los valores de los botones
  Serial.println("---- Control ---- Start ----");
  Serial.print("Botón Start: ");
  Serial.println(btnStart);

  // Gestionar cambio de modo
  if (btnStart) {
    manualMode = !manualMode;
  }

  // Modo manual
  if (manualMode) {
    Serial.println("Modo manual");
    handleManualMode();
  }
  // Modo autónomo
  else {
    Serial.println("Modo autónomo");
    handleAutonomousMode();
  }

}

// Función para manejar el modo manual
void handleManualMode() {
  // Leer los valores de los joysticks
  int joyLHori = xboxController.xboxNotif.joyLHori;
  int joyLVert = xboxController.xboxNotif.joyLVert;
  int joyRHori = xboxController.xboxNotif.joyRHori;
  int joyRVert = xboxController.xboxNotif.joyRVert;
  int trigLT = xboxController.xboxNotif.trigLT;
  int trigRT = xboxController.xboxNotif.trigRT;
  bool btnA = xboxController.xboxNotif.btnA;
  bool btnB = xboxController.xboxNotif.btnB;
  bool btnX = xboxController.xboxNotif.btnX;

  Serial.print("Botón A: ");
  Serial.println(btnA);
  Serial.print("Botón B: ");
  Serial.println(btnB);
  Serial.print("Botón X: ");
  Serial.println(btnX);
  Serial.print("Gatillo izquierdo: ");
  Serial.println(trigLT);
  Serial.print("Gatillo derecho: ");
  Serial.println(trigRT);
  Serial.print("Joystick izquierdo horizontal: ");
  Serial.println(joyLHori);
  Serial.print("Joystick izquierdo vertical: ");
  Serial.println(joyLVert);
  Serial.print("Joystick derecho horizontal: ");
  Serial.println(joyRHori);
  Serial.print("Joystick derecho vertical: ");
  Serial.println(joyRVert);
  Serial.println("---- Control ---- End ----");

  DIRECTION dir = STOP;
  int speed = BRAKER;

 // Control dirección y velocidad
  int maxSpeed = 135;

  if (trigRT > 300) {
    // Direcciones hacia adelante
    if (joyLHori >= 48000) {
      dir = FORWARD_RIGHT;
    } else if (joyLHori <= 16000) {
      dir = FORWARD_LEFT;
    } else {
      dir = FORWARD;
    }
    speed = map(trigRT, 0, 1023, 0, maxSpeed);
  } else if (trigLT > 300) {
    // Direcciones hacia atrás
    if (joyLHori >= 48000) {
      dir = BACKWARD_RIGHT;
    } else if (joyLHori <= 16000) {
      dir = BACKWARD_LEFT;
    } else {
      dir = BACKWARD;
    }
    speed = map(trigLT, 0, 1023, 0, maxSpeed);
  } else {
    // Direcciones laterales
    if (joyLHori >= 48000) {
      dir = RIGHT;
      speed = maxSpeed * 0.8;
    } else if (joyLHori <= 16000) {
      dir = LEFT;
      speed = maxSpeed * 0.8;
    } else {
      dir = STOP;
      speed = 0;
    }
  }

  // Control velocidad
//  if (btnA) {
//    speed = MEDIUM;
//  } else if (btnX) {
//     speed = TOP;
//   } else if (btnB) {
//     speed = SLOW;
//   } else {
//     speed = BRAKER;
//   }

  controlEngine(speed, dir);
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
void controlEngine(int speed, DIRECTION dir) {
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
      digitalWrite(ENG1PIN1, HIGH);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, HIGH);
      direction = "RIGHT";
      break;
    case LEFT:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, HIGH);
      digitalWrite(ENG2PIN1, HIGH);
      digitalWrite(ENG2PIN2, LOW);
      direction = "LEFT";
      break;
    case FORWARD_RIGHT:
      digitalWrite(ENG1PIN1, HIGH);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, LOW);
      direction = "FORWARD_RIGHT";
      break;
    case FORWARD_LEFT:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, HIGH);
      digitalWrite(ENG2PIN2, LOW);
      direction = "FORWARD_LEFT";
      break;
    case BACKWARD_RIGHT:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, HIGH);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, LOW);
      direction = "BACKWARD_RIGHT";
      break;
    case BACKWARD_LEFT:
      digitalWrite(ENG1PIN1, LOW);
      digitalWrite(ENG1PIN2, LOW);
      digitalWrite(ENG2PIN1, LOW);
      digitalWrite(ENG2PIN2, HIGH);
      direction = "BACKWARD_LEFT";
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
  // controlSpeed(speed);
    ledcWrite(ENABLE1PIN, speed); // Canal PWM para motor izquierdo
    ledcWrite(ENABLE2PIN, speed); // Canal PWM para motor derecho
    velocidad = speed;
}

// Función para controlar velocidad
void controlSpeed(VELOCIDAD speed) {
  switch (speed) {
    case SLOW:
      ledcWrite(ENABLE1PIN, slow); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, slow); // Canal PWM para motor derecho
      velocidad = slow;
    case MEDIUM:
      ledcWrite(ENABLE1PIN, medium); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, medium); // Canal PWM para motor derecho
      velocidad = medium;
    case TOP:
      ledcWrite(ENABLE1PIN, medium); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, medium); // Canal PWM para motor derecho
      velocidad = top;
    case BRAKER:
    default:
      ledcWrite(ENABLE1PIN, braker); // Canal PWM para motor izquierdo
      ledcWrite(ENABLE2PIN, braker); // Canal PWM para motor derecho
      velocidad = braker;
  }
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

// Función para manejar el modo autónomo
void handleAutonomousMode() {
  // Leer distancias desde los sensores ultrasónicos
  float distanceFront = readDistance(SOUND1TRIGGER, SOUND1ECHO);
  float distanceSide = readDistance(SOUND2TRIGGER, SOUND2ECHO);

  // Leer los valores de los sensores infrarrojos
  bool irSensor1 = digitalRead(IR1PIN);
  bool irSensor2 = digitalRead(IR2PIN);


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
