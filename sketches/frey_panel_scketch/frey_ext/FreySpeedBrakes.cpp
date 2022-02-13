#include <FreyCommand.h>
#include <GyverTM1637.h>
#include <EncButton.h>


FreySpeedBrakes::FreySpeedBrakes(
        unsigned char pinEncCLK, unsigned char pinEncDIO,
          unsigned char pinDisplayCLK, unsigned char pinDisplayDIO) {
    unsigned char _pinEncCLK = pinEncCLK;
    unsigned char _pinEncDIO = pinEncDIO;
    unsigned char _pinDisplayCLK = pinEncCLK;
    unsigned char _pinDisplayDIO = pinEncDIO;
};

FreySpeedBrakes::prepare() {
    _flapsVal = 0;
    pinMode(_pinEncCLK, INPUT_PULLUP);
    pinMode(_pinEncDIO, INPUT_PULLUP);
    pinMode(_pinDisplayCLK, OUTPUT);
    pinMode(_pinDisplayDIO, OUTPUT);
    GyverTM1637 _enc(_pinEncCLK, _pinEncDIO);
    EncButton _display(_pinDisplayCLK, _pinDisplayDIO);
};

FreySpeedBrakes::lap() {
    _enc.tick();
    if (_enc.left()) {
        sendPanelCommand("SPEEDBRAKES_DOWN_ONE");
    } else if (_enc.right()) {
        sendPanelCommand("SPEEDBRAKES_UP_ONE");
    };
}

FreySpeedBrakes::readFullState(string fullState) {
  // read flaps value from fullString

  int rbrake = fullState.substring(32, 32).toInt();
  switch (rbrake) {
    case 0:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _O);
      _display.displayByte(2, _F);
      _display.displayByte(3, _F);
      break;
    case -50:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _A);
      _display.displayByte(2, _r);
      _display.displayByte(3, _d);
      break;
    case 98:
      _display.displayByte(0, _F);
      _display.displayByte(1, _l);
      _display.displayByte(2, _D);
      _display.displayByte(3, _t);
      break;
    case 100:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _empty);
      _display.displayByte(2, _U);
      _display.displayByte(3, _P);
      break;
    default:
      _display.displayByte(0, _empty);
      _display.displayByte(1, _E);
      _display.displayByte(2, _r);
      _display.displayByte(3, _r);
      break;
  };

};
