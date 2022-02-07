import re
import time
import serial


pipe = serial.Serial('COM8', baudrate=9600, timeout=0)


class Race:

    def __init__(self):
        self.l_num = 0

    def lap(self):
        self.l_num += 1

    def log(self, msg):
        print(msg)


class ArduinoToXplane(Race):
    """
    Читаем из COM. Если находим команду по нужной маске, пишем в
    """
    command_regexp = re.compile(r'\[frey-cmd-a] (\w+)', flags=re.I)
    command_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_a.log'
    log_regexp = re.compile(r'\[frey-log-a] (\w+)', flags=re.I)
    log_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey.log'

    def log(self, msg):
        return super().log('[a->x] ' + msg)

    def lap(self):
        super().lap()

        cmd = pipe.readline().decode('utf-8').strip()
        if cmd:
            self.log(f'got cmd: {cmd!r}')
            self.handle_message(cmd)

    def handle_message(self, msg):
        cmd = self.command_regexp.search(msg)
        if cmd is not None:
            self.handle_command(msg)
            return

        log_msg = self.log_regexp.search(msg)
        if log_msg is not None:
            self.handle_log(msg)
            return

        # если перехватили чужое сообщение, переотправим его обратно
        xp_match = XplaneToArduino.command_regexp.search(msg)
        if xp_match:
            self.log(f'Send message {msg} again to ard')
            pipe.write(msg.encode('utf-8'))
            return

        self.log(f'Непонятное сообщение __{msg}__')

    def handle_command(self, cmd):
        with open(self.command_path, 'a', encoding='utf-8') as f:
            f.write(cmd + '\n')

    def handle_log(self, msg):
        with open(self.log_path, 'a', encoding='utf-8') as f:
            f.write(msg + '\n')


class XplaneToArduino(Race):
    command_regexp = re.compile(r'^\[frey-cmd-x] ([\w ]+)\s*$', flags=re.I)
    command_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_x.log'
    log_regexp = re.compile(r'\[frey-log-x] (\w+)', flags=re.I)
    log_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey.log'

    def __init__(self):
        super().__init__()
        self.future_commands = []

    def lap(self):
        super().lap()
        with open(self.command_path, 'r', encoding='utf-8') as f:
            msg_lines = f.readlines()

        for line in msg_lines:
            if not line.strip():
                continue

            self.log(f'handling {line!r}')
            self.handle_message(line)

        with open(self.command_path, 'w', encoding='utf-8') as f:
            f.truncate()

    def handle_message(self, msg):
        cmd = self.command_regexp.match(msg)
        if cmd is not None:
            self.handle_command(msg)
            return
        if msg.startswith('[frey-cmd-x]'):
            self.future_commands.append(msg)
        self.log(f'Непонятное сообщение: {msg!r}')

    def handle_command(self, cmd):
        self.log(f'send command {cmd} to arduino')
        pipe.write((cmd + '\n').encode('utf-8'))
        time.sleep(0.11)

    def handle_log(self, msg):
        with open(self.log_path, 'a') as f:
            f.write(msg)

    def log(self, msg):
        return super().log('[x->a] ' + msg)


ar = ArduinoToXplane()
xp = XplaneToArduino()


while True:
    xp.lap()
    time.sleep(0.5)
    ar.lap()
