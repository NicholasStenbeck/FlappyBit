#include <Arduino.h>
#include <oled85.h>

#define BUTTON PB1
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
#define INITIAL_PLAYER_SPEED 1
#define PILLAR_SPEED 1

struct Pillar {
  uint8_t x;
  uint8_t holeY;
};

int LEDPin = PB3;

int buttonState = 0;
int lastButtonState;
bool buttonHasPressed = false;

const uint8_t debounceDelay = 20;
unsigned long lastTimestamp = 0;

const uint16_t fallDelay = 250;
unsigned long lastFallTimestamp = 0;

uint8_t playerHeight = SCREEN_HEIGHT / 2;
int8_t playerSpeed = INITIAL_PLAYER_SPEED;
Pillar pillars[8];

OLED85 oled85;

void setup()
{
  pinMode(LEDPin, OUTPUT);  //sets LEDPin to output
  pinMode (BUTTON, INPUT_PULLUP);  //sets buttonPin to input
}

uint8_t calculatePlayerHeight() {
  return playerHeight + playerSpeed;
}

boolean fall() {
  if ((millis() - lastFallTimestamp) <= fallDelay) {
    return false;
  }

  oled85.removeBlock(2, playerHeight);
  playerSpeed--;
  playerHeight = calculatePlayerHeight();
  lastFallTimestamp = millis();
  return true;
}

void flap() {
  playerSpeed = INITIAL_PLAYER_SPEED;
}

boolean checkButtonPress(){
  int reading = digitalRead(BUTTON);

  if (reading != buttonState) {
    lastTimestamp = millis();
    buttonState = reading;
    buttonHasPressed = false;
  }

  if ((millis() - lastTimestamp) > debounceDelay){
    if (!buttonHasPressed && buttonState == LOW){
      digitalWrite(LEDPin, !digitalRead(LEDPin));
      flap();
      buttonHasPressed = true;
      return true;
    }
  }

  return false;
}

void loop()
{
  if (fall()) {
    oled85.drawBlock(2, playerHeight, 0, 0, 0x7e);
  }
  checkButtonPress();
}