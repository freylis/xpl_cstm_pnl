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
