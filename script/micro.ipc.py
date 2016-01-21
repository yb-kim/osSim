import sys
import datetime
import os
from subprocess import call
import re

nCores = [i for i in range(10, 110, 10)] + [i for i in range(150, 1050, 50)]
nSets = [1, 2, 4, 8, 16, 32, 64, 128]
ipcCosts = [(3, 17), (6, 34), (12, 68)]
nServices = 4   #except NS

configFilePath = '/home/ybkim/workspace/osSim/config/aim7_shared'
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
            replacement = r'\1: "micro",'
            cfg = re.sub(pat, replacement, cfg)

            pat = r'("unitTick"): (.+)'
            replacement = r'\1: 10,'
            cfg = re.sub(pat, replacement, cfg)

            pat = r'("maxTick"): (.+)'
            replacement = r'\1: 50000,'
            cfg = re.sub(pat, replacement, cfg)

            tempCfgFile.write(cfg)

        for i in nSets:
            if i*(nServices+1) >= n:
                data.append((n, i, 0, 0, 0, 0))
                continue;

            for ipcCost in ipcCosts:
                with open(outFilePath+"/sim_logs/result_%d_cores_nSet_%d_ipcCost_%d.txt" % (n, i, ipcCost[0]), "w+") as outFile, \
                open(configFilePath+"/micro/system.json", "r") as appsBaseFile, \
                open("./config/micro/system.json", "w") as appsFile:

                    appsConfig = appsBaseFile.read()
                    pattern = r'("nSet"): (\d*)'
                    appsConfig = re.sub(pattern, '"nSet": '+str(i), appsConfig)

                    """
                    pattern = r'("ipcCost_die"): (\d*)'
                    appsConfig = re.sub(pattern, '"ipcCost_die": '+str(ipcCost[0]), appsConfig)

                    pattern = r'("ipcCost_hop"): (\d*)'
                    appsConfig = re.sub(pattern, '"ipcCost_hop": '+str(ipcCost[1]), appsConfig)

                    pattern = r'("ipcCost_2hops"): (\d*)'
                    appsConfig = re.sub(pattern, '"ipcCost_2hops": '+str(ipcCost[2]), appsConfig)
                    """
                    pattern = r'("ipc_die"): (\d*)'
                    appsConfig = re.sub(pattern, '"ipc_die": '+str(ipcCost[0]), appsConfig)

                    pattern = r'("ipc_remote"): (\d*)'
                    appsConfig = re.sub(pattern, '"ipc_remote": '+str(ipcCost[1]), appsConfig)

                    appsFile.write(appsConfig)
                    appsFile.close()

                    resultFile.write("simulating with nCores = %d, nSet = %f\n" % (n, i))
                    print "start running #cores: %d, nSet: %d, ipcCost: (%d, %d)" % (n, i, ipcCost[0], ipcCost[1])
                    call(["../osSim", "./config/"], stdout = outFile)
                    outFile.seek(-100, 2)
                    tail = outFile.read()
                    appsProcessed = int(re.search(r'total apps processed: (\d+)\n', tail).groups()[0])
                    resultFile.write("total processed apps = %d\n" % appsProcessed)
                    resultFile.write("processed apps / core = %f\n\n" % (float(appsProcessed)/float(n)))

                    data.append((n, i, ipcCost[0], ipcCost[1], ipcCost[1], appsProcessed))

                call(["rm", outFilePath+"/sim_logs/result_%d_cores_nSet_%d_ipcCost_%d.txt" % (n, i, ipcCost[0])])
        resultFile.write('\n')

"""
with open("data_plot.dat", "w") as plotData:
    plotData.write("%s %s %s %s %s %s\n" % ("#nCores", "nSets", "ipcCost_inDie", "ipcCost_hop", "ipcCost_2hops", "proccessed"))
    for line in data:
        plotData.write("%d %d %d %d %d %d\n" % (line[0], line[1], line[2], line[3], line[4], line[5]))
"""

result = []
for nCore in nCores:
    for ipcCost in ipcCosts:
        values = [item[5] for item in data if item[0]==nCore and item[2]==ipcCost[0]]
        processed = max(values)
        result.append((nCore, ipcCost[0], processed))

with open("data_plot.dat", "w") as plotData:
    plotData.write("%s %s %s\n" % ("#nCores", "ipcCost_inDie", "processed"))
    for line in result:
        plotData.write("%d %d %d\n" % (line[0], line[1], line[2]))

#draw plot
inDieCosts = [cost[0] for cost in ipcCosts]
with open("gnuplot_in", "w") as commands:
    commands.write("""
set xlabel "nCores"
set ylabel "terminated jobs"
set term png
set output "plot.png"
lastx = NaN; lasty = NaN
plot """)
    for i in ipcCosts:
        commands.write('"data_plot.dat" u ($2==%d?($1, lastx=$1):lastx):($2==%d?($3, lasty=$3):lasty) title "ipcCost=%d:%d" with linespoints' % (i[0], i[0], i[0], i[1]))
        if i[0] != inDieCosts[-1]:
            commands.write(',lastx=NaN, lasty=NaN\\')
        commands.write('\n')

call(["gnuplot", "gnuplot_in"])
call(["mv", "gnuplot_in", "data_plot.dat", outFilePath])

now = datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S")

call(["cp", "plot.png", "/home/ybkim/dropbox/Inbox/osSim/plot_%s.png" % (now)])
call(["cp", outFilePath + "/data_plot.dat", "/home/ybkim/dropbox/Inbox/osSim/data_%s.dat" % (now)])
