#include <FreyCommand.h>
#include <GyverTM1637.h>
#include <EncButton.h>


FreyFlaps::FreyFlaps(
        unsigned char pinEncCLK, unsigned char pinEncDIO,
          unsigned char pinDisplayCLK, unsigned char pinDisplayDIO) {
    unsigned char _pinEncCLK = pinEncCLK;
    unsigned char _pinEncDIO = pinEncDIO;
    unsigned char _pinDisplayCLK = pinEncCLK;
    unsigned char _pinDisplayDIO = pinEncDIO;
};

FreyFlaps::prepare() {
    _flapsVal = 0;
    pinMode(_pinEncCLK, INPUT_PULLUP);
    pinMode(_pinEncDIO, INPUT_PULLUP);
    pinMode(_pinDisplayCLK, OUTPUT);
    pinMode(_pinDisplayDIO, OUTPUT);
    GyverTM1637 _enc(_pinEncCLK, _pinEncDIO);
    EncButton _display(_pinDisplayCLK, _pinDisplayDIO);
};

FreyFlaps::lap() {
    _enc.tick();
    if (_enc.left()) {
        sendPanelCommand("FLAPS_DOWN_ONE");
    } else if (_enc.right()) {
        sendPanelCommand("FLAPS_UP_ONE");
    };
}

FreyFlaps::readFullState(string fullState) {
  // read flaps value from fullString

  unsigned int dFlaps = 10
  switch (dFlaps) {
    case 0:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.displayByte(2, _empty);
      _display.display(3, 0);
      break;
    case 1:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.displayByte(2, _empty);
      _display.display(3, 1);
      break;
    case 2:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.displayByte(2, _empty);
      _display.display(3, 2);
      break;
    case 5:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.displayByte(2, _empty);
      _display.display(3, 5);
      break;
    case 10:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.display(2, 1);
      _display.display(3, 0);
      break;
    case 15:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.display(2, 1);
      _display.display(3, 5);
      break;
    case 25:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.display(2, 2);
      _display.display(3, 5);
      break;
    case 30:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.display(2, 3);
      _display.display(3, 0);
      break;
    case 40:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.display(2, 4);
      _display.display(3, 0);
      break;
    default:
      _diplay.displayInt(dFlaps);
      _display.displayByte(0, _dash);
      break;
  }

};
