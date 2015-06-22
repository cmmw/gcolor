import subprocess
import os

program = "./gcolor"
timelimit = "3600000"
benchmarkdir = "benchmarks/"
p= "5"
tl = "0.4729"

filelist = ['queen11_11.col' 
			]

for benchmark in filelist:
    
    outfile = open(benchmark+".txt", "w")
    try:
	    subprocess.call([program, "--alg", str(1), "--instanceFile", benchmarkdir+benchmark, "--timelimit", timelimit, "--randomWalkProbability", p, "--tl", tl ], stdout=outfile )
    finally:
        outfile.close()
