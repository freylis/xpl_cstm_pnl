import re
import time
import serial
import logging
from logging import handlers

from pipe import Pipe
import watcher_utils


# prepare logger
logging.addLevelName(logging.WARNING - 1, 'XCMD')
logging.addLevelName(logging.WARNING - 2, 'ACMD')

logger = logging.getLogger('frey-panel-watcher')
logger.setLevel(logging.INFO)
setattr(logger, 'XCMD', logging.WARNING - 1)
setattr(logger, 'ACMD', logging.WARNING - 2)

formatter = logging.Formatter('[%(asctime)s] %(levelname)s: %(message)s')

f_handler = handlers.TimedRotatingFileHandler(
    'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey_watcher.log',
    when='D', encoding='utf-8'
)
f_handler.setFormatter(formatter)
c_handler = logging.StreamHandler()
c_handler.setFormatter(formatter)

logger.addHandler(f_handler)
logger.addHandler(c_handler)


try:
    pipe = Pipe.init()
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

    def _clean_messages(self, messages):
        cleaned_messages = []
        last_message = None
        for msg in messages:
            if last_message != msg:
                cleaned_messages.append(msg)
                last_message = msg
        return cleaned_messages


class ArduinoToXplane(Race):
    """
    Читаем из COM. Если находим команду по нужной маске, пишем в
    """
    command_regexp = re.compile(r'\[frey-cmd-a] (\w+)', flags=re.I)
    command_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey_cmd_a.log'
    log_regexp = re.compile(r'\[frey-log-a] (\w+)', flags=re.I)
    log_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey.log'
    CMD_LEVEL = logger.ACMD

    def log(self, msg):
        return super().log('[a->x] ' + msg)

    @watcher_utils.timer
    def lap(self):
        super().lap()

        try:
            commands = pipe.readlines()
        except Exception as exc:
            logger.error(f'Error read message: {exc}')
            return
        else:
            if not commands:
                return

        for cmd in commands:
            cmd = cmd.decode('utf-8').strip()
            if cmd:
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
            pipe.write(msg.encode('utf-8'))
            return

        if msg == pipe.EXPECT_ANSWER:
            self.log('Ping-pong answer found')
            return

        self.log(f'Непонятное сообщение __{msg}__')

    def handle_command(self, cmd):
        with open(self.command_path, 'a', encoding='utf-8') as f:
            self.log_cmd(cmd)
            f.write(cmd + '\n')

    def handle_log(self, msg):
        with open(self.log_path, 'a', encoding='utf-8') as f:
            self.log(msg)
            f.write(msg + '\n')


class XplaneToArduino(Race):
    command_regexp = re.compile(r'^\[frey-cmd-x] ([\w ]+)\s*$', flags=re.I)
    command_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey_cmd_x.log'
    log_regexp = re.compile(r'\[frey-log-x] (\w+)', flags=re.I)
    log_path = 'D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey.log'
    CMD_LEVEL = logger.XCMD

    def __init__(self):
        super().__init__()
        self.future_commands = []

    @watcher_utils.timer
    def lap(self):
        super().lap()
        try:
            with open(self.command_path, 'r', encoding='utf-8') as f:
                msg_lines = f.readlines()
        except FileNotFoundError:
            with open(self.command_path, 'w', encoding='utf-8') as f:
                msg_lines = []

        if not msg_lines:
            return

        msg_lines = self._clean_messages(messages=msg_lines)
        for line in msg_lines:
            if not line.strip():
                continue

            self.handle_message(line)

        with open(self.command_path, 'w', encoding='utf-8') as f:
            f.truncate()

        # если остались непрочитанные сообщения - их нужно отправить опять в arduino
        if self.future_commands:
            for msg in self.future_commands:
                try:
                    pipe.write(msg.encode('utf-8'))
                except Exception as exc:
                    logger.error(f'Error send message: {exc}')
                    return
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
        bdata = (cmd + '\n').encode('utf-8')
        try:
            pipe.write(bdata)
        except Exception as exc:
            logger.error(f'Error send message {bdata!r}: {exc}')
        self.log_cmd(cmd)
        time.sleep(0.11)

    def handle_log(self, msg):
        with open(self.log_path, 'a') as f:
            f.write(msg)

    def log(self, msg):
        return super().log('[x->a] ' + msg)


if __name__ == '__main__':
    ar = ArduinoToXplane()
    xp = XplaneToArduino()

    logger.info('ready to show?')
    time.sleep(5)
    logger.info('lets fly')

    while True:
        xp.lap()
        time.sleep(0.5)
        ar.lap()
