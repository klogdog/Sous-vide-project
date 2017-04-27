#include <DallasTemperature.h>
#include <OneWire.h>
// this one wire is the no resistor one wire library
// Temp sensor  pinout red/black are +/- power, and green is data.
#define tempSensor 5
float currentTemp = 0;
#define heater 10

#define tempUpButton 12
#define tempDownButton 11
float tempSetting = 25;
float tempSettingHigh = 26;
float tempSettingLow = 24;
int loopLock = 0;
int addressChecked = 0;
float tempReadTimer = 0;
int tempDeviceAddressFloat = 0;
int updateScreen = 0;
float tempAdjustTimer = 0;
OneWire oneWire(tempSensor);
DallasTemperature sensors(&oneWire);

//LCD Dependencies 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#define TFT_CS     2
#define TFT_RST    3  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     4

// Option 2: use any pins but a little slower!
#define TFT_SCLK 7   // set these to be whatever pins you like! (SCL on sainsmart)
#define TFT_MOSI 6   // set these to be whatever pins you like! (SDA on sainsmart)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  tft.initR(INITR_BLACKTAB); 
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
  pinMode(tempUpButton,INPUT);
  pinMode(tempDownButton,INPUT);
  pinMode(heater,OUTPUT);
  digitalWrite(heater,LOW);
}

void loop() {
  DeviceAddress tempDeviceAddress;  
  buttonChecker();
  delay(100);
  if (millis() >= tempReadTimer){
    tempSettingHigh = tempSetting + 1;
    tempSettingLow = tempSetting - 1;
    sensors.getAddress(tempDeviceAddress, 0);  
    sensors.requestTemperatures(); 
    currentTemp = sensors.getTempC(tempDeviceAddress);
    tempReadTimer = millis() + 5000;
    updateScreen = 1;
  }
  if (updateScreen == 1){
    tft.setCursor(0,0);
    tft.fillScreen(ST7735_BLACK);
    tft.println("actual Temp"); 
    tft.println(" ");   
    tft.print(currentTemp, 1); 
    tft.println(" ");  
    tft.println("set Temp"); 
    tft.println(" ");   
    tft.print(tempSetting, 1); 
    updateScreen = 0;
  }
  if (currentTemp == -127){
    currentTemp = tempSetting;
  }
  if (millis() >= tempAdjustTimer){
    if (currentTemp >= tempSettingHigh){
      digitalWrite(heater,HIGH);
      tempAdjustTimer = millis() + 5000;
     }
    if (currentTemp <= tempSettingLow){
      digitalWrite(heater,LOW);
      tempAdjustTimer = millis() + 5000;
     }
  }
}

void buttonChecker(){
  int tempUpCheck = digitalRead(tempUpButton);
  if (tempUpCheck == HIGH){
    tempSetting = tempSetting + 1;
  }
  int tempDownCheck = digitalRead(tempDownButton);
  if (tempDownCheck == HIGH){
    tempSetting = tempSetting - 1;
  }
}

