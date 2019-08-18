// Web Server with ESP8266

#include <SoftwareSerial.h>

//RX pin 2, TX pin 3
SoftwareSerial esp8266(2, 3);
 
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(19200);
 
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  delay(3000);
  // Shows the IP address
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}
 
void loop()
{
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;

      String headers = "";
      headers += "HTTP/1.1 200 OK\r\n";
      headers += "Expires: -1\r\n";
      headers += "Cache-Control: private, max-age=0\r\n";
      headers += "Content-Type: text/html; charset=UTF-8\r\n";
 
      //String webpage = getMainPage();
      String webpage = "ok";
 
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += headers.length() + webpage.length() + 2;
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(headers, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
    }
  }
}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.println("-------------");
    Serial.println(response);
    Serial.println("-------------");
  }

  return response;
}

String getMainPage() {
  String webpage = "";
  webpage += "<html>";
  webpage += "<head>";
  webpage += "<script src=""https://code.jquery.com/jquery-3.4.1.min.js"" integrity=""sha256-CSXorXvZcTkaix6Yvo6HppcZGetbYMGWSFlBw8HfCJo="" crossorigin=""anonymous""></script>";
  webpage += "</head>";
  webpage += "<body>";
  webpage += "<h1><u>ESP8266 - Web Server</u></h1>";
  webpage += "<h2>Digital Port 8: ";
  int a = digitalRead(8);
  webpage += a;
  webpage += "</h2>";
  webpage += "<h2>Digital Port 9: ";
  int b = digitalRead(9);
  webpage += b;
  webpage += "</h2>";
  webpage += "<button>Verificar</button>";
  webpage += "<script>";
  webpage += "$('button').click(function(){alert('test')});";
  webpage += "</script>";
  webpage += "</body>";
  webpage += "</html>";

  return webpage;
}
