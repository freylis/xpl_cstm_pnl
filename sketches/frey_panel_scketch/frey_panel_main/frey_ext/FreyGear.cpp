#include "FreyGear.h";
#include "Arduino.h";
#include "FreyCommand.h";

/*
STATES:
    0 - up
    2 - down
STATEPOSITION:
    0 - up
    1 - middle/up
    2 - down
Usage:
>> FreyGear gear(1, 2, 3, 4);
>> void setup() {
>>   gear.prepare();
>> };
>> void loop() {
>>    // read buttons and send event to PC
>>    gear.lap();
>>    // fullState = '[frey-cmd-x] FULLSTATE 1...'
>>    // read real gear state
>>    gear.readFullState(fullState);
>>  };
*/
#define FREY_GEAR_STATE_UP 1;
#define FREY_GEAR_STATE_MIDDLE 2;
#define FREY_GEAR_STATE_DOWN 3;


FreyGear::FreyGear(unsigned char pinUp, unsigned char pinDown, unsigned char pinLightUp, unsigned char pinLightDown) {
    unsigned char _statePosition = 0;
    unsigned char _realState = 0;
    unsigned char _pinUp = pinUp;
    unsigned char _pinDown = pinDown;
    unsigned char _pinLightUp = pinLightUp;
    unsigned char _pinLightDown = pinLightDown;
};

FreyGear::prepare() {
    pinMode(_pinUp, INPUT_PULLUP);
    pinMode(_pinDown, INPUT_PULLUP);
    pinMode(_pinLightUp, OUTPUT);
    pinMode(_pinLightDown, OUTPUT);
};

FreyGear::_gearUp() {
    sendPanelCommand('GEARUP');
};
FreyGear::_gearDown() {
    sendPanelCommand('GEARDOWN');
};

void FreyGear::lap() {
    /* проверяем какое состояние у переключателя шасси сейчас */

    // если если включено и последний статус не включен, то отправить эвент
    if (!digitalRead(_pinDown) && _statePosition != FREY_GEAR_STATE_DOWN) {
        _statePosition = FREY_GEAR_STATE_DOWN;
        _gearDown();
    };
    // если положение верхнее, нужно отправлять эвент только если ещё не отправляли
    else if (!digitalRead(_pinUp)) {
        if (_statePosition != FREY_GEAR_STATE_UP) {
            _gearUp();
        };
        _statePosition = FREY_GEAR_STATE_UP;
    // ничего не включено. для 737 это среднее положение, для других, по сути, верхнее
    } else {
        if (_statePosition == FREY_GEAR_STATE_UP) {_gearDown();}
        else {_gearUp();}
        _statePosition = FREY_GEAR_STATE_MIDDLE;
    };
};

FreyGear::readFullState(string fullState) {
    /*
    Прочитать реальное состояние из xplane
    */
    unsigned char currentState = fullState[23];
    if (currentState == '1') {
        digitalWrite(_pinLightDown, HIGH);
        digitalWrite(_pinLightUp, LOW);
    } else {
        digitalWrite(_pinLightDown, LOW);
        digitalWrite(_pinLightUp, HIGH);
    };
};
