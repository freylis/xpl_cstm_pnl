#ifndef FreyCourse_h
#define FreyCourse_h

#include "Arduino.h"
#include "FreyCommand.h"
#include "FreyEncoder.h"

#include <GyverTM1637.h>

const unsigned int pinCourseEncoderCLK = 28;
const unsigned int pinCourseEncoderDIO = 29;
const unsigned int pinCourseDisplayCLK = 44;
const unsigned int pinCourseDisplayDIO = 45;


GyverTM1637 courseDisplay(pinCourseDisplayCLK, pinCourseDisplayDIO);


class FreyCourse {

  private:
    int courseValue = 0;
    bool valueChanged;
    int lastSended;
    void _setCourseDisplay(unsigned int courseValue) {
      if (courseValue > 360 || courseValue < 0) {
        sendLog("Invalid course: " + (String)courseValue);
        courseDisplay.displayByte(0, _empty);
        courseDisplay.displayByte(1, _E);
        courseDisplay.displayByte(2, _r);
        courseDisplay.displayByte(3, _r);
        return;
      };

      String sCourseValue = (String)courseValue;
      if (courseValue == 0) {
          courseDisplay.displayByte(0, _empty);
          courseDisplay.displayByte(1, _empty);
          courseDisplay.displayByte(2, _empty);
          courseDisplay.display(3, 0);

      } else if (courseValue < 10) {
          // 1..9
          courseDisplay.displayByte(0, _empty);
          courseDisplay.displayByte(1, _empty);
          courseDisplay.displayByte(2, _empty);
          courseDisplay.display(3, ((String)sCourseValue[0]).toInt());

      } else if (courseValue < 100) {
          // 10..99
          courseDisplay.displayByte(0, _empty);
          courseDisplay.displayByte(1, _empty);
          courseDisplay.display(2, ((String)sCourseValue[0]).toInt());
          courseDisplay.display(3, ((String)sCourseValue[1]).toInt());

      } else {
          // 100..360
          courseDisplay.displayByte(0, _empty);
          courseDisplay.display(1, ((String)sCourseValue[0]).toInt());
          courseDisplay.display(2, ((String)sCourseValue[1]).toInt());
          courseDisplay.display(3, ((String)sCourseValue[2]).toInt());
      };
    };

  public:
    FreyCourse() {};

    /* call it once in setup func */
    void prepare() {
        valueChanged = false;
        pinMode(pinCourseEncoderCLK, INPUT_PULLUP);
        pinMode(pinCourseEncoderDIO, INPUT_PULLUP);
        pinMode(pinCourseDisplayCLK, OUTPUT);
        pinMode(pinCourseDisplayDIO, OUTPUT);

        courseDisplay.clear();
        courseDisplay.brightness(5);
        delay(100);

        courseDisplay.displayByte(0, _b);
        courseDisplay.display(1, 7);
        courseDisplay.display(2, 3);
        courseDisplay.display(3, 7);
        delay(100);

        _setCourseDisplay(courseValue);
    };

    /* call it each loop and relax */
    void lap() {
        encoders[2].tick();
        if (encoders[2].turn()) {
          if (encoders[2].left()) {
            if (encoders[2].fast()) {
                courseValue -= 10;
            } else {
                courseValue -= 1;
            }
          } else {
            if (encoders[2].fast()) {
                courseValue += 10;
            } else {
                courseValue += 1;
            };
          };
          if (courseValue < 0) {courseValue = 360;}
          else if (courseValue > 360) {courseValue = 0;};
          _setCourseDisplay(courseValue);
          hardSendState();
        };

    };

    void readFullState(String fullState) {};

    void hardSendState() {
        sendPanelCommand("SET_COURSE_" + (String)courseValue);
    };
};

#endif
