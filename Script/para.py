import os
import sys
import shutil
import myclass as mc
from shutil import copyfile
import shutil
import logging

isRunTest = True
# isRunTest = False
# isDebug = True
isDebug = False


class ParaClass(object):
    """parameter class
    """

    def __init__(self):
        self.caes_id = 0
        self.case_name = "null"
        self.para = {"NumEmployBee": 10,
                     "NumOnlookerBee": 10,
                     "MaxScountCount": 50,
                     "MaxABCIter": 500,
                     "RewardImproveGlobal": 10,
                     "RewardImproveLocal": 10,
                     "RewardWorse": 0,
                     "ReactionFactor": 0.5,
					 "SelectOperator":"ALNS"
					#  "SelectOperator":"Uni"
                     }

    def print_abc_para(self, net=""):
        """
        """
        if net is "Wang":
            with open("..//InPut//WangNetwork//ABCPara.txt", "w+") as f:
                for key, value in self.para.items():
                    print("{0},{1}".format(key, value), file=f)
        else:
            logging.debug("Input network is not specified for print ABC para")

    def adjust_abc_para(self, _adjust):
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
    SFileToDFile(sourcefile=mf.root_folder+"LinJuanJuan\\NetworkResilience\\", fileclass='.cpp',
                 #  destinationfile=mp.root_folder+"\\IPTOP\\IOPT\\OutPut\\BackupCpp\\")
                 destinationfile=mf.root_folder+"LinJuanJuan\\Output\\BackupCpp\\")

    SFileToDFile(sourcefile=mf.root_folder+"LinJuanJuan\\NetworkResilience\\", fileclass='.h',
                 #  destinationfile=mp.root_folder+"\\IPTOP\\IOPT\\OutPut\\BackupCpp\\")
                 destinationfile=mf.root_folder+"LinJuanJuan\\Output\\BackupCpp\\")

    SFileToDFile(sourcefile=mf.root_folder+"LinJuanJuan\\Script\\", fileclass='.py',
                 #  destinationfile=mp.root_folder+"\\IPTOP\\IOPT\\Output\\BackupScript\\")
                 destinationfile=mf.root_folder+"LinJuanJuan\\Output\\BackupScript\\")
