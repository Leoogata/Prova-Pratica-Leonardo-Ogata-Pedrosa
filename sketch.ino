#include <WiFi.h>
#include <HTTPClient.h>

#define led_amarelo 9 // Pino utilizado para controle do led amarelo
#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define buttonPin 18  // Pino utilizado para controle do botão
#define ldrPin 4  // Pino utilizado para controle do LDR

int buttonState = 0;  // variável que define estado do botão
int threshold=600; // variável que define claro e escuro
int estadoSinal; // variável que define estado dos LEDs

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT); // Inicializa o LED amarelo como output
  pinMode(led_verde,OUTPUT); // Inicializa o LED amarelo como output
  pinMode(led_vermelho,OUTPUT); // Inicializa o LED amarelo como output
  pinMode(ldrPin, INPUT); // Inicializa o botão com input
  pinMode(buttonPin, INPUT_PULLUP); // Inicializa o botão com input_pullup

  // Inicialização das entradas
  digitalWrite(led_verde, LOW);   // Inicializa o LED verde desligado
  digitalWrite(led_amarelo, LOW);  // Inicializa o LED amarelo desligado 
  digitalWrite(led_vermelho, LOW); // Inicializa o LED vermelho desligado

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void darkMode(){ // Função que ativa o modo escuro 
  digitalWrite(led_amarelo, HIGH);
  delay(1000);
  digitalWrite(led_amarelo, LOW);
  delay(1000);
}

void convMode(){ // Função que ativa o modo convencional
  digitalWrite(led_verde, HIGH);
  estadoSinal = 0;
  delay(3000);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, HIGH);
  estadoSinal = 1;
  delay(2000);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, HIGH);
  estadoSinal = 2;
  delay(5000);
}

void openButton(){ // função que permite ligar o sinal verde caso o botão seja pressionado
  if(estadoSinal == 2 && buttonState == LOW){
    digitalWrite(led_verde, HIGH);
    delay(1000);
    digitalWrite(led_verde, LOW);
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){ // função que define como claro e escuro com base no LDR
    Serial.print("its dark turn on led");
    Serial.println(ldrstatus);
    darkMode();
    
  }else{
    Serial.print("its bright turn off light");
    Serial.println(ldrstatus);
    convMode();
    openButton();
  }
}


