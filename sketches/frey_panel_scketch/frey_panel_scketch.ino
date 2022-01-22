// welcome to freylis plane panel sketch


bool lastGearState = false;
bool isGearUp = false;
const int PIN_GEAR_DISPLAY = 11;
const int PIN_GEAR_BUTTON = 10;



void sendLog(String message) {
  Serial.println("[frey-log-a] " + message);
}



void sendCommand(String command) {
  sendLog("do command " + command);
}


void sendSelfCommand(String command) {
  sendLog("do self command " + command);
  Serial.println("[frey-cmd-a] " + command);
}


void commandGearUp() {
  sendCommand("GEARUP");
  isGearUp = true;
  digitalWrite(PIN_GEAR_DISPLAY, LOW);
}

void commandGearDown() {
  sendCommand("GEARDOWN");
  isGearUp = false;
  digitalWrite(PIN_GEAR_DISPLAY, HIGH);
}

void handleExternalCommand() {
  /*
   * Если из пришла команда извне - выполним ее
   * Но только если это команда из "хорошего" списка
   */
  String message = "";
  if (Serial.available() > 0) {
    message = Serial.readString();
  };
  if (message == "") {
    return;
  }
  message.trim();
  executeCommand(message);
}


void calcGear() {
  /* 
   *  Выясняем, меняется ли состояние шасси
   */
   boolean gearState = !digitalRead(PIN_GEAR_BUTTON);
   if (gearState && !lastGearState) {
      lastGearState = true;
      if (isGearUp) {
        commandGearDown();
        sendSelfCommand("GEARDOWN");
      } else {
        commandGearUp();
        sendSelfCommand("GEARUP");
      }
   }
   if (!gearState && lastGearState) {
      lastGearState = false;
   }
}


void listenMyself() {
  /*
   * Слушать себя самого, выяснить какие команды нужно послать
   */
  calcGear();
}


void executeCommand(String command) {
  command.trim();
  if (command == "[frey-cmd-x] GEARUP") {
      commandGearUp();
  }
  else if (command == "[frey-cmd-x] GEARDOWN") {
      commandGearDown();
  } else {
      sendLog("Unknown command '" + command + "'");
  }
}


void startUp() {
  /*
   * Комплекс мероприятий по подготовке при старте
   */
  preparePins();
  loadCurrentValues();
}


void preparePins() {
  /*
   * Подготовим пины к работе
   */
   pinMode(PIN_GEAR_DISPLAY, OUTPUT);
   pinMode(PIN_GEAR_BUTTON, INPUT_PULLUP);
}


void loadCurrentValues() {
}


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  startUp();
}

void loop() {
  // проверим наличие внешней команды. Если есть и она хорошая - выполним ее
  handleExternalCommand();
  // calc myself commands
  listenMyself();
}
