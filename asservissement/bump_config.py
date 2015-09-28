#!/usr/bin/env python

__doc__ = "Met à jour le numéro de révision d'un fichier config.txt"

import sys
import re
import datetime

if len(sys.argv) != 2:
    print(__doc__)
    print("Utilisation : %s <config.txt>" % sys.argv[0])
    sys.exit(1)


def bump_version(match):
    new_version = int(datetime.datetime.now().strftime('%Y%m%d') + '01')
    version = int(match.group(1))

    if version >= new_version:
        new_version = version + 1

    return 'configVersion = %d' % new_version

with open(sys.argv[1], 'r') as f:
    data = f.read()

data = re.sub(r'configVersion = (\d+)', bump_version, data)

with open(sys.argv[1], 'w') as f:
    f.write(data)

