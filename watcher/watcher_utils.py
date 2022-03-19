import functools
import time


def timer(func):
    @functools.wraps(func)
    def handler(*args, **kwargs):
        # start = time.monotonic()
        result = func(*args, **kwargs)
        # end = time.monotonic()
        # print(f'Execute {func}: {end - start}')
        return result
    return handler
