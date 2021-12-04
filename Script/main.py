
import numpy as np
import matplotlib.pyplot as plt
import os
import sys
from shutil import copyfile
import shutil
import run
import para
import myclass as mc
import myplot


def copy_file(_from, _to):
    copyfile(_from, _to)


def copy_folder(_from, _todir):
    if os.path.exists(_todir):
        shutil.rmtree(_todir)
    shutil.copytree(_from, _todir)


def TestOneCase(_mf, _adjust, _case_id, _case_name):
    """
        Test for one case
    """
    ps = para.ParaClass()
    ps.caes_id = _case_id
    ps.adjust_abc_para(_adjust)
    ps.case_name = _case_name
    ps.print_abc_para(net="Wang")
    run.run_test(_mf)
    myplot.plot_converge(ps, _mf.root_folder +
                         "LinJuanJuan\\Output\\ABCConverge.txt")
    from_folder = _mf.root_folder + "LinJuanJuan\\OutPut"
    to_folder = _mf.root_folder + "LinJuanJuan\\Tests\\" + str(_case_id) +"_" + _case_name
    copy_folder(from_folder, to_folder)


if __name__ == "__main__":
    mf = mc.FileFolderClass()
    print("Working folder is", mf.root_folder)
    # input()
    # from_folder = mf.root_folder + "LinJuanJuan\\OutPut"
    # to_folder = mf.root_folder + "LinJuanJuan\\Tests\\" +"try"
    # copy_folder(from_folder, to_folder)
    # input()
 
    para.Copy_input_and_test_files(mf)
    adjust_para = {
        "RewardImproveGlobal": 20,
        "RewardImproveLocal": 10,
        "RewardWorse": 0,
        "ReactionFactor": 0.5,
        "SelectOperator": "ALNS"
    }
    TestOneCase(mf, adjust_para, _case_id=1, _case_name="ALNS")
    adjust_para ={ "SelectOperator":"Uni" }
    TestOneCase(mf,adjust_para,_case_id=2,_case_name="Uni")
