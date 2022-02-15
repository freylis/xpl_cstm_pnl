#ifndef FreyGear_h
#define FreyGear_h

#include "Arduino.h"
#include "FreyCommand.h"

const unsigned char pinGearUp = A15;
const unsigned char pinGearDown = A14;
const unsigned char pinGearLightUp = A13;
const unsigned char pinGearLightDown = A12;

const unsigned char FREY_GEAR_STATE_UP = 1;
const unsigned char FREY_GEAR_STATE_MIDDLE = 2;
const unsigned char FREY_GEAR_STATE_DOWN = 3;


class FreyGear {

  private:
    /* send command if position changed */
    void _gearUp() {
        sendPanelCommand("GEARUP");
    };
    void _gearDown() {
        sendPanelCommand("GEARDOWN");
    };
    /* service variable for remember last state */
    unsigned char _statePosition;
    unsigned char _realState;

  public:
    /* initialize with pins
            pinUp - pin button up
            pinDown - pin button down
            pinGearLightUp - send HIGH volt to here if gearUp
            pinGearLightDown - send HIGH volt to here if gearDown
     */
    FreyGear() {
        unsigned char _statePosition = 0;
        unsigned char _realState = 0;
    };

    void prepare() {
        pinMode(pinGearDown, INPUT_PULLUP);
        pinMode(pinGearUp, INPUT_PULLUP);
        pinMode(pinGearLightUp, OUTPUT);
        pinMode(pinGearLightDown, OUTPUT);
    };
    /* call it each loop and relax */
    void lap() {
        /* проверяем какое состояние у переключателя шасси сейчас */

        // если если включено и последний статус не включен, то отправить эвент
        if (!digitalRead(pinGearDown) && _statePosition != FREY_GEAR_STATE_DOWN) {
            _statePosition = FREY_GEAR_STATE_DOWN;
            _gearDown();
        }
        // если положение верхнее, нужно отправлять эвент только если ещё не отправляли
        else if (!digitalRead(pinGearUp)) {
            if (_statePosition != FREY_GEAR_STATE_UP) {
                _gearUp();
            };
            _statePosition = FREY_GEAR_STATE_UP;
        }
        // ничего не включено. для 737 это среднее положение, для других, по сути, верхнее
        else {
            if (_statePosition == FREY_GEAR_STATE_UP) {_gearDown();}
            else {_gearUp();}
            _statePosition = FREY_GEAR_STATE_MIDDLE;
        };
    };

    void readFullState(String fullState) {
        unsigned char currentState = fullState[23];
        if (currentState == '1') {
            digitalWrite(pinGearLightDown, HIGH);
            digitalWrite(pinGearLightUp, LOW);
        } else {
            digitalWrite(pinGearLightDown, LOW);
            digitalWrite(pinGearLightUp, HIGH);
        };
    };

};

#endif
