#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Servo.h>
#include <WiFi.h>
//---------------------SENSORES
const char* ssid = "ratalison";
const char* password = "faraones23";
const int pir = 25;
const int zumb =  2;
const int foto = 36;
const int mq4 = 39;
#define DHTTYPE DHT11   // DHT 11 
#define DHTPIN 32  
static const int servoPin = 4;
Servo servo1;
//-------------------PULSADORES
const int pulsador1 = 15;
const int pulsador2 = 16;
const int pulsador4 = 19;
//-----------------------LEDS
const int led1 = 13;
const int led2 = 12;
 int led3;
const int led4 = 14;
const int led5 = 17;
const int led6 = 5;
const int ledsGarage = 26;
const int ledsExterior = 18;
int triger = 27;
int eco = 34;
int distancia;
int duracion;
//----------------------INTERRUPTORES
int interruptor1;
int interruptor2;
int interruptor4;
int interruptor5;
int estado;
int estadod = 1;
int alarmaStatus;
int alarmaStatus2;
LiquidCrystal_I2C lcd (0x27, 16,2);  
int nivelLuz = 0;
int valorMapeado = 0;
int sensorValue = 0;
int pren = 0;
int h;
int cont;
int t ;
String t1;
String timed;
String air;
int f;
WiFiServer server(80);
char linebuf[80];
int charcount = 0;
DHT dht(DHTPIN, DHTTYPE, 30);
//-----------------------FUNCIONES----------------------------------
//----------------------------SETUP-------------------------------
void setup(){
  Serial.begin(9600);
  pinMode(triger,OUTPUT);
  pinMode(eco,INPUT);
  pinMode(pulsador1,INPUT);
  pinMode(pulsador2,INPUT);
  pinMode(pulsador4,INPUT);
  pinMode(pir,INPUT);
  pinMode(foto,INPUT); 
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(led6,OUTPUT);
  pinMode(zumb,OUTPUT);
  pinMode(ledsGarage,OUTPUT);
  pinMode(ledsExterior,OUTPUT);
  lcd.init ();
  lcd.backlight ();
  dht.begin();
  servo1.attach(servoPin);
  Serial.printf("Conectando a:%s\n",ssid);
  WiFi.begin(ssid,password);
  delay(4000);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Conectando...");
    delay(2000);
    }
    Serial.println("Conectado.");
    Serial.println("Direccion ip del modulo:");
    Serial.println(WiFi.localIP());
    server.begin();
  }
//------------------------------MQ4--------------------------------
void smog(){
    if(analogRead(39)>2800){
    alarmaStatus = 1;
    air = "PELIGRO";
    }else if(analogRead(39)>2500){
     air = "SUCIO ";
      }else if(analogRead(39)>100){
        air ="LIMPIO ";
        }
  }
//-----------------------------PULSADORES
void pulsadores(){
  if(digitalRead(pulsador1) == 1){
  if(interruptor1 == 1){
     interruptor1 = 0;
   delay(200);
    }else{
     interruptor1 = 1;
        delay(200);    
      }
  }else if(digitalRead(pulsador2) == 1){
  if(interruptor2 == 1){
     interruptor2 = 0;
   delay(200);
    }else{
     interruptor2 = 1;
        delay(200);    
      }
  }else if(digitalRead(pulsador4) == 1){
  if(estado == 1){
     interruptor4 = 0;
     alarmaStatus = 0;
     alarmaBip();
      }else if(alarmaStatus == 1){
     alarmaBip();
     alarmaStatus = 0; 
        }else{
     interruptor4 = 1;
     alarmaBip();
      }
  }
 delay(10);
  }
  //--------------------------------------ALARMA--------------------------------
void alarmaBip(){
       for(int i = 0; i<4;i++){
      digitalWrite(led4,0);
      digitalWrite(zumb,0);
      delay(70);
      digitalWrite(led4,1);
      digitalWrite(zumb,1);
      delay(70);
      digitalWrite(zumb,0);     } 
  }
  void alarma(){  
if(alarmaStatus == 1){
    digitalWrite(zumb,1);
    digitalWrite(led4,1);
  }else if(alarmaStatus == 0){
    digitalWrite(zumb,0);
   // digitalWrite(led4,0);
  }}
  //-----------------------SENSOR-MOVIMIENTO---------------------------------------------
