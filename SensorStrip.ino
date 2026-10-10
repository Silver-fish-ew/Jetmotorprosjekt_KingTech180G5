#include <Adafruit_NeoPixel.h>
#include <TM1637Display.h>

#define NEO_LEDS_PIN 6
#define DISPLAY_CLK_PIN 2
#define DISPLAY_DIO_PIN 3

#define NUMPIXELS 16

#define STRIP_MAX 1023
#define STRIP_MIN 0

int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};
int x = 0;

TM1637Display tm1637(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
Adafruit_NeoPixel pixels(NUMPIXELS, NEO_LEDS_PIN, NEO_GRB + NEO_KHZ800);

void displayData(int stripVal) {
  pixels.clear();

  TimeDisp[0] = stripVal / 1000;
  TimeDisp[1] = (stripVal % 1000) / 100;
  TimeDisp[2] = stripVal % 100 / 10;
  TimeDisp[3] = stripVal % 10;
  
  tm1637.display(TimeDisp);

  int numPixels = map(stripVal, STRIP_MIN, STRIP_MAX, 0, 16);
  for (int i = 0; i <= numPixels; i++) {
    if (i-1 < NUMPIXELS/3) {
      pixels.setPixelColor(i, pixels.Color(0, 30, 0));
    } else if (i-1 < 2*NUMPIXELS / 3) {
      pixels.setPixelColor(i, pixels.Color(15, 15, 0));
    } else {
      pixels.setPixelColor(i, pixels.Color(30, 0, 0));
    }
  }
  pixels.show();
}

void setup() {  
  pixels.begin();
  tm1637.set();
  tm1637.init();
  pinMode(A0, INPUT);

}


void loop() {
  x = analogRead(A0);
  displayData();
}