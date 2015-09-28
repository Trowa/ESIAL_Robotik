#!/usr/bin/env python

__doc__ = "Met à jour le fichier config.txt passé en paramètre à l'aide du fichier params.h"

import sys
import re

re_cfgline = re.compile(r'^\s*([^=\s]+)\s*=\s*([^\s]+)')
re_param = re.compile(r'^\s*PARAM\s*\([^,]+,\s*([^\s)]+)\s*\)')

defaults = {}

if len(sys.argv) != 3:
    print(__doc__)
    print('Utilisation : %s <params.h> <config.txt>' % sys.argv[0])
    sys.exit(1)

param_filename = sys.argv[1]
cfg_filename = sys.argv[2]


try:
    cfg_file = open(cfg_filename, 'r')

    for line in cfg_file:
        match = re_cfgline.match(line.strip())
        if match:
            defaults[match.group(1)] = match.group(2)

    cfg_file.close()
except IOError:
    pass

cfg_file = open(cfg_filename, 'w')
param_file = open(param_filename, 'r')

for line in param_file:
    line = line.strip()
    if line.startswith('//') or not line:
        cfg_file.write('%s\n' % line)
    else:
        match = re_param.match(line)
        if match:
            name = match.group(1)
            cfg_file.write('%s = %s\n' % (name, defaults.get(name, '0')))

param_file.close()
