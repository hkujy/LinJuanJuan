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
	for fo in range(0, len(dirs)):
		f = _folder+"\\"+dirs[fo] + "\\ABC_Converge_ave.txt"
		df = pd.read_csv(f,header=None)
		gap = []
		for j in range(0, NumberOfIter):
			gap.append(df[1][j])
		plt.plot(gap, label=dirs[fo]) 
	
	plt.ion()
	plt.legend()
	plt.pause(1)
	plt.savefig(_folder+"\\"+_name+".png", bbox_inches='tight', dpi=600)
	plt.close()
 
	
 

if __name__=='__main__':
	"""[summary]
	"""
	OperatorFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/EffectOfOperators/"
	CompareOneFolder(_folder=OperatorFolder,_name="CompareOperator")
	# TODO Process the best solution under a certain iterations
	SelectoFolder = r"C:\Users\phdji\OneDrive - Danmarks Tekniske Universitet\JuanJuanLin\SiouxFall_FailureLink_0/SelectNei/"
	CompareOneFolder(_folder=SelectoFolder,_name="CompareSelectNei")