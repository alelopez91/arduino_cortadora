#include "cortadora.cpp"

CortadoraClass cortadora;
volatile int ticks;

void setup(){
  Serial.begin(9600);
  cortadora.init();
  ticks = 0
  cortadora.ticks_der = -2;
  // cortadora.ticks_izq = -2;

  // pinMode(ENCODER_DER, INPUT);
  // digitalWrite(ENCODER_DER, LOW);
  attachInterrupt(digitalPinToInterrupt(cortadora.ENCODER_IZQ), cuenta_vueltas_der, RISING);
  // attachInterrupt(digitalPinToInterrupt(cortadora.ENCODER_IZQ), cuenta_vueltas_izq, RISING);



  Serial.println("1...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("Comenzando...");
}

void loop(){
  cortadora.mover_adelante(50, 0);
  delay(2000);
  // cortadora.mover_atras(120, 120);
  // delay(4000);
  // cortadora.girar_der(200, 200);
  // delay(4000);
  // cortadora.girar_izq(120, 120);


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
  delay(2000);
};

void cuenta_vueltas_der(){
  ticks+=1;
  if(ticks == 50){
    cortadora.bandera == true;
    ticks = 0;
  }
}

// void cuenta_vueltas_izq(){
//   // Serial.print("Ticks izq: ");
//   // Serial.print(cortadora.ticks_izq); Serial.print("\n");
//   cortadora.ticks_izq += 1;
// }