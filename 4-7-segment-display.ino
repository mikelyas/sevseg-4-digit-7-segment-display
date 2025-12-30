#include <SevSeg.h>

SevSeg sevseg;
const uint8_t buttonPins[4] = {14, 15, 16, 17};
const uint8_t debounceMs = 25;
bool buttonPressed[4] = {false, false, false, false};
unsigned long lastPressTime[4] = {0, 0, 0, 0};
uint16_t counter = 0;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; 
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);

  for (uint8_t buttonPin : buttonPins) {
    pinMode(buttonPin, INPUT);
  }

  Serial.begin(9600);
  Serial.println("Program has started. Waiting for input...");
}

void loop() {
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t reading = digitalRead(buttonPins[i]);
    if (reading == LOW) {
      if (!buttonPressed[i] && (millis() - lastPressTime[i] > debounceMs)) {
        buttonPressed[i] = true;
        lastPressTime[i] = millis();
        Serial.println("Button pressed: " + String(i));
        counter = (counter + (uint16_t)(pow(10, i) + 0.5)) % 10000;
      }
    } else {
      buttonPressed[i] = false;
    } 
  }
  sevseg.setNumber(counter);
  sevseg.refreshDisplay();
}
