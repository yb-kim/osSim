import sys
import os
from subprocess import call
import re

nCores = [2, 4, 8, 16, 32, 64, 128, 256, 512]
baseLatency = [0, 2, 5, 10, 20, 50]
oneHopWeight = 2
twoHopWeight = 2.5

configFilePath = '/home/ybkim/workspace/osSim/config/example'
outFilePath = './out'

data = []

if not os.path.exists("./out"):
    call(["mkdir", "out"])

if not os.path.exists("./out/sim_logs"):
    call(["mkdir", "./out/sim_logs"])

if os.path.exists("./config"):
    call(["rm", "-r", "./config"])

call(["cp", "-r", configFilePath, "./config"])

with open(outFilePath+"/result.out", "w") as resultFile:
    for n in nCores:
        with open(configFilePath+'/system.json') as cfgFile, \
        open("./config/system.json", "w") as tempCfgFile:
        #open("./config/mono/apps.json", "w+") as tempAppsFile:
            cfg = cfgFile.read()
            pat = r'("nCores"): (\d+)'
            replacement = r'\1: %d' % n
            cfg = re.sub(pat, replacement, cfg)

            pat = r'("nApps"): (\d+)'
            replacement = r'\1: %d' % (n*2)
            cfg = re.sub(pat, replacement, cfg)

            pat = r'("osType"): (.+)'
            replacement = r'\1: "mono",'
            cfg = re.sub(pat, replacement, cfg)

            tempCfgFile.write(cfg)


        for i in baseLatency:
            
            with open(outFilePath+"/sim_logs/result_%d_cores_baseLatency_%d.txt" % (n, i), "w+") as outFile, \
            open(configFilePath+"/mono/system.json", "r") as appsBaseFile, \
            open("./config/mono/system.json", "w") as appsFile:

                appsConfig = appsBaseFile.read()
                pattern = r'("coherencyRequestTicksInDie"): (\d*)'
                appsConfig = re.sub(pattern, '"coherencyRequestTicksInDie": '+str(i), appsConfig)
                pattern = r'("coherencyRequestTicksOneHop"): (\d*)'
                appsConfig = re.sub(pattern, '"coherencyRequestTicksOneHop": '+str(i*oneHopWeight), appsConfig)
                pattern = r'("coherencyRequestTicksTwoHops"): (\d*)'
                appsConfig = re.sub(pattern, '"coherencyRequestTicksTwoHops": '+str(int(i*2.5)), appsConfig)
                appsFile.write(appsConfig)
                appsFile.close()
                resultFile.write("simulating with nCores = %d, baseLatency = %d\n" % (n, i))
                call(["../osSim", "./config/"], stdout = outFile)
                outFile.seek(-100, 2)
                tail = outFile.read()
                appsProcessed = int(re.search(r'total apps processed: (\d+)\n', tail).groups()[0])
                resultFile.write("total processed apps = %d\n" % appsProcessed)
                resultFile.write("processed apps / core = %f\n\n" % (float(appsProcessed)/float(n)))

                data.append((n, i, appsProcessed))
            resultFile.write('\n')

with open("data_plot.dat", "w") as plotData:
    plotData.write("%s %s\n" % ("baseLatency", "proccessed"))
    for line in data:
        plotData.write("%d %d %d\n" % (line[0], line[1], line[2]))

#draw plot
with open("gnuplot_in", "w") as commands:
    commands.write("""
set xlabel "nCores"
set ylabel "terminated jobs"
set term png
set output "plot.png"
lastx = NaN; lasty = NaN
plot """)
    for i in baseLatency:
        commands.write('"data_plot.dat" u ($2==%d?($1, lastx=$1):lastx):($2==%d?($3, lasty=$3):lasty) title "baseLatency=%d" with linespoints' % (i, i, i))
        if i != baseLatency[-1]:
            commands.write(',lastx=NaN, lasty=NaN\\')
        commands.write('\n')

call(["gnuplot", "gnuplot_in"])
call(["mv", "gnuplot_in", "data_plot.dat", outFilePath])
