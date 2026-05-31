/*
polar function graph drawing

***
complex polar plot! The function is:
r(θ)=ecos⁡(θ)−2cos⁡(4θ)+sin⁡5(θ12)
for θθ from 00 to 24π24π.
***

OLED wire connecction
 Wiring: I2C OLED
OLED Pin	Arduino Uno / Mega
VCC	5V
GND	GND
SDA	A4 (Uno) / 20 (Mega)
SCL	A5 (Uno) / 21 (Mega)

*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int centerX = SCREEN_WIDTH / 2;
const int centerY = SCREEN_HEIGHT / 2;

// Parameters for polar plot
const float thetaMax = 24 * PI;
const float scaleFactor = 6.0; // Smaller to shrink the graph

void drawAxes() {
  // Draw x-axis (horizontal)
  display.drawLine(0, centerY, SCREEN_WIDTH - 1, centerY, SSD1306_WHITE);

  // Draw y-axis (vertical)
  display.drawLine(centerX, 0, centerX, SCREEN_HEIGHT - 1, SSD1306_WHITE);

  // Draw tick marks on x-axis (no labels)
  for (int x = centerX; x < SCREEN_WIDTH; x += 10) {
    display.drawLine(x, centerY - 1, x, centerY + 1, SSD1306_WHITE);
  }
  for (int x = centerX; x >= 0; x -= 10) {
    display.drawLine(x, centerY - 1, x, centerY + 1, SSD1306_WHITE);
  }

  // Draw tick marks on y-axis (no labels)
  for (int y = centerY; y < SCREEN_HEIGHT; y += 10) {
    display.drawLine(centerX - 1, y, centerX + 1, y, SSD1306_WHITE);
  }
  for (int y = centerY; y >= 0; y -= 10) {
    display.drawLine(centerX - 1, y, centerX + 1, y, SSD1306_WHITE);
  }
}

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Title
  display.setCursor(0, 5);
  display.println("Polar Function");

  // Equation line 1
  display.setCursor(0, 20);
  display.println("r = e^cos(θ) - 2cos(4θ)");

  // Equation line 2
  display.setCursor(0, 30);
  display.println("    + sin⁵(θ/12)");

  display.display();
  delay(3000);
}


void loop() {
  display.clearDisplay();

  drawAxes();

  float lastX = 0, lastY = 0;
  bool firstPoint = true;

  for (float theta = 0; theta <= thetaMax; theta += 0.05) {
    float r = exp(cos(theta)) 
              - 2 * cos(4 * theta) 
              + pow(sin(theta / 12), 5);

    float rScaled = r * scaleFactor;

    float x = rScaled * cos(theta);
    float y = rScaled * sin(theta);

    int px = centerX + (int)x;
    int py = centerY - (int)y;

    if (!firstPoint) {
      display.drawLine((int)lastX, (int)lastY, px, py, SSD1306_WHITE);
    } else {
      firstPoint = false;
    }

    lastX = px;
    lastY = py;
  }

  display.display();
  delay(3000);
}
