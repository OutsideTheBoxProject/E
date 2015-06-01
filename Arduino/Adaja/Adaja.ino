//*******************************************************
// OutsideTheBox (http://outsidethebox.at)
// @author Christopher Frauenberger
// @created 27 May 2015
//
//*******************************************************

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SD.h>
#include <SPI.h>

// If we are using the hardware SPI interface, these are the pins (for future ref)
#define sclk 13 // CL - SPI clock
#define miso 12 // SO - SPI miso 
#define mosi 11 // SI - SPI mosi
#define sdcs 10 // SC - SD card chip select
#define rst 9 // R - OLED reset 
#define dc 8 // DC - OLED dc
#define cs 4 // OC - OLED chip select

// Audio
#define audioIn 0
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

// drawing 
int x= 0;
int y= 0;
int w, h;

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// to draw images from the SD card, we will share the hardware SPI interface
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

// For Arduino Uno/Duemilanove, etc
//  connect the SD card with MOSI going to pin 11, MISO going to pin 12 and SCK going to pin 13 (standard)
//  Then pin 10 goes to CS (or whatever you have set up)
#define SD_CS sdcs    // Set the chip select line to whatever you use (10 doesnt conflict with the library)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   
  // Chipselect for OLED
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
     
  // initialize the OLED
  tft.begin();

  Serial.println("init");
  
  tft.fillScreen(BLUE);
  delay(500);
   
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("SD OK!");
}

void loop() {
  if (x==0) 
    tft.fillScreen(BLACK);
  double level = getLevel();
  if (level > 0.4) {
    tft.fillScreen(BLACK);
    x = 0;
    tft.setCursor(0, 40);
    tft.setTextColor(RED);  
    tft.setTextSize(3);
    tft.println("ZU LAUT");
    delay (4000);
  } else {
    y = tft.height()*level;
//  Serial.print(level);
//  Serial.print("  ");
//  Serial.print(y);
    tft.drawFastVLine(x, tft.height()/2-y, 2*y, MAGENTA);
    x++;
    x = x%tft.width();
//  Serial.print("  ");
//  Serial.println(x);
  }
}
