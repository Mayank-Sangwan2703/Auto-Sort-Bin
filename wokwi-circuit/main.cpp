#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "Wokwi-GUEST";
const char *password = "";
const char *server = "https://api.thingspeak.com/channels/2695927/fields**********";
const char *serverName = "http://api.thingspeak.com/update";
String apiKey = "CA43L8URVM5*****";
String writeapiKey = "J807GJT1YDO*****";
int channelID = 2695***;

int prev = 10098;
const int numBins = 6;

Servo servos[numBins];
const int servoPins[numBins] = {19, 22, 21, 18, 5, 4};

const int trigPins[numBins] = {23, 14, 27, 26, 32, 15};
const int echoPins[numBins] = {13, 12, 33, 25, 35, 34};

int bin_status_full[numBins] = {0, 0, 0, 0, 0, 0};

const int binFullThreshold = 10;
LiquidCrystal_I2C lcd(0x27, 16, 2);
void openBin(int binIndex);
bool isBinFull(int binIndex);
int getThingSpeakData();
void giveThingspeakBinno(int binNo);
void checkEveryBinForFull();

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  getThingSpeakData();

  Wire.begin(16, 17);

  while (!Serial)
    ;
  Serial.println("I2C Scanner");

  for (byte address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);

    if (Wire.endTransmission() == 0)
    {
      Serial.print("Found I2C device at address 0x");
      Serial.println(address, HEX);
    }
  }
  Serial.println("Scan completed");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Bin Sorting Sys");

  delay(2000);
  lcd.clear();

  for (int i = 0; i < numBins; i++)
  {
    servos[i].attach(servoPins[i], 500, 2400);
    servos[i].write(0);
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  lcd.setCursor(0, 0);
  lcd.print("Enter 1 to start");
  lcd.setCursor(0, 1);
  lcd.print("a bin.");
  Serial.println("Enter any no to start : ");
}

void loop()
{
  if (Serial.available())
  {
    checkEveryBinForFull();
    int binNumber = getThingSpeakData();
    if (binNumber != prev)
    {
      prev = binNumber;
      binNumber = prev / 100;

      if (binNumber >= 1 && binNumber <= numBins)
      {
        int binIndex = binNumber - 1;

        if (isBinFull(binIndex))
        {
          Serial.print("Bin ");
          Serial.print(binNumber);
          Serial.println(" is full. Cannot open.");

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Bin ");
          lcd.print(binNumber);
          lcd.print(" full!");

          if (binIndex == 0)
          {
            lcd.setCursor(0, 1);
            lcd.print("Of e waste");
          }
          else if (binIndex == 1)
          {
            lcd.setCursor(0, 1);
            lcd.print("Of glass waste");
          }
          else if (binIndex == 2)
          {
            lcd.setCursor(0, 1);
            lcd.print("Of metal waste");
          }
          else if (binIndex == 3)
          {
            lcd.setCursor(0, 1);
            lcd.print("Of organic waste");
          }
          else if (binIndex == 4)
          {
            lcd.setCursor(0, 1);
            lcd.print("Of paper waste");
          }
          else if (binIndex == 5)
          {
            lcd.setCursor(0, 1);
            lcd.print("Of plastic waste");
          }

          delay(2000);
          lcd.clear();
        }
        else
        {
          openBin(binIndex);
        }
      }
    }
  }

  delay(500);
}

void checkEveryBinForFull()
{
  for (int i = 0; i < numBins; i++)
  {
    int bin_report = (int)(isBinFull(i));
    if (bin_report == 0)
    {
      bin_status_full[i] = 0;
    }
    else if (bin_report == 1 && bin_status_full[i] == 1)
    {
      bin_status_full[i] = 1;
    }
    else if (bin_report == 1 && bin_status_full[i] == 0)
    {
      giveThingspeakBinno(i + 1);
      bin_status_full[i] = 1;
    }
  }
}

void openBin(int binIndex)
{
  Serial.print("Opening bin ");
  Serial.println(binIndex + 1);
  lcd.clear();
  if (binIndex == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print("Of e waste");
  }
  else if (binIndex == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Of glass waste");
  }
  else if (binIndex == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print("Of metal waste");
  }
  else if (binIndex == 3)
  {
    lcd.setCursor(0, 1);
    lcd.print("Of organic waste");
  }
  else if (binIndex == 4)
  {
    lcd.setCursor(0, 1);
    lcd.print("Of paper waste");
  }
  else if (binIndex == 5)
  {
    lcd.setCursor(0, 1);
    lcd.print("Of plastic waste");
  }
  lcd.setCursor(0, 0);
  lcd.print("Opening Bin ");
  lcd.print(binIndex + 1);
  servos[binIndex].write(90);
  servos[binIndex].write(90);
  delay(5000);

  servos[binIndex].write(0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Closing Bin ");
  lcd.print(binIndex + 1);
  delay(1000);

  lcd.clear();
}
bool isBinFull(int binIndex)
{
  long duration, distance;
  digitalWrite(trigPins[binIndex], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPins[binIndex], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[binIndex], LOW);
  duration = pulseIn(echoPins[binIndex], HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= binFullThreshold)
  {
    Serial.print("Bin ");
    Serial.print(binIndex + 1);
    Serial.println(" is full!");

    return true;
  }
  else
  {
    Serial.print("Bin ");
    Serial.print(binIndex + 1);
    Serial.print(" distance: ");
    Serial.print(distance);
    Serial.println(" cm (not full)");
    return false;
  }
}

int getThingSpeakData()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String url = String(server) + "/channels/" + String(channelID) + "/fields/1.json?api_key=" + apiKey + "&results=2";

    http.begin(url);
    int httpCode = http.GET();

    const char *field1_value1;
    const char *field1_value2;

    if (httpCode > 0)
    {
      String payload = http.getString();

      Serial.println("Data received from ThingSpeak:");
      Serial.println(payload);

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error)
      {
        field1_value1 = doc["feeds"][0]["field1"];
        field1_value2 = doc["feeds"][1]["field1"];

        Serial.print("Latest Field 1 Value: ");
        Serial.println(field1_value1);

        Serial.print("Previous Field 1 Value: ");
        Serial.println(field1_value2);
      }
      else
      {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.c_str());
      }
    }
    else
    {
      Serial.println("Error in HTTP request");
    }
    http.end();
    int intValue1 = atoi(field1_value1);
    int intValue2 = atoi(field1_value2);
    return intValue2;
  }
  return -1;
}

void giveThingspeakBinno(int binNo)
{
  while (1)
  {
    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;

      HTTPClient http;

      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");

      String httpRequestData = "{\"api_key\":\"" + writeapiKey + "\",\"field2\":\"" + String(binNo) + "\"}";
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("HTTP Response code: ");

      Serial.println(httpResponseCode);
      http.end();
      if (httpResponseCode == 200)
      {
        break;
      }
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
  }
}
