#include "adaf.h"
AdafruitIO_Feed *counter = io.feed("counter");
/////////////////////////////////////////1
#define TR1 5 // = pin 1
#define EC1 4 // =´pin 2
#define buzzer 13 // = pin 7
#define vibrador 15 // = pin 8

long duration, distance;
long distancec;

void setup() {
  Serial.begin(9600);
  pinMode(TR1, OUTPUT);
  pinMode(EC1, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(vibrador, OUTPUT);
  //////////////////////// Conecta a la red adafruit
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".-");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}
void loop() {
  
  distancec = distanciar(TR1, EC1);
  delay(100);
  Serial.println("/////// 2");
  //////////////////////////////////////////////////
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(distancec);
  counter->save(distancec);

  delay(500);
////////////////////////////////////////////////
}

long distanciar(int trigger, int echo)
{
  digitalWrite(trigger, LOW);  
  delayMicroseconds(2); 

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;

  Serial.print("Centimeter: ");
  Serial.println(distance);
  delay(500);

  if(distance <= 25)
  {
    digitalWrite(vibrador, 150);
    Serial.println("vibrando");
      if(distance <= 15)
      {
      analogWrite(buzzer, 10);
      }
   }else
   {
     digitalWrite(vibrador, 0);
   }
   if(distance > 15)
   {
      analogWrite(buzzer, LOW);
   }
  
  return distance;
}
