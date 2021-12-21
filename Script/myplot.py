import os
from numpy.lib.function_base import append
import pandas as pd
import matplotlib.pyplot as plt
import logging


def plot_converge(_ps, _file, _numOfiter, algName):
    """
        plot the convergence of different seed 
    """
    print("gapfile =", _file)
    if not os.path.isfile(_file):
        print("Warning: the gap file does not exist and need to check the nam of the file")

    data = pd.read_csv(_file, header=None)
    num_of_row = data.shape[0]
    print("NumRow = {0}".format(num_of_row))
    # num_of_seed = int(num_of_row/_ps.para["MaxABCIter"])
    num_of_seed = int(num_of_row/_numOfiter)
    print("Seed = {0}".format(num_of_seed))
    gap = []
    row = 0
    for s in range(0, num_of_seed):
        g = []
        # for i in range(0, _ps.para["MaxABCIter"]):
        for i in range(0, _numOfiter):
            g.append(float(data[2][row]))
            row = row + 1
        # print(g)
        plt.plot(g, label="seed"+str(s))
        gap.append(g)
    plt.title(algName)
    plt.ion()
    plt.legend()
    plt.pause(1)
    plt.savefig("..//Output//"+algName+"_Converge_seed.png",
                bbox_inches='tight', dpi=600)
    plt.close()

    ave = []
    minv = []
    # for i in range(0, _ps.para["MaxABCIter"]):
    for i in range(0, _numOfiter):
        val = 0.0
        minval = 9.9e20
        for s in range(0, num_of_seed):
            val = val + float(gap[s][i])
            minval = min(minval,float(gap[s][i]))
        val = val/num_of_seed
        minv.append(minval)
        ave.append(val)
    plt.plot(ave, label="ave gap")
    plt.title(algName)
    plt.ion()
    plt.legend()
    plt.pause(2)
    # plt.savefig("..//Output//Converge_ave.png", bbox_inches='tight', dpi=600)
    plt.savefig("..//Output//"+algName+"_Converge_ave.png",
                bbox_inches='tight', dpi=600)
    plt.close()
    plt.plot(minv,label="min gap")
    plt.ion()
    plt.legend()
    plt.pause(2)
    # plt.savefig("..//Output//Converge_ave.png", bbox_inches='tight', dpi=600)
    plt.savefig("..//Output//"+algName+"_Converge_min.png", bbox_inches='tight', dpi=600)
    plt.close()
    plt.title(algName)

    # with open("..//Output//Converge_ave.txt", "w+") as f:
    with open("..//Output//" + algName+"_Converge_ave.txt", "w+") as f:
        for i in range(0, len(ave)):
            print("{0},{1}".format(i, ave[i]), file=f)
    with open("..//Output//" + algName+"_Converge_min.txt", "w+") as f:
        for i in range(0, len(minv)):
            print("{0},{1}".format(i, minv[i]), file=f)
