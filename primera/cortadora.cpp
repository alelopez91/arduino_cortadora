#include "Arduino.h"
#include "Libraries/eFLL/FuzzyRule.h"
#include "Libraries/eFLL/FuzzyComposition.h"
#include "Libraries/eFLL/Fuzzy.h"
#include "Libraries/eFLL/FuzzyRuleConsequent.h"
#include "Libraries/eFLL/FuzzyOutput.h"
#include "Libraries/eFLL/FuzzyInput.h"
#include "Libraries/eFLL/FuzzyIO.h"
#include "Libraries/eFLL/FuzzySet.h"
#include "Libraries/eFLL/FuzzyRuleAntecedent.h"

class CortadoraClass{
  private:
    //Ruedas izquierda - derecha
    int RD_ATRAS;
    int RD_ADELANTE;
    int RI_ATRAS;
    int RI_ADELANTE;
    int VELOCIDAD;

    // Ultrasonido izquierdo - derecho
    int US_ADELANTE_TRIG;
    int US_ADELANTE_ECHO;
    int US_IZQ_TRIG;
    int US_IZQ_ECHO;

    
  public:
    bool tiene_posicion_inicial;
    bool busca_contorno;
    bool encontro_pared;
    unsigned long time;

  void init(){
    Serial.println("Comenzando...");
    tiene_posicion_inicial = false;
    busca_contorno = false;
    encontro_pared = false;
    
    RI_ATRAS = 4;
    RI_ADELANTE = 5;
    RD_ADELANTE = 6;
    RD_ATRAS = 7;
    US_ADELANTE_ECHO = 8;
    US_ADELANTE_TRIG = 9;
    US_IZQ_ECHO = 10;
    US_IZQ_TRIG = 11;
    VELOCIDAD = 3;

    // declaracion de pines dentro de arduino
    pinMode(RI_ATRAS,OUTPUT);
    pinMode(RI_ADELANTE,OUTPUT);
    pinMode(RD_ATRAS,OUTPUT);
    pinMode(RD_ADELANTE,OUTPUT);
    pinMode(US_ADELANTE_TRIG, OUTPUT);
    pinMode(US_ADELANTE_ECHO, INPUT);
    pinMode(US_IZQ_TRIG, OUTPUT);
    pinMode(US_IZQ_ECHO, INPUT);
    pinMode(VELOCIDAD,OUTPUT);

    //Estados iniciales de pines
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    digitalWrite(US_ADELANTE_TRIG, LOW);
    digitalWrite(US_IZQ_TRIG, LOW);

    Serial.println("Setup hecho.");
  }

  void girar_izq(){
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, HIGH);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    // Serial.println("Gira izq");
    analogWrite(VELOCIDAD,130);
  }

  void girar_der(){
    analogWrite(VELOCIDAD,0);
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, HIGH);
    digitalWrite(RD_ADELANTE, LOW);
    // Serial.println("gira derecha");
    analogWrite(VELOCIDAD,130);
  }

  void corregir_izq(){
    analogWrite(VELOCIDAD,0);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    // Serial.println("corrige izq");
    analogWrite(VELOCIDAD,130);
  }

  void corregir_der(){
    analogWrite(VELOCIDAD,0);
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    // Serial.println("corrige derecha");
    analogWrite(VELOCIDAD,130);
     
  }

  void mover_adelante(){
    analogWrite(VELOCIDAD,0);
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    // Serial.println("adelante");
    analogWrite(VELOCIDAD,130);
  }

  void mover_atras(){
    analogWrite(VELOCIDAD,0);

    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, HIGH);
    digitalWrite(RD_ATRAS, HIGH);
    digitalWrite(RD_ADELANTE, LOW);
    // Serial.println("atras");
    analogWrite(VELOCIDAD,130);
  }  

  bool hay_obstaculo(int trig, int echo){
    long distancia;
    long tiempo;

    digitalWrite(trig,LOW); /* Por cuestión de estabilización del sensor*/
    delayMicroseconds(2);
    digitalWrite(trig, HIGH); /* envío del pulso ultrasónico*/
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
      
    /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío del pulso ultrasónico y cuando el sensor recibe el rebote*/
    tiempo=pulseIn(echo, HIGH); 
      
    /*fórmula para calcular la distancia obteniendo un valor entero*/
    distancia= int((tiempo/2)/29); 
      
    // Serial.print("Distancia ");
    // Serial.print(distancia);
    // Serial.print(" cm - Pin echo: ");
    // Serial.println(echo);

    if(distancia<6){
      // Serial.println("muy cerca");
      corregir_der();
      return true;
    }
    else{
      if(distancia>=6 and distancia<=8){
        // Serial.println("Ok");
        return true;
      }
      else{
        if(distancia>8 and distancia<=10){
          // Serial.println("Lejos");
          corregir_izq();
          return true;
        }
        else{
          return false;
        }
      }
    }
  }

 void detectar_posicion_inicial(){
    if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == true){
      // Serial.println("Guarda pos inicial");
      tiene_posicion_inicial = true;
      busca_contorno = true;
      }else{
        if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true){
          girar_der();
        }
        else{          
          mover_adelante();
        }
      }
    }

  void recorrer_contorno(){

    if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == true){
      encontro_pared = false;
      Serial.println("adelante true - izq true");
      girar_der();
    }

    if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == false){
      Serial.println("adelante true - izq false");
      girar_der();
    }
    else{
      if(hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == false and encontro_pared == true){
        Serial.println("izq false - pared true");
        mover_adelante();
        girar_izq();
        mover_adelante();
      }
    }

    if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == false and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == true){
      Serial.println("adelante false - izq true");
      encontro_pared = true;
      mover_adelante();
    }

    if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == false and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == false){
      Serial.println("adelante false - izq false");
      mover_adelante();
    }
  } 
};
