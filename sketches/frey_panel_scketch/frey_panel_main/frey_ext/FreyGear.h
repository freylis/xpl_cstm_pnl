#ifndef FreyGear_h
#define FreyGear_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <EncButton.h>

const unsigned char pinGearUp = A12;
const unsigned char pinGearDown = A13;
const unsigned char pinGearLightUp = A14;
const unsigned char pinGearLightDown = A15;

const unsigned char FREY_GEAR_STATE_UP = 1;
const unsigned char FREY_GEAR_STATE_MIDDLE = 2;
const unsigned char FREY_GEAR_STATE_DOWN = 3;


EncButton<EB_TICK, pinGearUp> gearButtonUp;
EncButton<EB_TICK, pinGearDown> gearButtonDown;


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
        //sendLog((String)!digitalRead(pinGearDown) + " | " + (String)!digitalRead(pinGearUp) + " | " + (String)_statePosition);

        bool is_up = !digitalRead(pinGearUp);
        bool is_down = !digitalRead(pinGearDown);

        gearButtonUp.tick();
        gearButtonDown.tick();
        // если если включено и последний статус не включен, то отправить эвент
        //if (is_down && _statePosition != FREY_GEAR_STATE_DOWN) {
        if (gearButtonUp.press()) {
            _statePosition = FREY_GEAR_STATE_DOWN;
            _gearDown();
        }
        // если положение среднее, значит ничего не включено. отправить эвент единожды
        //else if (!is_down && !is_up && _statePosition != FREY_GEAR_STATE_MIDDLE) {
        else if (gearButtonUp.release() || gearButtonDown.release()) {
            if (_statePosition == FREY_GEAR_STATE_DOWN) {
                _gearUp();
            } else {
                _gearDown();
            };
            _statePosition = FREY_GEAR_STATE_MIDDLE;
        }
        // шасси вверх
        // else if (is_up && _statePosition != FREY_GEAR_STATE_UP) {
        else if (gearButtonUp.press()) {
            _gearUp();
            _statePosition = FREY_GEAR_STATE_UP;
        };
    };

    void readFullState(String fullState) {
        int currentState = fullState.substring(23, 24).toInt();
        sendLog("Fullstate gear=" + (String)currentState);
        if (currentState == 1) {
            analogWrite(pinGearLightDown, KD2_LIGHT);
            digitalWrite(pinGearLightUp, LOW);
        } else {
            digitalWrite(pinGearLightDown, LOW);
            analogWrite(pinGearLightUp, KD2_LIGHT);
        };
    };

    void hardSendState() {
        if (_statePosition == FREY_GEAR_STATE_DOWN || _statePosition == FREY_GEAR_STATE_MIDDLE) {_gearDown();}
         else {_gearUp();};
    };

};

#endif
