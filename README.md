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
    TOGGLE - 13
    LIGHT - A0

ALTITUDE TOGGLE
    TOGGLE - 12
    LIGHT - A1

AUTOPILOT CMD
    TOGGLE - 11
    LIGHT - A2

/*
ОТКЛЮЧАЮ, жду пины
AUTOPILOT CWS
    TOGGLE - 10
    LIGHT - A3
*/

AUTOPILOT DISSENGAGE
    TOGGLE - 9

AUTOPILOT LEVEL CHANGED
    TOGGLE - 8
    LIGHT - A5

AUTOPILOT LNAV
    TOGGLE - 7
    LIGHT - A6

AUTOPILOT VNAV
    TOGGLE - 6
    LIGHT - A7

AUTOPILOT VOR LOC
    TOGGLE - 5
    LIGHT - A8

AUTOPILOT APPROACH
    TOGGLE - 4
    LIGHT - A9

AUTOPILOT SPEED TOGGLE
    TOGGLE - 3
    LIGHT - A10
```

##### Encoder pins order
```
HEADING
    CLK - 22
    DIO - 23

ALTITUDE
    CLK - 24
    DIO - 25

SPEED
    CLK - 26
    DIO - 27

COURSE
    CLK - 28
    DIO - 29

NAV1 MHz (Int)
    CLK - 30
    DIO - 31

NAV1 khz (Float)
    CLK - 32
    DIO - 33

FLAPS
    CLK - 34
    DIO - 35

SPEED BRAKES
    CLK - 36
    DIO - 37

/*
!!!!!! отключаю ввиду отсутствия пинов
VERTICAL TRIMER
    CLK - 36
    DIO - 37
*/
```

##### Display pins order

```
HEADING
    CLK - 38
    DIO - 39

ALTITUDE
    CLK - 40
    DIO - 41

SPEED
    CLK - 42
    DIO - 43

COURSE
    CLK - 44
    DIO - 45

NAV1 ACTIVE
    CLK - 46
    DIO - 47

NAV1 STANDBY
    CLK - 48
    DIO - 49

FLAPS
    CLK - 50
    DIO - 51

SPEED BRAKES
    CLK - 52
    DIO - 53

/*
Отключаю, не пользуемся за ненадобностью
VERTICAL TRIM
    CLK - A15
    DIO - A14
*/
```

##### ON[-OFF]-ON Button pin orders
```
GEAR ON-OFF-ON
    ON - A13
    OFF - A12

AUTO THROTTLE
    ON - A11

FLIGHT DIRECTOR
    ON - A3

NAV1
    TOGGLE - 10
```

##### Light pin orders
```
GEAR
    /*
    Пока некуда подключать
    RED - A5
    */

    GREEN - 2
```


Просто все пины. Поможет мне следить за свободными
```
A0 - KD2 HEADING LIGHT
A1 - KD2 ALTITUDE LIGHT
A2 - KD2 CMD LIGHT
A3 - ON-ON FLIGHDIRECTOR ON
A4 - KD2 CWS LIGHT
A5 - KD2 LEVEL CHANGED LIGHT
A6 - KD2 LNAV LIGHT
A7 - KD2 VNAV LIGHT
A8 - KD2 VORLOC LIGHT
A9 - KD2 APPROACH LIGHT
A10 - KD2 SPEED TOGGLE LIGHT
A11 - ON-ON AUTOTHOTTLE
A12 - ON-ON BUTTON GEAR UP
A13 - ON-ON BUTTON GEAR DOWN
A14 - LED GEAR UP
A15 - LED GEAR DOWN

2 - KD2 CWS TOGGLE
3 - KD2 SPEED TOGGLE
4 - KD2 APPROCH TOGGLE
5 - KD2 VORLOC TOGGLE
6 - KD2 VNAV TOGGLE
7 - KD2 LNAV TOGGLE
8 - KD2 LEVEL CHANGED TOGGLE
9 - KD2 DISSENGAGE TOGGLE
10 - ON-ON NAV1 TOGGLE
11 - KD2 CMD TOGGLE
12 - KD2 ALTITUDE TOGGLE
13 - KD2 HEADING TOGGLE
22 - ENCODER HEADING CLK
23 - ENCODER HEADING DIO
24 - ENCODER ALTITUDE CLK
25 - ENCODER ALTITUDE DIO
26 - ENCODER SPEED CLK
27 - ENCODER SPEED DIO
28 - ENCODER COURSE CLK
29 - ENCODER COURSE DIO
30 - ENCODER NAV1_MHZ CLK
31 - ENCODER NAV1_MHZ DIO
32 - ENCODER NAV1_KHZ CLK
33 - ENCODER NAV1_KHZ DIO
34 - ENCIDER FLAPS CLK
35 - ENCODER FLAPS DIO
36 - ENCODER SPEED BRAKES CLK
37 - ENCODER SPEED BRAKES DIO
38 - DISPLAY HEADING CLK
39 - DISPLAY HEADING DIO
40 - DISPLAY ALTITUDE CLK
41 - DISPLAY ALTITUDE DIO
42 - DISPLAY SPEED CLK
43 - DISPLAY SPEED DIO
44 - DISPLAY COURSE CLK
45 - DISPLAY COURSE DIO
46 - DISPLAY NAV1 ACTIVE CLK
47 - DISPLAY NAV1 ACTIVE DIO
48 - DISPLAY NAV1 STANDBY CLK
49 - DISPLAY NAV1 STANDBY DIO
50 - DISPLAY FLAPS CLK
51 - DISPLAY FLAPS DIO
52 - DISPLAY SPEED BRAKES CLK
53 - DISPLAY SPEED BRAKES DIO
```
