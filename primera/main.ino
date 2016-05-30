#include "cortadora.cpp"

CortadoraClass cortadora;

void setup(){
  Serial.begin(9600);
  cortadora.init();
}

void loop(){
  cortadora.prueba();
  // if(cortadora.tiene_posicion_inicial == false){
  //   cortadora.detectar_posicion_inicial();
  // } else {
  //   if (cortadora.busca_contorno == true){
  //     cortadora.recorrer_contorno();
  //   }
  // }

  // delay(500);
  // digitalWrite(3, HIGH);
  // digitalWrite(4, HIGH);
  // digitalWrite(5, HIGH);
  // digitalWrite(6, HIGH);
  // delay(500);
  // digitalWrite(3, LOW);
  // digitalWrite(4, LOW);
  // digitalWrite(5, LOW);
  // digitalWrite(6, LOW);



  // Serial.println("Adelante");
  // cortadora.hay_obstaculo(9,8);
  //delay(2000);
  // Serial.println("Izquierda");
  // cortadora.hay_obstaculo(11,10);
  //delay(2000);
  
  // cortadora.mover_adelante2(3000);
  
  // cortadora.mover_atras2(3000);

  // cortadora.mover_adelante(3000);
  
  // cortadora.mover_atras(3000);


};
