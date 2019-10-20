filepath = "estimated-genetrees_FN_score_15-taxon.txt"

x = []
y1 = []
y2 = []
y3 = []

with open(filepath) as fp:
	line = fp.readline()
	#cnt = 1
	while line:
		vals = line.split('\n')[0].split(':')
		x.append(vals[0])
		y1.append(float(vals[1].split('>')[-1]))
		y2.append(float(vals[2].split('>')[-1]))
		y3.append(float(vals[3].split('>')[-1]))
		line = fp.readline()

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy

#data
x_ = numpy.arange(len(y1))
#lx_ = numpy.arange(len(ly1))

#plot data
#use zorder to put bars in front of grid
bar_width = 0.25
plt.bar(x_, y1, width=bar_width, color='yellow', zorder=2)
plt.bar(x_ + bar_width, y2, width=bar_width, color='red', zorder=2)
plt.bar(x_ + bar_width*2, y3, width=bar_width, color='blue', zorder=2)

#labels
#adjust x until it is centered
plt.xticks(x_ + bar_width, x)
plt.title('Comparison of FN rate between ASTRAL, STELAR and MP-EST')
plt.xlabel('Replicates')
plt.ylabel('FN rate')

#legend
yellow_patch = mpatches.Patch(color='yellow', label='ASTRAL')
red_patch = mpatches.Patch(color='red', label='STELAR')
blue_patch = mpatches.Patch(color='blue', label='MP-EST')
plt.legend(handles=[yellow_patch, red_patch, blue_patch])

#grid
plt.grid(axis='y')
plt.savefig('15-taxon_barchart.png')
