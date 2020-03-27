#define TR1 5
#define EC1 4
#define TR2 7
#define EC2 6

long duration, distance;
int BUILTIN_LED = 2;

void setup() {
  Serial.begin (9600);
  pinMode(TR1, OUTPUT);
  pinMode(EC1, INPUT);
  pinMode(TR2, OUTPUT);
  pinMode(EC2, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}
void loop() {
  distanciarLateral(TR1, EC1);
  delay(100);
  Serial.println("/////// 2");
  distanciarLateral(TR2, EC2);
}

void distanciarLateral(int trigger, int echo)
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
}
