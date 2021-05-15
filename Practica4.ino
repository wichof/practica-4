//Práctica #4

//Configuración de librerías.
#include <WiFi.h>
#include <DHT.h>
#include <Servo.h>

//Configuramos el sevomotor
Servo ObjServo; // Hacemos el objeto del servomotor desde la librería

//Sensor DHT
#define DHTPIN 2
#define DHTTYPE DHT11

//Fotoresistencia
#define LDR 34

//Configuración del servidor inicial.
//Replace with your network credentials
//const char* ssid = "name";
//const char* password = "password";
const char* ssid = "Nostromo";
const char* password = "Alex19817";
// Set web server port number to 80
WiFiServer server(80); //Programaramos el ESP como un servidor.
//Inicializamos el sensor DHT
DHT dht(DHTPIN, DHTTYPE);// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
//Lectura de Sensores
String SensoresEstado = "off";

//Definimos nuestras variables y constantes del sistema difuso
const int rules = 125; //Definimos nuestro núnero de reglas
const int inputs = 3; //Nuestras entradas del sistema. NTI=NTV-1
double yf = 0.0;
double X[3];


//Definimos nuestra matriz de reglas (DataBase).
int DB[125][4] =
{ {1, 1, 1, 1},
  {1, 1, 2, 1},
  {1, 1, 3, 1},
  {1, 1, 4, 1},
  {1, 1, 5, 1},
  {1, 2, 1, 1},
  {1, 2, 2, 1},
  {1, 2, 3, 1},
  {1, 2, 4, 1},
  {1, 2, 5, 1},
  {1, 3, 1, 1},
  {1, 3, 2, 1},
  {1, 3, 3, 2},
  {1, 3, 4, 2},
  {1, 3, 5, 2},
  {1, 4, 1, 1},
  {1, 4, 2, 1},
  {1, 4, 3, 1},
  {1, 4, 4, 1},
  {1, 4, 5, 1},
  {1, 5, 1, 1},
  {1, 5, 2, 1},
  {1, 5, 3, 1},
  {1, 5, 4, 1},
  {1, 5, 5, 1},
  {2, 1, 1, 1},
  {2, 1, 2, 1},
  {2, 1, 3, 2},
  {2, 1, 4, 2},
  {2, 1, 5, 2},
  {2, 2, 1, 1},
  {2, 2, 2, 2},
  {2, 2, 3, 2},
  {2, 2, 4, 2},
  {2, 2, 5, 2},
  {2, 3, 1, 1},
  {2, 3, 2, 1},
  {2, 3, 3, 2},
  {2, 3, 4, 2},
  {2, 3, 5, 2},
  {2, 4, 1, 2},
  {2, 4, 2, 1},
  {2, 4, 3, 1},
  {2, 4, 4, 2},
  {2, 4, 5, 2},
  {2, 5, 1, 1},
  {2, 5, 2, 1},
  {2, 5, 3, 1},
  {2, 5, 4, 2},
  {2, 5, 5, 2},
  {3, 1, 1, 3},
  {3, 1, 2, 3},
  {3, 1, 3, 3},
  {3, 1, 4, 4},
  {3, 1, 5, 4},
  {3, 2, 1, 3},
  {3, 2, 2, 3},
  {3, 2, 3, 4},
  {3, 2, 4, 4},
  {3, 2, 5, 4},
  {3, 3, 1, 3},
  {3, 3, 2, 4},
  {3, 3, 3, 5},
  {3, 3, 4, 5},
  {3, 3, 5, 5},
  {3, 4, 1, 3},
  {3, 4, 2, 3},
  {3, 4, 3, 4},
  {3, 4, 4, 4},
  {3, 4, 5, 4},
  {3, 5, 1, 3},
  {3, 5, 2, 3},
  {3, 5, 3, 4},
  {3, 5, 4, 4},
  {3, 5, 5, 4},
  {4, 1, 1, 3},
  {4, 1, 2, 3},
  {4, 1, 3, 4},
  {4, 1, 4, 4},
  {4, 1, 5, 4},
  {4, 2, 1, 2},
  {4, 2, 2, 2},
  {4, 2, 3, 3},
  {4, 2, 4, 3},
  {4, 2, 5, 3},
  {4, 3, 1, 2},
  {4, 3, 2, 2},
  {4, 3, 3, 3},
  {4, 3, 4, 3},
  {4, 3, 5, 3},
  {4, 4, 1, 1},
  {4, 4, 2, 1},
  {4, 4, 3, 2},
  {4, 4, 4, 2},
  {4, 4, 5, 2},
  {4, 5, 1, 1},
  {4, 5, 2, 1},
  {4, 5, 3, 1},
  {4, 5, 4, 1},
  {4, 5, 5, 1},
  {5, 1, 1, 1},
  {5, 1, 2, 1},
  {5, 1, 3, 1},
  {5, 1, 4, 1},
  {5, 1, 5, 2},
  {5, 2, 1, 1},
  {5, 2, 2, 1},
  {5, 2, 3, 1},
  {5, 2, 4, 1},
  {5, 2, 5, 1},
  {5, 3, 1, 1},
  {5, 3, 2, 1},
  {5, 3, 3, 1},
  {5, 3, 4, 1},
  {5, 3, 5, 1},
  {5, 4, 1, 1},
  {5, 4, 2, 1},
  {5, 4, 3, 1},
  {5, 4, 4, 1},
  {5, 4, 5, 1},
  {5, 5, 1, 1},
  {5, 5, 2, 1},
  {5, 5, 3, 1},
  {5, 5, 4, 1},
  {5, 5, 5, 1}
};

