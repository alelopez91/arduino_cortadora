long distancia;
long tiempo;
int trigPin = 9;
int echoPin = 8;
int IN1= 6; /*IN del puente H*/
int IN2=7;  /*IN del puente H*/

void setup(){
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
}

void loop(){
  digitalWrite(trigPin,LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  tiempo=pulseIn(echoPin, HIGH); 
  distancia= int((tiempo/2)/29); 

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  horario();
  antihorario();
  parado();
  
  delay(50);
}


void horario(){
  if(distancia > 0 && distancia <= 20){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);}
  }


  void antihorario(){
    if(distancia > 50){
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);}
    }

    void parado(){
      if(distancia > 20 && distancia < 50){
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,HIGH);}
      }
