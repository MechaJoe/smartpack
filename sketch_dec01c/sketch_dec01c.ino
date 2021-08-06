#include <ArduinoTimer.h>;
#include <DHT.h>;
#include <ESP8266HTTPClient.h>;

#ifdef ARDUINO_ARCH_SAMD
#elif defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#else
#error Wrong platform
#endif

#include <WifiLocation.h>

const char* googleApiKey = "AIzaSyCAMQbUSvxpjGnDrJFbQCZ4B8zRhOxaBRU";
WifiLocation location(googleApiKey);

/* DHT SENSOR SETUP */
#define DHTTYPE DHT11
#define DHTPIN  2

DHT dht(DHTPIN, DHTTYPE, 11);
float humidity1, temp, humidity2;  // Values read from

const char* ssid = "AirPennNet-Device";
const char* password = "penn1740wifi";

//define the parameters for the IFTTT
//const char* HOSTIFTTT = "maker.ifttt.com/trigger/JOEESP_DHT11/with/key/bABfRwfA93OSAGm3kEXGsV";
String HOSTIFTTT = "maker.ifttt.com";
String EVENTO = "JOEESP_DHT11";
String IFTTTKEY = "bABfRwfA93OSAGm3kEXGsV";

String url = "/trigger/JOEESP_DHT11/with/key/bABfRwfA93OSAGm3kEXGsV";
ArduinoTimer GPSTimer;

void setup() {
  Serial.begin(115200);
#ifdef ARDUINO_ARCH_ESP32
  WiFi.mode(WIFI_MODE_STA);
#endif
#ifdef ARDUINO_ARCH_ESP8266
  WiFi.mode(WIFI_STA);
#endif
  delay(10);
  dht.begin();
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
}

void loop() {
  humidity1 = dht.readHumidity();       // Read Humidity (percent)
  temp = dht.readTemperature();         // Read Temperature (Degree Celsius)
  delay(5000);
  humidity2 = dht.readHumidity();
  float humidityChange = humidity2 - humidity1;

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humi: ");
  Serial.println(humidity2);

  if (WiFi.status() == WL_CONNECTED && humidityChange > 20) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    String toSend = "http://" + HOSTIFTTT + "/trigger/JOEESP_DHT11/with/key/bABfRwfA93OSAGm3kEXGsV" + "?value1=" + temp + "&value2=" + humidity2;
    http.begin(toSend);  //Specify request destination
    int httpCode = http.GET();     //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
  } 
  
  if (GPSTimer.TimePassed_Hours(1)) {
    location_t loc = location.getGeoFromWiFi();
//    Serial.println("Location request data");
//    Serial.println(location.getSurroundingWiFiJson());
    String latitude = String(loc.lat, 7);
    Serial.println("Latitude: " + latitude);
    String longitude = String(loc.lon, 7);
    Serial.println("Longitude: " + longitude);
    String accuracy = String(loc.accuracy);
    Serial.println("Accuracy: " + accuracy + " meters");

    HTTPClient http;  //Declare an object of class HTTPClient
    String toSend = "http://" + HOSTIFTTT + "/trigger/JOEWIFI_GPS/with/key/bABfRwfA93OSAGm3kEXGsV" + "?value1=" + latitude + "&value2=" + longitude + "&value3=" + accuracy;
    http.begin(toSend);  //Specify request destination
    int httpCode = http.GET();     //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection 
  }
}
