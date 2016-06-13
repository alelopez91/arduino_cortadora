#include "cortadora.cpp"

CortadoraClass cortadora;
int ENCODER_DER = 2;
int contador = 0;
int pos_x = 0;
int pos_y = 10;
void setup(){
  Serial.begin(9600);
  cortadora.init();
  cortadora.ticks_der = -2;
  cortadora.ticks_izq = -2;

  pinMode(ENCODER_DER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DER), cuenta_vueltas_der, CHANGE);
  // attachInterrupt(0, cuenta_vueltas_izq, RISING);



  Serial.println("1...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("Comenzando...");
}

void loop(){
  
  // Serial.print(digitalRead(cortadora.ENCODER_IZQ));
  // Serial.println("    ");
  cortadora.mover_adelante(50, 0);
  // Serial.println("asdasdasdasdasdasdsasdasdasdlk");
  // delay(3000);
  // cortadora.mover_atras(120, 120);
  // delay(4000);
  // cortadora.girar_der(200, 200);
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
  Serial.print(cortadora.ticks_der); Serial.print("\n");
  // Serial.print("Ticks der: ");
  cortadora.ticks_der += 1;
}

void cuenta_vueltas_izq(){
  // Serial.print("Ticks izq: ");
  // Serial.print(cortadora.ticks_izq); Serial.print("\n");
  cortadora.ticks_izq += 1;
}