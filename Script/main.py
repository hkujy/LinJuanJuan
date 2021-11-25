
import numpy as np
import matplotlib.pyplot as plt
import os,sys
from shutil import copyfile
import shutil
import run
import para
import myclass as mc


if __name__ == "__main__":
    mf = mc.FileFolderClass()
    print("Working folder is", mf.root_folder)
    para.Copy_input_and_test_files(mf)
    run.run_test(mf)

    # my_files.ini(para.root_folder+"\\mm\\Output\\ISTTT24")
    # test_paras = para.ini_para() 
    # para.print_para_setting(test_paras,my_files.test_para)
    # run.run_test()
    # if para.test_case['IsPrintAllFiles'] is "Yes":
    #     # (gap,edges,paths,modes) = rd.read_data_main(my_files)
    #     gap = rd.read_gap(my_files.gap_file)
    #     mpt.plot_converge(gap)
    #     # mpt.plot_main(gap,edges,paths,modes)
    # sys.exit("Complete and good luck")
    # pass