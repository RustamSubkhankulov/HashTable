
from matplotlib import pyplot as plt
import sys
 
#------------------------------------------------

#Read input
src = open(sys.argv[1], "r")

#Read every value by line
input = src.readlines()
src.close()

number = len(input)

#------------------------------------------------

# 6 graphs for 6 types of hash functions

for graph_ct in range (number):

    y = input[graph_ct].split(',')

    number = len(y)

    for i in range(0, number):
        y[i] = int(y[i])

    x = []
    for i in range(number):
        x.append(i)

    # Creating plot
    plt.plot(x, y)

    plt.xlabel('list number')
    plt.ylabel('number of elements')

    # Saveplot
    plt.savefig('pictures/graph_{}.png'.format(graph_ct))
    
    # Clear plot
    plt.clf()