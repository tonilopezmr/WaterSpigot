//Pins
#define SPIGOT_OUTPUT_PIN     2
#define BUTTON_ONE_PIN        4
#define BUTTON_TWO_PIN        7
#define BUTTON_THREE_PIN      8
#define BUTTON_FOUR_PIN       13
#define SPIGOT_IN_PIN         5
#define WATER_SENSOR          12
#define NEEDS_WATER_LED       9
#define BUTTON_CONFIGURATION  10
#define CONFIGURATION_LED     10

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
  pinMode(NEEDS_WATER_LED, OUTPUT);
  pinMode(CONFIGURATION_LED, OUTPUT);
  pinMode(BUTTON_CONFIGURATION, INPUT);
}

void loop() {
  unsigned long time = millis();

  if (digitalRead(CONFIGURATION_LED)) {
    isConfigurationMode = !isConfigurationMode;
  }

  if (isConfigurationMode) {
    if (time - timeLed >= INTERVAL_BLINK_IN_MILLIS) {
      timeLed = time;
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
      inTimeAction(BUTTON_ONE_PIN, timePinOne);
    } else if (action == PIN_TWO) {
      inTimeAction(BUTTON_TWO_PIN, timePinTwo);
    } else if (action == PIN_THREE) {
      inTimeAction(BUTTON_THREE_PIN, timePinThree);
    } else if (action == PIN_FOUR) {
      inTimeAction(BUTTON_FOUR_PIN_PIN, timePinFour);
    }
  } else {
    if (digitalRead(SPIGOT_IN_PIN)) {
      action = SPIGOT;
      openSpigot();
    } else if (digitalRead(BUTTON_ONE_PIN)) {
      action = PIN_ONE;
      timeOnClick = time;
      openSpigot();
    } else if (digitalRead(BUTTON_TWO_PIN)) {
      action = PIN_TWO;
      timeOnClick = time;
      openSpigot();
    } else if (digitalRead(BUTTON_THREE_PIN)) {
      action = PIN_THREE;
      timeOnClick = time;
      openSpigot();
    } else if (digitalRead(BUTTON_FOUR_PIN)) {
      action = PIN_FOUR;
      timeOnClick = time;
      openSpigot();
    }
  }
}

void spigotAction() {
  int spigotIsOpen = digitalRead(SPIGOT_IN_PIN);
  digitalWrite(SPIGOT_OUTPUT_PIN, spigotIsOpen);
  if (!spigotIsOpen) {
    isOpen = false;
  }
}

void inTimeAction(int pin, long timeIntervalClose) {
  if (isConfigurationMode) {
    int spigotIsOpen = digitalRead(pin);
    digitalWrite(SPIGOT_OUTPUT_PIN, spigotIsOpen);
    if (!spigotIsOpen) {
      timeIntervalClose = time - timeOnClick;
      isOpen = false;
    }
  } else {
    if (time - timeOnClick >= timeIntervalClose) {
      isOpen = false;
    }
  }
}

void openSpigot() {
  isOpen = true;
  digitalWrite(SPIGOT_OUTPUT_PIN, HIGH);
}

void closeSpigot() {
  isOpen = false;

}
