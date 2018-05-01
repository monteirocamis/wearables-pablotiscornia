#include <ESP8266WiFi.h>
 
const char* ssid = "#Sala_HULK";
const char* password = "HULK#2017";
 
int ledPinr = 02; // vermelho
int ledPinv = 14; // verde
int ledPina = 12; // azul


WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPinr, OUTPUT);
  digitalWrite(ledPinr, LOW);

pinMode(ledPinv, OUTPUT);
  digitalWrite(ledPinv, LOW);

pinMode(ledPina, OUTPUT);
  digitalWrite(ledPina, LOW);

  
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  
  
  if (request.indexOf("/A") != -1)  {
    digitalWrite(ledPinr, HIGH);  
  } //liga o vermelho

  
  if (request.indexOf("/B") != -1)  {
    digitalWrite(ledPinr, LOW);
  } //desliga o vermelho
  
    if (request.indexOf("/C") != -1)  {
    digitalWrite(ledPinv, HIGH);  
  } // liga o verde
  
  if (request.indexOf("/D") != -1)  {
    digitalWrite(ledPinv, LOW);
  } // desliga o verde

  
if (request.indexOf("/E") != -1)  {
    digitalWrite(ledPina, HIGH);
  } // liga o azul

  
  if (request.indexOf("/F") != -1)  {
    digitalWrite(ledPina, LOW);
  } // desliga o azul
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("");//
 
   
   
 
    client.print("COR");
  
  client.println("<br><br>");
  client.println("<a href=\"/A\"\"><button>A </button></a>");
  client.println("<a href=\"/B\"\"><button>B </button></a>");
  client.println("<a href=\"/C\"\"><button>C </button></a>");
  client.println("<a href=\"/D\"\"><button>D </button></a>");
  client.println("<a href=\"/E\"\"><button>E </button></a>");
  client.println("<a href=\"/F\"\"><button>F </button></a>");
  client.println("</html>");

  
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
