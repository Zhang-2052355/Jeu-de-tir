const int PIN_BUTTON = 5;       // D1 
const int PIN_BUZZER = 16;      // D0 
const int PIN_MUTE = 4;         // D2

bool soundEnabled = true;

const int PIN_595_DATA = 13;    // D7
const int PIN_595_CLOCK = 14;   // D5
const int PIN_595_LATCH = 12;   // D6

const byte BIT_TARGET_1 = 0x02;
const byte BIT_TARGET_2 = 0x04;
const byte BIT_TARGET_3 = 0x08;

const byte BIT_SCORE_1 = 0x10;
const byte BIT_SCORE_2 = 0x20;
const byte BIT_SCORE_3 = 0x40;

const byte BIT_MISS = 0x80;

const byte BIT_PWR_1 = 0x02;
const byte BIT_PWR_2 = 0x04;
const byte BIT_PWR_3 = 0x08;
const byte BIT_PWR_4 = 0x10;
const byte BIT_PWR_5 = 0x20;


byte data_led = 0x00;
byte data_power = 0x00;
byte statusBits = 0x00;

int roundNumber = 1;

int score = 0;
int misses = 0;
int currentTarget = 0;
int powerLevel = 0;

unsigned long stateTimer = 0;
unsigned long chargeStartTime = 0;


enum GameState {
  STATE_START,
  STATE_NEXT_ROUND,
  STATE_SHOW_TARGET,
  STATE_WAIT_INPUT,
  STATE_CHARGING,
  STATE_SHOOT,
  STATE_EVAL,
  STATE_GAME_OVER,
  STATE_WIN,
  STATE_LOSE,
  STATE_RESET
};

GameState currentState = STATE_START;

void updateShiftRegisters() {

  digitalWrite(PIN_595_LATCH, LOW);

  shiftOut(
    PIN_595_DATA,
    PIN_595_CLOCK,
    MSBFIRST,
    data_power
  );

  shiftOut(
    PIN_595_DATA,
    PIN_595_CLOCK,
    MSBFIRST,
    data_led
  );

  digitalWrite(PIN_595_LATCH, HIGH);
}


void updatePowerLeds(int level) {

  data_power = 0x00;

  if (level >= 1)
    data_power |= BIT_PWR_1;

  if (level >= 2)
    data_power |= BIT_PWR_2;

  if (level >= 3)
    data_power |= BIT_PWR_3;

  if (level >= 4)
    data_power |= BIT_PWR_4;

  if (level >= 5)
    data_power |= BIT_PWR_5;

  updateShiftRegisters();
}

void beep(int duration) {
  if (!soundEnabled)
    return;

  if (duration <= 0)
    return;

  digitalWrite(PIN_BUZZER, HIGH);

  delay(duration);

  digitalWrite(PIN_BUZZER, LOW);
}


void playSoundEffect(int type) {

  switch (type) {

    case 1:
      beep(30);
      delay(30);

      beep(30);
      delay(30);

      beep(60);
      break;


    case 2:
      beep(20);
      break;


    case 3:
      beep(40);
      delay(30);

      beep(60);
      break;


    case 4:
      beep(100);
      break;


    case 5:

      for (int i = 0; i < 6; i++) {

        beep(30);
        delay(30);
      }

      break;


    case 6:

      beep(100);
      delay(60);

      beep(150);
      break;
  }
}

byte getTargetBit(int target) {

  if (target == 1)
    return BIT_TARGET_1;

  if (target == 2)
    return BIT_TARGET_2;

  if (target == 3)
    return BIT_TARGET_3;

  return 0;
}


void setup() {

  Serial.begin(115200);

  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pinMode(PIN_MUTE, INPUT_PULLUP);

  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_595_DATA, OUTPUT);
  pinMode(PIN_595_CLOCK, OUTPUT);
  pinMode(PIN_595_LATCH, OUTPUT);

  digitalWrite(PIN_BUZZER, LOW);

  data_led = 0x00;
  data_power = 0x00;
  statusBits = 0x00;

  updateShiftRegisters();

  randomSeed(analogRead(A0));


  stateTimer = millis();
}

