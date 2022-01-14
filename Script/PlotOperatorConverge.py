""" 
	this is read and plot the average select probability of each operator
"""
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from pylab import mpl


# folder = '../Output/'
folder = 'C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1\SelectNei\9_ALNS/'
file = 'OperatorsMeasure.txt'
df = pd.read_csv(folder+file)
print(df)

num_seed = 2
num_iter = 10
num_op = 9

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
plt.show()
plt.close()