"""Post process the resuls data
"""
import pandas as pd
import matplotlib.pyplot as plt
import os

from pandas.core.accessor import PandasDelegate

def CompareOneFolder(_folder:str,_name:str):
	root, dirs, files = next(os.walk(_folder), ([],[],[]))
	print(dirs)
# step 1: print the gap of operators 
# f = OperatorFolder+"\\"+dirs[0] + "\\ABC_Converge_ave.txt"
# df = pd.read_csv(f,header=None)
# print(df)
# for d in range(0,len(df)):
# 	print(df[1][d])
	NumberOfIter = 200
	allGaps = []
	for fo in range(0, len(dirs)):
		f = _folder+"\\"+dirs[fo] + "\\ABC_Converge_ave.txt"
		df = pd.read_csv(f,header=None)
		gap = []
		for j in range(0, NumberOfIter):
			gap.append(df[1][j])
		allGaps.append(gap)
		plt.plot(gap, label=dirs[fo]) 
	
	plt.ion()
	plt.legend()
	plt.pause(1)
	plt.savefig(_folder+"\\"+_name+".png", bbox_inches='tight', dpi=600)
	plt.close()

	d=[]
	for i in range(0,len(dirs)):
		d.append([str(i),allGaps[i][NumberOfIter-1]])
	df = pd.DataFrame(d, columns = ['Exp','MinObj'])
	with open(_folder+"_ObjTable.txt","w+") as f:
		print(df,file=f)
	print(df)



 
	

def TuneReward():
	"""
		test for tuning the reward parameters
	"""
	# improveFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\\SiouxFall_TunePara\improve=3/"

	# CompareOneFolder(_folder=improveFolder,_name="improve=3")
	# exit()


	root = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\\SiouxFall_TunePara\\"
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


if __name__=='__main__':
	"""
	"""
	TuneReward()
	exit()
	# OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/EffectOfOperators/"
	OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1/EffectOfOperators/"
	CompareOneFolder(_folder=OperatorFolder,_name="CompareOperator")
	# TODO Process the best solution under a certain iterations
	# SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/SelectNei/"
	SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_1/SelectNei/"
	CompareOneFolder(_folder=SelectoFolder,_name="CompareSelectNei")