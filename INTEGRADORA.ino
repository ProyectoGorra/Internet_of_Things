#include "adaf.h"
AdafruitIO_Feed *counter = io.feed("counter");
/////////////////////////////////////////
#define TR1 5 // = pin 1
#define EC1 4 // =´pin 2
#define TR2 13 // = pin 7
#define EC2 15 // = pin 8

long duration, distance;
long distancec, distancec2;

void setup() {
  Serial.begin(9600);
  pinMode(TR1, OUTPUT);
  pinMode(EC1, INPUT);
  pinMode(TR2, OUTPUT);
  pinMode(EC2, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  //////////////////////// Conecta a la red adafruit
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}
void loop() {
  
  distancec = distanciarLateral(TR1, EC1);
  delay(100);
  Serial.println("/////// 2");
  distancec2 = distanciarLateral(TR2, EC2);
  //////////////////////////////////////////////////
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(distancec);
  counter->save(distancec);
  counter->save(distancec2);

  // increment the count by 1

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);
////////////////////////////////////////////////
}

long distanciarLateral(int trigger, int echo)
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
  return distance;
}
