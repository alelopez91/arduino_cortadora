#include "cortadora.cpp"

CortadoraClass cortadora;

void setup(){
  Serial.begin(9600);
  cortadora.init();

  Serial.println("Comienza Delay");
  delay(3000);
  Serial.println("Termina Delay");
}

void loop(){
  if(cortadora.tiene_posicion_inicial == false){
    cortadora.detectar_posicion_inicial();
  } else {
    if (cortadora.busca_contorno == true){
      cortadora.recorrer_contorno();
    }
  }
};
