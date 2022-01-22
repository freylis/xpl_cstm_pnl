from . import commands


CALLBACK_STRUCTURE = {

    # flaps
    'frey_utils.CallbackFlapsUpDown': (
        commands.CommandFullState.cmd,
    ),

    # gear up/down
    # b737 works
    'frey_utils.callbacks.CallbackGearUpDown': (
        commands.CommandFullState.cmd,
    ),

    # engine (does not work for joystick)
    'frey_utils.CallbackThrottleChange': (
        commands.CommandFullState.cmd,
    ),

    # speed brakes
    'frey_utils.CallbackSpeedBrakesUpDown': (
        commands.CommandFullState.cmd,
    ),

    # vertical trimmer
    'frey_utils.CallbackVerticalTrimUpDown': (
        commands.CommandFullState.cmd,
    ),
}
