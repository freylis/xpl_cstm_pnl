#include <FreyCommand.h>
#include <Arduino.h>


void sendPanelCommand(string cmd) {
	Serial.println('[frey-cmd-a] ' + cmd);
};
