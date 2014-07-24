from subprocess import call

dirs = ['src']

env = Environment()
env.Append(CPPPATH = ['#/include/'])
Export('env')

SConscript(dirs=dirs, variant_dir="build", duplicate=0)

call(["ln", "-s", "build/osSim"])
