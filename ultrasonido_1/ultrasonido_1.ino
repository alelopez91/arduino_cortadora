long distancia;
long tiempo;
int trigPin = 9;
int echoPin = 8;

void setup(){
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); /*activación del pin 9 como salida: para el pulso ultrasónico*/
  pinMode(echoPin, INPUT); /*activación del pin 8 como entrada: tiempo del rebote del ultrasonido*/
}

void loop(){
  digitalWrite(trigPin,LOW); /* Por cuestión de estabilización del sensor*/
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); /* envío del pulso ultrasónico*/
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío del pulso ultrasónico y cuando el sensor recibe el rebote*/
  tiempo=pulseIn(echoPin, HIGH); 
  
  /*fórmula para calcular la distancia obteniendo un valor entero*/
  distancia= int((tiempo/2)/29); 

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  delay(50);
}
