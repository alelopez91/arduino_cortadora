#include "Arduino.h"

class CortadoraClass{
  private:
    //Ruedas izquierda - derecha
    int RD_ATRAS;
    int RD_ADELANTE;
    int RI_ATRAS;
    int RI_ADELANTE;

    // Ultrasonido izquierdo - derecho
    int us_adelante_trig;
    int us_adelante_echo;
    int us_izq_trig;
    int us_izq_echo;

    



  public:
  bool tiene_posicion_inicial;
  bool busca_contorno;
  bool encontro_pared;

  void init(){
    Serial.println("Comenzando...");
    tiene_posicion_inicial = false;
    busca_contorno = false;
    encontro_pared = false;
    

    int RD_ATRAS = 3;
    int RD_ADELANTE = 4;
    int RI_ATRAS = 5;
    int RI_ADELANTE = 6;
    int us_adelante_trig = 9;
    int us_adelante_echo = 8;
    int us_izq_trig = 11;
    int us_izq_echo = 10;

      // declaracion de pines dentro de arduino
    pinMode(RI_ATRAS,OUTPUT);
    pinMode(RI_ADELANTE,OUTPUT);
    pinMode(RD_ATRAS,OUTPUT);
    pinMode(RD_ADELANTE,OUTPUT);
    pinMode(us_adelante_trig, OUTPUT);
    pinMode(us_adelante_echo, INPUT);
    pinMode(us_izq_trig, OUTPUT);
    pinMode(us_izq_echo, INPUT);

    //Estados iniciales de pines
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    digitalWrite(us_adelante_trig, LOW);
    digitalWrite(us_izq_trig, LOW);

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

    if(distancia<6){
      Serial.println("muy cerca");
      corregir_der(150);
      return true;
    }
    else{
      if(distancia>=6 and distancia<=8){
        Serial.println("Ok");
        return true;
      }
      else{
        if(distancia>8 and distancia<=10){
          Serial.println("Lejos");
          corregir_izq(150);
          return true;
        }
        else{
          return false;
        }
      }
    }
  }

  void detectar_posicion_inicial(){
    if(hay_obstaculo(us_adelante_trig, us_adelante_echo) == true and hay_obstaculo(us_izq_trig, us_izq_echo) == true){
      Serial.println("Guarda pos inicial");
      tiene_posicion_inicial = true;
      busca_contorno = true;
      }else{
        if(hay_obstaculo(us_adelante_trig, us_adelante_echo) == true){
          girar_der(3000);
        }
        else{          
          mover_adelante(3000);
        }
      }
    }

    void recorrer_contorno(){

      Serial.println("tiene pos");
      if(hay_obstaculo(us_adelante_trig, us_adelante_echo) == true and hay_obstaculo(us_izq_trig, us_izq_echo) == true){
        encontro_pared = false;
        girar_der(3000);
      }

      if(hay_obstaculo(us_adelante_trig, us_adelante_echo) == true and hay_obstaculo(us_izq_trig, us_izq_echo) == false){
        girar_der(3000);
      }
      else{
        if(hay_obstaculo(us_izq_trig, us_izq_echo) == false and encontro_pared == true){
          mover_adelante(3000);
          girar_izq(3000);
          mover_adelante(3000);
        }
      }

      if(hay_obstaculo(us_adelante_trig, us_adelante_echo) == false and hay_obstaculo(us_izq_trig, us_izq_echo) == true){
        encontro_pared = true;
        mover_adelante(3000);
      }

      if(hay_obstaculo(us_adelante_trig, us_adelante_echo) == false and hay_obstaculo(us_izq_trig, us_izq_echo) == false){
        mover_adelante(3000);
      }
    } 
};
