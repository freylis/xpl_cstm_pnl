import xp

from .. import utils


class State:
    ref = NotImplemented
    repr_length = 3

    @property
    def data_ref(self):
        return xp.findDataRef(self.ref)

    def get_value(self):
        return None

    def get_percent_value(self):
        return None

    def get_cmd_part(self):
        """
        Получить подстроку с состоянием для команды
        """
        val = str(self.get_percent_value())[:self.repr_length]
        val = val.rjust(self.repr_length, '0')
        return val[:self.repr_length]


class StateInt(State):

    def get_value(self):
        try:
            val = xp.getDatai(self.data_ref)
        except Exception as exc:
            utils.echo(f'Catn get valueI for dataref {self.ref} / {self.data_ref}: {exc}')
            return '0'
        return val

    def get_percent_value(self):
        return min(self.get_value() * 100, 100)


class StateSmallInt(StateInt):

    def get_cmd_part(self):
        return str(self.get_value())


class StateFloat(State):

    def get_value(self):
        val = xp.getDataf(self.data_ref)
        return val

    def get_percent_value(self):
        return int(self.get_value() * 100)
