#include <ESP8266WiFi.h>
 
const char* ssid = "#Sala_HULK";
const char* password = "HULK#2017";
 
int ledPinr = 14; 
int ledPino = 15; 

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPinr, OUTPUT);
  digitalWrite(ledPinr, LOW);
  
  pinMode(ledPino, OUTPUT);
  digitalWrite(ledPino, LOW);
  if (request.indexOf("/um") != -1)  {
    digitalWrite(ledPinr, HIGH);
  }
  if (request.indexOf("/dois") != -1)  {
    digitalWrite(ledPino, HIGH);
    
    
  
  if (request.indexOf("/tres") != -1)  {
    digitalWrite(ledPinr, LOW);
  }
  if (request.indexOf("/quatro") != -1)  {
    digitalWrite(ledPino, LOW);      
    
  }
    

 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("");//
 
   
   
 
    client.print("LUZ");
  
  client.println("<br><br>");
  client.println("<a href=\"/um\"\"><button>LIGApinr </button></a>");
  client.println("<a href=\"/dois\"\"><button>LIGApino </button></a>");
  client.println("<a href=\"/tres\"\"><button>DESLIGApinr </button></a>");
  client.println("<a href=\"/quatro\"\"><button>DESLIGApino </button></a>");
