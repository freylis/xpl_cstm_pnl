import xp

from . import base
from .. import state_manager
from .. import utils


class CommandFullState(base.Command):
    """
    Полное состояние самолета обрабатывается данным объектом
    """
    positions = (
        state_manager.StateGear,
        state_manager.StateFlaps,
        state_manager.StateSpeedBrakes,
        state_manager.StateVerticalTrim,
        state_manager.StateCourse,
        state_manager.StateAPSpeed,
        state_manager.StateVNAV,
        state_manager.StateLNAV,
        state_manager.StateVORLOCK,
        state_manager.StateAPPROACH,
        state_manager.StateHeading,
        state_manager.StateAltitude,
    )
    cmd = 'frey/command/send_full_state'

    @classmethod
    def register(cls):
        xp.createCommand(cls.cmd, description='Send full state from x-plane to freylis arduino panel')

    def get_command(self):
        """
        Команда отправки состояния всего что можно в панельку
        Соглашение по формату следующее:
            [frey-cmd-x] FULLSTATE 001__002__003__004
            1 - состояние шасси
                0 - убрано
                1 - выпущено
            002 - состояние закрылков в %
                001 - выпущены на 1%
                060 - выпущены на 60%
                100 - выпущены на 100%
            003 - состояние воздушных тормозов.
                0 - убраны
                1 - armed
                2 - flight detent
                3 - UP
            004 - состояние тримера руля высоты. УТОЧНИТЬ
        """
        parts = []
        for state_cls in self.positions:
            state = state_cls()

            state_value = state.get_cmd_part()
            if not isinstance(state_value, str):
                utils.echo(f'Unknown result format ({state_value}, {type(state_value)} for {state}')
                state_value = '0'
            parts.append(state_value)
        result_str = '__'.join(parts)
        return f'[frey-cmd-x] STATEFULL {result_str}'
