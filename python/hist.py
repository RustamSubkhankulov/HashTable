
from matplotlib import pyplot as plt
import numpy as np
 
#------------------------------------------------

#Read input
src = open("/home/rustam/Desktop/code/HashTable/text_files/res.txt", "r")

#Read every value by line
input = src.readlines()
src.close()

#------------------------------------------------

for graph_ct in range (6):

    y = input[0].split(',')

    number = len(y)

    for i in range(0, number):
        y[i] = int(y[i])

    x = []
    for i in range(number):
        x.append(i)

    #print(x)
    #print(y)

    # Creating plot
    plt.plot(x, y)

    plt.xlabel('list number')

    plt.ylabel('number of elements')

    # Show plot
    plt.show()