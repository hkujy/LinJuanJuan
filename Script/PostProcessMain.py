"""
    main entry of post process results data and plot
"""

import PlotHeatMap
import PostProcessDataFuns as psf
import PlotOperatorConverge
import para
import PlotGantt 

# root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022/"
# root_folder = r'C:/GitCodes/Res/'
# root_folder = r'C:/GitCodes/LearnByCompare/'
# root_folder = r'C:/GitCodes/RandomDemand/'
# root_folder = r'M:/LinJuan/500Iter/'
# plot 1 plot the heatmap for the optimal pattern
plot_opt_case_folder = r"M:/LinJuan/0.001ConvergeNoLearning/Operator/1_TestOp_8/"

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


def PlotFinalRelation(test_folder:str):
    """visulise the dominate relationship
    """
    # test_folder = "M:/LinJuan/0_ALNS/"
    # test_folder = "C:/GitCodes/0_ALNS/"
    bs = psf.getBestSeed(test_folder)
    print("Best Seed = {0}".format(bs))
    psf.plotRelation(test_folder)

if __name__ == "__main__":
    # EffectOfOperators()
    # CompareThree()
    # test_folder = r'C:/GitCodes/1_TestOp_8/'
    # PlotFinalRelation(test_folder)
    

    # testfolder =  root_folder + "/RemoveOperator/"
    # psf.CompareOneFolder(testfolder,"RemoveOp")


    # remark: I may need to adjust the heatmap to the dominate score map??
    # op_folder = r"M:\LinJuan\0.001ConvergeNoLearning\Operator\1_TestOp_8"
    # PlotHeatMap.plot_Patten_heat_map(para.FailureLinks,num_of_seed=para.NumOfTestSeed,_folder=op_folder)
    # PlotOperatorConverge.change_operator_prob_over_iterations(op_folder,_num_operators=9)

    # plot gantt chart
    best_seed = psf.getBestSeed(plot_opt_case_folder)
    # psf.print_best_seed_sol(plot_opt_case_folder,best_seed)
    # psf.print_best_seed_period(plot_opt_case_folder,best_seed)
    psf.plot_best_seed_period(plot_opt_case_folder)
    PlotGantt.plot_general_Gant_chart("Gantt_SiouxFall",plot_opt_case_folder,best_seed)



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
