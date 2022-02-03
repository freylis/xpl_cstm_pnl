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
const unsigned int pinGearToggleButtonIn = 2;
const unsigned int pinGearToggleButtonOut = 3;
// speed brakes
const unsigned int pinSpeedBrakesEncoderCLK = 4;
const unsigned int pinSpeedBrakesEncoderDIO = 5;
const unsigned int pinSpeedBrakesDisplayCLK = 6;
const unsigned int pinSpeedBrakesDisplayDIO = 7;
// flaps
const unsigned int pinFlapsEncoderCLK = 8;
const unsigned int pinFlapsEncoderDIO = 9;
const unsigned int pinFlapsDisplayCLK = 10;
const unsigned int pinFlapsDisplayDIO = 11;
// vertical trim
const unsigned int pinVertTrimEncoderCLK = 12;
const unsigned int pinVertTrimEncoderDIO = 13;
const unsigned int pinVertTrimDisplayCLK = 12;
const unsigned int pinVertTrimDisplayDIO = 13;


GyverTM1637 displaySpeedBrakes(pinSpeedBrakesDisplayCLK, pinSpeedBrakesDisplayDIO);
GyverTM1637 displayFlaps(pinFlapsDisplayCLK, pinFlapsDisplayDIO);
GyverTM1637 displayVertTrim(pinVertTrimDisplayCLK, pinVertTrimDisplayDIO);
EncButton<EB_TICK, pinSpeedBrakesEncoderCLK, pinSpeedBrakesEncoderDIO> encoderSpeedBrakes;
EncButton<EB_TICK, pinFlapsEncoderCLK, pinFlapsEncoderDIO> encoderFlaps;
EncButton<EB_TICK, pinVertTrimEncoderCLK, pinVertTrimEncoderDIO> encoderVertTrim;


bool lastGearState = false;
bool isGearUp = false;
int flapsDegrees = 0;
int speedBrakeState = 0;
int verticalTrimValue = 0;


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
  digitalWrite(pinGearToggleButtonOut, LOW);
}

void commandGearDown() {
  sendCommand("GEARDOWN");
  isGearUp = false;
  digitalWrite(pinGearToggleButtonOut, HIGH);
}

void speedBrakeDown() {
  displaySpeedBrakes.displayByte(0, _D);
  displaySpeedBrakes.displayByte(1, _o);
  displaySpeedBrakes.displayByte(2, _u);
  displaySpeedBrakes.displayByte(3, _n);
  speedBrakeState = 0;
  sendLog("Draw sbrake down");
}

void speedBrakeArmed() {
  displaySpeedBrakes.displayByte(0, _empty);
  displaySpeedBrakes.displayByte(1, _A);
  displaySpeedBrakes.displayByte(2, _r);
  displaySpeedBrakes.displayByte(3, _d);

  speedBrakeState = 1;
  sendLog("Draw sbrake armed");
}
void speedBrakeFlightDetent() {
  displaySpeedBrakes.displayByte(0, _F);
  displaySpeedBrakes.displayByte(1, _l);
  displaySpeedBrakes.displayByte(2, _D);
  displaySpeedBrakes.displayByte(3, _t);

  speedBrakeState = 2;
  sendLog("Draw sbrake flight detent");
}
void speedBrakeUp() {
  displaySpeedBrakes.displayByte(0, _empty);
  displaySpeedBrakes.displayByte(1, _empty);
  displaySpeedBrakes.displayByte(2, _U);
  displaySpeedBrakes.displayByte(3, _P);

  speedBrakeState = 3;
  sendLog("Draw sbrake flight up");
}
void speedBrakeUnknown() {
  displaySpeedBrakes.displayByte(0, _empty);
  displaySpeedBrakes.displayByte(1, _E);
  displaySpeedBrakes.displayByte(2, _r);
  displaySpeedBrakes.displayByte(3, _r);

  speedBrakeState = 0;
  sendLog("Draw sbrake error");
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
  int rbrake = fullState.substring(28, 31).toInt();
  sendLog("Handle rspeedbrabkes " + (String)rbrake);
  switch (rbrake) {
    case 0:
      speedBrakeDown();
      break;
    case -50:
      speedBrakeArmed();
      break;
    case 98:
      speedBrakeFlightDetent();
      break;
    case 100:
      speedBrakeUp();
      break;
    default:
      speedBrakeUnknown();
      break;
  };
}


void setFlaps(int dFlaps) {
  flapsDegrees = dFlaps;
  switch (dFlaps) {
    case 0:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.displayByte(2, _empty);
      displayFlaps.display(3, 0);
      break;
    case 1:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.displayByte(2, _empty);
      displayFlaps.display(3, 1);
      break;
    case 2:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.displayByte(2, _empty);
      displayFlaps.display(3, 2);
      break;
    case 5:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.displayByte(2, _empty);
      displayFlaps.display(3, 5);
      break;
    case 10:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.display(2, 1);
      displayFlaps.display(3, 0);
      break;
    case 15:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.display(2, 1);
      displayFlaps.display(3, 5);
      break;
    case 25:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.display(2, 2);
      displayFlaps.display(3, 5);
      break;
    case 30:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.display(2, 3);
      displayFlaps.display(3, 0);
      break;
    case 40:
      displayFlaps.displayByte(0, _empty);
      displayFlaps.displayByte(1, _empty);
      displayFlaps.display(2, 4);
      displayFlaps.display(3, 0);
      break;
    default:
      displayFlaps.displayInt(dFlaps);
      displayFlaps.displayByte(0, _dash);
      break;

  }
  
  
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
    case 100:
      dFlaps = 40;
      break;
    default:
      dFlaps = - rFlaps;
      break;
  };
  setFlaps(dFlaps);
}


