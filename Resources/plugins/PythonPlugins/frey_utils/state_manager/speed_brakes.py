from . import base


class StateSpeedBrakes(base.StateFloat):
    ref = 'sim/cockpit2/controls/speedbrake_ratio'
    mapping = {
        '000': 0,
        '-50': 1,
        '098': 2,
        '100': 3,
    }

    def get_cmd_part(self):
        """
        Получить подстроку с состоянием для команды
        """
        val = str(self.get_percent_value())[:self.repr_length]
        val = val.rjust(self.repr_length, '0')
        if val in self.mapping:
            return self.mapping[val]
        return '500'
