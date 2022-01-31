/* 
 * Welcome to freylis plane panel sketch
 */

#define EB_HALFSTEP_ENC true;

#include <GyverTM1637.h>
#include <EncButton.h>



bool lastGearState = false;
bool isGearUp = false;
unsigned int flapsDegrees = 0;

const int PIN_GEAR_DISPLAY = 11;
const int PIN_GEAR_BUTTON = 10;


void sendLog(String message) {
  Serial.println("[frey-log-a] " + message);
}


void sendCommand(String command) {
  sendLog("do command " + command);
}


void sendSelfCommand(String command) {
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


void handleGearFromFullState(String fullState) {
  String gearString = fullState.substring(23, 26);
  if (gearString == "100") {
    commandGearDown();
  } else if (gearString == "000") {
    commandGearUp();
  } else {
    sendLog("Unknown gear value=" + gearString + " from fullstate");
  }
}


void handleSpeedBrakeFromFullState(String fullState) {
  String sbrake = fullState.substring(28, 29);
  sendLog("Handle speedbrabkes " + sbrake);
}


void setFlaps(int dFlaps) {
  flapsDegrees = dFlaps;
  sendLog("Write " + (String)dFlaps + " in flaps display");
}


void handleFlapsFromFullState(String fullState) {
  unsigned int rFlaps = fullState.substring(33, 36).toInt();
  unsigned int dFlaps = 0;
  /* zibo737 only */
  switch (rFlaps) {
    case 0:
      dFlaps = 0;
      break;
    case 12:
      dFlaps = 1;
      break;
    case 25:
      dFlaps = 2;
      break;
    case 37:
      dFlaps = 5;
      break;
    case 50:
      dFlaps = 10;
      break;
    case 62:
      dFlaps = 15;
      break;
    case 75:
      dFlaps = 25;
      break;
    case 87:
      dFlaps = 30;
      break;
    case 40:
      dFlaps = 100;
      break;
    default:
      dFlaps = -1;
      break;
  };
  if (dFlaps > 0) {
    setFlaps(dFlaps);
  } else {
    sendLog("Unknown rflaps value" + (String)rFlaps);
  };
}


void setFullState(String fullState) {
  /* Соглашение следующее
   * [frey-cmd-x] STATEFULL aaa__bbb__ccc__ddd
   * 01234567890123456789012345678901234567890123456789
   * gear - 23..26
   * flaps - 28..31
   * speedbrake - 33..36
   * vert_trim - 38..41
   */
   handleGearFromFullState(fullState);
   handleFlapsFromFullState(fullState);
   handleSpeedBrakeFromFullState(fullState);
}


void executeCommand(String command) {
  command.trim();
  /* 
   * попытка обработать внешнюю команду STATEFULL 
   */
  if (command.startsWith("[frey-cmd-x] STATEFULL ")) {
      setFullState(command);
  }
  else if (command == "[frey-cmd-x] GEARUP") {
      commandGearUp();
  }
  else if (command == "[frey-cmd-x] GEARDOWN") {
      commandGearDown();
  } else {
      sendLog("Unknown command '" + command + "'");
  }
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


void startUp() {
  /*
   * Комплекс мероприятий по подготовке при старте
   */
  preparePins();
}


void preparePins() {
  /*
   * Подготовим пины к работе
   */
   
}

int pos_encoder = 1; // Первоначальная позиция енкодера равна 0
int Last;            // Тут всегда будет лежать предыдущие значение положения енкодера 
int DT;
boolean left;
int CLK = 12;
int DIO = 13;

const int pin_CLK = 7;
const int pin_DT = 8;


GyverTM1637 disp(CLK, DIO);
EncButton<EB_TICK, pin_CLK, pin_DT> enc;


void setup() { 
   pinMode (pin_DT, INPUT);
   pinMode (pin_CLK, INPUT);
   Last = digitalRead(pin_DT); // Считываем значение на выходе DT и запоминаем его
   Serial.begin (9600);
   disp.clear();
   disp.brightness(5);
   disp.displayInt(pos_encoder);
}

void loop() {
  // listen to external command
  handleExternalCommand();

  enc.tick();

  if (enc.turn()) {
    if (enc.left()) {
      if (enc.fast()) {
        Serial.println("isfast left");
        pos_encoder -= 10;
      } else {
        pos_encoder -= 1;
      }
    } else if (enc.right()) {
      if (enc.fast()) {
        pos_encoder += 10;
        Serial.println("isfast right");
      } else {
        pos_encoder += 1;
      }
    };
    if (pos_encoder > 360) {
      pos_encoder = 1;
    }
    else if (pos_encoder < 1) {
      pos_encoder = 360;
    };
    disp.displayInt(pos_encoder);
    Serial.println("write " + (String)pos_encoder);
  };

} 
