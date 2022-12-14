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
    return handler


def echo(msg, error=False):
    if error:
        msg = f'\tERROR\t: {msg}'
    xp.log(msg)
    now = datetime.datetime.now()
    with open('E:\\SteamLibrary\\steamapps\\common\\X-Plane 12\\frey.log', 'a') as f:
        f.write(f'\n[{now}]: {msg}')


def import_smthng(path):
    components = path.split('.')
    mod = __import__(components[0])
    for comp in components[1:]:
        try:
            mod = getattr(mod, comp)
        except AttributeError as exc:
            raise ImportError(f'Cant import {path!r}: {exc}')
    return mod
