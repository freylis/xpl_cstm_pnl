import serial
from .. import utils


class CommandSender:

    def __init__(self, command):
        self.command = command

    def send(self):
        utils.echo(f'Send command {self.command}')


class COMCommandSender(CommandSender):

    def __init__(self, command):
        super().__init__(command)
        self.pipe = serial.Serial('COM8', baudrate=9600, timeout=0.1)

    def send(self):
        self.pipe.write(self.command.get_command())
