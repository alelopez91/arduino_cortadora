#include "cortadora.cpp"

CortadoraClass cortadora;
// int ENCODER_DER = 2;

void setup(){
  Serial.begin(9600);
  cortadora.init();
  cortadora.ticks_der = -2;
  cortadora.ticks_izq = -2;

  // pinMode(ENCODER_DER, INPUT);
  // digitalWrite(ENCODER_DER, LOW);
  attachInterrupt(digitalPinToInterrupt(cortadora.ENCODER_DER), cuenta_vueltas_der, CHANGE);
  attachInterrupt(digitalPinToInterrupt(cortadora.ENCODER_IZQ), cuenta_vueltas_izq, CHANGE);



  Serial.println("1...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("Comenzando...");
}

void loop(){
  // delay(500);
  cortadora.mover_adelante(50, 50);
  delay(3000);
  cortadora.mover_atras(50, 50);
  // delay(3000);

  // if(cortadora.pared_adelante == false){
  //   cortadora.buscar_pared();
  // }
  // else{
  //   if(cortadora.tiene_posicion_inicial == false){
  //     cortadora.detectar_posicion_inicial();
  //   }
  //   else{
  //     if (cortadora.busca_contorno == true){
  //        cortadora.recorrer_contorno();
  //      }
  //   }    
  // }
  delay(1000);
};

void cuenta_vueltas_der(){
  Serial.print("Ticks: ");
  Serial.print(cortadora.ticks_der); Serial.print("\n");
  cortadora.ticks_der += 1;
}

void cuenta_vueltas_izq(){
  cortadora.ticks_izq += 1;
}