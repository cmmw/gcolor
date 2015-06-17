import subprocess
import os

program = "./gcolor"
timelimit = "300000"
benchmarkdir = "benchmarks/"
p= "5"
tl = "0.4729"

filelist = ['flat1000_60_0.col.b',
            'fpsol2.i.2.col',
     'inithx.i.2.col',
      'le450_25b.col',
       'mulsol.i.2.col',
       'miles1000.col', 
     'queen11_11.col' 
			]

for benchmark in filelist:
    
    outfile = open(benchmark+".txt", "w")
    try:
	    subprocess.call([program, "--alg", str(1), "--instanceFile", benchmarkdir+benchmark, "--timelimit", timelimit, "--randomWalkProbability", p, "--tl", tl ], stdout=outfile )
    finally:
        outfile.close()
