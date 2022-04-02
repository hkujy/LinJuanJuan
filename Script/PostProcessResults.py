"""
    main entry of post process results data and plot
"""

import PlotHeatMap
import PostProcessDataFuns as psf
import PlotOperatorConverge

root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022/"
# plot 1 plot the heatmap for the optimal pattern

# convergence the algorithm 
## each operator 
OperatorFolder  = root_folder + "/EffectOfOperators/"
psf.effect_of_operators(OperatorFolder)
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
