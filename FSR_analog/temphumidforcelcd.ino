/*******************************************************

   Description:
   Reads an analog input on pin 1, prints the result to the LCD.
   This program takes the temperture sensor LM35 for example.

   Connection:
   Plug the LCD Keypad to the UNO(or other controllers)
   Temperture sensor:
   S(blue) -- A1()
     Note: A0 has been occupied.
   VCC(red) -- VCC
   GND(black) -- GND

********************************************************/

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel

unsigned long tepTimer ;
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 13

int fsrPin = 1;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
String fsrMessage;

void setup() {
  lcd.begin(16, 2);                       // start the library
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);                   // set the LCD cursor   position
  double tempdata;                           // variable to store the temperature value coming from the conversion formula
  tempdata = DHT.temperature;                   // read the analog in value:
  double humidata = DHT.humidity;


  if (millis() - tepTimer > 500) {       // output a temperature value per 500ms
    tepTimer = millis();

    int chk;
    Serial.print("DHT11, \t");
    chk = DHT.read(DHT11_PIN);    // READ DATA
    switch (chk) {
      case DHTLIB_OK:
        Serial.print("OK,\t");
        break;
      case DHTLIB_ERROR_CHECKSUM:
        Serial.print("Checksum error,\t");
        break;
      case DHTLIB_ERROR_TIMEOUT:
        Serial.print("Time out error,\t");
        break;
      default:
        Serial.print("Unknown error,\t");
        break;
    }

    fsrReading = analogRead(fsrPin);

    if (fsrReading < 10) {
          Serial.println(" - No pressure");
        fsrMessage = "Nothing";
    } 
      else   if (fsrReading < 200) {
            Serial.println(" - Light touch");
         fsrMessage = "Lightest";
      }
      else if (fsrReading < 500) {
          Serial.println(" - Light squeeze");
        fsrMessage = "Light";
      }
      else if (fsrReading < 800) {
          Serial.println(" - Medium squeeze");
        fsrMessage = "Medium";
      }
      else {
        Serial.println(" - Big squeeze");
        fsrMessage = "Heavy";
    }


  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);

  // print the temperature and humidity data to the lcd
  lcd.print("T: ");
  lcd.print(tempdata);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidata);
  lcd.print("%");
  delay(1500);

  // print the force sensor data to the lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Force: ");
  lcd.print(fsrMessage);
  delay(1500);
}
}
