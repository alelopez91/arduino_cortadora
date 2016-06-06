#include "Arduino.h"

#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>
//#include "Ultrasonic.h"

class CortadoraClass{
  private:
    //Ruedas izquierda - derecha
    int RD_ATRAS;
    int RD_ADELANTE;
    int RI_ATRAS;
    int RI_ADELANTE;
    int VEL_IZQ;
    int VEL_DER;
    
    

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

    //Velocidad de motor izquierdo:
    FuzzySet* izq_detener         = new FuzzySet(0, 0, 0, 0);             // Velocidad para detener     
    FuzzySet* izq_adelante        = new FuzzySet(0,60,60, 120);           // Velocidad hacia adelante
    FuzzySet* izq_adelante_rapido = new FuzzySet(100, 140, 200, 200);     // Velocidad hacia adelante rapida

    //Velocidad de motor derecho:
    FuzzySet* der_detener         = new FuzzySet(0, 0, 0, 0);             // Velocidad para detener     
    FuzzySet* der_adelante        = new FuzzySet(0,60,60, 120);           // Velocidad hacia adelante
    FuzzySet* der_adelante_rapido = new FuzzySet(100, 140, 200, 200);     // Velocidad hacia adelante rapida
    
    public:
    bool tiene_posicion_inicial;
    bool busca_contorno;
    bool encontro_pared;
    bool moviendo;
    bool girar_derecha;
    bool girar_izquierda;
    Fuzzy* fuzzy;
    

