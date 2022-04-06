
import numpy as np
# import Tkinter as tk
import matplotlib.pyplot as plt
import os
import sys
from shutil import copyfile
import shutil
import run
import para
import myclass as mc
import myplot
# import myga

# TODO: Write seed list in python
# TODO: write the script for a benchmark test of the algorithm
# Todo: write the formal test for each operator and the overall algorithm

stop_number = -1

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
    ps.adjust_para(_adjust)
    ps.case_name = _case_name
    ps.print_para()
    print("py:model input para is printed")
    run.run_test(_mf)
    myplot.plot_converge(ps, _mf.root_folder +
                         "LinJuanJuan\\Output\\ABCConverge.txt", ps.para["MaxABCIter"], "ABC")
    from_folder = _mf.root_folder + "LinJuanJuan\\OutPut"
    to_folder = _mf.root_folder + "LinJuanJuan\\Tests\\" + \
        str(_case_id) + "_" + _case_name
    copy_folder(from_folder, to_folder)


def TestGa(_mf):
    """
        call the ga test
    """
    adjust_para = {
        "UseMyOwn": "False",
    }
    ps = para.ParaClass()
    ps.caes_id = -1
    ps.adjust_para(adjust_para)
    ps.case_name = "TestGA"
    ps.print_para()
    print("ga para setting is write")
    # myga.gamain(_mf, ps)
    from_folder = _mf.root_folder + "LinJuanJuan\\OutPut"
    to_folder = _mf.root_folder + "LinJuanJuan\\Tests\\GA"
    copy_folder(from_folder, to_folder)

def TestSingleOperator(mf):
    """Function for test single operator effect
    """
    for i in range(0, 9):
        adjust_para = {
            "UseMyOwn": "True",
            # "NetworkIndex": 5,  # WangNet
            # "NetworkIndex": 3,  # SiouxFall
            "isTestSingleOp": "True",
            "SingleOpIndex": i,
            "ReactionFactor": 0.5,
            "SelectOperator": "Uni"
        }
        para.global_case_id = para.global_case_id + 1
        if para.global_case_id >= stop_number:
            TestOneCase(mf, adjust_para, _case_id=para.global_case_id,
            _case_name="TestOp_"+str(i))

def TestRewardVal(mf):
    """
        test the combination of reward value 
    """
    for j in range(4, 6):   # this change to 9
        for i in range(2, 4):  # this is change to 5
            adjust_para = {
                "UseMyOwn": "True",
                "RewardImproveGlobal": i*0.5,
                "RewardImproveLocal": i*0.5,
                "RewardWorse": 0,
                "ReactionFactor": 0.1*(j+1),
                "SelectOperator": "ALNS"
            }
            para.global_case_id = para.global_case_id + 1
            if para.global_case_id>=stop_number:
                TestOneCase(mf, adjust_para, _case_id=para.global_case_id,
                        _case_name="Test_Rect_"+str(j)+"_imporve_"+str(i))

def TestDifferntOpSelect(mf):
    """
        select different operator
    """
    adjust_para = {
        "UseMyOwn": "True",
        # "NetworkIndex": 5,  # WangNet
        # "NetworkIndex": 3,  # SiouxFall
        # "NumEmployBee": 10,
        # "NumOnlookerBee":10,
        # "MaxScountCount": 50,
        # "MaxABCIter": 200,
        "RewardImproveGlobal": 1,
        "RewardImproveLocal": 1,
        "RewardWorse": 0,
        "ReactionFactor": 0.5,
        # "SelectOperator": "ALNS"
    }
    # Notes: 2022 March 2
    # - it seems I do not need to test the ALNS, as it has been tested in other cases
    adjust_para['SelectOperator'] = "ALNS"
    para.global_case_id = para.global_case_id + 1
    TestOneCase(mf, adjust_para, _case_id=para.global_case_id,
                _case_name="ALNS")

    adjust_para['SelectOperator'] = "Uni"
    para.global_case_id = para.global_case_id + 1
    if para.global_case_id>=stop_number:
        TestOneCase(mf, adjust_para, _case_id=para.global_case_id, 
        _case_name="Uni")


def BenchmarkParadoxNet(mf):
    """
    benchmark the algorithm to see whether it gets solution
    """
    adjust_para = {
        "UseMyOwn": "True",
        "NetworkIndex": 4,  # Paradox
        # "NumEmployBee": 10,
        # "NumOnlookerBee": 10,
        # "MaxScountCount": 50,
        # "MaxABCIter": 200,
        "RewardImproveGlobal": 20,
        "RewardImproveLocal": 10,
        "RewardWorse": 0,
        "ReactionFactor": 0.5,
        "SelectOperator": "ALNS"
    }
    
    print("---------need to check---------")
    print("1. demand is 6")
    print("2. the data used is FailureLinks - Case 2.txt")

    ps = para.ParaClass()
    ps.adjust_para(adjust_para)
    ps.print_para()

    # para.global_case_id = para.global_case_id + 1
    # TestOneCase(mf, adjust_para, _case_id=para.global_case_id, _case_name="ALNS")
    adjust_para['SelectOperator'] = "Uni"
    para.global_case_id = para.global_case_id + 1
    TestOneCase(mf, adjust_para, _case_id = para.global_case_id,
                _case_name="Uni")


if __name__ == "__main__":

    para.global_case_id = -1
    if len(para.SeedPool) < para.NumOfTestSeed:
        print("PyWarning: Number of Test Seed is larger than the seed pool")
        exit()
    mf = mc.FileFolderClass()
    print("Working folder is", mf.root_folder)
    print("# notes on the test")
    remarks = input()
    with open(mf.root_folder+"LinJuanJuan\\Output\\TestRemarks.txt", "w+") as f:
        print(remarks,file=f)
 
    # step 0 print seed
    with open(mf.root_folder+"LinJuanJuan\\Input\\Seed.txt", "w+") as f:
        for i in range(0, para.NumOfTestSeed):
            print("{0}".format(para.SeedPool[i]), file=f)
        print(-1, file=f)
    print("Python seed file is printed")
    para.Copy_input_and_test_files(mf)

    # BenchmarkParadoxNet(mf)
    # TestSingleOperator(mf)
    # TestRewardVal(mf)
    TestDifferntOpSelect(mf)

    # ------------------Test GA Function
    # TestGa(mf)
    # -------------------------------
