#### X-PLANE custom panel interface

This is my implementation interface between
my arduino-based custom panel and xplane.
It works only with zibo737.

This plugin [xppython3](https://xppython3.readthedocs.io/en/stable/index.html) based

### How to install
0. Install XPPython plugin https://xppython3.readthedocs.io/
1. Copy `./Resources/plugins/PythonPlugins/frey_utils` and `./Resources/plugins/PythonPlugins/PI_xplCstmPnlFreylis.py`
to `X-Plane/Resources/plugins/PythonPlugins/.` directory
2. Write `sketches/frey_panel_scketch/frey_panel_main/frey_panel_main.ino` to Arduino MEGA
3. Run `watcher/watcher.py` with `python3`

### Events
##### Events from xplane to panel
```
FULLSTATE message format:
    [frey-cmd-x] FULLSTATE 1__100__2__101__102__3__4__5__6__7__8__9__1__2__3
        1 - [23] - GEAR
        100 - [26..28] - FLAPS
        2 - [31] - SPEED BRAKES
        101 - [34..37] - VTRIM
        102 - [39..42] - COURSE
        3 - [44] - AP SPEED enabed/disabled
        4 - [47] - VNAV
        5 - [50] - LNAV
        6 - [53] - VORLOCK
        7 - [56] - APPROACH
        8 - [59] - CMD
        9 - [62] - CWS
        1 - [65] - HEADING enabled\disabled
        2 - [68] - ALTITUDE enabled\disabled
        3 - [71] - Level changed
```

##### Events from panel to xplane
Каждое сообщение начинается с подстроки `[frey-cmd-a] `, т.о. эвент выглядит примерно `[frey-cmd-a] AP_SPEED_TOGGLE`

Сообщения, отправляемые панелью в сторону xplane:
```
AP_ALTITUDE_TOGGLE - вкл/выкл автопилот высоты
AP_ALTITUDE_XXX - где XXX - высота в сотнях футов.
    Т.е. сообщение AP_ALTITUDE_200, означает задатчик высоты в 20000 фт
AP_HEADING_TOGGLE - вкл/выкл автопилот курса
AP_HEADING_XXX - сам курс. от 0 до 360. в градусах
AP_SPEED_TOGGLE - вкл/выкл автопилот скорости
AP_SPEED_XXX - скорость в узлах. от 100 до 500
DISENGAGE_TOGGLE - ВЫКЛ автопилот. Вкл НЕ этой кнопкой.
CWS_TOGGLE - вкл/выкл следование по текущей скорости, положению носа и курсу
CMD_TOGGLE - вкл/выкл базовый автопилот
APPROACH_TOGGLE - вкл/выкл подход к аэродрому в автоматическом режиме
VOR_LOCK_TOGGLE - вкл/выкл подход к аэродрому по VOR маяку (только по горизонтали)
LNAV_TOGGLE - вкл/выкл следование по горизонтальному маршруту, заданному в FMC
VNAV_TOGGLE - вкл/выкл следование по вертикальному маршруту, заданному в FMC
SET_COURSE_XXX - курс маяка для автоматической посадки. XXX - градусы.
FLAPS_UP_ONE - переключить закрылки вверх на одну позицию
FLAPS_DOWN_ONE - переключить закрылки вниз на одну позицию
GEARUP - шасси вверх
GEARDOWN - шасси вниз
NAV_FREQ_XXX.YY_ZZZ.KK - установить передатчик NAV1
    Active на частоту XXX.YY
    Standby на частоту ZZZ.KK
SPEEDBRAKES_UP_ONE - воздушный тормоз на 1 позицию вверх
SPEEDBRAKES_DOWN_ONE - воздушный тормоз на 1 позицию вниз
VTRIM_UP_ONE - тример руля высоты вверх на 1 поз
VTRIM_DOWN_ONE - тример руля высоты вниз на 1 поз
AUTO_THROTTLE_1 - A/T ON
AUTO_THROTTLE_0 - A/T OFF
FLIGHT_DIRECTOR_1 - F/D ON
FLIGHT_DIRECTOR_0 - F/D OFF
LEVEL_CHANGED - 0/1
```

### Pin numbers

###### KD2-22 Button pins order
```
HEADING TOGGLE
    TOGGLE - 1
    LIGHT - 2

ALTITUDE TOGGLE
    TOGGLE - 3
    LIGHT - 4

AUTOPILOT CMD
    TOGGLE - 5
    LIGHT - 6

AUTOPILOT CWS
    TOGGLE - 7
    LIGHT - 8

AUTOPILOT DISSENGAGE
    TOGGLE - 9
    LIGHT - 10

AUTOPILOT LEVEL CHANGED
    TOGGLE - 11
    LIGHT - 12

AUTOPILOT LNAV
    TOGGLE - 13
    LIGHT - 14

AUTOPILOT VNAV
    TOGGLE - 15
    LIGHT - 16

AUTOPILOT VOR LOC
    TOGGLE - 17
    LIGHT - 18

AUTOPILOT APPROACH
    TOGGLE - 19
    LIGHT - 20

AUTOPILOT SPEED TOGGLE
    TOGGLE - 21
    LIGHT - 22
```

##### Encoder pins order
```
HEADING
    CLK - 23
    DIO - 24

ALTITUDE
    CLK - 25
    DIO - 26

SPEED
    CLK - 27
    DIO - 28

COURSE
    CLK - 29
    DIO - 30

NAV1 MHz (Int)
    CLK - 31
    DIO - 32

NAV1 khz (Float)
    CLK - 33
    DIO - 34

FLAPS
    CLK - 36
    DIO - 37

SPEED BRAKES
    CLK - 38
    DIO - 39

VERTICAL TRIMER
    CLK - 40
    DIO - 41
```

##### Display pins order

```
HEADING
    CLK - 42
    DIO - 43

ALTITUDE
    CLK - 44
    DIO - 45

SPEED
    CLK - 46
    DIO - 47

COURSE
    CLK - 48
    DIO - 49

NAV1 ACTIVE
    CLK - 50
    DIO - 51

NAV1 STANDBY
    CLK - 52
    DIO - 53

FLAPS
    CLK - A15
    DIO - A14

SPEED BRAKES
    CLK - A11
    DIO - A10

VERTICAL TRIM
    CLK - A13
    DIO - A12
```

##### ON[-OFF]-ON Button pin orders
```
GEAR ON-OFF-ON
    ON - A9
    OFF - A8

AUTO THROTTLE
    ON - A7

FLIGHT DIRECTOR
    ON - A6

NAV1
    TOGGLE - 35
```

##### Light pin orders
```
GEAR
    RED - A5
    GREEN - A4
```