    void init(){
      tiene_posicion_inicial = false;
      busca_contorno = false;
      moviendo = false;
      girar_derecha = false;
      girar_izquierda = false;
      fuzzy = new Fuzzy();

      RI_ATRAS = 4;
      RI_ADELANTE = 2;
      RD_ADELANTE = 6;
      RD_ATRAS = 7;
      US_ADELANTE_ECHO = 8;
      US_ADELANTE_TRIG = 9;
      US_IZQ_ECHO = 10;
      US_IZQ_TRIG = 11;
      VEL_IZQ = 3;
      VEL_DER = 5;

    // declaracion de pines dentro de arduino
    pinMode(RI_ATRAS,OUTPUT);
    pinMode(RI_ADELANTE,OUTPUT);
    pinMode(RD_ATRAS,OUTPUT);
    pinMode(RD_ADELANTE,OUTPUT);
    pinMode(US_ADELANTE_TRIG, OUTPUT);
    pinMode(US_ADELANTE_ECHO, INPUT);
    pinMode(US_IZQ_TRIG, OUTPUT);
    pinMode(US_IZQ_ECHO, INPUT);
    pinMode(VEL_IZQ,OUTPUT);
    pinMode(VEL_DER,OUTPUT);

    
    

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
    FuzzyOutput* izq = new FuzzyOutput(1);
    izq->addFuzzySet(izq_detener);            // Agregar fuzzyset detener
    izq->addFuzzySet(izq_adelante);           // Agregar fuzzyset adelante
    izq->addFuzzySet(izq_adelante_rapido);    // Agregar fuzzyset adelante rapido     
    fuzzy->addFuzzyOutput(izq);           // Agrega entrada difusa al objeto difuso

    //Asignar las fciones de pertenencia de salida rueda derecha
    FuzzyOutput* der = new FuzzyOutput(2);
    der->addFuzzySet(der_detener);            // Agregar fuzzyset detener
    der->addFuzzySet(der_adelante);           // Agregar fuzzyset adelante
    der->addFuzzySet(der_adelante_rapido);    // Agregar fuzzyset adelante rapido     
    fuzzy->addFuzzyOutput(der);           // Agrega entrada difusa al objeto difuso
    
    //Declaracion de antecedentes
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

    
    //Declaracion de consecuentes

    FuzzyRuleConsequent* then_vel_izq_adelante_and_der_adelante = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
    then_vel_izq_adelante_and_der_adelante->addOutput(izq_adelante);                                    // Agregando FuzzySet correspondiente a la consecuencia
    then_vel_izq_adelante_and_der_adelante->addOutput(der_adelante);                                    // Agregando FuzzySet correspondiente a la consecuencia
    
    FuzzyRuleConsequent* then_vel_izq_adelante_rapido_and_der_detener = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
    then_vel_izq_adelante_rapido_and_der_detener->addOutput(izq_adelante_rapido);                                          // Agregando FuzzySet correspondiente a la consecuencia
    then_vel_izq_adelante_rapido_and_der_detener->addOutput(der_detener);                                          // Agregando FuzzySet correspondiente a la consecuencia
    
    FuzzyRuleConsequent* then_vel_izq_adelante_rapido_and_der_adelante_rapido = new FuzzyRuleConsequent(); // Inicializando consecuencia de la expresión
    then_vel_izq_adelante_rapido_and_der_adelante_rapido->addOutput(izq_adelante_rapido);                      // Agregando FuzzySet correspondiente a la consecuencia
    then_vel_izq_adelante_rapido_and_der_adelante_rapido->addOutput(der_adelante_rapido);


    //Reglas del motor rueda izquierda
    // FuzzyRule 1 "IF adelante = cerca and izquierda = muy cerca THEN vel = izq adelante rapido y der atras rapido"
    FuzzyRule* fuzzyRule01 = new FuzzyRule(1, if_dist_adelante_cerca_and_dist_izq_muy_cerca, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule01); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 2 "IF adelante = cerca and izquierda = cerca THEN vel = izq adelante rapido y der atras rapido"
    FuzzyRule* fuzzyRule02 = new FuzzyRule(2, if_dist_adelante_cerca_and_dist_izq_cerca, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule02); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 3 "IF adelante = cerca and izquierda = perfecto THEN vel = izq adelante rapido y der atras rapido"
    FuzzyRule* fuzzyRule03 = new FuzzyRule(3, if_dist_adelante_cerca_and_dist_izq_perfecto, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule03); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 4 "IF adelante = cerca and izquierda = lejos THEN vel = izq adelante rapido y der atras rapido"
    FuzzyRule* fuzzyRule04 = new FuzzyRule(4, if_dist_adelante_cerca_and_dist_izq_lejos, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule04); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 5 "IF adelante = cerca and izquierda = muy lejos THEN vel = izq adelante rapido y der atras rapido"
    FuzzyRule* fuzzyRule05 = new FuzzyRule(5, if_dist_adelante_cerca_and_dist_izq_muy_lejos, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule05); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 6 "IF adelante = bien and izquierda = muy cerca THEN vel = izq adelante rapido y der atras rapido"
    FuzzyRule* fuzzyRule06 = new FuzzyRule(6, if_dist_adelante_bien_and_dist_izq_muy_cerca, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule06); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 7 "IF adelante = bien and izquierda = cerca THEN vel = izq adelante y der atras"
    FuzzyRule* fuzzyRule07 = new FuzzyRule(7, if_dist_adelante_bien_and_dist_izq_cerca, then_vel_izq_adelante_and_der_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule07); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 8 "IF adelante = bien and izquierda = perfecto THEN vel = izq adelante y der atras"
    FuzzyRule* fuzzyRule08 = new FuzzyRule(8, if_dist_adelante_bien_and_dist_izq_perfecto, then_vel_izq_adelante_and_der_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule08); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 9 "IF adelante = bien and izquierda = lejos THEN vel = izq adelante y der atras"
    FuzzyRule* fuzzyRule09 = new FuzzyRule(9, if_dist_adelante_bien_and_dist_izq_lejos, then_vel_izq_adelante_and_der_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule09); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 10 "IF adelante = bien and izquierda = muy lejos THEN vel = izq adelante y der atras"
    FuzzyRule* fuzzyRule010 = new FuzzyRule(10, if_dist_adelante_bien_and_dist_izq_muy_lejos, then_vel_izq_adelante_and_der_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule010); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 11 "IF adelante = lejos and izquierda = muy cerca THEN vel = izq adelante rapido y der detener"
    FuzzyRule* fuzzyRule011 = new FuzzyRule(11, if_dist_adelante_lejos_and_dist_izq_muy_cerca, then_vel_izq_adelante_rapido_and_der_detener);  
    fuzzy->addFuzzyRule(fuzzyRule011); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 12 "IF adelante = lejos and izquierda = cerca THEN vel = izq adelante y der atras"
    FuzzyRule* fuzzyRule012 = new FuzzyRule(12, if_dist_adelante_lejos_and_dist_izq_cerca, then_vel_izq_adelante_and_der_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule012); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 13 "IF adelante = lejos and izquierda = perfecto THEN vel = izq y der adelante rapido"
    FuzzyRule* fuzzyRule013 = new FuzzyRule(13, if_dist_adelante_lejos_and_dist_izq_perfecto, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule013); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 14 "IF adelante = lejos and izquierda = lejos THEN vel = izq atras y der adelante"
    FuzzyRule* fuzzyRule014 = new FuzzyRule(14, if_dist_adelante_lejos_and_dist_izq_lejos, then_vel_izq_adelante_and_der_adelante);  
    fuzzy->addFuzzyRule(fuzzyRule014); // Agrega regla difusa al objeto difuso.

    // FuzzyRule 15 "IF adelante = lejos and izquierda = muy lejos THEN vel = izq y der adelante rapido"
    FuzzyRule* fuzzyRule015 = new FuzzyRule(15, if_dist_adelante_lejos_and_dist_izq_muy_lejos, then_vel_izq_adelante_rapido_and_der_adelante_rapido);  
    fuzzy->addFuzzyRule(fuzzyRule015); // Agrega regla difusa al objeto difuso.

  }

