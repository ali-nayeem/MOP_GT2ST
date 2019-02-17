import os,sys
import numpy as np
import os.path
import re
def run(command):
	p1 = os.popen(command)
	temp = p1.readline()
	p1.close()
	return temp.rstrip()

def copy(path, subpath, i):
	old = path+ "/"+subpath+"/R"+str(i)+"/all_gt.tre"
	new = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	run ("cp "+old+" "+new)

def copy_mpest(path, subpath, i):
	old = path+ "/"+subpath+"/R"+str(i)+"/st_from_mpest.txt"
	new = path+ "/"+subpath+"/R"+str(i)+"/species.tre_mpest"
	run ("cp "+old+" "+new)
	run ("diff "+old+" "+new)

def copy_mpest_control(path, subpath, i):
	old = "control_R1"
	input = "data/10-taxon/"+ path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	new = path+ "/"+subpath+"/R"+str(i)+"/control_score"
	run ("echo "+input+" > "+new)
	run ("cat "+old+" >> "+new)

def astral(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral_bl"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	cmd = "java -jar ../../lib/ASTRAL/astral.5.6.3.jar -i "+input+ " -o " + out
	#run(cmd)
	cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	run(cmd)
def phylonet(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_phylonet_bl"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_phylonet"
	cmd = "java -jar ../../lib/phylonet/phylonet_v2_4.jar infer_st -m MDC -i "+input+ " | tail -1 | cut -d ' ' -f 1 > " + out
	#java -jar phylonet_v2_4.jar infer_st -m MDC -i gene.tre | tail -1 | cut -d ' ' -f 1 > out
	run(cmd)
	cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	run(cmd)

def check(path, subpath, i, pwd):
	estimateTree = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	trueTree = pwd + "/"+path+ "/true-speciestrees/" + "R"+str(i) + ".label.true.tre"
	cmd = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree
	result = run(cmd)
	if result != "0.0, 0.0, 0.0":
		print "R"+str(i)+ ": "+ result


def main():
	pwd = run("pwd")
	paths = ["higher-ILS", "lower-ILS"]
	subpaths = ["estimated-genetrees", "true-genetrees"] #, "true-genetrees"]
	for  path in paths:
		  print path
		  for subpath in subpaths:
				for i in range(1,21):
					copy_mpest_control(path, subpath, i)
					#check(path, subpath, i)
					#return
					#sys.exit(0)
					#print(run ("diff "+old+" "+new))

if __name__=="__main__":
      main()