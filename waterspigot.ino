//Pins
#define SPIGOT_OUTPUT_PIN     2
#define BUTTON_ONE_PIN        4
#define BUTTON_TWO_PIN        7
#define BUTTON_THREE_PIN      8
#define BUTTON_FOUR_PIN       13
#define SPIGOT_IN_PIN         5
#define WATER_SENSOR          12
#define BUTTON_CONFIGURATION  10
#define CONFIGURATION_LED     9

//Actions
#define SPIGOT     1
#define PIN_ONE    2
#define PIN_TWO    3
#define PIN_THREE  4
#define PIN_FOUR   5

#define INTERVAL_BLINK_IN_MILLIS   500


int action;
int isOpen;
int isConfigurationMode;
int ledState = LOW;

unsigned long timeOnClick = 0;
unsigned long timeLed = 0;
long timePinOne;
long timePinTwo;
long timePinThree;
long timePinFour;

void setup() {
  Serial.begin(9600);

  pinMode(SPIGOT_OUTPUT_PIN, OUTPUT);
  pinMode(BUTTON_ONE_PIN, INPUT);
  pinMode(BUTTON_TWO_PIN, INPUT);
  pinMode(BUTTON_THREE_PIN, INPUT);
  pinMode(BUTTON_FOUR_PIN, INPUT);
  pinMode(SPIGOT_IN_PIN, INPUT);
  pinMode(WATER_SENSOR, INPUT);
  pinMode(CONFIGURATION_LED, OUTPUT);
  pinMode(BUTTON_CONFIGURATION, INPUT);

  digitalWrite(SPIGOT_OUTPUT_PIN, LOW);
}

void loop() {
  unsigned long currentTime = millis();

  Serial.print(" Configuration mode: ");
  Serial.print(isConfigurationMode);
  Serial.print(" Is open: ");
  Serial.print(isOpen);
  Serial.print(" Last action: ");
  Serial.println(action);

  if (digitalRead(BUTTON_CONFIGURATION)) {
    isConfigurationMode = !isConfigurationMode;
  }

  if (isConfigurationMode) {
    if (currentTime - timeLed >= INTERVAL_BLINK_IN_MILLIS) {
      timeLed = currentTime;
      ledState = !ledState;
      digitalWrite(CONFIGURATION_LED, ledState);
    }
  } else {
    timeOnClick = 0;
    digitalWrite(CONFIGURATION_LED, LOW);
  }

  if (isOpen) {
    if (action == SPIGOT) {
      spigotAction();
    } else if (action == PIN_ONE) {
      inTimeAction(BUTTON_ONE_PIN, currentTime, timePinOne);
    } else if (action == PIN_TWO) {
      inTimeAction(BUTTON_TWO_PIN, currentTime, timePinTwo);
    } else if (action == PIN_THREE) {
      inTimeAction(BUTTON_THREE_PIN, currentTime, timePinThree);
    } else if (action == PIN_FOUR) {
      inTimeAction(BUTTON_FOUR_PIN, currentTime, timePinFour);
    }
  } else {
    if (digitalRead(SPIGOT_IN_PIN)) {
      action = SPIGOT;
      openSpigot();
    } else if (digitalRead(BUTTON_ONE_PIN)) {
      action = PIN_ONE;
      timeOnClick = currentTime;
      openSpigot();
    } else if (digitalRead(BUTTON_TWO_PIN)) {
      action = PIN_TWO;
      timeOnClick = currentTime;
      openSpigot();
    } else if (digitalRead(BUTTON_THREE_PIN)) {
      action = PIN_THREE;
      timeOnClick = currentTime;
      openSpigot();
    } else if (digitalRead(BUTTON_FOUR_PIN)) {
      action = PIN_FOUR;
      timeOnClick = currentTime;
      openSpigot();
    }
  }
}

void spigotAction() {
  int spigotIsOpen = digitalRead(SPIGOT_IN_PIN);
  digitalWrite(SPIGOT_OUTPUT_PIN, spigotIsOpen);
  if (!spigotIsOpen) {
    closeSpigot();
  }
}

void inTimeAction(int pin, long currentTime, long timeIntervalClose) {
  if (isConfigurationMode) {
    int spigotIsOpen = digitalRead(pin);
    digitalWrite(SPIGOT_OUTPUT_PIN, spigotIsOpen);
    if (!spigotIsOpen) {
      timeIntervalClose = currentTime - timeOnClick;
      closeSpigot();
    }
  } else {
    if (currentTime - timeOnClick >= timeIntervalClose) {
      closeSpigot();
    }
  }
}

void openSpigot() {
  isOpen = true;
  digitalWrite(SPIGOT_OUTPUT_PIN, HIGH);
}

void closeSpigot() {
  isOpen = false;
  digitalWrite(SPIGOT_OUTPUT_PIN, LOW);
}
