import os
from numpy.lib.function_base import append
import pandas as pd
import matplotlib.pyplot as plt
import logging


def plot_converge(_ps,_file):
    """
        plot the convergence of different seed 
    """
    print("gapfile =",_file)
    if not os.path.isfile(_file):
       print("Warning: the gap file does not exist and need to check the nam of the file")
       
    data = pd.read_csv(_file,header=None)
    num_of_row = data.shape[0]
    print("NumRow = {0}".format(num_of_row))
    num_of_seed = int(num_of_row/_ps.para["MaxABCIter"])
    print("Seed = {0}".format(num_of_seed))
    gap = []
    row = 0
    for s in range(0,num_of_seed):
        g = []
        for i in range(0,_ps.para["MaxABCIter"]):
            g.append(float(data[2][row]))
            row = row + 1
        print(g)
        plt.plot(g,label="seed"+str(s))
        gap.append(g)
    plt.ion()
    plt.legend()
    plt.pause(1)
    plt.savefig("..//Output//Converge_seed.png",bbox_inches='tight',dpi=600)
    plt.close()
    
    ave = []
    for i in range(0,_ps.para["MaxABCIter"]):
        val = 0.0
        for s in range(0,num_of_seed):
            val = val + float(gap[s][i])
        val = val/num_of_seed
        ave.append(val)
    plt.plot(ave,label="ave gap")
    plt.ion()
    plt.legend()
    plt.pause(1)
    plt.savefig("..//Output//Converge_ave.png",bbox_inches='tight',dpi=600)
    plt.close()
    

   