#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <LEDMatrixDriver.hpp>


WiFiClient client;
WiFiServer server(80);
#define button 4
//#define settime 5
int  led2=5;
//int led=12;
int day, month, year, dayOfWeek;
int summerTime = 0;
String date;
int epoch;
int h1=26,s1=30;
#define NUM_MAX 4
int out=0,out1=0;
// for NodeMCU 1.0
#define DIN_PIN 13  // D7
#define CS_PIN  15  // D8
#define CLK_PIN 14  // D5

#include "max7129.h"
#include "fonts.h"
#define HOSTNAME "ESP-Clock"
String texxt= "";
long last;
const uint8_t LEDMATRIX_CS_PIN = 15;

// Define LED Matrix dimensions (0-n) - eg: 32x8 = 31x7
const int LEDMATRIX_WIDTH = 31;  
const int LEDMATRIX_HEIGHT = 7;
const int LEDMATRIX_SEGMENTS = 4;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

//const char* ssid     = "S1030519";     // Thay Ä‘á»•i tÃªn wifi
//const char* password = "05041622";   // Thay Ä‘á»•i Password wifi
const char* ssid     = "Hihi";     // Thay Ä‘á»•i tÃªn wifi
const char* password = "07062002";   // Thay Ä‘á»•i Password wifi

 
byte  font1[][8]= {
    { 0,0,0,0,0,0,0,0 }, //space
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    };
int x=LEDMATRIX_WIDTH, y=0;   // start top left

// Marquee text
char text[75] = " Hello ";
int len = strlen(text);
// Marquee speed
const int ANIM_DELAY = 50;
unsigned long myTime=millis();
unsigned long intro=millis();
IPAddress ip ;

void setup() 
{
  Serial.begin(115200);
 
    lmd.setEnabled(true);  
    lmd.setIntensity(2);   // 0 = low, 10 = high  
    sprintf(text,"Connecting to %s",ssid);  
    len=strlen(text);
//  Serial.print("Connecting WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   // Serial.print(".");
  }
  server.begin();
  ip = WiFi.localIP();
  //Serial.println("");
  //Serial.print("IP: "); Serial.println(WiFi.localIP());
 
   String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
   texxt = ipStr;
  strcpy(text, ipStr.c_str());
 
  len=strlen(text);
  
pinMode (button,INPUT);
//pinMode (settime,INPUT);
//pinMode (led,OUTPUT);
pinMode (led2,OUTPUT);
digitalWrite(led2,1);
//attachInterrupt(button, ISR, FALLING);
}


// =============================DEFINE VARS==============================
#define MAX_DIGITS 20
int mode1=0;
int Mode=1;
byte dig[MAX_DIGITS]={0};
byte digold[MAX_DIGITS]={0};
byte digtrans[MAX_DIGITS]={0};
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
int dx=0;
int dy=0;
byte del=0;
int h,m,s;
float utcOffset = +7;
long localEpoc = 0;
long localMillisAtUpdate = 0;
int ktra=1,ktra1;
// =======================================================================

void loop()

