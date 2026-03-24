//Project 31

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// TFT pins
#define TFT_CS    10
#define TFT_DC    9
#define TFT_RST    8

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// TMP36 pin
const int tempPin = A5;

// temperature storage
int tcurrent = 0;
int tempArray[120];

// screen size
const int screenW = 320;
const int screenH = 240;

// graph area
const int graphLeft  = 40;
const int graphTop    = 20;
const int graphWidth  = 260;
const int graphHeight = 180;
const int graphRight  = graphLeft + graphWidth;
const int graphBottom = graphTop + graphHeight;

void getTemp();
void drawScreen();
int tempToY(int tempC);

void setup()
{
  // ST7789 240x320 display
  tft.init(240, 320);

  // landscape
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);

  for (int i = 0; i < 120; i++)
  {
    tempArray[i] = 0;
  }
}

void loop()
{
  getTemp();
  drawScreen();

  for (int a = 0; a < 20; a++)
  {
    delay(60000);
  }
}

void getTemp()
{
  float sensor = 0.0;
  float voltage = 0.0;
  float celsius = 0.0;

  sensor = analogRead(tempPin);
  voltage = (sensor * 5000.0) / 1024.0;
  voltage = voltage - 500.0;
  celsius = voltage / 10.0;

  tcurrent = (int)celsius;

  for (int a = 119; a > 0; a--)
  {
    tempArray[a] = tempArray[a - 1];
  }

  tempArray[0] = tcurrent;
}

int tempToY(int tempC)
{
  if (tempC < 0) tempC = 0;
  if (tempC > 50) tempC = 50;

  return map(tempC, 0, 50, graphBottom, graphTop);
}

void drawScreen()
{
  tft.fillScreen(ST77XX_BLACK);

  // title
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.print("Temperature Monitor");

  // graph border
  tft.drawRect(graphLeft, graphTop, graphWidth, graphHeight, ST77XX_WHITE);

  // horizontal grid + labels
  tft.setTextSize(1);
  for (int tempMark = 0; tempMark <= 50; tempMark += 10)
  {
    int y = tempToY(tempMark);

    tft.drawLine(graphLeft, y, graphRight, y, ST77XX_BLUE);

    tft.setCursor(5, y - 3);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(tempMark);
  }

  // x-axis labels
  tft.setCursor(graphLeft, graphBottom + 8);
  tft.print("Newest");

  tft.setCursor(graphRight - 20, graphBottom + 8);
  tft.print("Old");

  // current temperature
  tft.setTextSize(2);
  tft.setCursor(180, 220);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("Now:");
  tft.print(tcurrent);
  tft.print("C");

  // plot temperature history
  for (int i = 0; i < 119; i++)
  {
    int x1 = map(i, 0, 119, graphLeft, graphRight);
    int y1 = tempToY(tempArray[i]);

    int x2 = map(i + 1, 0, 119, graphLeft, graphRight);
    int y2 = tempToY(tempArray[i + 1]);

    tft.drawLine(x1, y1, x2, y2, ST77XX_GREEN);
    tft.drawPixel(x1, y1, ST77XX_RED);
  }

  int lastX = map(119, 0, 119, graphLeft, graphRight);
  int lastY = tempToY(tempArray[119]);
  tft.drawPixel(lastX, lastY, ST77XX_RED);
}

