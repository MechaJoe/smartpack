#include <ESP8266WiFi.h>;
#include <DHT.h>;

/* DHT SENSOR SETUP */
#define DHTTYPE DHT11
#define DHTPIN  2

DHT dht(DHTPIN, DHTTYPE, 11);
float humidity, temp;  // Values read from

const char* ssid = "AirPennNet-Device";
const char* password = "penn1740wifi";

//define the parameters for the IFTTT
//const char* HOSTIFTTT = "maker.ifttt.com/trigger/JOEESP_DHT11/with/key/bABfRwfA93OSAGm3kEXGsV";
const char* HOSTIFTTT = "maker.ifttt.com";
const char* EVENTO = "JOEESP_DHT11";
const char* IFTTTKEY = "bABfRwfA93OSAGm3kEXGsV";

String url = "/trigger/JOEESP_DHT11/with/key/bABfRwfA93OSAGm3kEXGsV";

void setup() {

  Serial.begin(115200);
  delay(10);
  dht.begin();
  delay(2000);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Waiting to connect to WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  humidity = dht.readHumidity();       // Read Humidity (percent)
  temp = dht.readTemperature();         // Read Temperature (Degree Celsius)
  Serial.print("Initial Temp: ");
  Serial.println(temp);
  Serial.print("Initial Humi: ");
  Serial.println(humidity);

  WiFiClient client;

  if (client.connected())
  {
    client.stop();
  }

  client.flush();
  if (client.connect(HOSTIFTTT,443)) {
    Serial.println("Connected");
    // build the HTTP request
    String toSend = "GET /trigger/";
    toSend += EVENTO;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += temp;
    toSend += "&value2=";
    toSend += humidity;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += HOSTIFTTT;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
    client.print(toSend);
    Serial.println(toSend);
  }
  client.flush();
  client.stop();  
  delay(500);
}

void loop() {
}
