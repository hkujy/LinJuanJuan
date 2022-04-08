# %%
# Importing the matplotlib.pyplot
from logging import PlaceHolder
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import numpy as np
from pandas.core import base
import matplotlib as mpl  
import seaborn as sns
import pandas as pd

def plotNetworPerformance(opt_sheet_name:str,rank_sheet_name:str):
	# sns.set_style('darkgrid')
	obj = pd.read_excel("./PlotSol.xlsx",sheet_name=opt_sheet_name)
	plt.plot(obj['Period'],obj['Cost'])
	plt.xlabel('Time',fontsize =12,fontname ='Times New Roman',fontweight='bold')
	plt.ylabel('Total Travel Cost',fontsize = 12,fontname='Times New Roman', fontweight ='bold')
	# plt.show()
	plt.savefig('cost.png',bbox_inches='tight',dpi=600)
	plt.close()

	base = float(obj['Cost'][0])
	ratio = []
	for i in range(0,len(obj)):
		r = 1.0-(float(obj['Cost'][i])-base)/base
		print("cost = {0},base = {1}, r={2}".format(obj['Cost'][i],base,r))
		ratio.append(r)

	plt.plot(obj['Period'],ratio)
	plt.xlabel('Time',fontsize =12,fontname ='Times New Roman',fontweight='bold')
	plt.ylabel('Network Performance',fontsize = 12,fontname='Times New Roman', fontweight ='bold')
	# plt.show()
	plt.savefig('Cost_Ratio.png',bbox_inches='tight',dpi=600)
	plt.close()

	base= float(obj['UNPM'][0])
	ratio = []
	for i in range(0,len(obj)):
		r = float(obj['UNPM'][i])/base
		ratio.append(r)

	plt.ylim([0.6, 1.2])
	plt.xlim([0,10])
	plt.plot(obj['Period'],ratio,label="Optimal schedule")
	plt.xlabel('Time',fontsize =12,fontname ='Times New Roman',fontweight='bold')
	plt.ylabel('Network Performance',fontsize = 12,fontname='Times New Roman', fontweight ='bold')
 
	# xtick = plt.gca().get_xticks()
	ytick = plt.gca().get_yticks()
	xtick = [0,1,2,3,4,5,6,7,8,9,10]
	plt.gca().xaxis.set_major_locator(mticker.FixedLocator(xtick))
	plt.gca().set_xticklabels(xtick, fontsize=10,fontname='Times New Roman')
	plt.gca().set_yticklabels(['{:.0f}%'.format(x*100) for x in ytick], fontsize=10,fontname='Times New Roman')
	xmajorFormatter = plt.FormatStrFormatter('%.0f')
	plt.gca().xaxis.set_major_formatter(xmajorFormatter)
	
	# plt.savefig(opt_sheet_name+'.png',bbox_inches='tight',dpi=600)
	# plt.show()
	if len(rank_sheet_name)>2:
		obj = pd.read_excel("./PlotSol.xlsx",sheet_name=rank_sheet_name)
		base= float(obj['UNPM'][0])
		ratio = []
		for i in range(0,len(obj)):
			r = float(obj['UNPM'][i])/base
			ratio.append(r)

		plt.ylim([0.6, 1.2])
		plt.xlim([0,10])
		plt.plot(obj['Period'],ratio,label ="Ranking-based schedule")
		plt.xlabel('Time',fontsize =12,fontname ='Times New Roman',fontweight='bold')
		plt.ylabel('Network Performance',fontsize = 12,fontname='Times New Roman', fontweight ='bold')

	# xtick = plt.gca().get_xticks()
		ytick = plt.gca().get_yticks()
		xtick = [0,1,2,3,4,5,6,7,8,9,10]
		plt.gca().xaxis.set_major_locator(mticker.FixedLocator(xtick))
		plt.gca().set_xticklabels(xtick, fontsize=10,fontname='Times New Roman')
		plt.gca().set_yticklabels(['{:.0f}%'.format(x*100) for x in ytick], fontsize=10,fontname='Times New Roman')

		xmajorFormatter = plt.FormatStrFormatter('%.0f')
		# ymajorFormatter = plt.FormatStrFormatter('%.2f')
		plt.gca().xaxis.set_major_formatter(xmajorFormatter)
		# plt.gca().yaxis.set_major_formatter(ymajorFormatter)
		plt.legend(prop={'family':'Times New Roman', 'size':12},
			bbox_to_anchor=(0.5, 1.12), loc='upper center', ncol=2)
		# plt.show()
		plt.savefig('CompareUnpmRatio.png',bbox_inches='tight',dpi=600)
	else:
		plt.savefig(opt_sheet_name+'_netPer_.png',bbox_inches='tight',dpi=600)

