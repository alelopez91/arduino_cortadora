#include "cortadora.cpp"

CortadoraClass cortadora;

void setup(){
  Serial.begin(9600);
  cortadora.init();

  Serial.println("1...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("Comenzando...");
}

void loop(){
  // delay(500);
  // cortadora.mover_adelante(50, 50);
  // delay(3000);
  // cortadora.mover_atras(50, 50);
  // delay(3000);

  if(cortadora.pared_adelante == false){
    cortadora.buscar_pared();
  }
  else{
    if(cortadora.tiene_posicion_inicial == false){
      cortadora.detectar_posicion_inicial();
    }
    else{
      if (cortadora.busca_contorno == true){
         cortadora.recorrer_contorno();
       }
    }    
  }
  // Serial.println("Loop.");

  // Serial.println(cortadora.medir(9,8));
  // Serial.println(cortadora.medir(11,10));
  // delay(2000);
};
