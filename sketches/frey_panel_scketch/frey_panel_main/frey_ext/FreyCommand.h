#ifndef FreyCommand_h
#define FreyCommand_h


void sendPanelCommand(String cmd) {
    Serial.println("[frey-cmd-a] " + cmd);
};


#endif
