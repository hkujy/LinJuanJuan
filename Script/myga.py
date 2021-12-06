"""
	use existing ga to solve the model
"""
import numpy as np
from geneticalgorithm import geneticalgorithm as ga
import subprocess
import myplot
from numpy.lib.function_base import vectorize
import para
import pandas as pd


def deCode(X):
    """covert the x value to ls
    """
    dp = {}
    for i in range(0, len(X)):
        key = str(i)
        dp[key] = X[i]
    # print("------------")
    sd = sorted(dp.items(), key=lambda d: d[1])
    # print(sd)
    # print("------------")
    return sd


def f(X):
    # print the sol in file
    sd = deCode(X)
    aug = ""
    for e in range(0, len(sd)-1):
        aug = aug + str(sd[e][0])+" "
    aug = aug + str(sd[-1][0])
    program = 'c:/GitCodes/LinJuanJuan/x64/Release/NetworkResilience.exe'
    # print(aug)
    excute = program + " " + aug
    # print(excute)
    p = subprocess.Popen(excute,
                         shell=True,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT,
                         close_fds=True,
                         encoding='utf-8')
    # print(float(p.communicate()[0]))
    strval = p.communicate()[0]
    sv = strval.split(sep='\n')
    # print(sv[-1])
    # input()
    res = float(sv[-1])
    # print(res)
    # return np.sum(X)
    return res

def gafun(num_links, seed_val,iter):
    """main ga code
    """
    varbound = np.array([[0, 1]]*num_links)
    algorithm_param = {
                       'max_num_iteration': iter,
                       'population_size': 4,
                       'mutation_probability': 0.1,
                       'elit_ratio': 0.01,
                       'crossover_probability': 0.5,
                       'parents_portion': 0.3,
                       'crossover_type': 'uniform',
                       'max_iteration_without_improv': None,
                       'random_seed': seed_val}
    model = ga(function=f,
               dimension=num_links,
               variable_type='real',
               variable_boundaries=varbound,
               algorithm_parameters=algorithm_param,
               convergence_curve=False)
    model.run()
    convergence = model.report
    solution = model.best_variable
    return (convergence, solution)

def gamain(_mf,_ps):
    """
    main procedure for the ga 
    """
    GaIter = 5 
    file = ""
    if _ps.para["ModelIndex"] == 3:
        file = "..//InPut//SiouxFallsNetwork//FailureLinks.txt"
    elif _ps.para["ModelIndex"] == 4:
        file = "..//InPut//ParadoxNet//FailureLinks.txt"
    elif _ps.para["ModelIndex"] == 5:
        file = "..//InPut//WangNetwork//FailureLinks.txt"
    else:
        print("failure input file is not specified")

    data = pd.read_csv(file, header=None)
    row = data.shape[0]
    links = []
    for i in range(0, row-1):
        links.append(int(data[0][i]))
    seed =[1]
    with open (_mf.root_folder+"LinJuanJuan\\OutPut\\GaPrintSol.txt","w+") as sf:
        print("Seed,Link,St,Et",file=sf)
        with open (_mf.root_folder + "LinJuanJuan\\Output\\GAConverge.txt","w+") as f:
            for i in range(0, len(seed)):
                (gap, best) = gafun(num_links=len(links), seed_val=seed[i],iter=GaIter)
                sd = deCode(best)
                vec = []
                for e in range(0, len(sd)):
                    vec.append(int(sd[e][0]))
                for v in range(0,len(gap)):
                    print("{0},{1},{2}".format(i,v,gap[v]),file=f)
                for j in range(0,len(vec)):
                    print("{0},{1},-1,-1".format(i,links[vec[j]]),file=sf)
            print("Complete one GA seed {0}".format(i))
    print("GA seed loop complete")
    myplot.plot_converge(_ps, _mf.root_folder + "LinJuanJuan\\Output\\GAConverge.txt",GaIter,"GA")


