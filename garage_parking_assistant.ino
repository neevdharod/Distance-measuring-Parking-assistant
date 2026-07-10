#include <Adafruit_NeoPixel.h>

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 6;
const int ledCount = 16;

Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);

const int detectDistance = 250;
const int greenDistance = 150;
const int yellowDistance = 80;
const int redDistance = 40;

unsigned long lastSeen = 0;
const unsigned long offDelay = 30000;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  strip.begin();
  strip.setBrightness(120);
  strip.show();
}

void loop() {
  int distance = getDistance();

  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= detectDistance) {
    lastSeen = millis();

    if (distance > greenDistance) {
      color(0, 255, 0);
    } else if (distance > yellowDistance) {
      color(255, 160, 0);
    } else if (distance > redDistance) {
      color(255, 0, 0);
    } else {
      blinkRed();
    }
  } else {
    if (millis() - lastSeen > offDelay) {
      color(0, 0, 0);
    }
  }

  delay(100);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.034 / 2;
}

void color(int r, int g, int b) {
  for (int i = 0; i < ledCount; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void blinkRed() {
  static bool on = false;
  static unsigned long lastBlink = 0;

  if (millis() - lastBlink >= 250) {
    lastBlink = millis();
    on = !on;

    if (on) {
      color(255, 0, 0);
    } else {
      color(0, 0, 0);
    }
  }
}