{
 
  if(digitalRead(button)==0)
  {
    delay(50);
    if(digitalRead(button)==0)
    {
     // Serial.println("DA NHAN MODE");
      while(digitalRead(button)==0);
      
      if(Mode==0)
      {
        
  strcpy(text, texxt.c_str());
  len=strlen(text);
        Mode=1;
        
        }
        else
        {
          
          Mode=0;
          }
      
     }
   }
   
//  // set Ä‘á»™ sÃ¡ng 
//// 12am to 6am, lowest intensity 0
if ( (h == 0) || ((h >= 1) && (h <= 6)) ) sendCmdAll(CMD_INTENSITY, 0);
//// 6pm to 11pm, intensity = 2
else if ( (h >=18) && (h <= 23) ) sendCmdAll(CMD_INTENSITY, 1);
//// max brightness during bright daylight
  else sendCmdAll(CMD_INTENSITY, 2); 
  if ( Mode==0)
  {
    
    if(ktra==0)
    {
      ok();
      ktra=1;
    }
    if(out==1)
    {
      text[4]=48;out=0;
     }
 //Serial.println(h1);
    //Serial.println(s1);
  /*if(digitalRead(settime)==0)
  {
    
    delay(50);
    {
      if(digitalRead(settime)==0)
      {
        while(digitalRead(settime)==0);
        Serial.println(" DA NHAN LED");
        
        if(mode1==0)
        {
          mode1=1;
           digitalWrite(led, HIGH);
          Serial.println(" Mode1= ");
          Serial.println(digitalRead(led));
         
        }
        else
        {
          mode1=0;
          digitalWrite(led, LOW);
          Serial.println(" Mode1= ");
         Serial.println(digitalRead(led));
          //Serial.println(digitalRead(led2));
         }
         
      }
     }
   }*/
   
 // if(mode1==1)
  //{
   // Serial.print("LED2 TRUOC:  ");
   // Serial.println(digitalRead(led2));
    //Serial.println("Mode = 1");
    if((h==h1&&m==s1))
    {
      //Serial.println("LED2=1");
      digitalWrite(led2, 0);  
     // Serial.println(digitalRead(led2));


  }
  else
  {
    //Serial.println("LED2=0");
      digitalWrite(led2, 1);  
    }
  if(updCnt<=0) { // every 10 scrolls, ~450s=7.5m
    updCnt = 60;
   // Serial.println("Getting data ...");
    
    getTime();
   // Serial.println("Data loaded");
    clkTime = millis();
  }
 
  if(millis()-clkTime > 60000 && !del && dots) { // clock for 30s, then scrolls for about 30s
//    printStringWithShift(date.c_str(),40);   Ngày, tháng , năm
//    delay(2000);
    updCnt--;
    clkTime = millis();
  }
  if(millis()-dotTime > 500) {
    dotTime = millis();
    dots = !dots;
  }
  updateTime();
  showAnimClock();
  }
if (Mode==1)
{
 if(ktra==1)
 { 
 // printStringWithShift(Clear.c_str(),50);
 initMAX7219();
   ktra=0;
   LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);
   lmd.setEnabled(true);  
 lmd.setIntensity(2);
 }
  
   //len=strlen(text1);
     if(out1==1)
    {
    text[0]= 'G';
    text[1]= 'O';
    text[2]= 'O';
    text[3]= 'D';
    text[4]= ' ';
    text[5]= 'N';
    text[6]= 'I';
    text[7]= 'G';
    text[8]= 'H';
    text[9]= 'T';
    out1=0;
    len=10;
      }
   displayText(text);
  // printStringWithShift(text,50);
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait for data from client to become available
  while(client.connected() && !client.available()){
    displayText(text);
    delay(1);
  }
   if(((text[0]>47&&text[0]<59)&&(text[1]>47&&text[1]<59))&&(text[3]>47&&text[3]<59))
   {
    out1=1;
    h1=(text[0]-48)*10+text[1]-48;
    s1=(text[2]-48)*10+text[3]-48;
    if(text[4]==49)
     {
      out=1;
    text[0]= 'G';
    text[1]= 'O';
    text[2]= 'O';
    text[3]= 'D';
    text[4]= ' ';
    text[5]= 'N';
    text[6]= 'I';
    text[7]= 'G';
    text[8]= 'H';
    text[9]= 'T';
    out1=0;
    len=10;
    last=millis();
    while((millis()-5500)<last)
    {
      delay(1);
    displayText(text);
    }
      text[4]==48;
      Mode=0;
     // Serial.println("PPPP");
      }
    
    // Serial.println(h1);
  //  Serial.println(s1);
    
    }
  
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    return;
  }
  req = req.substring(addr_start + 1, addr_end);
  client.flush();
  
  String s;
  String answer;
  int pos;
  if (req.indexOf('?')>0) {

    
     //Change url tags to text
     answer=req.substring(req.indexOf('=')+1);
    answer.replace('+',' ');
    //Conver HTML URL Encode to Text:
    //https://www.w3schools.com/tags/ref_urlencode.asp
    answer.replace("%21","\!");
    answer.replace("%22","\"");
    answer.replace("%23","\#");
    answer.replace("%24","\$");
    answer.replace("%25","%");
    answer.replace("%26","&");
    answer.replace("%27","\'");
    answer.replace("%28","\(");
    answer.replace("%29","\)");
    answer.replace("%2B","\+");
    answer.replace("%2C","\,");
    answer.replace("%2D","\-");
    answer.replace("%2E","\.");
    answer.replace("%2F","\/");
    answer.replace("%3A","\:");
    answer.replace("%3B","\;");
    answer.replace("%3C","\<");
    answer.replace("%3D","\=");
    answer.replace("%3E","\>");
    answer.replace("%3F","\?");
    answer.replace("%40","\@");
    answer.replace("%5B","\[");
    answer.replace("%5C","\\");
    answer.replace("%5D","\]");
    answer.replace("%5E","\^");
    answer.replace("%7B","\{");
    answer.replace("%7C","\|");
    answer.replace("%7D","\}");
     
      strcpy(text, answer.c_str());
      len=strlen(text);
      x=LEDMATRIX_WIDTH;
    lmd.clear();
    
    //Serial.println(req.substring(req.indexOf('=')+1));
    s="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><style> #header{ min-height: 20px; background-color: #FF9999; } #menu{ min-height: 20px; margin-top: 1%; background-color: #999999; } #body{ min-height: 200px; margin-top: 1%; } #footer{ min-height: 20px; margin-top: 1%; background-color: #FF9999; } #header, #menu, #body, #footer{ margin-left: 10%; margin-right: 10%; box-shadow: 3px 5px 7px #666666; border: 1px solid black; } @viewport{ zoom: 1.0; width: extend-to-zoom; } @-ms-viewport{ width: extend-to-zoom; zoom: 1.0; } </style> <html lang='en'> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>ESP8266 Web Server</title> </head> <body> <div id='header'><center><h1>LED MATRIX 32x8 AND NODEMCU</H1></center></div>";
    s+=" <div id='menu'><center><H2>CHAY CHU TREN 4 LED MATRIX</h2></center></div> ";
    s+="<div id='body'><center><div><div><br/><H3>";
    s+=answer;
    s+="<p><a href='./'>Back</a></p>";
    s+="</H3></div></div> </center> </div> <div id='footer'> </div> </body></html> ";
    //s+=req.substring(req.indexOf('=')+1);
    s+"</body></html>\r\n\r\n";
    client.print(s);
    return;
  }  
  else
  {
    
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s+="<!doctype html> <style> \#header{ min-height: 20px; background-color: \#E3EDF7; } \#menu{ min-height: 20px; margin-top: 1%; background-color: \#E3EDF7; } \#body{ min-height: 200px; margin-top: 1%;  background-color:#E3EDF7;} \#footer{ min-height: 20px; margin-top: 1%; background-color: \#FF9999; } \#header, \#menu, \#body, \#footer{ margin-left: 10%; margin-right: 10%; box-shadow: 3px 5px 7px \#666666; border: 1px solid #E3EDF7; } @viewport{ zoom: 1.0; width: extend-to-zoom; } @-ms-viewport{ width: extend-to-zoom; zoom: 1.0; } </style> <html lang='en'> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>ESP8266 Web Server</title> </head> <body> <div id='header'><center><h1>LED MATRIX</H1></center></div> <div id='body'><center><div><div><br> <form action='esp8266'> <br>Enter the text to be displayed<br><input type='text' maxlength='70' name='max' value=''><br><br> <input type='submit' value='Submit'></form> </div></div> </center> </div>  </div> </body>";
    s += "</html>\r\n\r\n";
    client.print(s);
   return;
  }
    
  
 }
  
}

