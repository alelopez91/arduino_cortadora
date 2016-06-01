#include "Arduino.h"
//#include "Ultrasonic.h"
#include "Libraries/eFLL/FuzzyRule.h"
#include "Libraries/eFLL/FuzzyComposition.h"
// #include "Libraries/eFLL/Fuzzy.h"
#include "Libraries/eFLL/Fuzzy.cpp"
#include "Libraries/eFLL/FuzzyRuleConsequent.h"
#include "Libraries/eFLL/FuzzyOutput.cpp"
#include "Libraries/eFLL/FuzzyInput.cpp"
#include "Libraries/eFLL/FuzzyIO.cpp"
//#include "Libraries/eFLL/FuzzySet.h"
#include "Libraries/eFLL/FuzzySet.cpp"
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

    int dist;

    //Conjuntos difusos de entrada para los ultrasonidos

    //Ultrasonido delantero:
    FuzzySet* adelante_cerca = new FuzzySet(0, 0, 3, 7);        //Conjunto difuso Distancia cerca
    FuzzySet* adelante_bien  = new FuzzySet(5, 8, 8, 11);       //Conjunto difuso Distancia deseada
    FuzzySet* adelante_lejos = new FuzzySet(10, 15, 15, 15);    //Conjunto difuso Distancia lejos
    //Ultrasonido izquierdo:
    FuzzySet* izq_muy_cerca = new FuzzySet(0, 0, 2, 5);         //Conjunto difuso Distancia demasiado cerca
    FuzzySet* izq_cerca     = new FuzzySet(3, 6, 6, 8);         //Conjunto difuso Distancia cerca
    FuzzySet* izq_perfecto  = new FuzzySet(7, 9, 9, 11);        //Conjunto difuso Distancia deseada
    FuzzySet* izq_lejos     = new FuzzySet(10, 12, 12, 15);     //Conjunto difuso Distancia lejos
    FuzzySet* izq_muy_lejos = new FuzzySet(13, 15, 15, 15);     //Conjunto difuso Distancia demasiado lejos

    //Conjuntos difusos de salida para la velocidad de los motores

    //Velocidad de motor:
    FuzzySet* atras_rapido    = new FuzzySet(-200, -200, -140, -100); // Velocidad marcha atras rapida
    FuzzySet* atras           = new FuzzySet(-120,-60,-60, 0);        // Velocidad marcha atras suave
    FuzzySet* detener         = new FuzzySet(0, 0, 0, 0);             // Velocidad para detener     
    FuzzySet* adelante        = new FuzzySet(0,60,60, 120);           // Velocidad hacia adelante
    FuzzySet* adelante_rapido = new FuzzySet(100, 140, 200, 200);     // Velocidad hacia adelante rapida
    
  public:
    bool tiene_posicion_inicial;
    bool busca_contorno;
    bool encontro_pared;
    Fuzzy* fuzzy;
    

  void init(){
    Serial.println("Comenzando...");
    tiene_posicion_inicial = false;
    busca_contorno = false;
    encontro_pared = false;
    fuzzy = new Fuzzy();
    
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

    //Asignar las fciones de pertenencia de entrada
    FuzzyInput* dist_adelante = new FuzzyInput(1);
    dist_adelante->addFuzzySet(adelante_cerca);    //Agrega fuzzyset demasiado cerca a distancia
    dist_adelante->addFuzzySet(adelante_bien);     //Agrega fuzzyset muy cerca a distancia
    dist_adelante->addFuzzySet(adelante_lejos);    //Agrega fuzzyset cerca a distancia
    fuzzy->addFuzzyInput(dist_adelante);           //Agrega entrada difusa al objeto difuso

    FuzzyInput* dist_izq = new FuzzyInput(2);
    dist_izq->addFuzzySet(izq_muy_cerca);     //Agrega fuzzyset demasiado cerca a distancia
    dist_izq->addFuzzySet(izq_cerca);         //Agrega fuzzyset muy cerca a distancia
    dist_izq->addFuzzySet(izq_perfecto);      //Agrega fuzzyset cerca a distancia
    dist_izq->addFuzzySet(izq_lejos);         //Agrega fuzzyset media a distancia
    dist_izq->addFuzzySet(izq_muy_lejos);     //Agrega fuzzyset lejos a distancia
    fuzzy->addFuzzyInput(dist_izq);           //Agrega entrada difusa al objeto difuso

    
    //Asignar las fciones de pertenencia de salida rueda izquierda
    FuzzyOutput* vel_izq = new FuzzyOutput(1);
    vel_izq->addFuzzySet(atras_rapido);       // Agregar fuzzyset marcha atras rapida
    vel_izq->addFuzzySet(atras);              // Agregar fuzzyset marcha atras suave
    vel_izq->addFuzzySet(detener);            // Agregar fuzzyset detener
    vel_izq->addFuzzySet(adelante);           // Agregar fuzzyset adelante
    vel_izq->addFuzzySet(adelante_rapido);    // Agregar fuzzyset adelante rapido     
    fuzzy->addFuzzyOutput(vel_izq);           // Agrega entrada difusa al objeto difuso

    //Asignar las fciones de pertenencia de salida rueda derecha
    FuzzyOutput* vel_der = new FuzzyOutput(1);
    vel_der->addFuzzySet(atras_rapido);       // Agregar fuzzyset marcha atras rapida
    vel_der->addFuzzySet(atras);              // Agregar fuzzyset marcha atras suave
    vel_der->addFuzzySet(detener);            // Agregar fuzzyset detener
    vel_der->addFuzzySet(adelante);           // Agregar fuzzyset adelante
    vel_der->addFuzzySet(adelante_rapido);    // Agregar fuzzyset adelante rapido     
    fuzzy->addFuzzyOutput(vel_der);           // Agrega entrada difusa al objeto difuso
    


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

  void reglas(){

    FuzzyRuleAntecedent* if_dist_adelante_cerca_and_dist_izq_muy_cerca = new FuzzyRuleAntecedent();// Inicializando antecedente de la expresión
    if_dist_adelante_cerca_and_dist_izq_muy_cerca->joinWithAND(adelante_cerca, izq_muy_cerca);     // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_cerca_and_dist_izq_cerca     = new FuzzyRuleAntecedent();// Inicializando antecedente de la expresión
    if_dist_adelante_cerca_and_dist_izq_cerca->joinWithAND(adelante_cerca, izq_cerca);             // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_cerca_and_dist_izq_perfecto = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_cerca_and_dist_izq_perfecto->joinWithAND(adelante_cerca, izq_perfecto);       // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_cerca_and_dist_izq_lejos     = new FuzzyRuleAntecedent();// Inicializando antecedente de la expresión
    if_dist_adelante_cerca_and_dist_izq_lejos->joinWithAND(adelante_cerca, izq_lejos);             // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_cerca_and_dist_izq_muy_lejos = new FuzzyRuleAntecedent();// Inicializando antecedente de la expresión
    if_dist_adelante_cerca_and_dist_izq_muy_lejos->joinWithAND(adelante_cerca, izq_muy_lejos);     // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_bien_and_dist_izq_muy_cerca  = new FuzzyRuleAntecedent();// Inicializando antecedente de la expresión
    if_dist_adelante_bien_and_dist_izq_muy_cerca->joinWithAND(adelante_bien, izq_muy_cerca);        // Agregando conjunto fuzzy correspondiente al antecedente                                        // Agregando FuzzySet correspondiente a la consecuencia
    FuzzyRuleAntecedent* if_dist_adelante_bien_and_dist_izq_cerca      = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_bien_and_dist_izq_cerca->joinWithAND(adelante_bien, izq_cerca);                // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_bien_and_dist_izq_perfecto   = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_bien_and_dist_izq_perfecto->joinWithAND(adelante_bien, izq_perfecto);          // Agregando conjunto fuzzy correspondiente al antecedente                                         // Agregando FuzzySet correspondiente a la consecuencia
    FuzzyRuleAntecedent* if_dist_adelante_bien_and_dist_izq_lejos      = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_bien_and_dist_izq_lejos->joinWithAND(adelante_bien, izq_lejos);                // Agregando conjunto fuzzy correspondiente al antecedente                                         // Agregando FuzzySet correspondiente a la consecuencia
    FuzzyRuleAntecedent* if_dist_adelante_bien_and_dist_izq_muy_lejos  = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_bien_and_dist_izq_muy_lejos->joinWithAND(adelante_bien, izq_muy_lejos);        // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_lejos_and_dist_izq_muy_cerca = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_lejos_and_dist_izq_muy_cerca->joinWithAND(adelante_lejos, izq_muy_cerca);      // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_lejos_and_dist_izq_cerca     = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_lejos_and_dist_izq_cerca->joinWithAND(adelante_lejos, izq_cerca);              // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_lejos_and_dist_izq_perfecto  = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_lejos_and_dist_izq_perfecto->joinWithAND(adelante_lejos, izq_perfecto);        // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_lejos_and_dist_izq_lejos     = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_lejos_and_dist_izq_lejos->joinWithAND(adelante_lejos, izq_lejos);              // Agregando conjunto fuzzy correspondiente al antecedente
    FuzzyRuleAntecedent* if_dist_adelante_lejos_and_dist_izq_muy_lejos = new FuzzyRuleAntecedent(); // Inicializando antecedente de la expresión
    if_dist_adelante_lejos_and_dist_izq_muy_lejos->joinWithAND(adelante_lejos, izq_muy_lejos);      // Agregando conjunto fuzzy correspondiente al antecedente
    
    FuzzyRuleConsequent* then_vel_adelante_rapido = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
    then_vel_adelante_rapido->addOutput(adelante_rapido);                      // Agregando FuzzySet correspondiente a la consecuencia
    FuzzyRuleConsequent* then_vel_adelante        = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
    then_vel_adelante->addOutput(adelante);                                    // Agregando FuzzySet correspondiente a la consecuencia
    FuzzyRuleConsequent* then_vel_atras           = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
    then_vel_atras->addOutput(atras);                                          // Agregando FuzzySet correspondiente a la consecuencia
    
    //Motor rueda izquierda
    // FuzzyRule 1 "IF adelante = cerca and izquierda = muy cerca THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule01 = new FuzzyRule(1, if_dist_adelante_cerca_and_dist_izq_muy_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule01); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 2 "IF adelante = cerca and izquierda = cerca THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule02 = new FuzzyRule(2, if_dist_adelante_cerca_and_dist_izq_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule02); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 3 "IF adelante = cerca and izquierda = perfecto THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule03 = new FuzzyRule(3, if_dist_adelante_cerca_and_dist_izq_perfecto, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule03); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 4 "IF adelante = cerca and izquierda = lejos THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule04 = new FuzzyRule(4, if_dist_adelante_cerca_and_dist_izq_lejos, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule04); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 5 "IF adelante = cerca and izquierda = muy lejos THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule05 = new FuzzyRule(5, if_dist_adelante_cerca_and_dist_izq_muy_lejos, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule05); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 6 "IF adelante = bien and izquierda = muy cerca THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule06 = new FuzzyRule(6, if_dist_adelante_bien_and_dist_izq_muy_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule06); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 7 "IF adelante = bien and izquierda = cerca THEN vel = adelante"
    FuzzyRule* fuzzyRule07 = new FuzzyRule(7, if_dist_adelante_bien_and_dist_izq_cerca, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule07); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 8 "IF adelante = bien and izquierda = perfecto THEN vel = adelante"
    FuzzyRule* fuzzyRule08 = new FuzzyRule(8, if_dist_adelante_bien_and_dist_izq_perfecto, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule08); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 9 "IF adelante = bien and izquierda = lejos THEN vel = adelante"
    FuzzyRule* fuzzyRule09 = new FuzzyRule(9, if_dist_adelante_bien_and_dist_izq_lejos, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule09); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 10 "IF adelante = bien and izquierda = muy lejos THEN vel = adelante"
    FuzzyRule* fuzzyRule010 = new FuzzyRule(10, if_dist_adelante_bien_and_dist_izq_muy_lejos, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule010); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 11 "IF adelante = lejos and izquierda = muy cerca THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule011 = new FuzzyRule(11, if_dist_adelante_lejos_and_dist_izq_muy_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule011); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 12 "IF adelante = lejos and izquierda = cerca THEN vel = adelante"
    FuzzyRule* fuzzyRule012 = new FuzzyRule(12, if_dist_adelante_lejos_and_dist_izq_cerca, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule012); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 13 "IF adelante = lejos and izquierda = perfecto THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule013 = new FuzzyRule(13, if_dist_adelante_lejos_and_dist_izq_perfecto, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule013); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 14 "IF adelante = lejos and izquierda = lejos THEN vel = atras"
    FuzzyRule* fuzzyRule014 = new FuzzyRule(14, if_dist_adelante_lejos_and_dist_izq_lejos, then_vel_atras);  
    fuzzy->addFuzzyRule(fuzzyRule014); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 15 "IF adelante = lejos and izquierda = muy lejos THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule015 = new FuzzyRule(15, if_dist_adelante_lejos_and_dist_izq_muy_lejos, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule015); // Agrega regla difusa al objeto difuso.

    //Motor rueda derecha:
    // FuzzyRule 16 "IF adelante = cerca and izquierda = muy cerca THEN vel = atras rapido"
    FuzzyRule* fuzzyRule016 = new FuzzyRule(16, if_dist_adelante_cerca_and_dist_izq_muy_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule016); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 17 "IF adelante = cerca and izquierda = cerca THEN vel = atras rapido"
    FuzzyRule* fuzzyRule017 = new FuzzyRule(17, if_dist_adelante_cerca_and_dist_izq_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule017); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 18 "IF adelante = cerca and izquierda = perfecto THEN vel = atras rapido"
    FuzzyRule* fuzzyRule018 = new FuzzyRule(18, if_dist_adelante_cerca_and_dist_izq_perfecto, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule018); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 19 "IF adelante = cerca and izquierda = lejos THEN vel = atras rapido"
    FuzzyRule* fuzzyRule019 = new FuzzyRule(19, if_dist_adelante_cerca_and_dist_izq_lejos, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule019); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 20 "IF adelante = cerca and izquierda = muy lejos THEN vel = atras rapido"
    FuzzyRule* fuzzyRule020 = new FuzzyRule(20, if_dist_adelante_cerca_and_dist_izq_muy_lejos, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule020); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 21 "IF adelante = bien and izquierda = muy cerca THEN vel = atras rapido"
    FuzzyRule* fuzzyRule021 = new FuzzyRule(21, if_dist_adelante_bien_and_dist_izq_muy_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule021); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 22 "IF adelante = bien and izquierda = cerca THEN vel = atras"
    FuzzyRule* fuzzyRule022 = new FuzzyRule(22, if_dist_adelante_bien_and_dist_izq_cerca, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule022); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 23 "IF adelante = bien and izquierda = perfecto THEN vel = atras"
    FuzzyRule* fuzzyRule023 = new FuzzyRule(23, if_dist_adelante_bien_and_dist_izq_perfecto, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule023); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 24 "IF adelante = bien and izquierda = lejos THEN vel = atras"
    FuzzyRule* fuzzyRule024 = new FuzzyRule(24, if_dist_adelante_bien_and_dist_izq_lejos, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule024); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 25 "IF adelante = bien and izquierda = muy lejos THEN vel = atras"
    FuzzyRule* fuzzyRule025 = new FuzzyRule(25, if_dist_adelante_bien_and_dist_izq_muy_lejos, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule025); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 26 "IF adelante = lejos and izquierda = muy cerca THEN vel = detener"
    FuzzyRule* fuzzyRule026 = new FuzzyRule(26, if_dist_adelante_lejos_and_dist_izq_muy_cerca, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule026); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 27 "IF adelante = lejos and izquierda = cerca THEN vel = atras"
    FuzzyRule* fuzzyRule027 = new FuzzyRule(27, if_dist_adelante_lejos_and_dist_izq_cerca, then_vel_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule027); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 28 "IF adelante = lejos and izquierda = perfecto THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule028 = new FuzzyRule(28, if_dist_adelante_lejos_and_dist_izq_perfecto, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule028); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 29 "IF adelante = lejos and izquierda = lejos THEN vel = adelante"
    FuzzyRule* fuzzyRule029 = new FuzzyRule(29, if_dist_adelante_lejos_and_dist_izq_lejos, then_vel_atras);  
    fuzzy->addFuzzyRule(fuzzyRule029); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 30 "IF adelante = lejos and izquierda = muy lejos THEN vel = adelante rapido"
    FuzzyRule* fuzzyRule030 = new FuzzyRule(30, if_dist_adelante_lejos_and_dist_izq_muy_lejos, then_vel_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule030); // Agrega regla difusa al objeto difuso.
  }
};