  void girar_izq(int rueda_izq, int rueda_der){
    Serial.print("Izquierda: ");
    Serial.print(rueda_izq);
    Serial.print(", Derecha: ");
    Serial.println(rueda_der);

    analogWrite(VEL_IZQ,0);
    analogWrite(VEL_DER,0);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, HIGH);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    // Serial.println("Gira izq");
    analogWrite(VEL_IZQ,rueda_izq);
    analogWrite(VEL_DER,rueda_der);
    delay(300);
  }

  void girar_der(int rueda_izq, int rueda_der){
    Serial.print("Izquierda: ");
    Serial.print(rueda_izq);
    Serial.print(", Derecha: ");
    Serial.println(rueda_der);

    analogWrite(VEL_IZQ,0);
    analogWrite(VEL_DER,0);
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, HIGH);
    digitalWrite(RD_ADELANTE, LOW);
    // Serial.println("gira derecha");
    analogWrite(VEL_IZQ,rueda_izq);
    analogWrite(VEL_DER,rueda_der);
    delay(300);
  }

  void corregir_izq(int rueda_izq, int rueda_der){
    // Serial.print("Izquierda: ");
    // Serial.print(rueda_izq);
    // Serial.print(", Derecha: ");
    // Serial.println(rueda_der);

    analogWrite(VEL_IZQ,0);
    analogWrite(VEL_DER,0);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    // Serial.println("corrige izq");
    analogWrite(VEL_IZQ,rueda_izq);
    analogWrite(VEL_DER,rueda_der);
    delay(500);
  }

  void corregir_der(int rueda_izq, int rueda_der){
    // Serial.print("Izquierda: ");
    // Serial.print(rueda_izq);
    // Serial.print(", Derecha: ");
    // Serial.println(rueda_der);

    analogWrite(VEL_IZQ,0);
    analogWrite(VEL_DER,0);
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, LOW);
    // Serial.println("corrige derecha");
    analogWrite(VEL_IZQ,rueda_izq);
    analogWrite(VEL_DER,rueda_der);
    delay(500);

  }

  void mover_adelante(int rueda_izq, int rueda_der){
    Serial.print("Izquierda: ");
    Serial.print(rueda_izq);
    Serial.print(", Derecha: ");
    Serial.println(rueda_der);

    analogWrite(VEL_IZQ,0);
    analogWrite(VEL_DER,0);
    digitalWrite(RI_ADELANTE, HIGH);
    digitalWrite(RI_ATRAS, LOW);
    digitalWrite(RD_ATRAS, LOW);
    digitalWrite(RD_ADELANTE, HIGH);
    // Serial.println("adelante");
    analogWrite(VEL_IZQ,rueda_izq);
    analogWrite(VEL_DER,rueda_der);
  }

  void mover_atras(int rueda_izq, int rueda_der){
    // Serial.print("Izquierda: ");
    // Serial.print(rueda_izq);
    // Serial.print(", Derecha: ");
    // Serial.println(rueda_der);

    analogWrite(VEL_IZQ,0);
    analogWrite(VEL_DER,0);
    digitalWrite(RI_ADELANTE, LOW);
    digitalWrite(RI_ATRAS, HIGH);
    digitalWrite(RD_ATRAS, HIGH);
    digitalWrite(RD_ADELANTE, LOW);
    // Serial.println("atras");
    analogWrite(VEL_IZQ,rueda_izq);
    analogWrite(VEL_DER,rueda_der);
    delay(500);
  }  

  int medir(int trig, int echo){
    int distancia;
    long tiempo;

    digitalWrite(trig,LOW); /* Por cuestión de estabilización del sensor*/
    delayMicroseconds(2);
    digitalWrite(trig, HIGH); /* envío del pulso ultrasónico*/
    delayMicroseconds(10);
    digitalWrite(trig,LOW);      
    /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío del pulso ultrasónico y cuando el sensor recibe el rebote*/
    tiempo=pulseIn(echo, HIGH);       
    /*fórmula para calcular la distancia obteniendo un valor entero*/

    return distancia = int((tiempo/2)/29); 

    // Serial.print("Distancia ");
    // Serial.print(distancia);
    // Serial.print(" cm - Pin echo: ");
    // Serial.println(echo);
    // Serial.println("\n");
  }  

  void detectar_posicion_inicial(){
    int medida_adelante = medir(US_ADELANTE_TRIG, US_ADELANTE_ECHO);
    int medida_izq = medir(US_IZQ_TRIG, US_IZQ_ECHO);

    if(medida_adelante <= 12 and  medida_izq <= 14){
      Serial.println("Guarda pos inicial");
      tiene_posicion_inicial = true;
      busca_contorno = true;
      girar_der(120,120);
    }
    else{
      if(medida_izq <= 14){
        mover_adelante(120,120);
        delay(300);
      }
      if(medida_adelante <= 12){
        girar_der(120,120);
        delay(300);
      }
      if(medida_adelante > 12 and  medida_izq > 14){
        mover_adelante(120,120);
        delay(300);
      }
    }
  }

  void recorrer_contorno(){
    int medida_adelante = medir(US_ADELANTE_TRIG, US_ADELANTE_ECHO);
    int medida_izq = medir(US_IZQ_TRIG, US_IZQ_ECHO);

    if(medida_adelante <= 10){
      moviendo = false;
      encontro_pared = true;
    }
    else{
      if(girar_derecha == true){
        girar_der(120,120);
      }
      if(girar_izquierda == true){
        girar_derecha = false;
        girar_izq(120,120);
      }
      moviendo = true;
    }    
    if(moviendo == true){
      mover_adelante(120,120);
      delay(300);
    }
    else{
      if(medida_adelante <= 10 and girar_derecha == true){
        girar_der(120,120);
        moviendo = false;
        delay(300);
      }
      else{
        if(medida_adelante <= 10){
          girar_der(120,120);
          moviendo = false;
          delay(300);
        }
      }
      if(medida_adelante <= 10 and girar_izquierda == true){
        girar_izq(120,120);
        moviendo = false;
        delay(300);
      }
      if(medida_izq <= 10 and encontro_pared == true){
        moviendo = true;
        encontro_pared = false;
        girar_derecha = true;
        girar_izquierda = false;
        mover_adelante(120,120);
        delay(300);
      }
      else{
        if(medida_izq <= 10){
          moviendo = true;
          mover_adelante(120,120);
          delay(300);
        }        
      }
    }      
  }
};