// =======================================================================

void showSimpleClock()
{
  dx=dy=0;
  clr();
  showDigit(h/10,  0, dig6x8);
  showDigit(h%10,  8, dig6x8);
  showDigit(m/10, 17, dig6x8);
  showDigit(m%10, 25, dig6x8);
  showDigit(s/10, 34, dig6x8);
  showDigit(s%10, 42, dig6x8);
  setCol(23,dots ? B01100110 : 0);
  setCol(23,dots ? B01100110 : 0);
  refreshAll();
}

// =======================================================================

void showAnimClock()
{
  byte digPos[6]={0,8,17,25,34,42};
  int digHt = 12;
  int num = 6; 
  int i;
  if(del==0) {
    del = digHt;
    for(i=0; i<num; i++) digold[i] = dig[i];
    dig[3] = h/10 ? h/10 : 0;
    dig[2] = h%10;
    dig[1] = m/10;
    dig[0] = m%10;
    //dig[4] = s/10;
    //dig[5] = s%10;
    for(i=0; i<num; i++)  digtrans[i] = (dig[i]==digold[i]) ? 0 : digHt;
  } else
    del--;
  
  clr();
  for(i=0; i<num; i++) {
    if(digtrans[i]==0) {
      dy=0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy = -digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;
    }
  }
  dy=0;
  setCol(23,dots ? B01100110 : 0);
  setCol(23,dots ? B01100110 : 0);
  refreshAll();
  delay(30);
}

