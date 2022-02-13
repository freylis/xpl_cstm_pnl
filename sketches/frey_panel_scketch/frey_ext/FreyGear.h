#ifndef FreyGear_h
#define FreyGear_h

class FreyGear {
  public:
    /* initialize with pins
            pinUp - pin button up
            pinDown - pin button down
            pinLightUp - send HIGH volt to here if gearUp
            pinLightDown - send HIGH volt to here if gearDown
     */
    FreyGear(unsigned char pinUp, unsigned char pinDown, unsigned char pinLightUp, unsigned char pinLightDown);
    void prepare();
    /* call it each loop and relax */
    void lap();
    void readFullState(String fullState);


  private:
    /* send command if position changed */
    void _gearUp();
    void _gearDown();
    /* service variable for remember last state */
    unsigned char _statePosition;
    unsigned char _realState;
    unsigned char _pinUp;
    unsigned char _pinDown;
    unsigned char _pinLightUp;
    unsigned char _pinLightDown;
};

#endif