void setVerticalTrimValue(int iVertTrim) {
   verticalTrimValue = iVertTrim;
   String sVertTrim = (String)iVertTrim;
   sendLog("Draw vtrim " + sVertTrim);
   if (iVertTrim < -9) {
      // -63 .. -10
      displayVertTrim.displayByte(3, _empty);
      displayVertTrim.displayByte(2, _dash);
      displayVertTrim.display(1, (int)sVertTrim[1]);
      displayVertTrim.display(0, (int)sVertTrim[2]);
      sendLog("D1:" + sVertTrim + "/" + sVertTrim[1] + "/" + sVertTrim[2]);

   } else if (iVertTrim < 0) {
      // -9..-1
      displayVertTrim.displayByte(0, _empty);
      displayVertTrim.displayByte(1, _empty);
      displayVertTrim.displayByte(2, _dash);
      displayVertTrim.display(3, (int)sVertTrim[1]);
      sendLog("D2");

   } else if (iVertTrim == 0) {
      // 0
      displayVertTrim.displayByte(0, _empty);
      displayVertTrim.displayByte(1, _empty);
      displayVertTrim.displayByte(2, _empty);
      displayVertTrim.display(3, 0);
      sendLog("D3");

   } else if (iVertTrim < 10) {
     // 1..9
     displayVertTrim.displayByte(0, _empty);
     displayVertTrim.displayByte(1, _empty);
     displayVertTrim.displayByte(2, _empty);
     displayVertTrim.display(3, iVertTrim);
     sendLog("D4");

   } else if (iVertTrim < 100) {
     // 11..99
     displayVertTrim.displayByte(0, _empty);
     displayVertTrim.displayByte(1, _empty);
     displayVertTrim.display(2, (int)sVertTrim[0]);
     displayVertTrim.display(3, (int)sVertTrim[1]);
     sendLog("D5");

   } else {
     // 100..999
     displayVertTrim.displayByte(0, _empty);
     displayVertTrim.display(1, (int)sVertTrim[0]);
     displayVertTrim.display(2, (int)sVertTrim[1]);
     displayVertTrim.display(3, (int)sVertTrim[2]);
     sendLog("D6");
   }
}


void handleVerticalTrimFromFullState(String fullState) {
  /*
   * [frey-cmd-x] STATEFULL aaa__bbb__ccc__ddd
   * vertical trim is ddd: 38-41 position
   */
   unsigned int iVertTrim = fullState.substring(38, 41).toInt();
   // из-за дисплея я вынужден рисовать каждый знак отдельно
   sendLog("Set vtrim " + (String)iVertTrim);
   setVerticalTrimValue(iVertTrim);
}


void setFullState(String fullState) {
  /* Соглашение следующее
   * [frey-cmd-x] STATEFULL aaa__bbb__ccc__ddd
   * 01234567890123456789012345678901234567890123456789
   * gear - 23..26
   * speedbrake - 28..31
   * flaps - 33..36
   * vert_trim - 38..41
   */
   handleGearFromFullState(fullState);
   handleFlapsFromFullState(fullState);
   handleSpeedBrakeFromFullState(fullState);
   handleVerticalTrimFromFullState(fullState);
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
   boolean gearState = !digitalRead(pinGearToggleButtonIn);
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


void afterStartCheckList() {
  sendLog("After start checklist");
  displaySpeedBrakes.clear();
  displaySpeedBrakes.brightness(5);
  displaySpeedBrakes.displayByte(0, _L);
  displaySpeedBrakes.displayByte(1, _O);
  displaySpeedBrakes.displayByte(2, _A);
  displaySpeedBrakes.displayByte(3, _D);

  displayFlaps.clear();
  displayFlaps.brightness(5);
  displayFlaps.displayByte(0, _L);
  displayFlaps.displayByte(1, _O);
  displayFlaps.displayByte(2, _A);
  displayFlaps.displayByte(3, _D);

  displayVertTrim.clear();
  displayVertTrim.brightness(5);
  displayVertTrim.displayByte(0, _L);
  displayVertTrim.displayByte(1, _O);
  displayVertTrim.displayByte(2, _A);
  displayVertTrim.displayByte(3, _D);

  delay(2000);

  executeCommand("[frey-cmd-x] STATEFULL 100__-50__037__-65");
  sendLog("After start checklist complete");
}


void setup() { 
  Serial.begin (9600);
  preparePins();
  afterStartCheckList();
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
  // listen panel
  listenMyself();

  // listen to external command
  handleExternalCommand();

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