void sensorMovimiento(){
  if(estado == 0){
        digitalWrite(zumb,0);
        digitalWrite(led4,0);
        delay(200);
    }
    if(estado == 1){
      if(digitalRead(pir)==1){
      alarmaStatus = 1;
        digitalWrite(led4,1);  
      delay(200);
    }           
      }
    if(digitalRead(pir) == 1){
       digitalWrite(ledsGarage,1);   
      }else if(digitalRead(pir) == 0){
       digitalWrite(ledsGarage,0);         
      }
    } 
//--------------------------------LCD-------------------------------
void screem(){
   lcd. setCursor (0, 0);
   // We write the number of seconds elapsed 
   lcd. print ("TEMP:"+String(t)+"o"+" "+"HUM:"+String(h)+"%");

   lcd. setCursor (0, 10);
    lcd. print (String(h)+"%");
   lcd. setCursor (0, 1);
   // We write the number of seconds elapsed 
   lcd. print(air+" "+timed);
//   delay (100);
  }
//------------------------------INTERRUPTORES---------------------------
  void interruptores(){
  if(interruptor1 == 1){
  digitalWrite(led1,1);
  }else{
  digitalWrite(led1,0);
  }
  if(interruptor2 == 1){
  digitalWrite(led2,1);
  }else{
  digitalWrite(led2,0);
  }
  if(interruptor4 == 1){
  estado = 1;
  }else{
  estado = 0;
  }}
 
  //----------------------------FOTORESISTENCIA---------------------------------
void fotoResistencia(){
 valorMapeado = map(analogRead(foto),2000,1100,0,255);
 analogWrite(ledsExterior,valorMapeado);
 if(analogRead(foto)>1099){
 analogWrite(ledsExterior,0);
 timed = "DIA  "; 
  }else
  if(analogRead(foto)>700){
  timed = "TARDE  ";   
    }else
    if(analogRead(foto)>100){
  timed = "NOCHE  ";   
    }
  }
//---------------------------------SENSOR-DISTANCIA--------------------------------
void proximidad(){
  digitalWrite(triger,LOW);
  delayMicroseconds(4);
  digitalWrite(triger,HIGH);
  delayMicroseconds(10); 
  digitalWrite(triger,LOW);
  duracion = pulseIn(eco,HIGH);
  duracion = duracion/2;
  distancia = duracion/29.2; 
 // Serial.println(distancia);
  if(distancia > 30 && distancia < 100){
    digitalWrite(led5,1);
    digitalWrite(led6,0);  
    servo1.write(0);
    servo1.write(0);
    }else if(distancia > 25 && distancia < 30){
      digitalWrite(led5,1);
      digitalWrite(led6,1);
      servo1.write(25);
    }else if(distancia > 20 && distancia < 25){
      digitalWrite(led5,1);
      digitalWrite(led6,1);
      servo1.write(45);
    }else if(distancia > 15 && distancia < 20){
      digitalWrite(led5,1);
      digitalWrite(led6,1);
      servo1.write(65);
    }else if(distancia > 10 && distancia < 15){
      digitalWrite(zumb,1);
      digitalWrite(led4,1);
      digitalWrite(led5,1);
      digitalWrite(led6,1);
      servo1.write(85);
    }else if(distancia > 1 && distancia < 10){
      digitalWrite(zumb,1);
      digitalWrite(led4,1);
      digitalWrite(led5,1);
      digitalWrite(led6,1);
      servo1.write(99);
    }else{
      digitalWrite(led5,0);
      digitalWrite(led6,0);
      servo1.write(0);  
      }
  }
  //----------------------------------DHT11-------------------------------
  void temp(){
   h = dht.readHumidity();
   t = dht.readTemperature();
   t1 = String(t);
   f = dht.readTemperature(true);
   if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    }