//Asignación de los actuadores.
const int LEDV1 = 18;
const int LEDV2 = 19;
const int LEDA = 21;
const int LEDR1 = 22;
const int LEDR2 = 23;
const int BUZ = 0;
const int servo = 4;
//Asignación de los elementos a sensar
float Temperatura;
float Humedad;
float nivel;
float normTemp;
float normHum;
float normniv;
float y;
int LDR_val;

//Funciones de la lógica difusa
//Definimos nuestros conjuntos difusos en funciones.
double trapezoidmf(double x, double a, double b, double c, double d) {
  double mf = max(min(min((x - a) / (b - a + 0.000001), 1.0), (d - x) / (d - c + 0.0000001)), 0.0);
  return mf;
}
double trianglemf(double x, double a, double b, double c) {
  double mf = max(min((x - a) / (b - a + 0.000001), (c - x) / (c - b + 0.000001)), 0.0);
  return mf;
}
//Definimos los valores linguinsticos.
double Type1FS(double x, int n, double V[]) {
  double a = V[0];
  double b = V[1];
  double c = V[2];
  double mf;
  if (n == 1) {
    mf = trapezoidmf(x, a - 1.0001, a, b, c); //Cuando n=1, el valor es muy bajoy se activa la función trapezoidal
    return mf;
  }
  if (n == 2) {
    mf = trianglemf(x, a, b, c); //Cuando n=2 el valor es triangular
    return mf;
  }
  if (n == 3) {
    mf = trianglemf(x, a, b, c); //Cuando n=3 el valor es triangular
    return mf;
  }
  if (n == 4) {
    mf = trianglemf(x, a, b, c); //Cuando n=4 el valor es triangular
    return mf;
  }
  if (n == 5) {
    mf = trapezoidmf(x, a, b, c, c + 1); ////Cuando n=5 el valor es trapezoidal
  }
  if (n == 0) {
    mf = 1;
  }
}

//Creamos el setup del programa, en el que hace la conexión Wi-Fi
void setup() {
  Serial.begin(115200);
  //Comenzamos el sensor DHT
  dht.begin();

  pinMode(LEDV1, OUTPUT);
  pinMode(LEDV2, OUTPUT);
  pinMode(LEDA, OUTPUT);
  pinMode(LEDR1, OUTPUT);
  pinMode(LEDR2, OUTPUT);
  pinMode(BUZ, OUTPUT);

  // Set outputs to LOW
  digitalWrite(LEDV1, LOW);
  digitalWrite(LEDV2, LOW);
  digitalWrite(LEDA, LOW);
  digitalWrite(LEDR1, LOW);
  digitalWrite(LEDR2, LOW);
  digitalWrite(BUZ, LOW);

  //Configuramos el objeto del servomotor para trabajar en el pin 13
  ObjServo.attach(servo);

  // Connect to Wi-Fi network with SSID and password
  //Estos son los comandos para podernos conectarnos a la red.
  Serial.print("Connecting to ");
  Serial.println(ssid); //Indicamos a donde nos vamos a conectar
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Mientas no estés conectado...
    delay(500);
    Serial.print("."); //Despliega un puntito.
  }
  // Print local IP address and start web server
  // Cuando se logra conectar aparece lo siguiente:
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

