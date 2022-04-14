"""Post process the resuls data
"""
# from tkinter import font
# from tkinter.font import _FontDict
# from tkinter.font import _FontDict
from audioop import avg
import pandas as pd
import matplotlib.pyplot as plt
from statistics import mean
import statistics as stat
import os
import para
import numpy as np
# root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022/"

label_font = {'family': 'Times New Roman',
              'weight': 'bold',
              'size': 12
              }

tick_font = {'family': 'Times New Roman',
             'weight': 'bold',
             'size': 10
             }


def getBestSeed(_folder: str):  # return the best seed number
    """Input is the folder
    """
    fn = _folder+'/BestSeed.txt'
    df = pd.read_csv(fn)
    best_seed = df["Seed"][0]
    return int(best_seed)


def updateOperatorName(fn: str):
    """
            Given the folder of name return the oprator number
            This is applied to test the effect of different operators
    """
    print("input fn ={0}".format(fn))
    if fn == "0_TestOp_0":
        return "NH_1"
    elif fn == "1_TestOp_1":
        return "NH_2"
    elif fn == "2_TestOp_2":
        return "NH_3"
    elif fn == "3_TestOp_3":
        return "NH_4"
    elif fn == "4_TestOp_4":
        return "NH_5"
    elif fn == "5_TestOp_5":
        return "NH_6"
    elif fn == "6_TestOp_6":
        return "NH_7"
    elif fn == "7_TestOp_7":
        return "NH_8"
    elif fn == "8_TestOp_8":
        return "NH_9"
    else:
        return fn


def CompareOneFolder(_folder: str, _name: str):
    root, dirs, files = next(os.walk(_folder), ([], [], []))
    print(dirs)

    print("plot one folder case")
    print("default number of iter is 200, need to check if it is not")
    NumberOfIter = para.NumofIter
    allGaps = []
    for fo in range(0, len(dirs)):
        f = _folder+"\\"+dirs[fo] + "\\ABC_Converge_ave.txt"
        df = pd.read_csv(f, header=None)
        gap = []
        for j in range(0, NumberOfIter):
            gap.append(df[1][j])
        allGaps.append(gap)
        # plt.plot(gap, label=dirs[fo])
        this_label = updateOperatorName(dirs[fo])
        plt.plot(gap, label=this_label)
    plt.xlabel("No. of Iterations", fontdict=label_font)
    plt.ylabel("Ave. Objective Value", fontdict=label_font)

    plt.ion()
    plt.legend()
    plt.pause(1)
    plt.savefig(_folder+"\\"+_name+"_ave.png", bbox_inches='tight', dpi=600)
    plt.close()

    d = []
    for i in range(0, len(dirs)):
        d.append([str(i), allGaps[i][NumberOfIter-1]])
    df = pd.DataFrame(d, columns=['Exp', 'MinObj'])
    with open(_folder+"min_ObjTable.txt", "w+") as f:
        print("Id,Obj", file=f)
        for r in range(0, len(df)):
            print("{0},{1}", df['Exp'][r], df['MinObj'][r], file=f)
    print(df)

    allGaps = []
    for fo in range(0, len(dirs)):
        f = _folder+"\\"+dirs[fo] + "\\ABC_Converge_min.txt"
        df = pd.read_csv(f, header=None)
        gap = []
        for j in range(0, NumberOfIter):
            gap.append(df[1][j])
        allGaps.append(gap)
        # plt.plot(gap, label=dirs[fo])
        this_label = updateOperatorName(dirs[fo])
        plt.plot(gap, label=this_label)
    plt.xlabel("No. of Iterations", fontdict=label_font)
    plt.ylabel("Min. Objective Value", fontdict=label_font)

    plt.ion()
    plt.legend()
    plt.pause(1)
    plt.savefig(_folder+"\\"+_name+"_min.png", bbox_inches='tight', dpi=600)
    plt.close()

    # next is to output the value, std, and min value
    allObjs = []
    ave = []
    minval = []
    std = []
    for fo in range(0, len(dirs)):
        f = _folder+"\\"+dirs[fo] + "\\ABCPrintSeedBestSolVal.txt"
        df = pd.read_csv(f)
        print(df)
        obj = []
        for j in range(0, para.NumOfTestSeed):
            obj.append(df["BestVal"][j])
        allObjs.append(obj)
        ave.append(mean(obj))
        minval.append(min(obj))
        std.append(stat.stdev(obj))
    rowLabel = []
    matrix = []
    for i in range(0, len(dirs)):
        matrix.append([-1]*3)
    colLabel = ["Ave", "Min", "Std"]
    for i in range(0, len(dirs)):
        rowLabel.append(dirs[i])
        matrix[i][0] = ave[i]
        matrix[i][1] = minval[i]
        matrix[i][2] = std[i]
    print(matrix)
    mydf = pd.DataFrame(matrix, columns=colLabel, index=rowLabel)
    plotTable(mydf, "Summary")

    with open(_folder+"Summary.txt", "w+") as f:
        print(mydf, file=f)
        # print("Id,Obj",file=f)
        # for r in range(0,len(df)):
        # print("{0},{1}",df['Exp'][r],df['MinObj'][r],file=f)


