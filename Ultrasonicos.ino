#include <TinyGPS++.h>                                  // Tiny GPS Plus Library
#include <SoftwareSerial.h>                             // Serial
#include <ESP8266WiFi.h>                                // ESP8266 Library

#include <ESP8266WiFi.h>
const char* ssid = "Papadrilo";
const char* password = "zapa12345";
const char* host = "192.168.0.2";  // IP del servidor( CHIP Pro)
const int   port = 8080;            // El puerto donde se encuentra el servidor
const int   watchdog = 5000;        // La frecuencia con la que se envía datos al servidor 
unsigned long previousMillis = millis(); 

static const int RXPin = 1, TXPin = 2;                // Pines para la conexión con el modulo GPS
static const uint32_t GPSBaud = 9600;                   // Tasa de Baudios para conectarse con el gps (9600)

const double Home_LAT = 4.632060;                      // Latitud de la posición del servidor
const double Home_LNG = -74.082119;                     //Longitud de la posición del servidor

double lati = 0;
double longi = 0;

TinyGPSPlus gps;                                        // Crea una instancia del objeto GPS
SoftwareSerial ss(RXPin, TXPin);                        // La conexión serial con el GPS

WiFiServer server(8080);

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

  // Conecta a la red wifi.
  Serial.println();
  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado con WiFi.");
 
  // Inicio del Servidor web.
  server.begin();
  Serial.println("Servidor web iniciado.");
 
  // Esta es la IP
  Serial.print("Esta es la IP para conectar: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop() {
  // Consulta si se ha conectado algún cliente.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Espera hasta que el cliente envíe datos.
  Serial.println("Nuevo cliente.");
  while(!client.available()){
    delay(1);
  }
  distanciarLateral(TR1, EC1);
  delay(100);
  Serial.println("/////// 2");
  distanciarLateral(TR2, EC2);

  ///////Modulo GPS////////////////////////////
  void loop() {

  smartDelay(500);                                  // Energizamos el GPS
  
  unsigned long currentMillis = millis();                        //Tiempo de datos
 
  if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;                             //Conectarse al  server cada cierto tiempo
    WiFiClient client;
  
    if (!client.connect(host, port)) {                           //Avisa cuando se desconecta del server
      Serial.println("connection failed");
      return;
    }
   lati = gps.location.lat();                                  //Variables donde guardamos la latitud
   longi = gps.location.lng();                                 //Variables donde guardamos la longitud

    String url = "/location?command=location&uptime=";          //Creamos una URL para conectarnos al server
    url += String(millis());                                    
    url += "&ip=";
    url += WiFi.localIP().toString();                           //Agregamos la IP del invidente
    url += "&lat=";
    url += String(lati,8);                                      //Agregamos la latitud del invidente
    url += "&long=";
    url += String(longi,8);                                     //Agregamos la longitud del invidenre

    Serial.println(gps.location.lat(),5);                       //Imprimimos la latitud en consola
    Serial.println(gps.location.lng(),5);                       //Imprimimos la longitud en consola
    Serial.println(gps.satellites.value());                     //Imprimimos la satelites en consola

    // Enviamos una solicitud (REQUEST) con la información
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");  //Si la comunicación con el server no es posible se desconecta
        client.stop();
        return;
      }
    }
  
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');    //Si la comunicación con el server es posible imprime en consola 
      Serial.print(line);
    }
  }
}
}

static void smartDelay(unsigned long ms)                //Este código asegura que el gps siempre este energizado
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
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
