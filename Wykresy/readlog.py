import numpy as np
from matplotlib import pyplot as plt
from matplotlib.pyplot import figure
import datetime
import os
import math
import itertools 
import scipy.interpolate
from scipy.interpolate import griddata
from matplotlib import cm

from os import listdir
from os.path import isfile, join


mypath = os.getcwd()
mypath = os.path.join(mypath, "build")
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f)) and "particlesLog" in f]

log_files_params = []

log_id=0
for f in onlyfiles:
    f= str(log_id)+'_'+f
    log_id+=1
    f = os.path.splitext(f)[0]
    f_params=f.split('_')
    log_files_params.append(f_params)

log_files_params.sort(key=lambda x: x[2]) # sort by process number

labels = [x[2]+'('+x[3]+')' for x in log_files_params]
sizes = [int(x[3]) for x in log_files_params]

fig1, ax1 = plt.subplots()
ax1.pie(sizes, labels=labels, autopct='%1.1f%%',
        shadow=True, startangle=90)
ax1.axis('equal')  
plt.title("Liczba iteracji dla każdego z procesów - OpenMPI")
# plt.show()

orderOfFiles = [x[0] for x in log_files_params]

allData = []

for i in range(0,len(log_files_params)):
    j = int(orderOfFiles[i])
    fileName = onlyfiles[j]

    content = []
    with open(os.path.join(mypath,fileName)) as f:
       content = [(line.rstrip()).split('_') for line in f]  
    allData.append(content)

allDataList = list(itertools.chain.from_iterable(allData)) 

costs = [float(x[4]) for x in allDataList]
globalMin = min(costs)
minpos = costs.index(globalMin) 

maxIter = max(sizes)

iterSortedLog = []
costHistory = [ [-1,float('inf') ] for _ in range(maxIter) ]

for i in range(0,minpos+1):    
    log = allDataList[i]
    it = int(log[0])-1
    if float(log[4]) < costHistory[it][1]:
        costHistory[it][1] = float(log[4])
        costHistory[it][0] = log[1] 

   
costs = [x[1] for x in costHistory]
iterations = range(0,maxIter)

plt.figure()
myTitle = f"Monte Carlo - OpenMPI: n={2}, l.czastek{20}, stop={0.001}"
plt.plot(iterations, costs)
# plt.xticks(np.arange(0, max(iterations)+2, 5.0))
plt.xlabel("iteracje")
plt.ylabel("koszt")
plt.xscale("log")
plt.title(myTitle)
plt.savefig("T2_MC_MPI_koszt.png",dpi=300)  

##########################################################################################


N = 100 # resolution for given range x_min - x_max

#ograniczenie 
half = 40
area_min = -half
area_max = half
x=np.linspace(area_min, area_max, N, endpoint=True)
y=np.linspace(area_min, area_max, N, endpoint=True)

meshList = list(itertools.product(x, y))

#funkcje ogolne, wielowymiarowe

def task1(particlesList):
    suma = 0
    product = 1
    
    for i in range(0,len(particlesList)):    
        suma = suma + pow(particlesList[i], 2)
        product = product * math.cos(particlesList[i] / (i + 1))

    return 1.0 / 40.0 * suma + 1 - product

def task2(particlesList):
    costFunctionValue = 0
    for i in range(0,len(particlesList)-1):    
        costFunctionValue = costFunctionValue + (math.pow((particlesList[i + 1] - math.pow(particlesList[i], 2)), 2) * 100 + math.pow((1 - particlesList[i]), 2))
    return costFunctionValue
    
#zmodyfikowane do przekrojow

def funkcja1(x,n):

    suma = np.sum(math.pow(x,2))   
    x = np.repeat(x,n)
    n = np.arange(1,n+1,1)
    x2 = x/n
    arg = [math.cos(i) for i in x2]    
    prod = np.prod(arg)   

    return 1/40 * suma + 1 - prod


def funkcja2(x, xi):
    return 100*math.pow((xi-math.pow(x,2)),2) + math.pow((1-x),2)

#przekroj resenbrocka

funk2=[]

for i in range(0,N-1):
   funk2.append(funkcja2(x[i],x[i+1]))

# plt.plot(x[0:len(funk2)], funk2)
# plt.title("Zad 2 - Funkcja rosenbrocka - przekrój")
# plt.show()

