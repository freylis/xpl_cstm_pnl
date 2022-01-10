import traceback
import functools
import datetime
import xp


def error_log(func):
    @functools.wraps(func)
    def handler(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as exc:
            echo(f'Error {exc}')
            echo(traceback.format_exc())
            raise


def echo(msg):
    xp.log(msg)
    now = datetime.datetime.now()
    with open('D:\\games\\SteamLibrary\\steamapps\\common\\X-Plane 11\\frey.log', 'a') as f:
        f.write(f'\n[{now}]: {msg}')