def TuneReward():
    """
            test for tuning the reward parameters
    """
    # improveFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\\SiouxFall_TunePara\improve=3/"

    # CompareOneFolder(_folder=improveFolder,_name="improve=3")
    # exit()

    root = root_folder
    # root = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\\SiouxFall_TunePara\\"
    folder = root + "j=0\\"
    CompareOneFolder(_folder=folder, _name="j=0")
    folder = root + "j=1\\"
    CompareOneFolder(_folder=folder, _name="j=1")
    folder = root + "j=2\\"
    CompareOneFolder(_folder=folder, _name="j=2")
    folder = root + "j=3\\"
    CompareOneFolder(_folder=folder, _name="j=3")
    folder = root + "j=4\\"
    CompareOneFolder(_folder=folder, _name="j=4")
    folder = root + "j=5\\"
    CompareOneFolder(_folder=folder, _name="j=5")
    folder = root + "j=6\\"
    CompareOneFolder(_folder=folder, _name="j=6")
    folder = root + "j=7\\"
    CompareOneFolder(_folder=folder, _name="j=7")
    folder = root + "j=8\\"
    CompareOneFolder(_folder=folder, _name="j=8")


def effect_of_operators(_folder):
    """test the effect of operators
    """
    # print("Now Plot the effect of operatros")
    # OperatorFolder  = root_folder + "Tests2022/EffectOfOperators/"
    print("-----Now start to plot the effecct for each operators ------")
    print("Read folder = {0}".format(_folder))
    CompareOneFolder(_folder=_folder, _name="CompareOperator")
    print("--------------------Complete operator effects--------------------------")


def get_files():
    """generate the list of files names
    """
    pass


def plotTable(df, _name: str):
    rcolors = plt.cm.BuPu(np.full(len(df.index), 0.1))
    ccolors = plt.cm.BuPu(np.full(len(df.columns), 0.1))
    the_table = plt.table(cellText=df.values,
                          rowLabels=df.index,
                          rowColours=rcolors,
                          colColours=ccolors,
                          colLabels=df.columns,
                          rowLoc='right',
                          loc='center')
    plt.box(on=None)
    plt.gca().get_xaxis().set_visible(False)
    plt.gca().get_yaxis().set_visible(False)
    the_table.scale(1, 1.5)
    the_table.set_fontsize(12)
    plt.ion()
    plt.pause(2)
    plt.savefig(_name+".png", bbox_inches='tight', dpi=600)
    plt.close()


def plotRelation(_folder: str):
    """read and plot relationship
    plot table reference
    https://towardsdatascience.com/simple-little-tables-with-matplotlib-9780ef5d0bc4
    """
    # plot table
    BestSeed = getBestSeed(_folder)
    fn = _folder + "/DomRelation.txt"
    data = pd.read_csv(fn)
    num_row = data.shape[0]
    print(num_row)
    matrix_afterScore = []
    matrix_BeforeScore = []
    matrix_SameScore = []
    matrix_Dom = []
    for i in range(0, len(para.FailureLinks)):
        matrix_afterScore.append([-1]*len(para.FailureLinks))
        matrix_BeforeScore.append([-1]*len(para.FailureLinks))
        matrix_SameScore.append([-1]*len(para.FailureLinks))
        matrix_Dom.append([-1]*len(para.FailureLinks))
    for i in range(0, num_row):
        if (data["Seed"][i] == BestSeed):
            RowLink = data["First"][i]
            ColLink = data["Second"][i]
            AfterScore = data["AferScore"][i]
            BeforeScore = data["BeforeScore"][i]
            SameScore = data["SameScore"][i]
            DomVal = data["Dom"][i]
            DomStatus = 'None'
            if DomVal == 0:
                DomStatus = "Equal"
            if DomVal == 1:
                DomStatus = "After"
            if DomVal == 2:
                DomStatus = "Before"
            if DomVal == 3:
                DomStatus = "Same"
            RowNum = para.FailureLinks.index(RowLink)
            ColNum = para.FailureLinks.index(ColLink)
            matrix_afterScore[RowNum][ColNum] = AfterScore
            matrix_BeforeScore[RowNum][ColNum] = BeforeScore
            matrix_SameScore[RowNum][ColNum] = SameScore
            matrix_Dom[RowNum][ColNum] = DomStatus
    # print(matrix)
    df = pd.DataFrame(matrix_afterScore,
                      columns=para.FailureLinks, index=para.FailureLinks)
    plotTable(df, "AfterScore")
    df = pd.DataFrame(matrix_BeforeScore,
                      columns=para.FailureLinks, index=para.FailureLinks)
    plotTable(df, "BeforeScore")
    df = pd.DataFrame(matrix_SameScore,
                      columns=para.FailureLinks, index=para.FailureLinks)
    plotTable(df, "SameScore")
    df = pd.DataFrame(matrix_Dom, columns=para.FailureLinks,
                      index=para.FailureLinks)
    plotTable(df, "DomStatus")


if __name__ == '__main__':
    """
    """
    # effect_of_operators()
    # root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin/"

    TuneReward()
    exit()
    # OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/EffectOfOperators/"
    OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1/EffectOfOperators/"
    CompareOneFolder(_folder=OperatorFolder, _name="CompareOperator")
    # TODO Process the best solution under a certain iterations
    # SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/SelectNei/"
    SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1/SelectNei/"
    CompareOneFolder(_folder=SelectoFolder, _name="CompareSelectNei")
