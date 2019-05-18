#!/usr/bin/env python
#import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from optparse import OptionParser

desc = '''
           This script draws histogram from a TreePerf file.
       '''

parser = OptionParser(usage = "usage: %prog [options]", description = desc)
parser.add_option("-f", "--file", dest = "tree_perf", help = "read tree perf from FILE", metavar = "FILE")
#parser.add_option("-v", "--varfile", dest = "var_file", help = "read list of estimated trees from FILE", metavar = "FILE")
#parser.add_option("-o", "--outfile", dest = "out_file", help = "write tree perf to FILE", metavar = "FILE")


(options, args) = parser.parse_args()
if (options.tree_perf == None):
    parser.error("values for both options must be specified\ntry running with the --help flag")

try:
    fn = np.genfromtxt(options.tree_perf, delimiter=',', usecols=1)
    fn.sort()
    #df = pd.read_csv(options.tree_perf, header=None, names=['fp', 'fn', 'w'])
    #df.hist(column='fn')
    #df.sort_values(by=['fn'])
    id = range(0,len(fn))
    plt.scatter(id ,fn)
    axes = plt.gca()
    axes.set_xlim([0,len(fn)])
    plt.savefig( options.tree_perf + '.png', format='png', bbox_inches='tight')
finally:
   plt.clf()
