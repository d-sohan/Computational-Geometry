import matplotlib.pyplot as plt
import numpy as np
import sys

input_file = str(sys.argv[1])
output_file = str(sys.argv[2])

fig = plt.figure()
ax = plt.subplot(111)

with open(input_file) as in_file:
    i = 0
    next(in_file)
    for edge in in_file:
      x1, y1, x2, y2 = [float(yy) for yy in edge.split()]
      if i==0:
        plt.plot([x1,x2],[y1,y2],'go',linestyle="-",label='Line Segment')
      else:
        plt.plot([x1,x2],[y1,y2],'go',linestyle="-")
      plt.text((x1+x2)/2+0.2, (y1+y2)/2+0.2, f'{i}')
      i+=1

    i=0
with open(output_file) as out_file:
    for edge in out_file:
        g1,h1=[float(yy) for yy in edge.split()]
        if i==0:
            plt.plot(g1,h1,'ro',label='Intersection Points')
            i+=1
        else:
            plt.plot(g1,h1,'ro')
        plt.text(g1+0.2, h1-0.2, '({}, {})'.format(g1, h1))

plt.legend(loc="upper right")
plt.grid()
plt.savefig('{}.png'.format(output_file[:-4]))