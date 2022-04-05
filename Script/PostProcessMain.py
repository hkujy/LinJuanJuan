"""
    main entry of post process results data and plot
"""

import PlotHeatMap
import PostProcessDataFuns as psf
import PlotOperatorConverge

# root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022/"
root_folder = r'C:/GitCodes/2022Apr3Test/'
# plot 1 plot the heatmap for the optimal pattern


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

exit()

## unified prob
### Just copy the plot from unfolder
## adaptive probability
### Just copy the plot from ALNS folder

## change of the prob rate over all iterations
OperatorCovergeFolder = root_folder +"/9_ALNS/"
PlotOperatorConverge.change_operator_prob_over_iterations(root_folder)
# remark: need to set the nodes read
PlotHeatMap.plot_Patten_heat_map(nodes=[])

# project schedule
## TODO: plot the gant chart for the general case


# compare with the gentic algorithm
