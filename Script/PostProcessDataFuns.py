"""Post process the resuls data
"""
# from tkinter import font
# from tkinter.font import _FontDict
# from tkinter.font import _FontDict
import pandas as pd
import matplotlib.pyplot as plt
import os

from pandas.core.accessor import PandasDelegate

# root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\Tests2022/"

label_font = {'family': 'Times New Roman', 
			  'weight': 'bold',
			  'size': 12
            }

tick_font = {'family': 'Times New Roman', 
			  'weight': 'bold',
			  'size': 10
            }


def updateOperatorName(fn:str):
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

def CompareOneFolder(_folder:str,_name:str):
	root, dirs, files = next(os.walk(_folder), ([],[],[]))
	print(dirs)

	print("plot one folder case")
	print("default number of iter is 200, need to check if it is not")
	NumberOfIter = 200
	allGaps = []
	for fo in range(0, len(dirs)):
		f = _folder+"\\"+dirs[fo] + "\\ABC_Converge_ave.txt"
		df = pd.read_csv(f,header=None)
		gap = []
		for j in range(0, NumberOfIter):
			gap.append(df[1][j])
		allGaps.append(gap)
		# plt.plot(gap, label=dirs[fo]) 
		this_label=updateOperatorName(dirs[fo])
		plt.plot(gap, label=this_label) 
	plt.xlabel("No. of Iterations",fontdict=label_font)
	plt.ylabel("Ave. Objective Value",fontdict=label_font)

	plt.ion()
	plt.legend()
	plt.pause(1)
	plt.savefig(_folder+"\\"+_name+"_ave.png", bbox_inches='tight', dpi=600)
	plt.close()

	d=[]
	for i in range(0,len(dirs)):
		d.append([str(i),allGaps[i][NumberOfIter-1]])
	df = pd.DataFrame(d, columns = ['Exp','MinObj'])
	with open(_folder+"min_ObjTable.txt","w+") as f:
		print("Id,Obj",file=f)
		for r in range(0,len(df)):
			print("{0},{1}",df['Exp'][r],df['MinObj'][r],file=f)
	print(df)

	allGaps = []
	for fo in range(0, len(dirs)):
		f = _folder+"\\"+dirs[fo] + "\\ABC_Converge_min.txt"
		df = pd.read_csv(f,header=None)
		gap = []
		for j in range(0, NumberOfIter):
			gap.append(df[1][j])
		allGaps.append(gap)
		# plt.plot(gap, label=dirs[fo]) 
		this_label=updateOperatorName(dirs[fo])
		plt.plot(gap, label=this_label) 
	plt.xlabel("No. of Iterations",fontdict=label_font)
	plt.ylabel("Min. Objective Value",fontdict=label_font)

	plt.ion()
	plt.legend()
	plt.pause(1)
	plt.savefig(_folder+"\\"+_name+"_min.png", bbox_inches='tight', dpi=600)
	plt.close()


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
	CompareOneFolder(_folder=folder,_name="j=0")
	folder = root + "j=1\\"
	CompareOneFolder(_folder=folder,_name="j=1")
	folder = root + "j=2\\"
	CompareOneFolder(_folder=folder,_name="j=2")
	folder = root + "j=3\\"
	CompareOneFolder(_folder=folder,_name="j=3")
	folder = root + "j=4\\"
	CompareOneFolder(_folder=folder,_name="j=4")
	folder = root + "j=5\\"
	CompareOneFolder(_folder=folder,_name="j=5")
	folder = root + "j=6\\"
	CompareOneFolder(_folder=folder,_name="j=6")
	folder = root + "j=7\\"
	CompareOneFolder(_folder=folder,_name="j=7")
	folder = root + "j=8\\"
	CompareOneFolder(_folder=folder,_name="j=8")


def effect_of_operators(_folder):
	"""test the effect of operators
	"""
	# print("Now Plot the effect of operatros")
	# OperatorFolder  = root_folder + "Tests2022/EffectOfOperators/"
	print("-----Now start to plot the effecct for each operators ------")
	print("Read folder = {0}".format(_folder))
	CompareOneFolder(_folder=_folder,_name="CompareOperator")
	print("--------------------Complete operator effects--------------------------")

def get_files():
	"""generate the list of files names
	"""
	pass	

if __name__=='__main__':
	"""
	"""
	# effect_of_operators()
	# root_folder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin/"
	
	TuneReward()
	exit()
	# OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/EffectOfOperators/"
	OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1/EffectOfOperators/"
	CompareOneFolder(_folder=OperatorFolder,_name="CompareOperator")
	# TODO Process the best solution under a certain iterations
	# SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/SelectNei/"
	SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1/SelectNei/"
	CompareOneFolder(_folder=SelectoFolder,_name="CompareSelectNei")