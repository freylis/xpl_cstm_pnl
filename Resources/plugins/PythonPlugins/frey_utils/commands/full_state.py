import xp

from . import base
from .. import utils
from .. import state_manager


class CommandFullState(base.Command):
    """
    Полное состояние самолета обрабатывается данным объектом
    """
    positions = [
        state_manager.StateGear,
        state_manager.StateFlaps,
        state_manager.StateSpeedBrakes,
        state_manager.StateVerticalTrim,
    ]
    cmd = 'frey/command/send_full_state'

    @classmethod
    def register(cls):
        xp.createCommand(cls.cmd, description='Send full state from xplane to freylis arduino panel')

    def get_command(self):
        """
        Команда отправки состояния всего что можно в панельку
        Соглашение по формату следующее:
            [frey-cmd-x] FULLSTATE 001__002__003__004
            001 - состояние шасси
                000 - убрано
                001 - выпущено
            002 - состояние закрылков в %
                001 - выпущены на 1%
                060 - выпущены на 60%
                100 - выпущены на 100%
            003 - состояние воздушных тормозов. УТОЧНИТЬ
            004 - состояние тримера руля высоты. УТОЧНИТЬ
        """
        parts = []
        for state_cls in self.positions:
            state = state_cls()
            parts.append(state.get_cmd_part())
        result_str = '__'.join(parts)
        return f'[frey-cmd-x] FULLSTATE {result_str}'