//---------------------------------LOOP----------------------------------
void loop(){
  sensorMovimiento();
  fotoResistencia(); 
  pulsadores();
  interruptores();
  smog();
  alarma();
  proximidad();
  screem();
  temp();
 // Serial.println(cont);
  WiFiClient client = server.available();
if(client){
  Serial.println("Nuevo Cliente");
  memset(linebuf,0,sizeof(linebuf));
  charcount = 0;
  boolean currentLineIsBlank = true;
  while (client.connected()){
    if(client.available()){
      char c = client.read();
      Serial.write(c);
      linebuf[charcount] = c;
      if(charcount<sizeof(linebuf)-1) charcount++;
      if(c == '\n' && currentLineIsBlank){
        client.println("HTTP/1.1 200 ok");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE HTML><html><head>");
        client.println("<meta http-equiv=\"refresh\" content=\"1\" >");
        client.println("<body id=\"body\" style=\"transition: background-color 2s; background: radial-gradient(circle, rgba(63,94,251,1) 7%, rgba(23,207,156,0.9411473207447041) 100%);display: flex; flex-direction: column; align-items: center; font-family: 'Maven Pro', sans-serif;font-size: 5rem; gap: 10px;\">");
        client.println("<link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">");
        client.println("<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>");
        client.println("<link href=\"https://fonts.googleapis.com/css2?family=Maven+Pro:wght@700&display=swap\" rel=\"stylesheet\">");
        client.println(" <p style=\"margin: 0; color:#ffffff ; font-size: 8rem;\" id=\"text1\">"+air+" "+timed+"</p>");
        client.println(" <p style=\"margin: 0; color:#ffffff ; font-size: 8rem;\" id=\"text1\">"+String(t)+" "+String(h)+" "+"</p>");
        client.println(" <p style=\"margin: 0; color:#ffffff\">LED 1 <a href=\"on1\"><button style=\"padding:100px ;font-size: 4rem; width: 300px; ; background: hsl(133, 84%, 48%);\">ON</button></a>&nbsp;<a href=\"off1\"><button style=\"padding:20px ; font-size: 4rem; width: 300px;; ; background:red;\">OFF</button></a></p>");
        client.println(" <p style=\"margin: 0; color:#ffffff\">LED 2 <a href=\"on2\"><button style=\"padding:100px ;font-size: 4rem; width: 300px; ; background: hsl(133, 84%, 48%);\">ON</button></a>&nbsp;<a href=\"off2\"><button style=\"padding:20px ; font-size: 4rem; width: 300px; ; background:red;\">OFF</button></a></p>");
        client.println("<script>");
        client.println("function recargar(){");
        client.println("var bodyt=document.getElementById(\"body\")");
        client.println("var temp =document.getElementById(\"text1\").innerHTML;");
        client.println("if(temp > 1000 && temp < 2000){");
        client.println("bodyt.style.background=\"radial-gradient(circle, rgba(63,94,251,1) 7%, rgba(125,23,207,0.9411473207447041) 100%)\"");
        client.println("}else if(temp>2000){");
        client.println("bodyt.style.background=\"radial-gradient(circle, rgba(233,63,251,1) 7%, rgba(207,23,42,0.9411473207447041) 100%)\"}}");
        client.println("recargar();</script>");
        client.println("</body></html>");        
        break;
        }
        if (c == '\n'){
          currentLineIsBlank = true;
          if(strstr(linebuf,"GET /on1")>0){
            Serial.println("LED 1 ON");
            interruptor1 = 1;
            }
            else if(strstr(linebuf,"GET /off1")>0){
              Serial.println("LED 1 OFF");
             interruptor1 = 0;   
            }
            else if(strstr(linebuf,"GET /on2")>0){
              Serial.println("LED 2 ON");
            interruptor4 = 1;   
            }
            else if(strstr(linebuf,"GET /off2")>0){             
              Serial.println("LED 2 OFF");
              cont++;
              if(cont > 0){
              interruptor4 = 0;               
                }else if(cont == 0){
            interruptor4 = 0;           
            alarmaStatus = 0;
           // alarmaBip();
            }  
            }
              currentLineIsBlank = true;
              memset(linebuf,0,sizeof(linebuf));
              charcount= 0;         
      }else if(c != '\r'){
        currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("cliente desconectado");
  }
     
temp();
 Serial.println(distancia);
 
  }