def plotGant(Case:str,StartPeriod, TotalLength):
	sol = pd.read_excel('.\PlotSol.xlsx',sheet_name=Case)
	numOfLink = sol.shape[0]
	widthOfEachBar = 3
	gapBetweenBar = 3
	yLim = (widthOfEachBar+gapBetweenBar)*numOfLink + gapBetweenBar
	ylabel = []
	ytick = []
	for i in range(0,numOfLink):
		ylabel.append(str(sol['Link'][i]))
		ytick.append(2*gapBetweenBar+i*(widthOfEachBar+gapBetweenBar)-0.5*widthOfEachBar)
	print(ylabel)
	print(ytick)
	sns.set_style('darkgrid')
	
	# Declaring a figure "gnt"
	fig, gnt = plt.subplots()

	# Setting Y-axis limits
	gnt.set_ylim(0, yLim)

	TotalLenthOfX = TotalLength*100
	NumOfPeriod = TotalLength   # number of recover
	LengthOfEachPeriod = int(TotalLenthOfX/NumOfPeriod)
	# Setting X-axis limits
	gnt.set_xlim(StartPeriod*100, StartPeriod*100+TotalLenthOfX)

	# Setting labels for x-axis and y-axis
	gnt.set_xlabel('Restoration Period',fontsize=12,fontname='Times New Roman',fontweight='bold')
	gnt.set_ylabel('Link',fontsize=12,fontname='Times New Roman', fontweight='bold')

	set_xtick = []
	set_xtick_lable = []
	for t in range(StartPeriod,StartPeriod+NumOfPeriod+1):
		set_xtick_lable.append(str(t))
		set_xtick.append(t*LengthOfEachPeriod)
	gnt.set_xticks(set_xtick)
	xmajorFormatter = plt.FormatStrFormatter('%.0f')
	ymajorFormatter = plt.FormatStrFormatter('%.0f')
	gnt.xaxis.set_major_formatter(xmajorFormatter)
	gnt.yaxis.set_major_formatter(ymajorFormatter)
	# Setting ticks on y-axis
	gnt.set_yticks(ytick)
	# Labelling tickes of y-axis
	gnt.set_yticklabels(ylabel,fontsize=10,fontname='Times New Roman')
	gnt.set_xticklabels(set_xtick_lable,fontsize=10,fontname='Times New Roman')

	# in the following plot each bar
	print(sol)
	for i in range(0,len(sol)):
		print("link i = {0}".format(sol['Link'][i]))
		st = int(sol['St'][i])
		et = int(sol['Et'][i])
		print("start = {0}".format(st))
		print("end = {0}".format(et))
		x_start = st*LengthOfEachPeriod
		x_end = (et-st)*LengthOfEachPeriod
		print("{0},{1}".format(x_start, x_end))
		y_start = ytick[i]-0.5*widthOfEachBar
		gnt.broken_barh([(x_start, x_end)], (y_start, widthOfEachBar), facecolors =('tab:blue'))

	plt.savefig(Case+'.png',bbox_inches='tight',dpi=600)
	plt.show()
	plt.close()
	# exit()


