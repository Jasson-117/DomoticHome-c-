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
int interruptor6;
int interruptor7;
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
String stak;
int f;
int contadors;
int contadork;
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
    if(analogRead(39)>3700){
    alarmaStatus = 1;
    air = "PELIGRO";
    }else if(analogRead(39)>3200){
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
       stak = "MOVIMIENTO";   
      }else if(digitalRead(pir) == 0){
        stak = "DEZPEJADO";
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
    if(interruptor6 == 1){
  digitalWrite(led1,1);
  }else{
  digitalWrite(led1,0);
  }
  if(interruptor7 == 1){
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
      String pagina = "<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"   
"<meta charset=\"UTF-8\">"
"<meta http-equiv=\"refresh\" content=\"1\" >"
    "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<title>Document</title>"
"</head>"
"<style>"
    "*{"
        "box-sizing: border-box;"
        "margin: 0;"
        "padding: 0;"
        "text-decoration: none;"
        "list-style: none;}"
    "body{"
       " margin: 0 1%;"
        "background: #12203e;"
        "font-size: 3rem;"
        "display: grid;"
        "height: 100vh;"
        "grid-template-columns: repeat(3,1fr);"
        "grid-template-rows: repeat(3,1fr);"
        "grid-template-areas: 'temp temp ligh'"
                             "'temp temp ligh1'"
                             "'alar dist mov';"
        "gap:1% ;"
   " }       body .temp{"
        "grid-area: temp;"
        "display: flex;"
        "font-size: 2rem;"
        "flex-direction: column;"
        "justify-content: center;"
        "align-items: center; "       
        "background: #55a8fd;"
        "border-radius: 2rem;"
        "color: brown;        }"
"body .temp .tempdata{"
 "   font-size: 8.9rem;"
    "font-weight: bold;"
    "color:  #f4fa3d;}"
"body .temp .tempdata2{"
 "   font-size: 6rem;"
    "font-weight: bold;"
    "color: #12203e;}"
"body .temp .p2{"
    "font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;"
    "font-size: 3rem;}"
"body .hab1{"
    "border: 2px solid whitesmoke;"
    "color: #12203e;"
    "background: #FAA43D;"
    "font-size: 4.5rem;"
    "font-weight: 100;"
    "height: 100%;"
    "font-weight: bold;"
    "border-radius: 2rem;"
    "text-align: center;"
    "padding-top: 2rem;"
    "line-height: 100%;   }"
"body .hab1 .ona{"
"display: inline-block;"
"padding: 1.3rem;"
"border-radius: 50%;"
"border: 2px solid black;"
"background: hsl(120,95%,65%);}"
"body .hab1 .ofa{"
"display: inline-block;"
"padding: 1.3rem;"
"border-radius: 50%;"
"border: 2px solid black;"
"background: hsl(0,95%,65%);}"
"body .hab2{"
    "color: antiquewhite;"
    "background: #34123e;"
    "border: 2px solid whitesmoke;}"

"body .hab2 .onk{"
    "display: inline-block;"
    "border-radius: 1rem;"
    "background: hsl(120,95%,65%);}"
"body .hab2 .ofk{"
    "display: inline-block;"
  "  border-radius: 1rem;"
 "   background: hsl(0,95%,65%);}"
"body .hab3{"
   " font-size: 7rem;   "
    "color: rgb(246, 239, 41);"
    "background: hsl(252,30%,10%);"
    "border: 2px solid whitesmoke;}"
"body .hab4{    "
 "   background: whitesmoke;"
   " color:rgb(58, 50, 92);"
    "border: 2px solid  hsl(252,30%,10%);"
  "  padding-top: 70px;}"
"body #dateDis{"
 "   font-size: 9rem;"
    "color: #FAA43D;}"
"@media (max-width: 640px) {"
 "   body{"
    "height: 188px;"
    "width: 100vh;"
    "display: flex;"
    "flex-direction: column; }"
    "body .temp{"
  "  height: 900px;"
   " font-weight: bold;"
    "display: flex;"
 "   font-size: 3rem;"
  "  flex-direction: column;"
   " justify-content: center;"
    "align-items: center; "       
  "  background: #55a8fd;"
   " border-radius: 2rem;"
    "color: brown;   }"
"body .temp .tempdata{"
 "   font-size: 12.9rem;"
    "font-weight: bold;"
    "color:  #f4fa3d;}"
"body .temp .p2{"
"    display: flex;"
 "   flex-direction: column;"   
  "  font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;"
   " font-size: 3rem;}"
"body .hab1{"
 "   border: 2px solid whitesmoke;"
  "  color: #12203e;"
 "   background: #FAA43D;"
   " font-size: 4.5rem;"
  "  font-weight: 100;"
   " height: 100%;"
   " font-weight: bold;"
   " border-radius: 2rem;"
  "  text-align: center;"
 "   padding-top: 2rem;}"
"body .hab2{"
 "   color: antiquewhite;"
  "  background: #34123e;"
 "   border: 2px solid whitesmoke;}}"
"</style>"
"<body>"
   " <div class=\"temp\">  <div><span class=\"span1\" >TEMP:<span class=\"tempdata\">"+String(t)+"°</span>&nbsp;</span> <span> HUM:<span class=\"tempdata\">"+String(h)+"%</span> </span> </div>     </br>"
"    <p class=\"p2\">TIEMPO:<span class=\"tempdata2\">"+timed+"</span>  &nbsp; AIRE:<span class=\"tempdata2\">"+air+"</span> </p></div>"
 "   <div><p class=\"hab1\">HABITACION <br><a href=\"on1\" class=\"ona\">ON</a> <a href=\"off1\" class=\"ofa\">OFF</a></p></div>"
  "  <div><p class=\"hab1\">  COMEDOR <br><a href=\"on2\" class=\"ona\">ON</a> <a href=\"off2\" class=\"ofa\">OFF</a></p></div>"
   " <div><p class=\"hab1 hab2\">ALARMA <br><a href=\"alarmon\" class=\"onk\">ON</a> <a href=\"alarmoff\" class=\"ofk\"> OFF</a></p></div>"
    "<div><p class=\"hab1 hab3\" id=\"dateDis1\"><span id=\"dateDis\">"+String(distancia)+"</span>cm</p></div>"
   " <div><p class=\"hab1 hab4\" id=\"dateDis2\">"+stak+"</p></div>"
 "<!--- <div><p>PUERTA<a href=\"\">ON</a> <a href="">OFF</a></p></div>---->"
"<script>"
  "  function interec(){"
  "     const items = document.getElementById('dateDis1'); "
   "    var dateDis = document.getElementById('dateDis').innerHTML;"
    "   const items2 = document.getElementById('dateDis2'); "
     "  var datek = document.getElementById('dateDis2').innerHTML;"
      " if(dateDis < 101 ){"
       " items.style.background = ' rgb(52, 14, 205)'    }"
   " if(dateDis < 31 ){"
    "    items.style.background = ' rgb(241, 108, 0)'    }"
    "if(dateDis < 13 ){"
     "   items.style.background = ' rgb(254, 6, 6)'   }"
    "if(datek == 'MOVIMIENTO' ){"
     "   items2.style.background = ' rgb(246, 239, 41)'  }"
   " console.log(datek);}"
   "interec();"
"</script>"
"</body>"
"</html>";
      if(c == '\n' && currentLineIsBlank){
        client.println("HTTP/1.1 200 ok");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println(pagina) ;  
        break;
        }
        if (c == '\n'){
          currentLineIsBlank = true;
         if(strstr(linebuf,"GET /on1")>0){
              Serial.println("LED 2 ON");
              interruptor6 = 1;   
            }
            else if(strstr(linebuf,"GET /off1")>0){
              Serial.println("LED 2 OFF");
                           interruptor6 = 0;   

            }
            
            else if(strstr(linebuf,"GET /on2")>0){
              Serial.println("LED 2 ON");
                         interruptor7 = 1;   

            }
            else if(strstr(linebuf,"GET /off2")>0){
              Serial.println("LED 2 OFF");
               interruptor7 = 0;     
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
 Serial.println(analogRead(39));
 
  }
/*  void setup(){
    Serial.begin(9600);
    pinMode(39,INPUT);
    }
    void loop(){
      Serial.println(analogRead(39));
      }*/
