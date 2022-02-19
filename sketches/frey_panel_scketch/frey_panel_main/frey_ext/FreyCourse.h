#ifndef FreyCourse_h
#define FreyCourse_h

#include "Arduino.h"
#include "FreyCommand.h"

#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned int pinCourseEncoderCLK = 4;
const unsigned int pinCourseEncoderDIO = 5;
const unsigned int pinCourseDisplayCLK = 2;
const unsigned int pinCourseDisplayDIO = 3;


EncButton<EB_TICK, pinCourseEncoderCLK, pinCourseEncoderDIO> courseEncoder;
GyverTM1637 courseDisplay(pinCourseDisplayCLK, pinCourseDisplayDIO);


class FreyCourse {

  private:
    void _setCourseDisplay(unsigned int courseValue) {
      if (courseValue > 360 || courseValue < 0) {
        sendLog("Invalid course: " + (String)courseValue);
        vTrimDisplay.displayByte(0, _empty);
        vTrimDisplay.displayByte(1, _E);
        vTrimDisplay.displayByte(2, _r);
        vTrimDisplay.displayByte(3, _r);
        return;
      };

      String sCourseValue = (String)courseValue;
      if (courseValue == 0) {
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.displayByte(2, _empty);
          vTrimDisplay.display(3, 0);

      } else if (courseValue < 10) {
          // 1..9
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.displayByte(2, _empty);
          vTrimDisplay.display(3, ((String)sCourseValue[0]).toInt());

      } else if (courseValue < 100) {
          // 10..99
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.displayByte(1, _empty);
          vTrimDisplay.display(2, ((String)sCourseValue[0]).toInt());
          vTrimDisplay.display(3, ((String)sCourseValue[1]).toInt());

      } else {
          // 100..360
          vTrimDisplay.displayByte(0, _empty);
          vTrimDisplay.display(1, ((String)sCourseValue[0]).toInt());
          vTrimDisplay.display(2, ((String)sCourseValue[1]).toInt());
          vTrimDisplay.display(3, ((String)sCourseValue[2]).toInt());
      };
    };

  public:
    FreyCourse() {};
    int courseValue = 0;

    /* call it once in setup func */
    void prepare() {
        pinMode(pinCourseEncoderCLK, INPUT_PULLUP);
        pinMode(pinCourseEncoderDIO, INPUT_PULLUP);
        pinMode(pinCourseDisplayCLK, OUTPUT);
        pinMode(pinCourseDisplayDIO, OUTPUT);
    };

    /* call it each loop and relax */
    void lap() {
       courseEncoder.tick();
       if (courseEncoder.turn()) {
           if (courseEncoder.left()) {
             if (courseEncoder.fast()) {
                courseValue -= 10;
             } else {
                courseValue -= 1;
             }
           } else if (courseEncoder.right()) {
             if (courseEncoder.fast()) {
                courseValue += 10;
             } else {
                courseValue += 1;
             };
           };
           if (courseValue < 0) {courseValue = 360;}
           else if (courseValue > 360) {courseValue = 0;};
           hardSendState();
           _setCourseDisplay(courseValue);
       };
    };

    void readFullState(String fullState) {
      String sCourse = fullState.substring(39, 42);
      sendLog("Got course: " + sCourse);
      int iCourseValue = sCourse.toInt();
      courseValue = iCourseValue;
      _setCourseDisplay(iCourseValue);
    };

    void hardSendState() {
        sendPanelCommand("SET_COURSE_" + (String)courseValue);
    };
};

#endif
