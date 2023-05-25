#include "LiquidCrystal.h";
#include "morse_code_translator.h";

const unsigned int LCD_WIDTH = 16;
const unsigned int LCD_HEIGHT = 2;

const unsigned int LED_PIN = 12;
const unsigned int BTN_PIN = 13;
const unsigned int BUZZER_PIN = 10;

const unsigned int DOT_MAX_MILLIS = 100;
const unsigned int DASH_MAX_MILLIS = 300;
const unsigned int GAP_MILLIS = 325;
const unsigned int DEBOUNCE_TIME = 25;

int lastBtnState = HIGH;
bool isBtnPushed = false;
bool wasMorseDisplayed = true;
unsigned int morseTreeNodeIndex = 0;

int btnState;
unsigned long pushMillis;
unsigned long releaseMillis;
unsigned long btnStateChangeMillis;

LiquidCrystal lcd(9, 8, 4, 5, 6, 7);
int charsPrinted = 0;
bool isFirstLcdRowFull = false;
char lastLcdRow[16] = "";

void setup() {
    Serial.begin(9600);
    pinMode(BTN_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
}

void loop() {
    btnState = digitalRead(BTN_PIN);
    if (btnState != lastBtnState) btnStateChangeMillis = millis();

    if (btnStateChangeMillis - millis() <= DEBOUNCE_TIME) return;
    lastBtnState = btnState;

    if (btnState) {
        tone(BUZZER_PIN, 300);
        if (!isBtnPushed) handleBtnPush();
        return;
    }

    if (!isBtnPushed) {
        noTone(BUZZER_PIN);
        if (!wasMorseDisplayed) checkMillisSinceRelease();
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

    if (millisDiff >= DASH_MAX_MILLIS)
        dashOrDotAdd = 2;
    else if (millisDiff >= DOT_MAX_MILLIS)
        dashOrDotAdd = 1;
    morseTreeNodeIndex = buildMorseTreeNodeIndex(morseTreeNodeIndex, dashOrDotAdd);

    isBtnPushed = false;
    wasMorseDisplayed = false;
}

void checkMillisSinceRelease() {
    const long currMillis = millis();
    if (currMillis - releaseMillis >= GAP_MILLIS) displayChar();
}

void displayChar() {
    char ch = MORSE_BINARY_INDEXED_TREE[morseTreeNodeIndex];
    if (morseTreeNodeIndex > sizeof(MORSE_BINARY_INDEXED_TREE) / sizeof(MORSE_BINARY_INDEXED_TREE[0])) ch = '_';
    Serial.println(ch);
    printLcd(ch);

    wasMorseDisplayed = true;
    morseTreeNodeIndex = 0;
}

void printLcd(char ch) {
    lcd.print(ch);
    lastLcdRow[charsPrinted % 16] = ch;
    charsPrinted++;

    if (charsPrinted % LCD_WIDTH) return;

    if (!isFirstLcdRowFull) {
        lcd.setCursor(0, 1);
        isFirstLcdRowFull = true;
    } else {
        scrollVerticallyLcd();
    }
}

void scrollVerticallyLcd() {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print(lastLcdRow);
    lcd.setCursor(0, 1);
}
