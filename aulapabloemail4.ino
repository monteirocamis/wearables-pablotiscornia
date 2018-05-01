//Se descomentar as linhas abaixo, habilita o debugging
#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> //Biblioteca que permite chamar o seu modulo ESP8266 na sua rede pelo nome ao inves do IP.



#define BLUEPIN 13  //Pino D7 do NodeMCU
#define REDPIN 12   //Pino D6 do NodeMCU
#define GREENPIN 14 //Pino D5 do NodeMCU

#define INICAR_CORES_ALEATORIAS 15000 //Tempo ocioso antes de começar a trocar as cores automaticamente

unsigned long ultimoAcessoHost = 0;
unsigned long ultimaTrocaCor = 0;
unsigned long ultimaTrocaCorAutomatica = 0;
boolean trocaAutomatica = 1; //Se voce mudar para 0 (zero), ira desligar a troca automatica de cores.
const char* host      = "pablo"; //Nome que seu ESP8266 (ou NodeMCU) tera na rede
const char* ssid      = "TP-LINK_45F5C6"; //Nome da rede wifi da sua casa
const char* password  = "47831575"; //Senha da rede wifi da sua casa
int RGB[3];
int cnt = 0;
int tempoTrocaCor = 50; //Velicidade que as cores trocam automaticamente
ESP8266WebServer servidorWeb(80); //Servidor Web na porta 80

//Esta e a pagina enviada para o navegador de internet
String paginaWeb = ""
"<!DOCTYPE html><html><head><title>Control RGB</title>"
"<meta name='mobile-web-app-capable' content='yes' />"
"<meta name='viewport' content='width=device-width' />"
"</head><body style='margin: 0px; padding: 0px;'>"
"<canvas id='colorspace'></canvas>"
"</body>"
"<script type='text/javascript'>"
"(function () {"
" var canvas = document.getElementById('colorspace');"
" var ctx = canvas.getContext('2d');"
" function drawCanvas() {"
" var colours = ctx.createLinearGradient(0, 0, window.innerWidth, 0);"
" for(var i=0; i <= 360; i+=10) {"
" colours.addColorStop(i/360, 'hsl(' + i + ', 100%, 50%)');"
" }"
" ctx.fillStyle = colours;"
" ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);"
" var luminance = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);"
" luminance.addColorStop(0, '#ffffff');"
" luminance.addColorStop(0.05, '#ffffff');"
" luminance.addColorStop(0.5, 'rgba(0,0,0,0)');"
" luminance.addColorStop(0.95, '#000000');"
" luminance.addColorStop(1, '#000000');"
" ctx.fillStyle = luminance;"
" ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);"
" }"
" var eventLocked = false;"
" function handleEvent(clientX, clientY) {"
" if(eventLocked) {"
" return;"
" }"
" function colourCorrect(v) {"
" return Math.round(1023-(v*v)/64);"
" }"
" var data = ctx.getImageData(clientX, clientY, 1, 1).data;"
" var params = ["
" 'r=' + colourCorrect(data[0]),"
" 'g=' + colourCorrect(data[1]),"
" 'b=' + colourCorrect(data[2])"
" ].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);"
" req.send();"
" eventLocked = true;"
" req.onreadystatechange = function() {"
" if(req.readyState == 4) {"
" eventLocked = false;"
" }"
" }"
" }"
" canvas.addEventListener('click', function(event) {"
" handleEvent(event.clientX, event.clientY, true);"
" }, false);"
" canvas.addEventListener('touchmove', function(event){"
" handleEvent(event.touches[0].clientX, event.touches[0].clientY);"
"}, false);"
" function resizeCanvas() {"
" canvas.width = window.innerWidth;"
" canvas.height = window.innerHeight;"
" drawCanvas();"
" }"
" window.addEventListener('resize', resizeCanvas, false);"
" resizeCanvas();"
" drawCanvas();"
" document.ontouchmove = function(e) {e.preventDefault()};"
" })();"
"</script></html>";

//////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    InicializaWifi();
    InicializaMDNS();
  }
  else {
    if (millis() - ultimoAcessoHost > 10) {
      servidorWeb.handleClient();
      ultimoAcessoHost = millis();
    }
    if (trocaAutomatica && (millis() - ultimaTrocaCor > INICAR_CORES_ALEATORIAS) && (millis() - ultimaTrocaCorAutomatica > tempoTrocaCor)) {
      ultimaTrocaCorAutomatica = millis();
      CoresAleatorias(cnt++, RGB);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void RecepcaoClienteWeb() {
  String red = servidorWeb.arg(0);
  String green = servidorWeb.arg(1);
  String blue = servidorWeb.arg(2);
  
  analogWrite(REDPIN, 1023 - red.toInt());
  analogWrite(GREENPIN, 1023 - green.toInt());
  analogWrite(BLUEPIN, 1023 - blue.toInt());

  ultimaTrocaCor = millis();
  
  servidorWeb.send(200, "text/html", paginaWeb);
}

//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////

void InicializaWifi() {
  //WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
  }
  servidorWeb.on("/", RecepcaoClienteWeb);
  
  DEBUGGING("Conectado!");
  DEBUGGING(WiFi.localIP());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void InicializaMDNS() {
  if (!MDNS.begin(host)) {
   DEBUGGING("Erro ao iniciar o servico mDNS!");
    while (1) {
      delay(1000);
    }
  }
  DEBUGGING("O servico mDNS foi iniciado com sucesso!");
  MDNS.addService("http", "tcp", 80);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void InicializaPinos(){
  //Iniciando os pinos como saida e com o valor alto (ligado)
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  analogWrite(REDPIN, HIGH);
  analogWrite(GREENPIN, HIGH);
  analogWrite(BLUEPIN, HIGH);
  
  delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CoresAleatorias(int PosicaoNaRoda, int* RGB) {
  RodaDeCores(PosicaoNaRoda, RGB);
  analogWrite(REDPIN, map(RGB[0], 0, 255, 0, 1023));
  analogWrite(GREENPIN, map(RGB[1], 0, 255, 0, 1023));
  analogWrite(BLUEPIN, map(RGB[2], 0, 255, 0, 1023));
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void RodaDeCores(int PosicaoNaRoda, int* RGB) {
  PosicaoNaRoda = PosicaoNaRoda % 256;

  if (PosicaoNaRoda < 85) {
    RGB[0] = PosicaoNaRoda * 3;
    RGB[1] = 255 - PosicaoNaRoda * 3;
    RGB[2] = 0;
  }
  else if (PosicaoNaRoda < 170) {
    PosicaoNaRoda -= 85;
    RGB[2] = PosicaoNaRoda * 3;
    RGB[0] = 255 - PosicaoNaRoda * 3;
    RGB[1] = 0;
  }
  else if (PosicaoNaRoda < 255) {
    PosicaoNaRoda -= 170;
    RGB[1] = PosicaoNaRoda * 3;
    RGB[2] = 255 - PosicaoNaRoda * 3;
    RGB[0] = 0;
  }
  else
  {
    PosicaoNaRoda -= 255;
    RGB[0] = PosicaoNaRoda * 3;
    RGB[1] = 255 - PosicaoNaRoda * 3;
    RGB[2] = 0;
  }
}
