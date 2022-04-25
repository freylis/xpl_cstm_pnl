import time
import serial
import logging

logger = logging.getLogger('frey-panel-watcher')


class Pipe(serial.Serial):
    PING_CMD = '[frey-ping]\n'.encode('utf-8')
    EXPECT_ANSWER = '[frey-pong] PANEL_1'

    def __init__(self, port, timeout=0):
        super().__init__(port, baudrate=9600, timeout=timeout, write_timeout=timeout)

    @classmethod
    def init(cls):
        lap_n = 1
        while True:
            for porn_n in range(1, 50):
                port_name = f'COM{porn_n}'

                try:
                    pipe = cls(port_name, timeout=1)
                except Exception:
                    continue

                try:
                    pipe.write(cls.PING_CMD)
                except Exception:
                    continue

                time.sleep(1)
                answers = pipe.readlines()
                if not answers:
                    continue

                for r_answer in answers:
                    answr = r_answer.decode('utf-8').strip()
                    if answr == cls.EXPECT_ANSWER:
                        pipe.close()
                        real_pipe = cls(port_name, timeout=0)
                        return real_pipe

            logger.warning(f'Cant find COM port with {lap_n} try')
            lap_n += 1
