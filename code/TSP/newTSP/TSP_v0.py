'''
New GA for TSP
Version 0
Incoross: merge
GeneTable: random
GeneToPerformance confilicts: sequencial search / random selection
'''

import numpy as np
from numpy import random
from math import sqrt

class TSP():
    def __init__(self, city_num, filename):
        self.city_num = city_num
        self.popsize = 300
        self.hybridrate = 1.0
        self.mutationrate = 0.1
        self.genaration = 30000
        self.performance = []
        self.fit = []
        self.lastbestind = []
        self.location = []
        self.distance = [[0 for i in range(self.city_num)] for j in range(self.city_num)]

        #生成显隐性基因表
        self.genTable = np.arange(self.city_num)
        np.random.shuffle(self.genTable)
        for i in range(self.city_num - 1):
            row = np.arange(self.city_num)
            np.random.shuffle(row)
            self.genTable = np.row_stack((self.genTable, row))
        #print(self.genTable)

        #生成种群
        self.pop = []
        for i in range(self.popsize):
            ind = []
            ind1 = np.arange(self.city_num)
            np.random.shuffle(ind1)
            ind2 = np.arange(self.city_num)
            np.random.shuffle(ind2)
            ind.append(ind1)
            ind.append(ind2)
            self.pop.append(ind)
        self.pop = np.array(self.pop)
        #print(self.pop)

        #读取城市数据
        f = open(filename, 'r')
        for i in range(self.city_num):
            line = f.readline()
            line = line.split()
            self.location.append([float(line[0]), float(line[1])])
        self.location = np.array(self.location)
        #print(self.location)

        #计算城市间距离
        for i in range(self.city_num):
            for j in range(self.city_num):
                dis = (self.location[i, 0]-self.location[j, 0])**2 + (self.location[i, 1]-self.location[j, 1])**2
                self.distance[i][j] = sqrt(dis)
        self.distance = np.array(self.distance)
        #print(self.distance)

    def get_performance(self):
        self.performance = []
        for i in range(self.popsize):
            self.performance.append(self.get_ind_perfor(i).copy())
        self.performance = np.array(self.performance)
        #print(self.performance)

    def get_ind_perfor(self, id):
        perfor = []
        exist = [False for i in range(self.city_num)]   #If the genes already exist in performance
        conflict = []       #The index of conflicted genes
        remain = []         #The genes not exist

        #get the origin performance with conflicted genes
        for i in range(self.city_num):
            if (self.genTable[i, self.pop[id, 0, i]] < self.genTable[i, self.pop[id, 1, i]]):
                perfor.append(self.pop[id, 0, i])
            else:
                perfor.append(self.pop[id, 1, i])
            if (exist[perfor[i]] == False):
                exist[perfor[i]] = True
            else:
                conflict.append(i)

        for i in range(self.city_num):
            if (exist[i] == False):
                remain.append(i)
        #random.shuffle(remain) #conflicts: random search

        for i in range(len(conflict)):
            perfor[conflict[i]] = remain[i]

        return np.array(perfor)

    def get_fit(self):
        self.fit = []
        for i in range(self.popsize):
            self.fit.append(self.get_ind_fit(i))
        self.fit = np.array(self.fit)
        #print(self.fit)

    def get_ind_fit(self, id):
        fit = 0
        for i in range(self.city_num - 1):
            fit += self.distance[self.performance[id, i], self.performance[id, i+1]]
        fit += self.distance[self.performance[id, self.city_num-1], self.performance[id, 0]]
        fit = 1.0/fit
        return fit

    def get_last_best(self):
        self.lastbestind = []
        l = list(self.fit)
        self.lastbestfit = max(l)
        self.lastbestindex = l.index(self.lastbestfit)
        self.lastbestind = self.pop[self.lastbestindex].copy()
        #print(self.lastbestfit, self.lastbestindex, self.lastbestind)

    def selection(self):
        new_fitvalue = []
        totalfit = sum(self.fit)
        accumulator = 0.0
        for val in self.fit:
            new_val = val*1.0/totalfit
            accumulator += new_val
            new_fitvalue.append(accumulator)
        ms = []
        for i in range(self.popsize):
            ms.append(random.random())
        ms.sort()
        fitin = 0
        newin = 0
        newpop = []
        while (newin < self.popsize):
            if(ms[newin] < new_fitvalue[fitin]):
                newpop.append(self.pop[fitin].copy())
                newin = newin + 1
            else:
                fitin = fitin + 1
        self.pop = np.array(newpop)
        random.shuffle(self.pop)
        #print(self.pop)
    
    def hybrid(self):
        for i in range(self.popsize):
            self.ind_cross(i)
        
        for i in range(0, self.popsize-1, 2):
            if (random.random() < self.hybridrate):
                if (random.random() < 0.5):
                    self.pop[[i, i+1], 0, :] = self.pop[[i+1, i], 0, :]
                else:
                    self.pop[[i, i+1], [0, 1], :] = self.pop[[i+1, i], [1, 0], :]
                

    def ind_cross(self, id):
        chain1 = []
        chain2 = []
        len1 = 0
        exist = [False for i in range(self.city_num)] #record the genes in chain1
        for i in range(self.city_num):
            if (len1 < self.city_num and exist[self.pop[id, 0, i]] == False):
                chain1.append(self.pop[id, 0, i])
                exist[self.pop[id, 0, i]] = True
                len1 += 1
            else:
                chain2.append(self.pop[id, 0, i])
            if (len1 < self.city_num and exist[self.pop[id, 1, i]] == False):
                chain1.append(self.pop[id, 1, i])
                exist[self.pop[id, 1, i]] = True
                len1 += 1
            else:
                chain2.append(self.pop[id, 1, i])
        
        chain1 = np.array(chain1)
        chain2 = np.array(chain2)
        self.pop[id, 0] = chain1.copy()
        self.pop[id, 1] = chain2.copy()
        
    def mutation(self):
        for i in range(self.popsize):
            for j in range(2):
                if (random.random() < self.mutationrate):
                    p1 = random.randint(0, self.city_num)
                    p2 = random.randint(0, self.city_num)
                    while (p1 == p2):
                        p2 = random.randint(0, self.city_num)
                    self.pop[i, j, [p1, p2]] = self.pop[i, j, [p2, p1]]

    def fit_ind(self, ind):
        perfor = []
        exist = [False for i in range(self.city_num)]   #If the genes already exist in performance
        conflict = []       #The index of conflicted genes
        remain = []         #The genes not exist

        #get the origin performance with conflicted genes
        for i in range(self.city_num):
            if (self.genTable[i, ind[0, i]] < self.genTable[i, ind[1, i]]):
                perfor.append(ind[0, i])
            else:
                perfor.append(ind[1, i])
            if (exist[perfor[i]] == False):
                exist[perfor[i]] = True
            else:
                conflict.append(i)

        for i in range(self.city_num):
            if (exist[i] == False):
                remain.append(i)
        random.shuffle(remain)

        for i in range(len(conflict)):
            perfor[conflict[i]] = remain[i]
        perfor = np.array(perfor)

        fit = 0
        for i in range(self.city_num - 1):
            fit += self.distance[perfor[i], perfor[i+1]]
        fit += self.distance[perfor[self.city_num-1], perfor[0]]
        fit = 1.0/fit
        return fit

    def elitism(self):
        l = list(self.fit)
        worstfit = min(l)
        worstindex = l.index(worstfit)
        self.pop[worstindex] = self.lastbestind.copy()
        self.performance[worstindex] = self.get_ind_perfor(worstindex).copy()
        self.fit[worstindex] = self.get_ind_fit(worstindex)


    def run(self):
        self.get_performance()
        self.get_fit()
        self.get_last_best()

        for i in range(self.genaration):
            print('generation:', i+1)
            #print('1', self.lastbestind)
            self.selection()
            self.hybrid()
            self.mutation()
            self.get_performance()
            self.get_fit()
            self.elitism()
            self.get_last_best()

            print('now best:', 1.0/self.lastbestfit)
            print(self.performance[self.lastbestindex])
            print('------------------')

tsp = TSP(30, 'in.txt')
tsp.run()