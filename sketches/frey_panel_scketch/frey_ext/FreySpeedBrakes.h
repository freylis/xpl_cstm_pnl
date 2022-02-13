#ifndef FreySpeedBrakes_h
#define FreySpeedBrakes_h

#include <GyverTM1637.h>
#include <EncButton.h>

class FreySpeedBrakes {
  public:
    FreySpeedBrakes(unsigned char pinEncCLK, unsigned char pinEncDIO, unsigned char pinDisplayCLK, unsigned char pinDisplayDIO);
    /* call it once in setup func */
    void prepare();
    /* call it each loop and relax */
    void lap();
    void readFullState(String fullState);

  private:
    unsigned char _brakesValue;
    unsigned char _pinEncCLK;
    unsigned char _pinEncDIO;
    unsigned char _pinDisplayCLK;
    unsigned char _pinDisplayDIO;
    EncButton _enc;
    GyverTM1637 _display;
};

#endif
