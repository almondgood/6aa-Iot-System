#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEBUG
//#define DEBUG_WIFI

#define AP_SSID "embC"
#define AP_PASS "embC1234"
#define SERVER_NAME "10.10.14.66"
#define SERVER_PORT 5000
#define LOGID "18"
#define PASSWD "PASSWD"

#define CDS_PIN A0
#define GAS_PIN A1
#define BUTTON_PIN 2
#define LED_LAMP_PIN 3
#define SERVO_PIN 5
#define WATER_LEVEL_PIN A0
#define WATER_TEMP_PIN 5
#define WIFIRX 6  //6:RX-->ESP8266 TX
#define WIFITX 7  //7:TX -->ESP8266 RX
#define LED_BUILTIN_PIN 13

#define CMD_SIZE 50
#define ARR_CNT 5
bool timerIsrFlag = false;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean ledOn = false;
boolean cdsFlag = false;

char sendId[10] = "22";
char sendBuf[CMD_SIZE];
char lcdLine1[17] = "Smart IoT By KSH";
char lcdLine2[17] = "WiFi Connecting!";

int cds = 0;
int gas = 0;
unsigned int secCount;
unsigned int myservoTime = 0;

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
} DATETIME;
DATETIME dateTime = {0, 0, 0, 12, 0, 0};
SoftwareSerial wifiSerial(WIFIRX, WIFITX);
WiFiEspClient client;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
OneWire oneWire(WATER_TEMP_PIN);
DallasTemperature sensors(&oneWire);

void setup() {
  lcd.init();
  lcd.backlight();
  lcdDisplay(0, 0, lcdLine1);
  lcdDisplay(0, 1, lcdLine2);

  pinMode(CDS_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_LAMP_PIN, OUTPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);

#ifdef DEBUG
  Serial.begin(115200);
#endif
  wifi_Setup();

  MsTimer2::set(1000, timerIsr);
  MsTimer2::start();

  sensors.begin();

  myservo.attach(SERVO_PIN);
  myservo.write(0);
  myservoTime = secCount;
}

void loop() {
  if (client.available()) {
    socketEvent();
  }

  if (timerIsrFlag) {
    timerIsrFlag = false;
    if (!(secCount % 5)) {

      cds = map(analogRead(CDS_PIN), 0, 1023, 0, 100);
      if ((cds >= 50) && cdsFlag)
      {
        cdsFlag = false;
        sprintf(sendBuf, "[%s]CDS@%d\n", sendId, cds);
        client.write(sendBuf, strlen(sendBuf));
        client.flush();
      } else if ((cds < 50) && !cdsFlag)
      {
        cdsFlag = true;
        sprintf(sendBuf, "[%s]CDS@%d\n", sendId, cds);
        client.write(sendBuf, strlen(sendBuf));
        client.flush();
      }

      gas = analogRead(GAS_PIN);
#ifdef DEBUG
      Serial.print("Gas: ");
      Serial.println(gas);
#endif
      sprintf(sendBuf, "[%s]GAS@%d\n", sendId, gas);
      client.write(sendBuf, strlen(sendBuf));
      client.flush();


      sensors.requestTemperatures();
      int waterLevel = map(analogRead(WATER_LEVEL_PIN), 0, 680, 0, 100);
      //waterLevel = analogRead(WATER_LEVEL_PIN);
      float waterTemp = sensors.getTempCByIndex(0);
          if (waterTemp != DEVICE_DISCONNECTED_C) {
        int waterTempInt = (int)(waterTemp * 100); // 소수점 이하 두 자리까지 표시하기 위해 100을 곱함
        sprintf(sendBuf, "[%s]WATER@%d.%02d@%d\n", sendId, waterTempInt / 100, waterTempInt % 100, waterLevel);
      } else {
        sprintf(sendBuf, "[%s]WATER@UNKNOWN\n", sendId);
      }
      client.write(sendBuf, strlen(sendBuf));
      client.flush();

      if (!client.connected()) {
        lcdDisplay(0, 1, "Server Down");
        server_Connect();
      }
    }
    if (myservo.attached() && myservoTime + 2 == secCount)
    {
      myservo.detach();
    }
    sprintf(lcdLine1, "%02d.%02d  %02d:%02d:%02d", dateTime.month, dateTime.day, dateTime.hour, dateTime.min, dateTime.sec );
    lcdDisplay(0, 0, lcdLine1);
  }

  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH)
  {
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN_PIN, ledOn);
    sprintf(sendBuf, "[  ", ledOn ? "ON" : "OFF");
    client.write(sendBuf, strlen(sendBuf));
    client.flush();
  }
  lastButton = currentButton;
}

void socketEvent()
{
  // socket event handling
}

void timerIsr()
{
  timerIsrFlag = true;
  secCount++;
  clock_calc(&dateTime);
}

void clock_calc(DATETIME *dateTime)
{
  int ret = 0;
  dateTime->sec++;
  if(dateTime->sec >= 60) 
  { 
      dateTime->sec = 0;
      dateTime->min++; 
      if(dateTime->min >= 60) 
      { 
          dateTime->min = 0;
          dateTime->hour++; 
          if(dateTime->hour == 24) 
          {
            dateTime->hour = 0;
          }
       }
    }
}

void wifi_Setup() {
  wifiSerial.begin(38400);
  wifi_Init();
  server_Connect();
}

void wifi_Init()
{
  do {
    WiFi.init(&wifiSerial);
    if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG_WIFI
      Serial.println("WiFi shield not present");
#endif
    }
    else
      break;
  } while (1);

#ifdef DEBUG_WIFI
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(AP_SSID);
#endif
  while (WiFi.begin(AP_SSID, AP_PASS) != WL_CONNECTED) {
#ifdef DEBUG_WIFI
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP_SSID);
#endif
  }
  sprintf(lcdLine1, "ID:%s", LOGID);
  lcdDisplay(0, 0, lcdLine1);
  sprintf(lcdLine2, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  lcdDisplay(0, 1, lcdLine2);

#ifdef DEBUG_WIFI
  Serial.println("You're connected to the network");
  printWifiStatus();
#endif
}

int server_Connect()
{
#ifdef DEBUG_WIFI
  Serial.println("Starting connection to server...");
#endif

  if (client.connect(SERVER_NAME, SERVER_PORT)) {
#ifdef DEBUG_WIFI
    Serial.println("Connect to server");
#endif
    client.print("["LOGID":"PASSWD"]");
  }
  else
  {
#ifdef DEBUG_WIFI
    Serial.println("server connection failure");
#endif
  }
}

void printWifiStatus()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void lcdDisplay(int x, int y, char * str)
{
  int len = 16 - strlen(str);
  lcd.setCursor(x, y);
  lcd.print(str);
  for (int i = len; i > 0; i--)
    lcd.write(' ');
}

boolean debounce(boolean last)
{
  boolean current = digitalRead(BUTTON_PIN);
  if (last != current)
  {
    delay(5);
    current = digitalRead(BUTTON_PIN);
  }
  return current;
}
