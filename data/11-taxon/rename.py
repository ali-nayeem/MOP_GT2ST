import os,sys
import numpy as np
import os.path
import re
def run(command):
	p1 = os.popen(command)
	temp = p1.readline()
	p1.close()
	return temp.rstrip()


def copy_mpest_control(path, subpath, i):
	old = "control_R1"
	input = "data/11-taxon/"+ path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	new = path+ "/"+subpath+"/R"+str(i)+"/control_score"
	run ("echo "+input+" > "+new)
	run ("cat "+old+" >> "+new)
def copy_true_species_tree(path, subpath, i):
	old = path+ "/true-speciestrees/"+"R"+str(i)+".label.true.tre"
	new = path+ "/"+subpath+"/R"+str(i)+"/true_st.tre"
	run ("cp "+old+" "+new)
	run ("diff "+old+" "+new)
def astral(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral_bl"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	cmd = "java -jar ../../lib/ASTRAL/astral.5.6.3.jar -i "+input+ " -o " + out
	#run(cmd)
	cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	run(cmd)
def astral_exact(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral_bl.x"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral.x"
	out3 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	cmd = "java -jar ../../lib/ASTRAL/astral.5.6.3.jar -i "+input+ " -o " + out + " -x"
	# print(cmd)
	# run(cmd)
	# cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	# run(cmd)
	print(run ("diff "+out2+" "+out3))
def stelar_exact(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar_bl.x"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar.x"
	out3 = path+ "/"+subpath+"/R"+str(i)+"/st_from_triplets.txt"
	cmd = "java -Xmx3000M -jar ../../lib/STELAR/STELAR_Mazhar.jar -i "+input+ " -o " + out + " -xt"
	# print(cmd)
	# #run(cmd)
	# cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	#run(cmd)
	print(run ("diff "+out2+" "+out3))
def phylonet(path, subpath, i):
	input = path+ "/"+subpath+"/R"+str(i)+"/gene.tre"
	out = path+ "/"+subpath+"/R"+str(i)+"/species.tre_phylonet_bl"
	out2 = path+ "/"+subpath+"/R"+str(i)+"/species.tre_phylonet"
	cmd = "java -jar ../../lib/phylonet/phylonet_v2_4.jar infer_st -m MDC -i "+input+ " | tail -1 | cut -d ' ' -f 1 > " + out
	#java -jar phylonet_v2_4.jar infer_st -m MDC -i gene.tre | tail -1 | cut -d ' ' -f 1 > out
	run(cmd)
	cmd = "perl strip_edge_support.pl -i " + out + " -o " + out2
	run(cmd)

def check_old(path, subpath, i, pwd):
	estimateTree = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral.x"
	trueTree = pwd + "/"+path+ "/true-speciestrees/" + "R"+str(i) + ".label.true.tre"
	cmd = "python /Users/ali_nayeem/Projects/PyTreePerf/getFpFn.py -t " + trueTree + " -e " + estimateTree
	result = run(cmd)
	if result != "0.0, 0.0, 0.0":
		print "R"+str(i)+ ": "+ result

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
	f = open(subpath+"_FN_score_10-taxon.txt", "a+")
	f.write(fnResult)
	f.close()

def check2(path, subpath, i, pwd):
	estimateTree1 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_astral"
	estimateTree2 = pwd + "/"+ path+ "/"+subpath+"/R"+str(i)+"/species.tre_stelar"
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
	f = open(subpath+"_FN_score_11-taxon2.txt", "a+")
	f.write(fnResult)
	f.close()

def main():
	pwd = run("pwd")
	paths = ["estimated-strongILS"] #, "lower-ILS"
	subpaths = ["50genes"] #, "true-genetrees"]
	for  path in paths:
		  print path
		  for subpath in subpaths:
				for i in range(1,21):
					#copy_true_species_tree(path, subpath, i)
					#astral_exact(path, subpath, i)
					check2(path, subpath, i, pwd)
					#return
					#sys.exit(0)
					#print(run ("diff "+old+" "+new))

if __name__=="__main__":
      main()