def plot_general_Gant_chart(Case:str):
	""""
	plot general gantt chart
	"""
	sol = pd.read_csv("./PrintSols.txt")
	numOfLink = sol.shape[0]
	st = []
	et = []
	for l in range(0, numOfLink):
		st.append(sol['St'][l]+1)
		et.append(sol['Et'][l]+1)
	
	StartPeriod = min(st)
	TotalLength = max(et) - StartPeriod

	widthOfEachBar = 3
	gapBetweenBar = 3
	yLim = (widthOfEachBar+gapBetweenBar)*numOfLink + gapBetweenBar
	ylabel = []
	ytick = []
	for i in range(0,numOfLink):
		ylabel.append(str(sol['Link'][i]))
		ytick.append(2*gapBetweenBar+i*(widthOfEachBar+gapBetweenBar)-0.5*widthOfEachBar)
	print(ylabel)
	print(ytick)
	sns.set_style('darkgrid')
	
	# Declaring a figure "gnt"
	fig, gnt = plt.subplots()

	# Setting Y-axis limits
	gnt.set_ylim(0, yLim)

	TotalLenthOfX = TotalLength*100
	NumOfPeriod = TotalLength   # number of recover
	LengthOfEachPeriod = int(TotalLenthOfX/NumOfPeriod)
	# Setting X-axis limits
	gnt.set_xlim(StartPeriod*100, StartPeriod*100+TotalLenthOfX)

	# Setting labels for x-axis and y-axis
	gnt.set_xlabel('Restoration Period',fontsize=12,fontname='Times New Roman',fontweight='bold')
	gnt.set_ylabel('Link',fontsize=12,fontname='Times New Roman', fontweight='bold')

	set_xtick = []
	set_xtick_lable = []
	for t in range(StartPeriod,StartPeriod+NumOfPeriod+1):
		set_xtick_lable.append(str(t))
		set_xtick.append(t*LengthOfEachPeriod)
	gnt.set_xticks(set_xtick)
	xmajorFormatter = plt.FormatStrFormatter('%.0f')
	ymajorFormatter = plt.FormatStrFormatter('%.0f')
	gnt.xaxis.set_major_formatter(xmajorFormatter)
	gnt.yaxis.set_major_formatter(ymajorFormatter)
	# Setting ticks on y-axis
	gnt.set_yticks(ytick)
	# Labelling tickes of y-axis
	gnt.set_yticklabels(ylabel,fontsize=10,fontname='Times New Roman')
	gnt.set_xticklabels(set_xtick_lable,fontsize=10,fontname='Times New Roman')

	# in the following plot each bar
	print(sol)
	for i in range(0,len(sol)):
		x_start = st[i]*LengthOfEachPeriod
		x_end = (et[i]-st[i])*LengthOfEachPeriod
		y_start = ytick[i]-0.5*widthOfEachBar
		gnt.broken_barh([(x_start, x_end)], (y_start, widthOfEachBar), facecolors =('tab:blue'))

	plt.savefig(Case+'.png',bbox_inches='tight',dpi=600)
	plt.show()
	plt.close()





if __name__ == '__main__':

	# plotNetworPerformance(opt_sheet_name='objOpt',rank_sheet_name='objRank')
	# plotGant(Case="PlotCase1",StartPeriod=3,TotalLength=6)
	#*********************************#
	# plot gantt chart for the paradox network
	plotGant(Case="LowDemand",StartPeriod=3,TotalLength=5)
	plotGant(Case="HighDemand",StartPeriod=3,TotalLength=5)
	plot_general_Gant_chart("Gantt_SiouxFall")
	# exit()
	#*********************************#
	# plotNetworPerformance(opt_sheet_name='Case1Obj',rank_sheet_name='')
	# plotNetworPerformance(opt_sheet_name='Case1Obj',rank_sheet_name='Case5Obj')
### Notes: the following codes are reference for plot the gant chart
# # Setting labels for x-axis and y-axis
# gnt.set_xlabel('Period')
# gnt.set_ylabel('Link')

# # Setting ticks on y-axis
# gnt.set_yticks([7.5, 17.5, 27.5, 37.5, 47.5, 57.5])
# # Labelling tickes of y-axis
# gnt.set_yticklabels(ylabel)

# set_xtick = []
# set_xtick_lable = []
# for t in range(0,NumOfPeriod+1):
# 	set_xtick_lable.append(str(t))
# 	set_xtick.append(t*LengthOfEachPeriod)
# gnt.set_xticks(set_xtick)
# gnt.set_xticklabels(set_xtick_lable)

# # Setting graph attribute
# gnt.grid(False)

# # Declaring a bar in schedule
# gnt.broken_barh([(40, 50)], (27.5, 5), facecolors =('tab:blue'))

# # Declaring multiple bars in at same level and same width
# gnt.broken_barh([(110, 10), (150, 10)], (17.5, 5),
# 						 facecolors ='tab:blue')

# gnt.broken_barh([(10, 50), (100, 20), (130, 10)], (7.5, 5),
# 								  facecolors =('tab:red'))

# # y = np.linspace(20,30,20)
# # x =np.linspace(1,150,20)

# plt.show()
# plt.savefig("gantt1.png")


