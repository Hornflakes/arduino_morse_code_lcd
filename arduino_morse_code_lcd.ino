#include "morse_code_translator.h";


const unsigned int LED_PIN = 7;
const unsigned int BTN_PIN = 9;

const unsigned int DOT_MAX_MILLIS = 50;
const unsigned int DASH_MAX_MILLIS = 150;
const unsigned int GAP_MILLIS = 175;

unsigned long pushMillis;
unsigned long releaseMillis;
bool btnState;
bool isBtnPushed;
bool wasMorseDisplayed;
unsigned int morseTreeNodeIndex;

void setup() {
  Serial.begin(9600);
  pinMode(BTN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  isBtnPushed = false;
  wasMorseDisplayed = true;
  morseTreeNodeIndex = 0;
}

void loop() {
  btnState = digitalRead(BTN_PIN);

  if(btnState) {
    if(!isBtnPushed) handleBtnPush();
    return;
  }

  if (!isBtnPushed) {
    if(!wasMorseDisplayed) checkMillisSinceRelease();
    return;
  }

  handleBtnRelease();
}

void handleBtnPush() {
  digitalWrite(LED_PIN, 1);
  pushMillis = millis();
  isBtnPushed = true;
}

void handleBtnRelease() {
  Serial.println("release");
  digitalWrite(LED_PIN, 0);
  
  releaseMillis = millis();
  const long millisDiff = releaseMillis - pushMillis;
  int dashOrDotAdd;
  
  if(millisDiff >= DASH_MAX_MILLIS) dashOrDotAdd = 2;
  else if(millisDiff >= DOT_MAX_MILLIS) dashOrDotAdd = 1;
  morseTreeNodeIndex = buildMorseTreeNodeIndex(morseTreeNodeIndex, dashOrDotAdd);
  
  isBtnPushed = false;
  wasMorseDisplayed = false;
}

void checkMillisSinceRelease() {
  const long currMillis = millis();
  if(currMillis - releaseMillis >= GAP_MILLIS) displayChar();
}

void displayChar() {
  Serial.println(MORSE_BINARY_INDEXED_TREE[morseTreeNodeIndex]);
  wasMorseDisplayed = true;
  morseTreeNodeIndex = 0;
}
