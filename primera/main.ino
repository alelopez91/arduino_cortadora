#include "cortadora.cpp"

CortadoraClass cortadora;
int ENCODER_DER = 3;
int ENCODER_IZQ = 2;
int contador = 0;
int velocidad =180;
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
  
};

void cuenta_vueltas_der(){
  // Serial.print(cortadora.ticks_der); Serial.print("\n");
  // Serial.print("Ticks der: ");
  cortadora.ticks_der += 1;
}

void cuenta_vueltas_izq(){
  // Serial.print("Ticks izq: ");
  // Serial.print(cortadora.ticks_izq); Serial.print("\n");
  cortadora.ticks_izq += 1;
}

  // while(contador<3){
    // digitalWrite(12, HIGH);
    // digitalWrite(13, LOW);
    // analogWrite(11, velocidad);
  // 
    // digitalWrite(8, HIGH);
    // digitalWrite(9, LOW);
    // analogWrite(10, velocidad);
    // 
    // delay(1000);
  // 
    // digitalWrite(12, LOW);
    // digitalWrite(13, HIGH);
    // analogWrite(11, velocidad);
  // 
    // digitalWrite(8, LOW);
    // digitalWrite(9, HIGH);
    // analogWrite(10, velocidad);
  // 
    // delay(1000);
    // contador++;
  // }
    // analogWrite(10, 0);
    // analogWrite(11, 0);