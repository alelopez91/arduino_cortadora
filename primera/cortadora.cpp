#include "Arduino.h"

class CortadoraClass{
  private:
    //Ruedas izquierda - derecha
    int RD_ATRAS;
    int RD_ADELANTE;
    int RI_ATRAS;
    int RI_ADELANTE;

    // Ultrasonido izquierdo - derecho
    int US_ADELANTE_TRIG;
    int US_ADELANTE_ECHO;
    int US_IZQ_TRIG;
    int US_IZQ_ECHO;

    
  public:
    bool tiene_posicion_inicial;
    bool busca_contorno;
    bool encontro_pared;

  void init(){
    Serial.println("Comenzando...");
    tiene_posicion_inicial = false;
    busca_contorno = false;
    encontro_pared = false;
    

    RD_ATRAS = 3;
    RD_ADELANTE = 4;
    RI_ATRAS = 5;
    RI_ADELANTE = 6;
    US_ADELANTE_TRIG = 8;
    US_ADELANTE_ECHO = 9;
    US_IZQ_TRIG = 10;
    US_IZQ_ECHO = 11;

      // declaracion de pines dentro de arduino
    pinMode(RI_ATRAS,OUTPUT);
    pinMode(RI_ADELANTE,OUTPUT);
    pinMode(RD_ATRAS,OUTPUT);
    pinMode(RD_ADELANTE,OUTPUT);
    pinMode(US_ADELANTE_TRIG, OUTPUT);
    pinMode(US_ADELANTE_ECHO, INPUT);
    pinMode(US_IZQ_TRIG, OUTPUT);
    pinMode(US_IZQ_ECHO, INPUT);

    //Estados iniciales de pines
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    digitalWrite(US_ADELANTE_TRIG, LOW);
    digitalWrite(US_IZQ_TRIG, LOW);

    Serial.println("Setup hecho.");
  }

  void girar_izq(int mov){
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, HIGH);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    Serial.println("gira izq");
    
    delay(mov);
    digitalWrite(RD_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, LOW);
    
  }

  void girar_der(int mov){
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, HIGH);
    digitalWrite(RD_ADELANTE, LOW);
    Serial.println("gira derecha");
    
    delay(mov);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RD_ATRAS, LOW);
    
  }

  void corregir_izq(int mov){
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    Serial.println("corrige izq");
    
    delay(mov);
    digitalWrite(RD_ADELANTE, LOW);
    
  }

  void corregir_der(int mov){
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    Serial.println("corrige derecha");
    
    delay(mov);
    digitalWrite(RI_ADELANTE, LOW);
     
  }

  void mover_adelante(int mov){
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    Serial.println("adelante");
    
    delay(mov);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    
  }

  void mover_atras(int mov){
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, HIGH);
    digitalWrite(RD_ATRAS, HIGH);
    digitalWrite(RD_ADELANTE, LOW);
    Serial.println("atras");
    
    delay(mov);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    
  }

  //--------------------------------------------------------------
  //--------------------------------------------------------------
  //--------------------------------------------------------------
  void mover_adelante2(int mov){
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    Serial.println("adelante2");
    
    delay(mov);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    
  }

  void mover_atras2(int mov){
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    Serial.println("atras2");
    
    delay(mov);
    digitalWrite(5, LOW);
    digitalWrite(3, LOW);
    
  }
  //------------------------------------------------------------
  //--------------------------------------------------------------
  //--------------------------------------------------------------

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
      
    Serial.print("Distancia ");
    Serial.print(distancia);
    Serial.print(" cm - Pin echo: ");
    Serial.println(echo);

    // if(distancia<6){
    //   Serial.println("muy cerca");
    //   corregir_der(150);
    //   return true;
    // }
    // else{
    //   if(distancia>=6 and distancia<=8){
    //     Serial.println("Ok");
    //     return true;
    //   }
    //   else{
    //     if(distancia>8 and distancia<=10){
    //       Serial.println("Lejos");
    //       corregir_izq(150);
    //       return true;
    //     }
    //     else{
    //       return false;
    //     }
    //   }
    // }
  }

  void prueba(){
    Serial.println("izquierdo");
    hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO);
    // Serial.println("adelante");
    // hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO);
  }

  void detectar_posicion_inicial(){
    if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == true){
      Serial.println("Guarda pos inicial");
      tiene_posicion_inicial = true;
      busca_contorno = true;
      }else{
        if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true){
          girar_der(3000);
        }
        else{          
          mover_adelante(3000);
        }
      }
    }

    void recorrer_contorno(){

      Serial.println("tiene pos");
      if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == true){
        encontro_pared = false;
        girar_der(3000);
      }

      if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == true and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == false){
        girar_der(3000);
      }
      else{
        if(hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == false and encontro_pared == true){
          mover_adelante(3000);
          girar_izq(3000);
          mover_adelante(3000);
        }
      }

      if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == false and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == true){
        encontro_pared = true;
        mover_adelante(3000);
      }

      if(hay_obstaculo(US_ADELANTE_TRIG, US_ADELANTE_ECHO) == false and hay_obstaculo(US_IZQ_TRIG, US_IZQ_ECHO) == false){
        mover_adelante(3000);
      }
    } 
};
