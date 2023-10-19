/* This is a simple demo of sending and receiving some data.
   Be sure to check out other examples!
   Fill-in information from Blynk Device Info here */

#define BLYNK_TEMPLATE_ID "TMPL64kx7BMnh"
#define BLYNK_TEMPLATE_NAME "relay"
#define BLYNK_AUTH_TOKEN "M2laaUXwKszwPezQeVTfHx8VyNrFnRGk"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

//#define OLED_Address 0x3C
//Adafruit_SSD1306 oled(128, 64, &Wire, OLED_Address);

#include "DHT.h"
#define DHTPIN 17
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ELINS";
char pass[] = "@olije.ac.id";

BlynkTimer timer;
bool modeAuto, relay;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);
  Serial.println(value);
  if (value == 0 && modeAuto == 0) {
    digitalWrite(18, LOW); // Turn off the relay (assuming LOW is off)
    relay = 0;
  } else if (value == 1 && modeAuto == 0) {
    digitalWrite(18, HIGH); // Turn on the relay (assuming HIGH is on)
    relay = 1;
  }
}

BLYNK_WRITE(V6)
{
  int value = param.asInt();
  Blynk.virtualWrite(V7, value);
  Serial.println(value);
  if (value == 0) {
    modeAuto = 0;
  } else {
    modeAuto = 1;
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  bacaDHT();
}

void bacaDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println(h, 2);
  Serial.println(t, 2);

  Blynk.virtualWrite(V4, h);
  Blynk.virtualWrite(V5, t);

 /* oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.print("Suhu: "); oled.println(t);
  oled.print("Hum: "); oled.println(h);
  oled.print("Mode: "); oled.println(modeAuto);
  oled.print("Relay: "); oled.println(relay);

  oled.display();*/
}

void setup() {
  // Debug console
  Serial.begin(115200);

  dht.begin();
  Wire.begin();

  /*oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
  oled.clearDisplay();

  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.println("Persiapan...");
  oled.display(); */

  pinMode(18, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // Setup a function to be called every second
  timer.setInterval(2000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk.
  // Remember to avoid delay() function
}
