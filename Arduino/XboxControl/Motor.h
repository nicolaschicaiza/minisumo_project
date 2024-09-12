#include "Arduino.h"
#include "Esp.h"

class Motor {
private:
  int Motor_A;
  int Motor_B;
  int Channel_1;
  int Channel_2;
  int FRECUENCIA = 5000;
  int RESOLUCION = 8;

public:
  Motor(int MA, int MB, int CH1, int CH2) {
    Motor_A = MA;
    Motor_B = MB;
    Channel_1 = CH1;
    Channel_2 = CH2;
    // Inicializar los pines de los motores como salidas
    ledcAttach(Channel_1, FRECUENCIA, RESOLUCION);
    ledcAttachPin(Motor_A, Channel_1);
    ledcSetup(Channel_2, FRECUENCIA, RESOLUCION);
    ledcAttachPin(Motor_B, Channel_2);
    ledcWrite(Channel_1, 0);
    ledcWrite(Channel_2, 0);
  }

  void Avanzar(int vel) {
    ledcWrite(Channel_1, vel);
    ledcWrite(Channel_2, 0);
  }

  void Retroceder(int vel) {
    ledcWrite(Channel_1, 0);
    ledcWrite(Channel_2, vel);
  }
};
