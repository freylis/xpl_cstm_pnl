#ifndef FreyVTrim_h
#define FreyVTrim_h

#include "Arduino.h"
#include "FreyCommand.h"

#include <GyverTM1637.h>

const unsigned int pinVertTrimEncoderCLK = 40;
const unsigned int pinVertTrimEncoderDIO = 41;
const unsigned int pinVertTrimDisplayCLK = A13;
const unsigned int pinVertTrimDisplayDIO = A12;

GyverTM1637 vTrimDisplay(pinVertTrimDisplayCLK, pinVertTrimDisplayDIO);


class FreyVTrim {

  public:
    FreyVTrim() {};

    /* call it once in setup func */
    void prepare() {
        pinMode(pinVertTrimEncoderCLK, INPUT_PULLUP);
        pinMode(pinVertTrimEncoderDIO, INPUT_PULLUP);
        pinMode(pinVertTrimDisplayCLK, OUTPUT);
        pinMode(pinVertTrimDisplayDIO, OUTPUT);
        vTrimDisplay.clear();
        vTrimDisplay.brightness(5);
        delay(100);

        vTrimDisplay.displayByte(0, _b);
        vTrimDisplay.display(1, 7);
        vTrimDisplay.display(2, 3);
        vTrimDisplay.display(3, 7);

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
      String sVertTrim = fullState.substring(34, 38);
      sendLog("Got vtrim: " + sVertTrim);
      int iVertTrim = sVertTrim.toInt();
      if (iVertTrim < -9) {
          // -63 .. -10
          sendLog("Draw vtrim -0xx");
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _dash);
          vTrimDisplay.display(2, ((String)sVertTrim.substring(2, 3)).toInt());
          vTrimDisplay.display(3, ((String)sVertTrim.substring(3, 4)).toInt());

       } else if (iVertTrim < 0) {
          // -9..-1
          sendLog("Draw vtrim -00x");
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.displayByte(2, _dash);
          vTrimDisplay.display(3, ((String)sVertTrim.substring(3, 4)).toInt());

       } else if (iVertTrim == 0) {
          // 0
          sendLog("Draw vtrim 0");
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.displayByte(2, _empty);
          vTrimDisplay.display(3, 0);

       } else if (iVertTrim < 10) {
         // 1..9
          sendLog("Draw vtrim x");
         vTrimDisplay.displayByte(0, _empty);
         vTrimDisplay.displayByte(1, _empty);
         vTrimDisplay.displayByte(2, _empty);
         vTrimDisplay.display(3, ((String)sVertTrim.substring(3, 4)).toInt());

       } else if (iVertTrim < 100) {
         // 11..99
          sendLog("Draw vtrim xx");
         vTrimDisplay.displayByte(0, _empty);
         vTrimDisplay.displayByte(1, _empty);
         vTrimDisplay.display(2, ((String)sVertTrim.substring(2, 3)).toInt());
         vTrimDisplay.display(3, ((String)sVertTrim.substring(3, 4)).toInt());

       } else {
         // 100..999
          sendLog("Draw vtrim xxx");
         vTrimDisplay.displayByte(0, _empty);
         vTrimDisplay.display(1, ((String)sVertTrim.substring(1, 2)).toInt());
         vTrimDisplay.display(2, ((String)sVertTrim.substring(2, 3)).toInt());
         vTrimDisplay.display(3, ((String)sVertTrim.substring(3, 4)).toInt());
       }
    };

    void hardSendState() {};

};

#endif
