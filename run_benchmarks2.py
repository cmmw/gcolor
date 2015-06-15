import subprocess
import os

program = "./gcolor"
timelimit = "3600000"
benchmarkdir = "benchmarks/"

filelist = ['DSJC1000.1.col',
			'DSJC1000.5.col',
			'DSJC1000.9.col',
			'DSJC125.1.col',
			'DSJC125.5.col',
			'DSJC125.9.col',
			'DSJC250.1.col',
			'DSJC250.5.col',
			'DSJC250.9.col',
			'DSJC500.1.col',
			'DSJC500.5.col',
			'DSJC500.9.col',
			'DSJR500.1.col',
			'DSJR500.1c.col',
			'DSJR500.5.col',
			'latin_square_10.col',
			'school1.col',
			'school1_nsh.col',
			'queen10_10.col',
			'queen12_12.col',
			'queen14_14.col',
			'queen15_15.col',
			'queen16_16.col'
			]

for benchmark in filelist:
    
    outfile = open(benchmark+".txt", "w")
    try:
        subprocess.call([program, "--alg", 1, "--instanceFile", benchmarkdir+benchmark, "--timelimit", timelimit], stdout=outfile)
    finally:
        outfile.close()
