#!/usr/bin/env python
import sys
sys.path.insert(0, 'spurce')
sys.path.insert(0, 'newick_modified')
from optparse import OptionParser
from spruce.unrooted import *
from spruce.metrics import *
from spruce.mrp import readTreesFromRatchet
from newick_modified import parse_tree

desc = '''
           This script prints the FP rate, FN rate, and Robinson-Foulds distance
           of a pair of trees.
       '''

parser = OptionParser(usage = "usage: %prog [options]", description = desc)
parser.add_option("-t", "--true", dest = "true_tree", help = "read true tree from FILE", metavar = "FILE")
parser.add_option("-v", "--varfile", dest = "var_file", help = "read list of estimated trees from FILE", metavar = "FILE")
parser.add_option("-o", "--outfile", dest = "out_file", help = "write tree perf to FILE", metavar = "FILE")


(options, args) = parser.parse_args()
if (options.var_file == None or options.true_tree == None):
    parser.error("values for both options must be specified\ntry running with the --help flag")

try:
    trueTree = readNewickFile(options.true_tree)
except:
    trueTree = parse_tree(readTreesFromRatchet(options.true_tree)[0])
with open(options.out_file, 'w') as the_file:

    with open(options.var_file) as fileHandler:
         for line in fileHandler:
            # As each line (except last one) will contain new line character, so strip that
            estimateTree = parse_tree(line.strip())
            a,b,c = getFpFnRfRates(trueTree, estimateTree)
            the_file.write(str(a) + ", " + str(b) + ", " + str(c) + '\n')

        #estimateTree = parse_tree(f.readlines())
        #print f.readline()
        #print getFpFnRfRates(trueTree, estimateTree)