//Inicio del programa en ciclo.
void loop() {
  WiFiClient client = server.available(); // Listen for incomingclients
  if (client) { // If a new client connects,
    Serial.println("New Client."); // print a message out in the
    //serial port
    String currentLine = ""; // make a String to hold
    //incoming data from the client
    while (client.connected()) { // loop while the client's connected
      if (client.available()) { // if there's bytes to read from the client,
      char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        header += c;
        if (c == '\n') { // if the byte is a newline character
          // if the current line is blank, you got two newline charactersin a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) { //El cliente no quizo nada
            // HTTP headers always start with a response code (e.g.HTTP / 1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            //¡¡¡¡Vamos a leer los sensores!!!!
            //Lectura de los sensores
            if (header.indexOf("GET /S/on") >= 0) {
              //Sensor de humedad
              Humedad = dht.readHumidity();
              //Sensor de temperatura
              Temperatura = dht.readTemperature();
              //Sensor de nivel de luz
              LDR_val = analogRead(LDR);
              nivel = (LDR_val * 100) / 4095;
              //Normalización de la temperatura
              normTemp = (Temperatura - 0) / (42 - 0);
              //Normalización de la humedad
              normHum = (Humedad - 0) / (100 - 0);
              //Normalización del nivel de luz.
              normniv = (nivel - 0) / (100 - 0);
              //Arreglo de los sensores normalizados.
              double X[3] = {normTemp, normHum, normniv};

              //Codificación de la logica difusa.
              double PARAM[5][3] = {{0, 0.1666, 0.3333},
                {0.1666, 0.3333, 0.5},
                {0.3333, 0.5, 0.6666},
                {0.5, 0.6666, 0.8333},
                {0.6666, 0.8333, 1}
              };
              double V[4];
              double AC[5] = {0, 0.25, 0.5, 0.75, 1};
              double Fo[125];
              for (int r = 0; r <= (rules - 1); r++) {
                double sumin = 1;
                int n;
                for (int i = 0; i <= (inputs - 1); i++) {
                  n = DB[r][i] - 1;
                  if (n > -1) {
                    V[0] = PARAM[n][0];
                    V[1] = PARAM[n][1];
                    V[2] = PARAM[n][2];
                  }
                  double mf = Type1FS(X[i], (n + 1), V);
                  sumin = min(sumin, mf);
                }
                Fo[r] = sumin;
              }
              //Desdifusificación por alturas.
              double sum1 = 0;
              double sum2 = 0.00000001;
              for (int r = 0; r <= (rules); r++) {
                sum1 = (sum1 + (Fo[r] * AC[DB[r][2] - 1]));
                sum2 = (sum2 + Fo[r]);
              }
              double yn = sum1 / sum2;
              //Desnormalización de la salida
              y = (yn * (4)) + 1;
              //Respuesta de los actuadores.
              if (y == 5) {
                digitalWrite(LEDV1, HIGH);
                digitalWrite(LEDV2, HIGH);
                digitalWrite(LEDA, HIGH);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, LOW);
                ObjServo.write(180);
              }
              if (y >= 4 && y < 5) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, HIGH);
                digitalWrite(LEDA, HIGH);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, LOW);
                ObjServo.write(144);
              }
              if (y >= 3 && y < 4) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, LOW);
                digitalWrite(LEDA, HIGH);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, LOW);
                ObjServo.write(108);
              }
              if (y >= 2 && y < 3) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, LOW);
                digitalWrite(LEDA, LOW);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, HIGH); ObjServo.write(70);
              }
              if (y >= 1 && y < 2) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, LOW);
                digitalWrite(LEDA, LOW);
                digitalWrite(LEDR1, LOW);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, HIGH);
                ObjServo.write(30);
              }
              Serial.println(y);
              Serial.println(normTemp);
              Serial.println(normHum);
              Serial.println(normniv);
              SensoresEstado = "on";
            } else if (header.indexOf("GET /S/off") >= 0) {

              //Sensor de humedad
              Humedad = dht.readHumidity();
              //Sensor de temperatura
              Temperatura = dht.readTemperature();
              //Sensor de nivel de luz
              LDR_val = analogRead(LDR);
              nivel = (LDR_val * 100) / 4095;
              //Normalización de la temperatura
              normTemp = (Temperatura - 0) / (42 - 0);
              //Normalización de la humedad
              normHum = (Humedad - 0) / (100 - 0);
              //Normalización del nivel de luz.
              normniv = (nivel - 0) / (100 - 0);
              //Arreglo de los sensores normalizados.
              double X[3] = {normTemp, normHum, normniv};

              //Codificación de la logica difusa.
              double PARAM[5][3] = {{0, 0.1666, 0.3333},
                {0.1666, 0.3333, 0.5},
                {0.3333, 0.5, 0.6666},
                {0.5, 0.6666, 0.8333},
                {0.6666, 0.8333, 1}
              };
              double V[4];
              double AC[5] = {0, 0.25, 0.5, 0.75, 1};
              double Fo[125];
              for (int r = 0; r <= (rules - 1); r++) {
                double sumin = 1;
                int n;
                for (int i = 0; i <= (inputs - 1); i++) {
                  n = DB[r][i] - 1;
                  if (n > -1) {
                    V[0] = PARAM[n][0];
                    V[1] = PARAM[n][1];
                    V[2] = PARAM[n][2];
                  }
                  double mf = Type1FS(X[i], (n + 1), V);
                  sumin = min(sumin, mf);
                }
                Fo[r] = sumin;
              }
              //Desdifusificación por alturas.
              double sum1 = 0;
              double sum2 = 0.00000001;
              for (int r = 0; r <= (rules); r++) {
                sum1 = (sum1 + (Fo[r] * AC[DB[r][2] - 1]));
                sum2 = (sum2 + Fo[r]);
              }
              double yn = sum1 / sum2;
              //Desnormalización de la salida
              y = (yn * (4)) + 1;
              //Respuesta de los actuadores.
              if (y == 5) {
                digitalWrite(LEDV1, HIGH);
                digitalWrite(LEDV2, HIGH);
                digitalWrite(LEDA, HIGH);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, LOW);
                ObjServo.write(180);
              }
              if (y >= 4 && y < 5) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, HIGH);
                digitalWrite(LEDA, HIGH);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, LOW);
                ObjServo.write(144);
              }
              if (y >= 3 && y < 4) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, LOW);
                digitalWrite(LEDA, HIGH);
                digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, LOW);
                ObjServo.write(108);
              }
              if (y >= 2 && y < 3) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, LOW);
                digitalWrite(LEDA, LOW); digitalWrite(LEDR1, HIGH);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, HIGH);
                ObjServo.write(70);
              }
              if (y >= 1 && y < 2) {
                digitalWrite(LEDV1, LOW);
                digitalWrite(LEDV2, LOW);
                digitalWrite(LEDA, LOW);
                digitalWrite(LEDR1, LOW);
                digitalWrite(LEDR2, HIGH);
                digitalWrite(BUZ, HIGH);
                ObjServo.write(30);
              }
              Serial.println(y);
              Serial.println(normTemp);
              Serial.println(normHum);
              Serial.println(normniv);
              SensoresEstado = "off";
            }

            // Display the HTML web page
            //Programa HTML completo.
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica;display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("<style>body { text-align: center; fontfamily: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");

            //Configuración del color para los sensores
            client.println(".button1 { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");

            //Configuración del color de presionado
            client.println(".button2 {background-color:#555555;}</style></head>");


            // Web Page Heading
            //Todo lo que diga client, se lo estará aventando al cliente.
            client.println("<body><h1>Practica #4.</h1>");
            //Despliega el dato de los sensores.
            client.println("<p>Nivel de Humedad: " + String (Humedad) + "%</p>" "<p>Temperatura: " + String (Temperatura) + "Grados C</p>""<p>Nivel de Luz: " + String (nivel) + "%</p>");
            // If the output5State is off, it displays the ON button
            if (SensoresEstado == "off") {
            client.println("<p><a href=\"/S/on\"><button class=\"button1\">Leer</button></a></p>");
            } else {
              client.println("<p><a href=\"/S/off\"><buttonclass=\"button1 button2\">Leer</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') { // if you got anything else but a carriage return character,
                 currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
