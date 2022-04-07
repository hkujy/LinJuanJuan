"""
    main entry of post process results data and plot
"""

import PlotHeatMap
import PostProcessDataFuns as psf
import PlotOperatorConverge

# root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022/"
# root_folder = r'C:/GitCodes/2022Apr3Test/'
root_folder = r'C:/GitCodes/RandomDemand/'
# plot 1 plot the heatmap for the optimal pattern

FailureLinks = [0, 6, 19, 22, 26, 27, 34, 38, 42, 55, 59, 73]
def EffectOfOperators():
    # convergence the algorithm 
    ## each operator 
    OperatorFolder  = root_folder + "/Operator/"
    psf.effect_of_operators(OperatorFolder)


def CompareThree():
    """
        compare three cases 
        1: Single operator 2. Uni. 2 adaptive
    """
    OperatorFolder  = root_folder + "/CompareThree/"
    psf.CompareOneFolder(OperatorFolder,"CompareThree")


if __name__ == "__main__":
    # EffectOfOperators()
    CompareThree()

    # testfolder =  root_folder + "/RemoveOperator/"
    # psf.CompareOneFolder(testfolder,"RemoveOp")


    # op_folder = root_folder +"/9_ALNS/"
    # PlotHeatMap.plot_Patten_heat_map(FailureLinks,num_of_seed=20,_folder=op_folder)
    # PlotOperatorConverge.change_operator_prob_over_iterations(op_folder,_num_operators=9)

    pass

exit()

## unified prob
### Just copy the plot from unfolder
## adaptive probability
### Just copy the plot from ALNS folder

## change of the prob rate over all iterations
OperatorCovergeFolder = root_folder +"/9_ALNS/"
PlotOperatorConverge.change_operator_prob_over_iterations(root_folder)
# remark: need to set the nodes read
PlotHeatMap.plot_Patten_heat_map(set_fail_links=[])

# project schedule
## TODO: plot the gant chart for the general case


# compare with the gentic algorithm
