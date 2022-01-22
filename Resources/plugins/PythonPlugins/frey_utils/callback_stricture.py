CALLBACK_STRUCTURE = {

    # flaps
    'frey_utils.CallbackFlapsUpDown': (
        'sim/flight_controls/flaps_up',
        'sim/flight_controls/flaps_down',
    ),

    # gear up/down
    # b737 works
    'frey_utils.callbacks.CallbackGearUpDown': (
        'sim/flight_controls/landing_gear_up',
        'sim/flight_controls/landing_gear_down',
        'sim/flight_controls/landing_gear_toggle',
    ),

    # engine (does not work for joystick)
    'frey_utils.CallbackThrottleChange': (
        'sim/engines/throttle_up',
        'sim/engines/throttle_down',
        'sim/engines/prop_down',
        'sim/engines/prop_up',
        'sim/engines/idle_hi_lo_toggle',
        # subscribe to dataref
        'sim/cockpit2/engine/actuators/throttle_ratio',

    ),

    # speed brakes
    'frey_utils.CallbackSpeedBrakesUpDown': (
        'sim/flight_controls/speed_brakes_down_one',
        'sim/flight_controls/speed_brakes_up_one',
        'sim/flight_controls/speed_brakes_down_all',
        'sim/flight_controls/speed_brakes_up_all',
        'sim/flight_controls/speed_brakes_toggle',
    ),

    # vertical trimmer
    'frey_utils.CallbackVerticalTrimUpDown': (
        'sim/flight_controls/pitch_trimA_up',
        'sim/flight_controls/pitch_trimA_down',
        'sim/flight_controls/pitch_trimB_up',
        'sim/flight_controls/pitch_trimB_down',
        'sim/flight_controls/pitch_trim_up',
        'sim/flight_controls/pitch_trim_down',
        'sim/flight_controls/pitch_trim_up_mech',
        'sim/flight_controls/pitch_trim_down_mech',
    )
}
