import xp
from .. import utils


def scheduled_callback(sinceLast, elapsedTime, counter, refCon):
    utils.echo(f'Scheduled callback: {sinceLast} / {elapsedTime} / {counter} / {refCon}')


    utils.echo('Throttle changed')
    throttle_ref = xp.findDataRef('sim/cockpit2/engine/actuators/throttle_ratio_all')
    throttle_val = xp.getDataf(throttle_ref)

    thr_ref_2 = xp.findDataRef('sim/cockpit2/engine/actuators/throttle_jet_rev_ratio_all')
    thr_ref_2_val = xp.getDataf(thr_ref_2)
    utils.echo(f'Current throttle: {throttle_val} / {thr_ref_2_val}')
    return 1