void loop() {

  static bool lastMuteButton = HIGH;

  bool muteButton = digitalRead(PIN_MUTE);

  if (lastMuteButton == HIGH && muteButton == LOW) {

    soundEnabled = !soundEnabled;

    if (!soundEnabled) {
      digitalWrite(PIN_BUZZER, LOW);
    }

    delay(50);
  }


  lastMuteButton = muteButton;

  switch (currentState) {

    case STATE_START:

      statusBits = 0x00;

      roundNumber = 1;

      score = 0;

      misses = 0;

      playSoundEffect(1);

      for (int i = 0; i < 3; i++) {

        data_led = 0x0E;

        updatePowerLeds(5);

        updateShiftRegisters();

        delay(250);


        data_led = 0xF0;

        updatePowerLeds(0);

        updateShiftRegisters();

        delay(250);
      }


      data_led = 0x00;

      updatePowerLeds(0);

      updateShiftRegisters();

      delay(300);


      currentState = STATE_NEXT_ROUND;

      break;

    case STATE_NEXT_ROUND:

      updatePowerLeds(0);

      powerLevel = 0;

      currentTarget = random(1, 4);


      data_led = statusBits;

      updateShiftRegisters();


      stateTimer = millis();

      currentState = STATE_SHOW_TARGET;

      break;


    case STATE_SHOW_TARGET:

      data_led =
        statusBits |
        getTargetBit(currentTarget);

      updateShiftRegisters();


      if (millis() - stateTimer >= 1000) {

        data_led = statusBits;

        updateShiftRegisters();

        currentState = STATE_WAIT_INPUT;
      }

      break;

    case STATE_WAIT_INPUT:

      if (digitalRead(PIN_BUTTON) == LOW) {

        chargeStartTime = millis();

        currentState = STATE_CHARGING;
      }

      break;

    case STATE_CHARGING:
    {
      unsigned long chargeDuration =
        millis() - chargeStartTime;


      int newPowerLevel =
        map(
          chargeDuration,
          0,
          1500,
          0,
          5
        );


      if (newPowerLevel > 5)
        newPowerLevel = 5;

      if (newPowerLevel > powerLevel) {

        beep(20);
      }


      powerLevel = newPowerLevel;


      updatePowerLeds(powerLevel);

      if (
        powerLevel >= 5 ||
        digitalRead(PIN_BUTTON) == HIGH
      ) {

        currentState = STATE_SHOOT;
      }

      break;
    }

    case STATE_SHOOT:

      playSoundEffect(2);

      delay(200);

      currentState = STATE_EVAL;

      break;

    case STATE_EVAL:
    {
      bool isSuccess =
        (powerLevel == currentTarget);


      byte newStatusBit = 0;


      if (isSuccess) {

        score++;


        if (roundNumber == 1)
          newStatusBit = BIT_SCORE_1;

        else if (roundNumber == 2)
          newStatusBit = BIT_SCORE_2;

        else if (roundNumber == 3)
          newStatusBit = BIT_SCORE_3;


        playSoundEffect(3);

      }

      else {

        misses++;

        newStatusBit = BIT_MISS;

        playSoundEffect(4);
      }


      statusBits |= newStatusBit;


      byte targetBit =
        getTargetBit(currentTarget);


      for (int i = 0; i < 3; i++) {

        data_led =
          targetBit |
          statusBits |
          newStatusBit;

        updateShiftRegisters();

        delay(150);


        data_led = statusBits;

        updateShiftRegisters();

        delay(150);
      }


      data_led = statusBits;

      updateShiftRegisters();


      stateTimer = millis();

      currentState = STATE_GAME_OVER;

      break;
    }


    case STATE_GAME_OVER:

      if (millis() - stateTimer > 1500) {

        if (score >= 3) {

          currentState = STATE_WIN;

        }

        else if (
          misses >= 2 ||
          roundNumber >= 3
        ) {

          currentState = STATE_LOSE;

        }

        else {

          roundNumber++;

          currentState = STATE_NEXT_ROUND;
        }
      }

      break;


    case STATE_WIN:

      data_led = 0x00;

      updatePowerLeds(0);

      updateShiftRegisters();

      delay(500);


      for (int i = 0; i < 3; i++) {

        data_led = BIT_SCORE_1;

        updateShiftRegisters();

        delay(250);


        data_led = BIT_SCORE_2;

        updateShiftRegisters();

        delay(250);


        data_led = BIT_SCORE_3;

        updateShiftRegisters();

        delay(250);


        data_led = 0x00;

        updateShiftRegisters();

        delay(250);
      }

      playSoundEffect(5);


      currentState = STATE_RESET;

      break;

    case STATE_LOSE:

      data_led = 0x00;

      updatePowerLeds(0);

      updateShiftRegisters();

      delay(500);


      for (int i = 0; i < 5; i++) {

        data_led = BIT_MISS;

        updateShiftRegisters();

        delay(250);


        data_led = 0x00;

        updateShiftRegisters();

        delay(250);
      }

      playSoundEffect(6);


      currentState = STATE_RESET;

      break;

    case STATE_RESET:

      data_led = 0x00;

      updatePowerLeds(0);

      delay(2000);


      currentState = STATE_START;

      break;
  }
}