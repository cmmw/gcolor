import subprocess
import os

program = "./gcolor"
timelimit = "10800000"
benchmarkdir = "benchmarks/"

filelist = ['david.col', 'huck.col', 'jean.col', 'queen5_5.col', 'queen6_6.col', 'queen7_7.col', 'queen8_12.col', 'queen8_8.col', 'queen9_9.col', 'myciel3.col', 'myciel4.col', 'myciel5.col', 'myciel6.col']

for benchmark in filelist:
    
    outfile = open(benchmark+".withMAC.txt", "w")
    try:
        subprocess.call([program, "--instanceFile", benchmarkdir+benchmark, "--mac", "1", "--timelimit", timelimit], stdout=outfile)
    finally:
        outfile.close()
    
    outfile2 = open(benchmark+".txt", "w")
    try:
        subprocess.call([program, "--instanceFile", benchmarkdir+benchmark, "--mac", "0", "--timelimit", timelimit], stdout=outfile2)
    finally:
        outfile2.close()

    outfile3 = open(benchmark+".withoutLCV.txt", "w")
    try:
        subprocess.call([program, "--instanceFile", benchmarkdir+benchmark, "--mac", "0", "--lcv", "0", "--timelimit", timelimit], stdout=outfile3)
    finally:
        outfile3.close()

