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
	old = path+ "/"+subpath+"/R"+str(i)+"/Best.1"
	#cmd = "perl strip_edge_support.pl -i " + old + " -o " + old
	#run(cmd)
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
def copy_true_species_tree(path, subpath, i):
	old = path+ "/true-speciestrees/"+"R"+str(i)+".label.true.tre"
	#old = path+ "/true.tree"
	new = path+ "/"+subpath+"/R"+str(i)+"/true_st.tre"
	run ("cp "+old+" "+new)
	run ("diff "+old+" "+new)
def astral(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral_bl"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	cmd = "java -jar ../../lib/ASTRAL/astral.5.6.3.jar -i "+input+ " -o " + out
	run(cmd)
	cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	run(cmd)
def astral_exact(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral_bl.x"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral.x"
	out3 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	cmd = "java  -jar ../../lib/ASTRAL/astral.5.6.3.jar -i "+input+ " -o " + out + " -x" #-Xmx3000M
	print(cmd)
	run(cmd)
	cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	run(cmd)
	#run ("diff "+out2+" "+out3)
def stelar_exact(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar_bl.x"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar.x"
	out3 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_mpest"
	# cmd = "java  -jar ../../lib/STELAR/STELAR_Mazhar.jar -i "+input+ " -o " + out + " -xt"
	# print(cmd)
	# run(cmd)
	# cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	# run(cmd)
	run ("diff "+out2+" "+out3)
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
	estimateTree1 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral.x"
	estimateTree2 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar.x"
	estimateTree3 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_mpest"
	trueTree = pwd + "/"+path+"/"+ subpath + "/R"+str(i) + "/true_st.tre"
	cmd1 = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree1
	cmd2 = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree2
	cmd3 = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree3
	#flag = 0
	result1 = run(cmd1)
	result2 = run(cmd2)
	result3 = run(cmd3)
	#if result != "0.0, 0.0, 0.0":
	#fnResult = "R"+str(i)+ ": astral-> "+ result1 + ": phylonet-> "+ result2
	fnResult = "R"+str(i)+ ": astral-> "+ result1[1:-1].split(',')[1] + ": stelar-> "+ result2[1:-1].split(',')[1] + ": mpest-> "+ result3[1:-1].split(',')[1] + "\n"
	print fnResult
	f = open(subpath+"_FN_score_15-taxon.txt", "a+")
	f.write(fnResult)
	f.close()

def check2(path, subpath, i, pwd):
	estimateTree1 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral.x"
	estimateTree2 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar.x"
	estimateTree3 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_mpest"
	trueTree = pwd + "/"+path+"/"+ subpath + "/R"+str(i) + "/true_st.tre"
	cmd1 = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree1
	cmd2 = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree2
	cmd3 = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree3
	#flag = 0
	result1 = run(cmd1)
	result2 = run(cmd2)
	result3 = run(cmd3)
	#if result != "0.0, 0.0, 0.0":
	#fnResult = "R"+str(i)+ ": astral-> "+ result1 + ": phylonet-> "+ result2
	fnResult = "R"+str(i)+",ASTRAL, "+ result1[1:-1].split(',')[1] + "\n" + "R"+str(i)+",STELAR,"+ result2[1:-1].split(',')[1] + "\n"+  "R"+str(i)+",MP-EST,"+ result3[1:-1].split(',')[1] + "\n"
	print fnResult
	f = open(subpath+"_FN_score_15-taxon2.txt", "a+")
	f.write(fnResult)
	f.close()

def check_old(path, subpath, i, pwd):
	estimateTree = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral.x"
	trueTree = pwd + "/"+path+"/"+ subpath + "/R"+str(i) + "/true_st.tre"
	cmd = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree
	result = run(cmd)
	if result != "0.0, 0.0, 0.0":
		print "R"+str(i)+ ": "+ result


def main():
	pwd = run("pwd")
	#paths = ["higher-ILS", "lower-ILS"]
	#subpaths = ["estimated-genetrees", "true-genetrees"] #, "true-genetrees"]
	paths = ["100gene-100bp"]
	subpaths = ["estimated-genetrees"]
	for  path in paths:
		  print path
		  for subpath in subpaths:
				for i in range(1,11):
					#copy(path, subpath, i)
					#copy_true_species_tree(path, subpath, i)
					#astral(path, subpath, i)
					#phylonet(path, subpath, i)
					check2(path, subpath, i, pwd)
					#stelar_exact(path, subpath, i)
					#return
					#sys.exit(0)
					#print(run ("diff "+old+" "+new))

if __name__=="__main__":
      main()
