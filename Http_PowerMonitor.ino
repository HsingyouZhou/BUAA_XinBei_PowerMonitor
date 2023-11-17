#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/  23, /* cs=*/ 10, /* dc=*/ 25, /* reset=*/ 26);

const char* ssid = "IoT_AP";              //WIFI SSID
const char* password =  "goodlife";      //WIFI password
const char* stucode = "22371558";       //Enter your student ID
const char *type = "1";

String Poster = "{\"stucode\":\"";
String PwrDisp = "";
StaticJsonDocument<200> jsonBuffer;

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  
  u8g2.drawUTF8(0,45,"远程电量监视系统");
  u8g2.sendBuffer();
  delay(3000);
  u8g2.clearBuffer();
  u8g2.drawUTF8(0,45,"X15-504");
  u8g2.sendBuffer();
  delay(3000);

  u8g2.clearBuffer();
  Serial.begin(115200);
  u8g2.drawUTF8(0,45,"准备建立连接");
  u8g2.sendBuffer();
  delay(4000);   //Delay needed before calling the WiFi.begin
  WiFi.begin(ssid, password);

  int dot_x = 0;
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(50);
    //Serial.println("Connecting to WiFi..");
    u8g2.drawUTF8(0,45,"建立远程连接");
    u8g2.drawStr(dot_x,64,".");
    u8g2.sendBuffer();
    dot_x += 10;
    if(dot_x >= 124){
      dot_x = 0;
      u8g2.clearBuffer();
    }
  }
  u8g2.clearBuffer();
  u8g2.drawUTF8(0,45,"远程连接就绪");
  u8g2.sendBuffer();
  Serial.println("Connected to the WiFi network");
  delay(1000);
  
  Poster += stucode;
  Poster += "\",\"type\":\"";
  Poster += type;
  Poster += "\"}";
}

void loop()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_chargen_92_tf);
  HTTPClient http;
  http.begin("http://weixin.lrgj.net.cn/ics/rest/wxdev/getlvalue");
  http.addHeader("Content-Type", "application/json;charset=UTF-8");
  http.addHeader("Content-Length", "31");
  http.addHeader("Connection", "keep-alive");
  http.addHeader("Host", "weixin.lrgj.net.cn");
  
  int httpResponseCode = http.POST(Poster);
  if(httpResponseCode>0){
    String response = http.getString();  //Get the response to the request
    char *des = const_cast<char*>(response.c_str());
    DeserializationError error = deserializeJson(jsonBuffer, des);
    if(error)
    {
      Serial.print("deserializeJson Err");
      Serial.println(error.f_str());
      return;
    }
    String res = jsonBuffer["data"]["provalue"];     //deserialize Json
    Serial.println(httpResponseCode);   //Print return code
    if(res.length() >= 3)               //Sometimes it may get empty "provalue" data
    {
      Serial.println(res);           //Print request answer
      PwrDisp += res;
      //u8g2.drawStr(0,45,PwrDisp.c_str());
    }
    else
    {
      Serial.println("NULL");   
      PwrDisp += "NULL";
      //u8g2.drawStr(0,45,PwrDisp.c_str());
    }
  }
  else{
    PwrDisp += "POST Error";
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  u8g2.drawButtonUTF8(62, 45, U8G2_BTN_SHADOW1|U8G2_BTN_HCENTER|U8G2_BTN_BW2, 0,  2,  2, PwrDisp.c_str());
  u8g2.sendBuffer();
  PwrDisp = "";
  delay(10000);
}
