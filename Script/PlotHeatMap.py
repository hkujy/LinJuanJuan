""" 
	this is read and plot the heat map of the pattern matrix
	https://www.askpython.com/python/examples/heatmaps-in-python
	adjust color
	https://zhuanlan.zhihu.com/p/165426873
"""
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from pylab import mpl



def heatmap(mat,name:str,nodes):
    mpl.rcParams['font.family'] = 'Times New Roman'
    mpl.rcParams['font.sans-serif'] = 'Times New Roman'
    font = {'family': 'sans-serif',
        'color': 'k',
        'weight': 'normal',
        'size': 10, }
# plt.style.use("seaborn")
    plt.rc('font', family='Times New Roman')
    f, ax = plt.subplots(figsize=(8, 8))
    sns.set(font_scale=0.8)
    annot_kws = {'fontsize': 10, 'fontfamily': 'Time New Roman',
             'color': "k",
             'alpha': 0.6}
#    'rotation':"vertical",
#    'verticalalignment':'center',
#    'backgroundcolor':'w'}
    cbar_kws = {
    # "orientation":"horizontal",
    # "shrink":1,
    # 'extend':'min',
    # 'extendfrac':0.1,
    # "ticks":np.arange(0,22),
    # "drawedges":True,
    }
    ax = sns.heatmap(mat, linewidth=1,
                    ax=ax, annot=True,
                    fmt=".2f", 
                    cmap=plt.get_cmap('YlGnBu'),
                    # cmap=plt.get_cmap('YlGn'),
                    # cmap=plt.get_cmap('Greens'),
                    # cmap=plt.get_cmap('tab20c'), 
                    # cmap=sns.diverging_palette(10, 220, sep=80, n=7),#区分度显著色盘：sns.diverging_palette()使用
                    annot_kws=annot_kws,
                    cbar_kws=cbar_kws)
    xtick = ax.get_xticks()
    xticklabel = ax.get_xmajorticklabels()
    yticklabel = ax.get_ymajorticklabels()
    for i in range(0, len(nodes)):
        xticklabel[i]._text = str(nodes[i])
        yticklabel[i]._text = str(nodes[i])
    ax.set_xticklabels(xticklabel)
    ax.set_yticklabels(yticklabel)
    ax.set_xlabel("Disrupted Links")
    ax.set_ylabel("Disrupted Links")

    cax = plt.gcf().axes[-1]
    cbar = ax.collections[0].colorbar
    cbar.set_label(r'$Prob$', fontdict=font)
    # print(xticklabel)
    # print(yticklabel)
    plt.title(name)
    plt.ion()
    # plt.legend()
    plt.pause(1)
    plt.savefig("HeatMap_"+name+".png", bbox_inches='tight', dpi=600)
    plt.close()
 

def get_index(i,j,num_of_nodes):
    """
        give the row and column index, return the location in a matrix
    """
    return (i*num_of_nodes+j) 


def plot_Patten_heat_map(nodes):
    num_seed = 2
    num_nodes = 12
    num_nodes = 12
    nodes = [0, 6, 19, 22, 26, 27, 34, 38, 42, 55, 59, 73]
    num_nodes = len(nodes)
    fn = "../Output/PrintPatternScore.txt"
    # print("Need to check the input of number of seed and number nodes")
    # sns.set_theme(font='Times New Roman')

    ave_matrix = np.random.rand(num_nodes, num_nodes)
    std_matrix = np.random.rand(num_nodes, num_nodes)
    # for i in range(0, num_nodes):
    #     for j in range(0, num_nodes):
    #         mat[i, j] = 0

    data = pd.read_csv(fn)

    vec =[]  # vector represention of a matrix
    for i in range(0,num_nodes*num_nodes):
        vec.append([])
    # print(data)
    for s in range(0, num_seed):
        for i in range(0,num_nodes):
            for j in range(0,num_nodes):
                row = s*(num_nodes*num_nodes)+i*num_nodes+j
                first = nodes[i]
                second =nodes[j]
                val = data["Prob"][row]
                loc = get_index(i,j,num_nodes)
                # print("s={0},i={1},j={2},row={3},val={4}".format(s,first,second,row,val))
                vec[loc].append(val)
    # print(vec)       

    for i in range(0, num_nodes):
        for j in range(0, num_nodes):
            loc = get_index(i,j,num_nodes)
            ave_matrix[i,j] = np.average(vec[loc])
            std_matrix[i,j] = np.std(vec[loc])

    # print(std_matrix)
    # print("-------------------")
    # print(ave_matrix)
    heatmap(ave_matrix,"ave",nodes)
    heatmap(std_matrix,"std",nodes)
    exit()



