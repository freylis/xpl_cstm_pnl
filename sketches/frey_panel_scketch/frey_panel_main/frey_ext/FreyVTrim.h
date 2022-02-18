#ifndef FreyVTrim_h
#define FreyVTrim_h

#include "Arduino.h"
#include "FreyCommand.h"

#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned int pinVertTrimEncoderCLK = 4;
const unsigned int pinVertTrimEncoderDIO = 5;
const unsigned int pinVertTrimDisplayCLK = 2;
const unsigned int pinVertTrimDisplayDIO = 3;


GyverTM1637 vTrimDisplay(pinVertTrimDisplayCLK, pinVertTrimDisplayDIO);
EncButton<EB_TICK, pinVertTrimEncoderCLK, pinVertTrimEncoderDIO> vTrimEncoder;


class FreyVTrim {

  public:
    FreyVTrim() {};

    /* call it once in setup func */
    void prepare() {
        pinMode(pinVertTrimEncoderCLK, INPUT_PULLUP);
        pinMode(pinVertTrimEncoderDIO, INPUT_PULLUP);
        pinMode(pinVertTrimDisplayCLK, OUTPUT);
        pinMode(pinVertTrimDisplayDIO, OUTPUT);
    };

    /* call it each loop and relax */
    void lap() {
       vTrimEncoder.tick();
       if (vTrimEncoder.left()) {
            sendPanelCommand("VTRIM_DOWN_ONE");
       } else if (vTrimEncoder.right()) {
            sendPanelCommand("VTRIM_UP_ONE");
       };
    };

    void readFullState(String fullState) {
      String sVertTrim = fullState.substring(34, 37);
      sendLog("Got vtrim: " + sVertTrim);
      int iVertTrim = sVertTrim.toInt();
      if (iVertTrim < -9) {
          // -63 .. -10
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _dash);
          vTrimDisplay.display(2, ((String)sVertTrim[1]).toInt());
          vTrimDisplay.display(3, ((String)sVertTrim[2]).toInt());

       } else if (iVertTrim < 0) {
          // -9..-1
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.displayByte(2, _dash);
          vTrimDisplay.display(3, ((String)sVertTrim[1]).toInt());

       } else if (iVertTrim == 0) {
          // 0
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.displayByte(2, _empty);
          vTrimDisplay.display(3, 0);

       } else if (iVertTrim < 10) {
         // 1..9
         vTrimDisplay.displayByte(0, _empty);
         vTrimDisplay.displayByte(1, _empty);
         vTrimDisplay.displayByte(2, _empty);
         vTrimDisplay.display(3, ((String)sVertTrim[0]).toInt());

       } else if (iVertTrim < 100) {
         // 11..99
         vTrimDisplay.displayByte(0, _empty);
         vTrimDisplay.displayByte(1, _empty);
         vTrimDisplay.display(2, ((String)sVertTrim[0]).toInt());
         vTrimDisplay.display(3, ((String)sVertTrim[1]).toInt());

       } else {
         // 100..999
         vTrimDisplay.displayByte(0, _empty);
         vTrimDisplay.display(1, ((String)sVertTrim[0]).toInt());
         vTrimDisplay.display(2, ((String)sVertTrim[1]).toInt());
         vTrimDisplay.display(3, ((String)sVertTrim[2]).toInt());
       }
    };

    void hardSendState() {};

};

#endif
