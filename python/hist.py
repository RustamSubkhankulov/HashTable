
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

    y_len = len(y)

    for i in range(0, y_len):
        y[i] = int(y[i])

    x = []
    for i in range(y_len):
        x.append(i)

    # Creating plot
    # plt.plot(x, y)

    # bins = 10
    # rang = (0, number)

    plt.bar(x, y, width = 1.1)

    plt.xlabel('list number')
    plt.ylabel('number of elements')

    # Saveplot
    plt.savefig('pictures/graph_{}.png'.format(graph_ct))

    # y_av = sum(y) / len (y)
    # accum = 0

    # print(y_av)

    # for j in range (y_len):
    #     accum += pow((y[j] - y_av), 2)

    # print(y_len)
    # print(accum/y_len)
    
    # Clear plot
    plt.clf()