// =======================================================================

void showDigit(char ch, int col, const uint8_t *data)
{
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<8*NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;
    }
}

// =======================================================================

void setCol(int col, byte v)
{
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<8*NUM_MAX)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}

// =======================================================================

int showChar(char ch, const uint8_t *data)
{
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

// =======================================================================
int dualChar = 0;

unsigned char convertPolish(unsigned char _c)
{
  unsigned char c = _c;
  if(c==196 || c==197 || c==195) {
    dualChar = c;
    return 0;
  }
  if(dualChar) {
    switch(_c) {
      case 133: c = 1+'~'; break; // 'Ä…'
      case 135: c = 2+'~'; break; // 'Ä‡'
      case 153: c = 3+'~'; break; // 'Ä™'
      case 130: c = 4+'~'; break; // 'Å‚'
      case 132: c = dualChar==197 ? 5+'~' : 10+'~'; break; // 'Å„' and 'Ä„'
      case 179: c = 6+'~'; break; // 'Ã³'
      case 155: c = 7+'~'; break; // 'Å›'
      case 186: c = 8+'~'; break; // 'Åº'
      case 188: c = 9+'~'; break; // 'Å¼'
      //case 132: c = 10+'~'; break; // 'Ä„'
      case 134: c = 11+'~'; break; // 'Ä†'
      case 152: c = 12+'~'; break; // 'Ä˜'
      case 129: c = 13+'~'; break; // 'Å'
      case 131: c = 14+'~'; break; // 'Åƒ'
      case 147: c = 15+'~'; break; // 'Ã“'
      case 154: c = 16+'~'; break; // 'Åš'
      case 185: c = 17+'~'; break; // 'Å¹'
      case 187: c = 18+'~'; break; // 'Å»'
      default:  break;
    }
    dualChar = 0;
    return c;
  }    
  switch(_c) {
    case 185: c = 1+'~'; break;
    case 230: c = 2+'~'; break;
    case 234: c = 3+'~'; break;
    case 179: c = 4+'~'; break;
    case 241: c = 5+'~'; break;
    case 243: c = 6+'~'; break;
    case 156: c = 7+'~'; break;
    case 159: c = 8+'~'; break;
    case 191: c = 9+'~'; break;
    case 165: c = 10+'~'; break;
    case 198: c = 11+'~'; break;
    case 202: c = 12+'~'; break;
    case 163: c = 13+'~'; break;
    case 209: c = 14+'~'; break;
    case 211: c = 15+'~'; break;
    case 140: c = 16+'~'; break;
    case 143: c = 17+'~'; break;
    case 175: c = 18+'~'; break;
    default:  break;
  }
  return c;
}

// =======================================================================

void printCharWithShift(unsigned char c, int shiftDelay) {
  c = convertPolish(c);
  if (c < ' ' || c > '~'+25) return;
  c -= 32;
  int w = showChar(c, font);
  for (int i=0; i<w+1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}

// =======================================================================

void printStringWithShift(const char* s, int shiftDelay){
  while (*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
  }
}
// =======================================================================

void getTime()
{
  WiFiClient client;
  if (!client.connect("www.google.com", 80)) {
   // Serial.println("connection to google failed");
    return;
  }

  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") +
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    //Serial.println(".");
    repeatCounter++;
  }

  String line;
  client.setNoDelay(false);
  while(client.connected() && client.available()) {
    line = client.readStringUntil('\n');
    line.toUpperCase();
    if (line.startsWith("DATE: ")) {
      date = "     "+line.substring(6, 22);
      date.toUpperCase();
//      decodeDate(date);
      h = line.substring(23, 25).toInt();
      m = line.substring(26, 28).toInt();
      s = line.substring(29, 31).toInt();
      summerTime = checkSummerTime();
        if(h+utcOffset+summerTime>23) {
          if(++day>31) { day=1; month++; };  // needs better patch
          if(++dayOfWeek>7) dayOfWeek=1; 
        }
      localMillisAtUpdate = millis();
      localEpoc = (h * 60 * 60 + m * 60 + s);
    }
  }
  client.stop();
}

// =======================================================================

int checkSummerTime()
{
  if(month>3 && month<10) return 1;
  if(month==3 && day>=31-(((5*year/4)+4)%7) ) return 1;
  if(month==10 && day<31-(((5*year/4)+1)%7) ) return 1;
  return 0;
}
// =======================================================================

// =======================================================================

void updateTime()
{
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  epoch = (long)round( curEpoch + 3600 * ( utcOffset + summerTime ) + 86400L ) % 86400L;
  h = ((epoch  % 86400L) / 3600) % 24;
  m = (epoch % 3600) / 60;
  s = epoch % 60;


   
}
void displayText ( char * theText)
{
   if ( myTime + ANIM_DELAY < millis()) 
   {
        myTime=millis();
        // Draw the text to the current position
        drawString(theText, len, x, 0);
        // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background... 
        // Toggle display of the new framebuffer
        lmd.display();  
        // Advance to next coordinate
        if( --x < len * -8 )
        {
            x = LEDMATRIX_WIDTH;
            lmd.clear();
        }
   }  
}

/**
 * This function draws a string of the given length to the given position.
 */
void drawString(char* text, int len, int x, int y )
{
  
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > LEDMATRIX_WIDTH )
      return;

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      drawSprite( font1[c], x + idx * 8, y, 8, 8 );
  }
}

/**
 * This draws a sprite to the given position using the width and height supplied (usually 8x8)
 */
void drawSprite( byte* sprite, int x, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;
  
  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      //Yes my font is backwards so I swap it around.
      //lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));
        lmd.setPixel(x + (width - ix), y + iy, (bool)(sprite[iy] & mask ));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}
void ok()
{
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,1);
  sendCmdAll(CMD_INTENSITY,0.4); // set Ä‘á»™ sÃ¡ng
 
 }
 
/*ICACHE_RAM_ATTR void ISR()
 
{  Serial.println("ngat ngat");
  delay(50000);
  
  if(Mode==0)
  {
    Mode=1;
      Serial.println("0 - > 1 ");
   }
   else if (Mode==1)
   {
    Mode=0;
      Serial.println("1 - > 0 ");
    }
   
}
 */

// =======================================================================