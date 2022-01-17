// welcome to freylis plane panel sketch

#define boolean isGearDown true;

const int pinGearDisplay = 11;


void sendLog(String message) {
  Serial.println("[log] " + message);
}



void sendCommand(String command) {
  sendLog("Send command '" + command + "'");
  Serial.println("[frey-cmd] " + command);
}



void commandGearUp() {
  sendLog("Execute command GEARUP");
  sendCommand("GEARUP");
  digitalWrite(pinGearDisplay, HIGH);
}

void commandGearDown() {
  sendLog("Execute command GEARDOWN");
  sendCommand("GEARDOWN");
  digitalWrite(pinGearDisplay, 0);
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
  executeCommand(message);
}


void executeCommand(String command) {
  sendLog("Try to execute command '" + command + "'");

  if (command == "[frey-cmd] GEARUP") {
      commandGearUp();
  }
  else if (command == "[frey-cmd] GEARDOWN") {
      commandGearDown();
  } else {
      sendLog("Unknown command '" + command + "'");
  }
}


void startUp() {
  /*
   * Комплекс мероприятий по подготовке при старте
   */
  sendLog("Freylis panel starts ...");
  preparePins();
  loadCurrentValues();
  sendLog("Panel load .. done");
}


void preparePins() {
  /*
   * Подготовим пины к работе
   */
   sendLog("Prepare pins");
   pinMode(pinGearDisplay, OUTPUT);
   sendLog("Prepare pins .. done");
}

void loadCurrentValues() {
  sendLog("Start to load default values");
}

void setup() {
  Serial.begin(9600);
  startUp();
}

void loop() {
  // проверим наличие внешней команды. Если есть и она хорошая - выполним ее
  handleExternalCommand();
}
