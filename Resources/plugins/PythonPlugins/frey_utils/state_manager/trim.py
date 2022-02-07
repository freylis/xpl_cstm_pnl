import xp

from .. import utils
from . import base


class StateVerticalTrim(base.StateFloat):
    repr_length = 4
    ref = 'sim/flightmodel2/controls/stabilizer_deflection_degrees'
