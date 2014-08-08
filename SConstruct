import os
from subprocess import call

AddOption(
        '--debug-build',
        dest = 'debug-build',
        action = 'store_true',
        help = 'turn on this for debug.',
        default = False)

dirs = ['src']

env = Environment()
env.Append(CPPPATH = ['#/include/'])
#Export('env')

if GetOption('debug-build'):
    env.Append(CPPFLAGS = '-g')
    variant_dir = "build/debug"
else:
    variant_dir="build/release"

SConscript(dirs=dirs, variant_dir=variant_dir, duplicate=0, exports="env")

if os.path.isfile('osSim'):
    os.remove('osSim')
call(["ln", "-s", variant_dir+"/osSim"])
