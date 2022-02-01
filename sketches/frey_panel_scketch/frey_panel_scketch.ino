/* 
 * Welcome to freylis plane panel sketch
 */

#define EB_HALFSTEP_ENC true;

#include <GyverTM1637.h>
#include <EncButton.h>


/*
 Define all pin constants
*/
// gear
const unsigned int pinGearToggleButtonIn = 53;
const unsigned int pinGearToggleButtonOut = 52;
// speed brakes
const unsigned int pinSpeedBrakesEncoderCLK = 51;
const unsigned int pinSpeedBrakesEncoderDIO = 50;
const unsigned int pinSpeedBrakesDisplayCLK = 49;
const unsigned int pinSpeedBrakesDisplayDIO = 48;
// flaps
const unsigned int pinFlapsEncoderCLK = 47;
const unsigned int pinFlapsEncoderDIO = 46;
const unsigned int pinFlapsDisplayCLK = 45;
const unsigned int pinFlapsDisplayDIO = 44;
// vertical trim
const unsigned int pinVertTrimEncoderCLK = 43;
const unsigned int pinVertTrimEncoderDIO = 42;
const unsigned int pinVertTrimDisplayCLK = 41;
const unsigned int pinVertTrimDisplayDIO = 40;


GyverTM1637 displaySpeedBrakes(pinSpeedBrakesDisplayCLK, pinSpeedBrakesDisplayDIO);
GyverTM1637 displayFlaps(pinFlapsDisplayCLK, pinFlapsDisplayDIO);
GyverTM1637 displayVertTrim(pinVertTrimDisplayCLK, pinVertTrimDisplayDIO);
EncButton<EB_TICK, pinSpeedBrakesEncoderCLK, pinSpeedBrakesEncoderDIO> encoderSpeedBrakes;
EncButton<EB_TICK, pinFlapsEncoderCLK, pinFlapsEncoderDIO> encoderFlaps;
EncButton<EB_TICK, pinVertTrimEncoderCLK, pinVertTrimEncoderDIO> encoderVertTrim;


bool lastGearState = false;
bool isGearUp = false;


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
  /*
  int rbrake = fullState.substring(28, 31).toInt();
  byte sbrake[4];
  sendLog("Handle rspeedbrabkes " + sbrake);
  switch (rbrake) {
    case 0:
      sbrake = {_D, _o, _u, _n};
      break;
    case -50:
      sbrake = {_A, _r, _d};
      break;
    case 98:
      sbrake = {_F, _l, _D, _t};
      break;
    case 100:
      sbrake[0] = _U;
      sbrake[1] = _P;
      break;
    default:
      sbrake[0] = _E;
      sbrake[1] = _r;
      sbrake[2] = _r;
      // {_E, _r, _r};
      break;
  };
  */
  //sendLog("Draw sbrake " + (String)sbrake);
}


void setFlaps(int dFlaps) {
  flasDegrees = dFlaps;
  sendLog("Draw " + (String)dFlaps + " in flaps display");
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
  pinMode(pinGearToggleButtonIn, INPUT_PULLUP);
  pinMode(pinGearToggleButtonOut, OUTPUT);
  // speed brakes
  pinMode(pinSpeedBrakesEncoderCLK, INPUT);
  pinMode(pinSpeedBrakesEncoderDIO, INPUT);
  pinMode(pinSpeedBrakesDisplayCLK, OUTPUT);
  pinMode(pinSpeedBrakesDisplayDIO, OUTPUT);
  // flaps
  pinMode(pinFlapsEncoderCLK, INPUT);
  pinMode(pinFlapsEncoderDIO, INPUT);
  pinMode(pinFlapsDisplayCLK, OUTPUT);
  pinMode(pinFlapsDisplayDIO, OUTPUT);
  // vertical trim
  pinMode(pinVertTrimEncoderCLK, INPUT);
  pinMode(pinVertTrimEncoderDIO, INPUT);
  pinMode(pinVertTrimDisplayCLK, OUTPUT);
  pinMode(pinVertTrimDisplayDIO, OUTPUT);

}


void setup() { 
  Serial.begin (9600);
  preparePins();
  /*
   * display + encoder
   pinMode (pin_DT, INPUT);
   pinMode (pin_CLK, INPUT);
   Last = digitalRead(pin_DT); // Считываем значение на выходе DT и запоминаем его
   disp.clear();
   disp.brightness(5);
   disp.displayInt(pos_encoder);
   */

}

void loop() {
  // listen to external command
  // handleExternalCommand();

  int lVal = digitalRead(toggleButtonLeft);
  int rVal = digitalRead(toggleButtonRight);
  Serial.println("l: " + (String)lVal + " | " + (String)rVal + " :r");

  /*
   * encoder + display
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
    */

}
