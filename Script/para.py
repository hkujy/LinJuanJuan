import os
import sys
import shutil
import myclass as mc
from shutil import copyfile
import shutil
import logging

isShowFig = False
# isShowFig = True
isRunTest = True
# isRunTest = False
# isDebug = True
isDebug = False
# SeedPool = [4601, 6677, 2887, 6421, 8471, 512, 3547, 2614, 8146, 
#             9629, 8266, 3045, 9770, 3048, 6705, 9591, 7905, 8223, 9044, 1091,
#             1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,16,18,19,20]
SeedPool = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,16,18,19,20]
NumOfTestSeed = 2
# FailureLinks = [0, 6, 19, 22, 26, 27, 34, 38, 42, 55, 59, 73]
FailureLinks = [0,2,6,12,16,19,22,26,27,29,31,34,42,50,55,59]

NumofIter = 50
global_case_id = 0

class ParaClass(object):
    """parameter class
    """
    def __init__(self):
        self.caes_id = 0
        self.case_name = "null"
        self.para = {
            "isUseMyOwn": "True",
            "networkIndex": 3,   # SiouxFall
            # "NetworkIndex":4,   # Paradox
            # "NetworkIndex": 5,   # Wang
            #  "UseMyOwn": "False",
            # "CompareScoreMethod":"Ave",
            "CompareScoreMethod":"Total",
            "isTestSingleOp": "False",
            "SingleOpIndex": -1,
            "NumEmployBee": 10,
            "NumOnlookerBee":10,
            "MaxScountCount": 50,
            "MaxABCIter": NumofIter,
            "MaxGAIter": 5,
            "RewardImproveGlobal": 1,
            "RewardImproveLocal": 1,
            "RewardWorse": 0,
            "ReactionFactor": 0.5,
            "SelectOperator": "ALNS",
            "oneDimEps": 0.0000001,
            "epsUE": 0.01,
            "maxIterUE": 500
        }

    def print_para(self):
        """
        """
        if self.para["networkIndex"] == 3:
            with open("..//InPut//SiouxFallsNetwork//ABCPara.txt", "w+") as f:
                for key, value in self.para.items():
                    print("{0},{1}".format(key, value), file=f)
        elif self.para["networkIndex"] == 4:
            with open("..//InPut//ParadoxNet//ABCPara.txt", "w+") as f:
                for key, value in self.para.items():
                    print("{0},{1}".format(key, value), file=f)
        elif self.para["networkIndex"] == 5:
            with open("..//InPut//WangNetwork//ABCPara.txt", "w+") as f:
                for key, value in self.para.items():
                    print("{0},{1}".format(key, value), file=f)
        else:
            logging.debug(
                "Input network and model index is not specified for printing ABC parameters")

        with open("..//InPut//ModelPara.txt", "w+") as f:
            print("networkIndex,{0}".format(self.para["networkIndex"]), file=f)
            print("isTestSingleOp,{0}".format( self.para['isTestSingleOp']), file=f)
            print("SingleOpIndex,{0}".format(self.para['SingleOpIndex']),file=f)
            print("isUseMyOwn,{0}".format(self.para["isUseMyOwn"]), file=f)
            print("oneDimEps,{0}".format(self.para["oneDimEps"]), file=f)
            print("epsUE,{0}".format(self.para["epsUE"]), file=f)
            print("maxIterUE,{0}".format(self.para["maxIterUE"]), file=f)

    def adjust_para(self, _adjust):
        """
            adjust the default parameters
        """
        for key, value in _adjust.items():
            print("adjust key = {0}, val = {1}".format(key, value))
            self.para[key] = value


def SFileToDFile(sourcefile, fileclass, destinationfile):
    if os.path.exists(destinationfile):
        pass
    else:
        os.mkdir(destinationfile)
    # 遍历目录和子目录
    for filenames in os.listdir(sourcefile):
        # 取得文件或文件名的绝对路径
        filepath = os.path.join(sourcefile, filenames)
        # 判断是否为文件夹
        if os.path.isdir(filepath):
            # 如果是文件夹，重新调用该函数
            SFileToDFile(filepath, fileclass, destinationfile)
        # 判断是否为文件
        elif os.path.isfile(filepath):
            # 如果该文件的后缀为用户指定的格式，则把该文件复制到用户指定的目录
            if filepath.endswith(fileclass):
                #dirname = os.path.split(filepath)[-1]
                # 给出提示信息Script
                # print('Copy %s'% filepath +' To ' + destinationfile)
                # 复制该文件到指定目录
                shutil.copy(filepath, destinationfile)


def Copy_input_and_test_files(mf):
    """
        copy files and back the tested code
        1. copy cpp files
        2. copy input mms data files
    """
    SFileToDFile(sourcefile=mf.root_folder+"NetworkResilience\\", fileclass='.cpp',
                 #  destinationfile=mp.root_folder+"\\IPTOP\\IOPT\\OutPut\\BackupCpp\\")
                 destinationfile=mf.root_folder+"Output\\BackupCpp\\")

    SFileToDFile(sourcefile=mf.root_folder+"NetworkResilience\\", fileclass='.h',
                 #  destinationfile=mp.root_folder+"\\IPTOP\\IOPT\\OutPut\\BackupCpp\\")
                 destinationfile=mf.root_folder+"Output\\BackupCpp\\")

    SFileToDFile(sourcefile=mf.root_folder+"Script\\", fileclass='.py',
                 #  destinationfile=mp.root_folder+"\\IPTOP\\IOPT\\Output\\BackupScript\\")
                 destinationfile=mf.root_folder+"Output\\BackupScript\\")
