""" contains project level functions """
import sys
from opensd import circuit as comp
# from opensd import HT_components as HTcomp

def get_comp(obj):
    find = False
    for circuit in comp.Circuit._registry:
        for item in circuit.pipes+circuit.nodes+circuit.bcs+circuit.pumps+circuit.orifices:
            if item.identifier == obj:
                obj = item
                find = True
                break
        if find == True:
            break
    if find == True:
        return obj
    # for item in HTcomp.HSlab._registry+HTcomp.LumpedMass._registry:
        # if item.identifier == obj:
            # obj = item
            # find = True
            # break
        # if find == True:
            # break
    # else:
        # sys.exit("object not found in project. stopping"+obj)
    return obj
