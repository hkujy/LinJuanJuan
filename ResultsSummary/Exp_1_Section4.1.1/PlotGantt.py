# %%
# Importing the matplotlib.pyplot
from logging import PlaceHolder
import matplotlib.pyplot as plt
import numpy as np
from pandas.core import base
import matplotlib as mpl  
import seaborn as sns
import pandas as pd

sol = pd.read_excel('.\PlotSol.xlsx',sheet_name='sol')
# 
obj = pd.read_excel("./PlotSol.xlsx",sheet_name='objOpt')
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

plt.ylim([0.5, 1.5])
plt.xlim([0,15])
plt.plot(obj['Period'],ratio,label="Optimal schedule")
plt.xlabel('Time',fontsize =12,fontname ='Times New Roman',fontweight='bold')
plt.ylabel('Network Performance',fontsize = 12,fontname='Times New Roman', fontweight ='bold')
# plt.show()
# plt.savefig('unpm_Optimal_ratio.png',bbox_inches='tight',dpi=600)


obj = pd.read_excel("./PlotSol.xlsx",sheet_name='objRank')
base= float(obj['UNPM'][0])
ratio = []
for i in range(0,len(obj)):
    r = float(obj['UNPM'][i])/base
    ratio.append(r)

plt.ylim([0.4, 1.6])
plt.xlim([0,15])
plt.plot(obj['Period'],ratio,label ="Ranking-based schedule")
plt.xlabel('Time',fontsize =12,fontname ='Times New Roman',fontweight='bold')
plt.ylabel('Network Performance',fontsize = 12,fontname='Times New Roman', fontweight ='bold')

xtick = plt.gca().get_xticks()
ytick = plt.gca().get_yticks()
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


exit()

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

TotalLenthOfX = 600
NumOfPeriod = 6   # number of recover
LengthOfEachPeriod = int(TotalLenthOfX/NumOfPeriod)
# Setting X-axis limits
gnt.set_xlim(0, TotalLenthOfX)

# Setting labels for x-axis and y-axis
gnt.set_xlabel('Restoration Period',fontsize=12,fontname='Times New Roman',fontweight='bold')
gnt.set_ylabel('Link',fontsize=12,fontname='Times New Roman', fontweight='bold')

set_xtick = []
set_xtick_lable = []
for t in range(0,NumOfPeriod+1):
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
# print(sol)
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


plt.savefig('gant.png',bbox_inches='tight',dpi=600)
plt.show()
exit()

# Setting labels for x-axis and y-axis
gnt.set_xlabel('Period')
gnt.set_ylabel('Link')

# Setting ticks on y-axis
gnt.set_yticks([7.5, 17.5, 27.5, 37.5, 47.5, 57.5])
# Labelling tickes of y-axis
gnt.set_yticklabels(ylabel)

set_xtick = []
set_xtick_lable = []
for t in range(0,NumOfPeriod+1):
	set_xtick_lable.append(str(t))
	set_xtick.append(t*LengthOfEachPeriod)
gnt.set_xticks(set_xtick)
gnt.set_xticklabels(set_xtick_lable)

# Setting graph attribute
gnt.grid(False)

# Declaring a bar in schedule
gnt.broken_barh([(40, 50)], (27.5, 5), facecolors =('tab:blue'))

# Declaring multiple bars in at same level and same width
gnt.broken_barh([(110, 10), (150, 10)], (17.5, 5),
						 facecolors ='tab:blue')

gnt.broken_barh([(10, 50), (100, 20), (130, 10)], (7.5, 5),
								  facecolors =('tab:red'))

# y = np.linspace(20,30,20)
# x =np.linspace(1,150,20)

plt.show()
plt.savefig("gantt1.png")


