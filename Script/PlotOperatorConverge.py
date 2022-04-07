""" 
	this is read and plot the average select probability of each operator
"""
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from pylab import mpl
import para


# folder = '../Output/'
# folder = 'C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1\SelectNei\9_ALNS/'

# folder = 'C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022\SelectNei\'

def change_operator_prob_over_iterations(_folder):
    """
        function relate to the changes in the probability rate over iterations
    """
    print("------Start plot change operator over iter----------")
    file = 'OperatorsMeasure.txt'
    df = pd.read_csv(_folder+file)
    num_seed = para.NumOfTestSeed
    num_op = 9
    num_iter = para.NumofIter
    print("Read file = {0}".format(df))
    print("Num of iter = {0}".format(num_iter))
    print("Num of seed = {0}".format(num_seed))

    plt.rc('font', family='Times New Roman')
    prob_matrix = np.random.rand(num_op,num_iter)
    row = 0
    for s in range(0,num_seed):
        for i in range(0,num_iter):
            for o in range(0, num_op):
                prob_matrix[o,i] = df['Prob'][row]
                row = row + 1
    # print(prob_matrix)
    ave_mat =[0]*num_op
    for o in range(0,num_op):
        ave_mat[o] = np.average(prob_matrix[o])
    print(ave_mat)

    for o in range(0,num_op):
        plt.plot(ave_mat,label="Operator:"+str(o))
    plt.legend()
    plt.xlabel("No. of Iterations",fontsize = 12,fontdict={'weight':'bold'})
    plt.ylabel("Probability",fontsize=12,fontdict={'weight':'bold'})

    plt.ion()
    plt.pause(1)
    # plt.savefig("..//Output//"+algName+"_Converge_min.png", bbox_inches='tight', dpi=600)
    plt.savefig(_folder+"Plot_operator_prob_convergepng", bbox_inches='tight', dpi=600)
    plt.close()
    print("-------------Complete plot change of the prob over iterations-------------")