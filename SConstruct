from subprocess import call

dirs = ['src']

env = Environment()
env.Append(CPPPATH = ['#/include/'])
env.Append(CPPFLAGS = '-g')
Export('env')

SConscript(dirs=dirs, variant_dir="build", duplicate=0)

call(["ln", "-s", "build/osSim"])
