#!/usr/bin/env python

__doc__ = "Convertit un fichier config.cpp en config.txt (en utilisant params.h)"

import sys
import re

re_cppline = re.compile(r'^\s*(?:double|int64_t|bool)\s+(\w+)\s*=\s*([^;]+);')
re_param = re.compile(r'^\s*PARAM\s*\([^,]+,\s*([^\s)]+)\s*\)')

defaults = {}

if len(sys.argv) != 4:
    print(__doc__)
    print("Utilisation : %s <params.h> <config.cpp> <config.txt>" % sys.argv[0])
    sys.exit(1)

param_filename = sys.argv[1]
cpp_filename = sys.argv[2]
cfg_filename = sys.argv[3]

cpp_file = open(cpp_filename, 'r')

for line in cpp_file:
    match = re_cppline.match(line.strip())
    if match:
        defaults[match.group(1)] = match.group(2)

cpp_file.close()

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
