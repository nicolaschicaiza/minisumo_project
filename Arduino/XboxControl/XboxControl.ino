#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include "Motor.h"

XboxSeriesXControllerESP32_asukiaaa::Core
xboxController("68:6C:E6:40:A5:0B");  //Juan David
//xboxController("0C:35:26:29:EA:1A");  //Sofia
//xboxController("68:6C:E6:96:99:7E");  //Nicolas


// Definir pines para control de motores
const int  Motor_1A = 26;
const int  Motor_1B = 25;
const int  Motor_2A = 33;
const int  Motor_2B = 32;
const int  Motor_3A = 13;
const int  Motor_3B = 12;
const int  Motor_4A = 14;
const int  Motor_4B = 27;
const int  Motor_5A = 17;
const int  Motor_5B = 5;
const int  Motor_6A = 19;
const int  Motor_6B = 21;

//VARIABLES
const int canal_1A = 0;
const int canal_1B = 1;
const int canal_2A = 2;
const int canal_2B = 3;
const int canal_3A = 4;
const int canal_3B = 5;
const int canal_4A = 6;
const int canal_4B = 7;
const int canal_5A = 8;
const int canal_5B = 9;
const int canal_6A = 10;
const int canal_6B = 11;


//Objetos
Motor motor1(Motor_3A, Motor_3B, canal_3A, canal_3B); // adelante-izquierda
Motor motor2(Motor_4A, Motor_4B, canal_4A, canal_4B); // adelante-derecha
//Motor motor3(Motor_6A, Motor_6B, canal_6A, canal_6B); // medio-izquierda
//Motor motor4(Motor_5A, Motor_5B, canal_5A, canal_5B); // medio-derecha
Motor motor5(Motor_2A, Motor_2B, canal_2A, canal_2B); // atrás-izquierda
Motor motor6(Motor_1A, Motor_1B, canal_1A, canal_1B); // atrás-derecha

void setup() {

  Serial.begin(115200);

  xboxController.begin();
  pinMode(18, OUTPUT);
  digitalWrite(18, HIGH);
}

void loop() {
  leerControl();
}

void leerControl(){
  // put your main code here, to run repeatedly:
  xboxController.onLoop();
  // Leer los valores de los joysticks
  int joyLHori = xboxController.xboxNotif.joyLHori;
  int joyLVert = xboxController.xboxNotif.joyLVert;
  int joyRHori = xboxController.xboxNotif.joyRHori;
  int joyRVert = xboxController.xboxNotif.joyRVert;
  int trigLT = xboxController.xboxNotif.trigLT;
  int trigRT = xboxController.xboxNotif.trigRT;
  bool btnA = xboxController.xboxNotif.btnA;

  if (joyLHori <= 16000 && trigRT >= 300 ){
    // Girar hacia la izquierda
    motor1.Avanzar(80);
    motor2.Avanzar(200);
    //motor3.Avanzar(90);
    //motor4.Avanzar(160);
    motor5.Avanzar(80);
    motor6.Avanzar(200);

  } else if(trigRT >= 300 && joyLHori >= 16000 && joyLHori <= 48000 && joyLVert >= 16000 && joyLVert <= 48000 ) {
    // Ir hacia adelante
    motor1.Avanzar(160);
    motor2.Avanzar(160);
    //motor3.Avanzar(160);
    //motor4.Avanzar(160);
    motor5.Avanzar(160);
    motor6.Avanzar(160);

  } else if(joyLHori >= 45000 && trigRT >= 300) {
    // Girar hacia la derecha
    motor1.Avanzar(200);
    motor2.Avanzar(80);
    //motor3.Avanzar(160);
    //motor4.Avanzar(90);
    motor5.Avanzar(200);
    motor6.Avanzar(80);
    
  } else if (joyLHori <= 16000 && trigRT == 0 && trigLT == 0){
    // Rotar sobre el eje hacia la izquierda
    motor1.Retroceder(160);
    motor2.Avanzar(160);
    //motor3.Retroceder(160);
    //motor4.Avanzar(160);
    motor5.Retroceder(160);
    motor6.Avanzar(160);
    
  } else if(trigLT == 0 && trigLT == 0 && joyLHori >= 16000 && joyLHori <= 48000 && joyLVert >= 16000 && joyLVert <= 48000 ) {
    // Se detiene
      motor1.Avanzar(0);
      motor2.Avanzar(0);
      //motor3.Avanzar(0);
      //motor4.Avanzar(0);
      motor5.Avanzar(0);
      motor6.Avanzar(0);
    
  } else if(joyLHori >= 48000 && trigLT == 0 && trigLT == 0) {
    // Rotar sobre el eje hacia la derecha
    motor1.Avanzar(160);
    motor2.Retroceder(160);
    //motor3.Avanzar(160);
    //motor4.Retroceder(160);
    motor5.Avanzar(160);
    motor6.Retroceder(160);
    
  } else if (joyLHori <= 16000 && trigLT >= 300 ){
    // Retroceder hacia la izquierda
    motor1.Retroceder(80);
    motor2.Retroceder(200);
    //motor3.Retroceder(90);
    //motor4.Retroceder(160);
    motor5.Retroceder(80);
    motor6.Retroceder(200);
    
  } else if(trigLT >= 300 && joyLHori >= 16000 && joyLHori <= 48000 && joyLVert >= 16000 && joyLVert <= 48000) {
    // Ir hacia atrás
    motor1.Retroceder(160);
    motor2.Retroceder(160);
    //motor3.Retroceder(160);
    //motor4.Retroceder(160);
    motor5.Retroceder(160);
    motor6.Retroceder(160);
    
  } else if(joyLHori >= 48000 && trigLT >= 300) {
    // Retroceder hacia la derecha
    motor1.Retroceder(200);
    motor2.Retroceder(80);
    //motor3.Retroceder(160);
    //motor4.Retroceder(90);
    motor5.Retroceder(200);
    motor6.Retroceder(80);
  } 
  else if(btnA == 1) {
    // máxima velocidad
    motor1.Avanzar(210);
    motor2.Avanzar(210);
    //motor3.Avanzar(160);
    //motor4.Avanzar(160);
    motor5.Avanzar(210);
    motor6.Avanzar(210);
}
}



