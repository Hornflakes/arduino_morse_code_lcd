const unsigned int LED_PIN = 7;
const unsigned int BTN_PIN = 9;

const unsigned int DOT_MAX_MILLIS = 50;
const unsigned int DASH_MAX_MILLIS = 150;

unsigned long prevMillis;
unsigned long currentMillis;
bool isBtnPushed;

void setup() {
  Serial.begin(9600);
  pinMode(BTN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int btnState = digitalRead(BTN_PIN);

  if(btnState) {
    if(isBtnPushed) return;
    onBtnPush();
    return;
  }

  digitalWrite(LED_PIN, 0);

  if (!isBtnPushed) return;
  onBtnRelease();
}

void onBtnPush() {
  digitalWrite(LED_PIN, 1);
  prevMillis = millis();
  isBtnPushed = true;
}

void onBtnRelease() {
  currentMillis = millis();
  long millisDiff = currentMillis - prevMillis;

  if(millisDiff >= DASH_MAX_MILLIS) Serial.println("dash");
  else if(millisDiff >= DOT_MAX_MILLIS) Serial.println("dot");
  isBtnPushed = false;
}