# wykres  naszej funkcji - poziomice/heatmapa oraz przekroj


plt.figure(1000)
figure(num=None, figsize=(15, 6), dpi=200, facecolor='w', edgecolor='k')

#obliczenia
#przekroj
res_2d_task1 = [task1([i]) for i in x] #TASK1
# res_2d_task2 = [task2([i]) for i in x] #TASK2

arr_task1 = np.empty((N*N,1), float)
arr_task2 = np.empty((N*N,1), float)

for i in range(0, len(meshList)):
    arr_task1[i,0] = task1(meshList[i])

for i in range(0, len(meshList)):
     arr_task2[i,0] = task2(meshList[i])


plt.subplot(121)
plt.plot(x[0:len(res_2d_task1)], res_2d_task1)
plt.title(f"{task} - przekrój x1=0")
plt.xlabel("x2")
plt.ylabel("z")

z = 0
z1 = np.reshape(arr_task1,(N,N))
z2 = np.reshape(arr_task2,(N,N))

if(task == "Zad1"):
    z = z1
else:
    z = z2

plt.subplot(122)

plt.imshow(z, vmin=z.min(), vmax=z.max(), origin='lower', extent=[x.min(), x.max(), y.min(), y.max()])
# plt.contour(x, y, z1) #TASK2
plt.title(f"{task} - wykres dla n=2")
plt.xlabel("x1")
plt.ylabel("x2")
cbar = plt.colorbar()
cbar.set_label("wartość funkcji kosztu")
# plt.savefig(f"{task}_2d_heatmap_.{ext}")  

plt.clf()

######################################################################
#WCZYTANIE WSZYSTKICH CZASTEK

# file: iteracja, id, x, y , vx, vy, cost

fileNameParticlesSplit = fileNameParticles.split("_") #TODO - put into function

name = fileNameParticlesSplit[0]
partNum = int(fileNameParticlesSplit[1])
dim = int(fileNameParticlesSplit[2])
stop = float(fileNameParticlesSplit[3])
date = fileNameParticlesSplit[4]
time = fileNameParticlesSplit[5]
taskType = fileNameParticlesSplit[6]


pyDate = stringToDate(date+time)

with open("logs/"+fileNameParticles, "r") as f:
    logParticles = f.readlines()

logParticles = [x.rstrip() for x in logParticles] 

cols= len(logParticles[0].split(',')) #iteracja, id, x, y , vx, vy, cost
arr = np.empty((0,cols), float)

for i in range (0,len(logParticles)):
    if(i>iterLimit):
        break
    lineSplit = logParticles[i].split(",")
    lineFloat = [float(x) for x in lineSplit]
    arr = np.append(arr, np.array([lineFloat]), axis=0)

costMax =max(arr[:,6])
rows = arr.shape[0]
iterations =int(rows / partNum)
arr = np.reshape(arr,(iterations, partNum, cols))

# RYSOWANIE

plt.figure()
marker_size=50
plt.xlim(-40, 40)
plt.ylim(-40, 40)
plt.scatter(arr[0,:,2], arr[0,:,3], marker_size,c=arr[0,:,6], vmin = 0, vmax = costMax)
plt.title(f"{task} - {alg}: n={dim}, l.czastek{partNum}")
plt.xlabel("x1")
plt.ylabel("x2")
cbar= plt.colorbar()
cbar.set_label("koszt czasteczki", labelpad=+5)
plt.savefig(f"{alg}_{task}_startPositions.{ext}")    

# wykresy do zrobienia gifa

for i in range(iterations):
    plt.clf()
    plt.xlim(-40, 40)
    plt.ylim(-40, 40)
    plt.xticks(np.arange(-40,50,10))
    plt.yticks(np.arange(-40,50,10))
    plt.scatter(arr[i,:,2], arr[i,:,3], marker_size, c=arr[i,:,6],vmin=0, vmax=costMax)
    plt.title(f"{task} - {alg}: n={dim}, l.czastek{partNum}, iter: {i}")
    plt.xlabel("x1")
    plt.ylabel("x2")
    cbar= plt.colorbar()
    cbar.set_label("koszt")
    plt.savefig(f"plots/plot_{i}.png")

#os.system("convert -delay 20 -loop 0 plots/plot_{0..33}.png plots/scatter4.gif") 

