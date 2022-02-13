import re
import time
import serial
import logging
from logging import handlers


# prepare logger
logging.addLevelName(logging.WARNING - 1, 'XCMD')
logging.addLevelName(logging.WARNING - 2, 'ACMD')

logger = logging.getLogger('frey-panel-watcher')
logger.setLevel(logging.INFO)
setattr(logger, 'XCMD', logging.WARNING - 1)
setattr(logger, 'ACMD', logging.WARNING - 2)

formatter = logging.Formatter('[%(asctime)s] %(levelname)s: %(message)s')

f_handler = handlers.TimedRotatingFileHandler(
    'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey.log',
    when='D', encoding='utf-8'
)
f_handler.setFormatter(formatter)
c_handler = logging.StreamHandler()
c_handler.setFormatter(formatter)

logger.addHandler(f_handler)
logger.addHandler(c_handler)


try:
    pipe = serial.Serial('COM8', baudrate=9600, timeout=0)
except serial.SerialException as exc:
    logger.error(str(exc))
    logger.error('Exit with status 0')
    import sys
    sys.exit(0)


class Race:
    CMD_LEVEL = NotImplemented

    def __init__(self):
        self.l_num = 0

    def lap(self):
        self.l_num += 1

    def log(self, msg):
        logger.info(msg)

    def log_cmd(self, msg):
        logger.log(self.CMD_LEVEL, msg)


class ArduinoToXplane(Race):
    """
    Читаем из COM. Если находим команду по нужной маске, пишем в
    """
    command_regexp = re.compile(r'\[frey-cmd-a] (\w+)', flags=re.I)
    command_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey_cmd_a.log'
    log_regexp = re.compile(r'\[frey-log-a] (\w+)', flags=re.I)
    log_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey.log'
    CMD_LEVEL = logger.ACMD

    def log(self, msg):
        return super().log('[a->x] ' + msg)

    def lap(self):
        super().lap()

        cmd = pipe.readline().decode('utf-8').strip()
        if cmd:
            self.log(cmd)
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
    CMD_LEVEL = logger.XCMD

    def __init__(self):
        super().__init__()
        self.future_commands = []

    def lap(self):
        super().lap()
        with open(self.command_path, 'r', encoding='utf-8') as f:
            msg_lines = f.readlines()
        if not msg_lines:
            return

        for line in msg_lines:
            if not line.strip():
                continue

            self.log(f'handling {line!r}')
            self.handle_message(line)

        with open(self.command_path, 'w', encoding='utf-8') as f:
            f.truncate()

        # если остались непрочитанные сообщения - их нужно отправить опять в arduino
        if self.future_commands:
            for msg in self.future_commands:
                pipe.write(msg.encode('utf-8'))
        self.future_commands = []

    def handle_message(self, msg):
        cmd = self.command_regexp.match(msg)
        if cmd is not None:
            self.handle_command(msg)
            return
        if msg.startswith('[frey-cmd-x]'):
            self.future_commands.append(msg)
            return
        self.log(f'Непонятное сообщение: {msg!r}')

    def handle_command(self, cmd):
        self.log(f'send command {cmd} to arduino')
        pipe.write((cmd + '\n').encode('utf-8'))
        self.log_cmd(cmd)
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
