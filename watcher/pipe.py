import serial


class Pipe(serial.Serial):

    def __init__(self, show_time=False):
        super().__init__('COM8', baudrate=9600, timeout=0)
