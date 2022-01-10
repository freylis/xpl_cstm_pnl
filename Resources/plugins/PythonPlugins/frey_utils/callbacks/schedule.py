from .. import utils


def scheduled_callback(sinceLast, elapsedTime, counter, refCon):
    utils.echo(f'Scheduled callback: {sinceLast} / {elapsedTime} / {counter} / {refCon}')
