/* Bibliotecas


DHT: "DHT sensor library" by Adafruit
BMP280: Adafruit BMP280 Library by Adafruit 


*/


#include <SPI.h>
#include <Ethernet.h>

#include <LiquidCrystal.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Crear una instancia del sensor BMP280
Adafruit_BMP280 bmp;

#define POTENCIOMETRO A5

#define ESTADO_MENU 0
#define ESTADO_VISUALIZAR 1
#define ESTADO_CONFIGURACION 2

#define OPCION_VISUALIZAR 0
#define OPCION_CONFIGURAR 1


#define PULSADOR_IZQUIERDA 3
#define PULSADOR_OK 4
#define PULSADOR_DERECHA 0

#define PULSADORES A0


#define CANTIDAD_OPCIONES 2


#define TIEMPO_REBOTE 200


#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11


int estado = ESTADO_VISUALIZAR;
bool banderaMensaje = true;
int miliSegundos = 0;


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


DHT dht(DHTPIN, DHTTYPE);


void presentacion(void);


int valorPotenciometro;
int opcionMenu = OPCION_VISUALIZAR;


float h;
float t;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 9, 121, 103);


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

int PulsadorPresionado(void);

void setup() {
  Serial.begin(9600);  //velocidad*/
  lcd.begin(16, 2);


  byte circulo[] = {
    B01110,
    B01010,
    B01110,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };


  lcd.createChar(0, circulo);


  byte acentoLetraO[] = {
    B00001,
    B00010,
    B01110,
    B10001,
    B10001,
    B10001,
    B01110,
    B00000
  };


  lcd.createChar(1, acentoLetraO);


  //pinMode(PULSADOR_IZQUIERDA, INPUT_PULLUP);
  //pinMode(PULSADOR_DERECHA, INPUT_PULLUP);
  //pinMode(PULSADOR_OK, INPUT_PULLUP);

  presentacion();

  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  lcd.clear();
  lcd.print("Conectando...");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");


  /*ETHERNET POR DHCP*/
  /*
// start the Ethernet connection:
Serial.println("Initialize Ethernet with DHCP:");
if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  } else if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  // no point in carrying on, so do nothing forevermore:
  while (true) {
    delay(1);
  }
}
 */


  /*ETHERNET IP FIJA*/


  /*

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);


  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);  // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }


  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


  lcd.print(Ethernet.localIP());


  delay(3000);

  */

  h = dht.readHumidity();
  t = dht.readTemperature();

  if (!bmp.begin(0x76)) {  // Verificar la inicialización del sensor BMP280
    Serial.println(F("El sensor BMP280 no fue identificado!"));
    //while (1); // Entrar en un bucle infinito si el sensor no está conectado
  }
}

