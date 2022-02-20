import xp

from . import base
from .. import state_manager
from .. import utils


class CommandFullState(base.Command):
    """
    Полное состояние самолета обрабатывается данным объектом
    """
    positions = (
        state_manager.StateGear,    # ok
        state_manager.StateFlaps,   # ok
        state_manager.StateSpeedBrakes, # ok
        state_manager.StateVerticalTrim,    # ok
        state_manager.StateCourse,
        state_manager.StateAPSpeed,  # ok
        state_manager.StateVNAV,    # ok
        state_manager.StateLNAV,    # ok
        state_manager.StateVORLOCK, # ok
        state_manager.StateAPPROACH,    # ok
        state_manager.StateCMD, # ok
        state_manager.StateCWS, # ok
        state_manager.StateHeading, # ok
        state_manager.StateAltitude,    # ok
        state_manager.StateLevelChanged,    # ok
    )
    cmd = 'frey/command/send_full_state'

    @classmethod
    def register(cls):
        xp.createCommand(cls.cmd, description='Send full state from x-plane to freylis arduino panel')

    def get_command(self):
        """
        Команда отправки состояния всего что можно в панельку
        Соглашение по формату https://github.com/freylis/xpl_cstm_pnl#events
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
