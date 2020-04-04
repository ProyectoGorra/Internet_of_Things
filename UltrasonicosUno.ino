const int vibr1 = 8; //buzer
const int vibr2 = 9; //buzer
//ultrasonico 1
int tr1 = 2; //tri 
int ech1 = 3; //echo
//ultrasonico 2
int tr2 = 4;
int ech2 = 5;
//ultrasonico 3
int tr3 = 6;
int ech3 = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(vivr1, OUTPUT);//vibrador 1
  pinMode(vivr2, OUTPUT);//vibrador 2
  pinMode(tr1, OUTPUT);
  pinMode(ech1, INPUT);
  pinMode(tr2, OUTPUT);
  pinMode(ech2, INPUT);
  pinMode(tr3, OUTPUT);
  pinMode(ech3, INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("//////////////////");
  distanciarLateral(tr1, ech1);
  distanciarLateral(tr2, ech2);
}
void distanciarLateral(int trigger, int echo)
{
  long duration, distance;
  
  digitalWrite(trigger, LOW);  
  delayMicroseconds(2); 

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;

  Serial.print("Centimeter: ");
  Serial.println(distance);
  delay(50);
  if(distance <= 5){
    digitalWrite(vivr1, 150);
    Serial.println("vibrando");
    }else
    {
      digitalWrite(vivr1, 0);
      Serial.println("NO vibrando");
    }
}