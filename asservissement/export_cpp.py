#!/usr/bin/env python

__doc__ = "Met à jour un fichier config.cpp en fonction de config.txt"

import sys
import re

re_cppline = re.compile(r'^\s*(double|int64_t|bool)\s+(\w+)\s*=\s*([^;]+);')
re_cfgline = re.compile(r'^\s*([^=\s]+)\s*=\s*([^\s]+)')

defaults = {}

if len(sys.argv) != 3:
    print(__doc__)
    print("Utilisation : %s <config.cpp> <config.txt>" % sys.argv[0])
    sys.exit(1)

cpp_filename = sys.argv[1]
cfg_filename = sys.argv[2]

cfg_file = open(cfg_filename, 'r')

for line in cfg_file:
    match = re_cfgline.match(line.strip())
    if match:
        defaults[match.group(1)] = match.group(2)

cfg_file.close()

newcpp = ''

cpp_file = open(cpp_filename, 'r')

def repl(m):
    type, name, val = m.groups()
    if name not in defaults:
        return match.group(0)

    val = defaults[name]

    if type == 'bool':
        val = 'true' if val in ('1', 'true') else 'false'

    return '%s %s = %s;' % (type, name, val)

for line in cpp_file:
    line = re_cppline.sub(repl , line)
    newcpp += line

cpp_file.close()

cpp_file = open(cpp_filename, 'w')
cpp_file.write(newcpp)
cpp_file.close()