void loop() {

  //Serial.println(estado);

  if (estado == ESTADO_MENU) {

    //lcd.clear();
    //lcd.print("ESTADO_MENU");

    //datos switch buscar
    /*MENU*/
    if (banderaMensaje == true) {
      banderaMensaje = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menu Principal:");
      lcd.setCursor(0, 1);
      switch (opcionMenu) {
        case 0:
          lcd.print("<  Visualizar  >");
          break;
        case 1:
          lcd.print("<  Configurar  >");
          break;
      }
    }

    if (PulsadorPresionado() == PULSADOR_DERECHA) {  // derecha
      delay(TIEMPO_REBOTE);

      banderaMensaje = true;

      opcionMenu++;

      if (opcionMenu == CANTIDAD_OPCIONES) {
        opcionMenu = 0;
      }
    }


    if (PulsadorPresionado() == PULSADOR_IZQUIERDA) {  // izquierda
      delay(TIEMPO_REBOTE);


      banderaMensaje = true;


      opcionMenu--;


      if (opcionMenu == -1) {
        opcionMenu = CANTIDAD_OPCIONES - 1;
      }
    }


    if (PulsadorPresionado() == PULSADOR_OK) {
      delay(TIEMPO_REBOTE);


      switch (opcionMenu) {
        case OPCION_VISUALIZAR:
          estado = ESTADO_VISUALIZAR;
          break;
      }
    }
  }

  if (estado == ESTADO_VISUALIZAR) {

    //valorPotenciometro = analogRead(POTENCIOMETRO);
    //lcd.print(valorPotenciometro);

    //lcd.clear();
    //lcd.print("VISUALIZAR");

    h = dht.readHumidity();
    t = dht.readTemperature();


    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      //return;
    }


    //lcd.print(F("Humidity: "));
    //lcd.print(h);
    //lcd.print(F("%  Temperature: "));


    if (miliSegundos >= 1000) {


      miliSegundos = 0;


      lcd.clear();


      lcd.setCursor(0, 0);
      lcd.print("T ");
      lcd.print(t);
      lcd.setCursor(6, 0);
      lcd.print(" ");
      lcd.write(byte(0));
      //lcd.write(int(0));
      lcd.print("C ");
      lcd.print("H ");
      lcd.print(h);
      lcd.setCursor(14,0);
      lcd.print(" %");
      lcd.setCursor(0, 1);
      lcd.print("P ");
      lcd.print(bmp.readPressure()/100);
      lcd.print(" hPa");
      //lcd.setCursor(0, 1);

      Serial.print(F("Temperatura: "));
      Serial.print(bmp.readTemperature());
      Serial.println(" *C (Grados Celsius)");

      Serial.print(F("Presión: "));
      Serial.print(bmp.readPressure());
      Serial.println(" Pa (Pascal)");

      Serial.print(F("Altitud aprox.: "));
      Serial.print(bmp.readAltitude(1013.25), 0);  // Ajusta el valor de presión al nivel del mar si es necesario
      Serial.println(" m (Metros)");

      Serial.println("-----------------------------------");
    }

    //Serial.println(PulsadorPresionado());
    if (PulsadorPresionado() == PULSADOR_OK) {
      delay(TIEMPO_REBOTE);

      banderaMensaje = true;

      opcionMenu = OPCION_VISUALIZAR;

      estado = ESTADO_MENU;
    }
  }

  /*

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");         // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
          client.println("</head>");
          // output the value of each analog input pin

          client.println("<H1 align='center'>");
          client.print("Estaci&oacuten Meteorol&oacutegica ETEC");
          client.println("<body bgcolor='a9cce3''>");
          client.println("<TABLE FRAME='void' RULES='rows'>");
          client.println("<TR>");
          client.println("<TD>");
          client.println("</TD>");
          client.println("</TR>");
          client.println("</H1>");


          client.println("<TR>");
          client.println("<TD>");
          client.println("<H6 align='right'>");
          client.print("Argentina, Buenos Aires");
          client.println("</H6>");
          client.println("<H2 align='center'>");
          client.print("&iquest;Que tal el ambiente hoy&#63");
          client.println("</H2>");
          client.println("</TD>");
          client.println("</TR>");


          client.println("<TD>");
          client.println("<TH ROWSPAN=2>");
          client.println("<H2 align='center'>");
          client.print("<img src='https://ssl.gstatic.com/onebox/weather/64/partly_cloudy.png'width='60' height='55'/>");
          client.println("<TH>");
          client.print("Temperatura: ");
          client.print(t);
          client.print("&ordmc");
          client.println("</TH>");
          client.println("</TR>");
          client.println("<br />");
          client.println("<TR>");
          client.println("<TH>");
          client.print("Humedad: ");
          client.print(h);
          client.print("%");
          client.println("</H2>");
          client.println("</TH>");
          client.println("</TR>");
          client.println("</TD>");


          client.println("<TABLE>");
          client.println("<TR>");
          client.println("<TD>");
          client.println("<H5 align='center'>");
          client.print("Lizbeth Alejo.G Divisi&oacuten 6&ordmB A&ntildeo:2023");
          client.println("</H5>");
          client.println("</TD>");
          client.println("</TR>");

          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

*/

  miliSegundos++;
  delay(1);
}

// Definicion de la funcion "presentacion()"

void presentacion(void) {

  lcd.clear();
  lcd.print("   ETEC - UBA   ");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("  Villa Lugano  ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Projecto:");
  delay(2000);

  lcd.setCursor(0, 1);
  lcd.print("Estaci");
  lcd.write(int(1));
  lcd.print("n");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Meteorol");
  lcd.write(int(1));
  lcd.print("gica");

  delay(2000);

  /*
 lcd.clear();  // Limpio el LCD
 lcd.print("Integrantes:");
 lcd.setCursor(0, 1);  // posiciono el cursor en la columna y fila determinada
 // Print a message to the LCD.
 lcd.print("Nahuel Diganchi");
 delay(2000);  // Wait for 1000 millisecond(s)
 */

  lcd.clear();
  lcd.print("Games  Espindola");
  lcd.setCursor(0, 1);  // posiciono el cursor en la columna y fila determinada
  lcd.print("  6to D - 2024  ");
  delay(2000);
  lcd.clear();
}

int PulsadorPresionado() {
  int keyPress = analogRead(0);

  if (keyPress < 65) {
    return 0;
  } else if (keyPress < 221) {
    return 1;
  } else if (keyPress < 395) {
    return 2;
  } else if (keyPress < 602) {
    return 3;
  } else if (keyPress < 873) {
    return 4;
  } else {
    return 5;
  }
}
