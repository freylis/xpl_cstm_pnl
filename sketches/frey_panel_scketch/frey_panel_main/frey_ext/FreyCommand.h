#ifndef FreyCommand_h
#define FreyCommand_h

const int SEND_COMMAND_EVERY_MS = 300;
const int KD2_LIGHT = 255;


void sendPanelCommand(String cmd) {
    Serial.println("[frey-cmd-a] " + cmd);
};


void sendLog(String msg) {
    #ifdef DEBUG
    Serial.println("[frey-log-a]:" + msg);
    #endif
};


void sendPong() {
    Serial.println("[frey-pong] PANEL_1");
};

#endif
