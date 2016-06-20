#include "cortadora.cpp"

CortadoraClass cortadora;
int ENCODER_DER = 2;
int ENCODER_IZQ = 3;
int contador = 0;
int pos_x = 0;
int pos_y = 10;

void setup(){
  Serial.begin(9600);
  cortadora.init();
  cortadora.ticks_der = -2;
  cortadora.ticks_izq = -2;

  pinMode(ENCODER_DER, INPUT_PULLUP);
  pinMode(ENCODER_IZQ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_DER), cuenta_vueltas_der, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_IZQ), cuenta_vueltas_izq, CHANGE);

  Serial.println("1...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("Comenzando...");
}

void loop(){

  // Serial.print(digitalRead(cortadora.ENCODER_IZQ));
  // Serial.println("    ");
  // cortadora.mover_adelante(50, 0);
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
  Serial.print("asdas");
  while(contador<4){

    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    analogWrite(10, 1);
  
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    analogWrite(11, 1);
    
    delay(500);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    delay(500);
  
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    analogWrite(10, 1);
  
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    analogWrite(11, 1);
  
    delay(500);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    delay(500);
    
    contador++;
  }
  
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