marker_size=25

plt.figure()
for i in range(iterations):
    # plt.clf()
    plt.xlim(-40, 40)
    plt.ylim(-40, 40)
    plt.xticks(np.arange(-40,50,10))
    plt.yticks(np.arange(-40,50,10))
    plt.scatter(arr[i,:,2], arr[i,:,3], marker_size, c=arr[i,:,6],vmin=0, vmax=costMax)


    # plt.savefig(f"plots/plot_{i}.png")
plt.title(f"{alg} n={dim}, l.czastek{partNum}, l.iteracji{iterations}")
plt.xlabel("x1")
plt.ylabel("x2")
cbar= plt.colorbar()
cbar.set_label("koszt")
plt.savefig(f"{alg}_{task}_scatter_allIters.{ext}")    

# get positions of particles with certain id or certain ids

def getBestSolutionHistory(dataArray, idVector):
    for i in range(0, (dataArray.shape)[0]):        
        array = dataArray[i][np.where(dataArray[i][:,1] == idVector[i])]
        global particlesBestArray
        particlesBestArray = np.append(particlesBestArray, array, axis=0)



def getParticleHistory(dataArray, id):
    for i in range(0, (dataArray.shape)[0]):
        array = dataArray[i][np.where(dataArray[i][:,1] == id)]
        global particlesArray
        particlesArray = np.append(particlesArray, array, axis=0)

# particlesArray = np.empty((0,cols), float)
# getParticleHistory(arr,5)    

# particlesBestArray = np.empty((0,cols), float)
# getBestSolutionHistory(arr,bestIds)

marker_size = 50
#TASK1

#TRAJEKTORIE WSZYSTKIE

for i in range (0,partNum):

    particlesArray = np.empty((0,cols), float)
    getParticleHistory(arr,i)  
    fig = plt.figure(i)
    plt.clf()
    plt.xlim(-40, 40)
    plt.ylim(-40, 40)
    plt.xticks(np.arange(-40,50,10))
    plt.yticks(np.arange(-40,50,10))
    plt.imshow(z, vmin=z.min(), vmax=z.max(), origin='lower', extent=[x.min(), x.max(), y.min(), y.max()], alpha=0.35, zorder = 0)
    plt.plot(particlesArray[:,2],particlesArray[:,3],'b', zorder=1)
    plt.scatter(particlesArray[:,2],particlesArray[:,3], marker_size, c=particlesArray[:,6],vmin=0, vmax=costMax, zorder=2)
    plt.title(f"{alg} n={dim}, id.czastki{i}, l.iteracji{iterations}")
    plt.xlabel("x1")
    plt.ylabel("x2")
    cbar= plt.colorbar()
    cbar.set_label("koszt")
    plt.savefig(f"trajectories/{task}_{alg}_plot_{i}.{ext}")


#BEST particles for solution    

particlesBestArray = np.empty((0,cols), float)
getBestSolutionHistory(arr,bestIds)  
fig = plt.figure()
plt.clf()
plt.xlim(-40, 40)
plt.ylim(-40, 40)
plt.xticks(np.arange(-40,50,10))
plt.yticks(np.arange(-40,50,10))
plt.imshow(z, vmin=z.min(), vmax=z.max(), origin='lower', extent=[x.min(), x.max(), y.min(), y.max()], alpha=0.35, zorder = 0)
plt.plot(particlesBestArray[:,2],particlesBestArray[:,3],'b', zorder=1)
plt.scatter(particlesBestArray[:,2],particlesBestArray[:,3], marker_size, c=particlesBestArray[:,6],vmin=0, vmax=costMax, zorder=2)
plt.title(f"{alg} n={dim}, najlepsze czastki dla iteracji, l.iteracji={iterations}")
plt.xlabel("x1")
plt.ylabel("x2")
cbar= plt.colorbar()
cbar.set_label("koszt")
plt.savefig(f"trajectories/{task}_{alg}_plot_BEST.{ext}")


#os.system("ffmpeg -r 5 -i plots/plot_%d.png -vcodec mpeg4 -y plots/scatter3.mp4")
#os.system("convert -delay 20 -loop 0 plots/plot_{0..33}.png plots/scatter4.gif") 
gifCommand = "convert -delay 20 -loop 0 plot_{0..33}.png MC_zad1_ANIM.gif"