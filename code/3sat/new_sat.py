import numpy as np
from numpy import random

class new_sat():
    def __init__(self, variable_num, clause_num, filename):
        self.vn = variable_num
        self.cn = clause_num
        self.hybridrate = 1.0
        self.mutationrate = 0.02
        self.genaration = 1000
        self.popsize = 40

        #生成显隐性基因表
        self.geneTable = random.randint(0, 2, self.vn)

        #生成种群
        self.pop = []
        for i in range(self.popsize):
            ind = []
            ind1 = []
            ind2 = []
            for j in range(self.vn):
                if (random.random() < 0.5):
                    ind1.append(0)   #0是false
                else:
                    ind1.append(1)
            for j in range(self.vn):
                if (random.random() < 0.5):
                    ind2.append(0)   #0是false
                else:
                    ind2.append(1)
            ind.append(ind1)
            ind.append(ind2)
            self.pop.append(ind)
        self.pop = np.array(self.pop)
        #print(self.pop.shape)

        #读取问题数据
        self.problem = []
        f = open(filename, 'r')
        line = f.readline()
        for i in range(self.cn):
            line = f.readline()
            line = line.split()
            self.problem.append([int(line[0]), int(line[1]), int(line[2])])
        self.problem = np.array(self.problem)
        #print(self.problem)

    def get_performance(self):
        self.performance = []
        for i in range(self.popsize):
            indper = []
            for j in range(self.vn):
                if (self.pop[i, 0, j] == self.pop[i, 1, j]):
                    indper.append(self.pop[i, 0, j])
                else:
                    indper.append(self.geneTable[j])
            self.performance.append(indper)
        self.performance = np.array(self.performance)
        #print(self.performance)

    def get_fit(self):
        self.fit = []
        for i in range(self.popsize):
            count = 0
            for j in range(self.cn):
                for k in range(3):
                    if (self.problem[j, k] < 0):
                        tmp = -1 * self.problem[j, k] - 1
                        if (self.performance[i, tmp] == 0):
                            count += 1
                            break
                    elif (self.problem[j, k] > 0):
                        tmp = self.problem[j, k] - 1
                        if (self.performance[i, tmp] == 1):
                            count += 1
                            break
            self.fit.append(count)
        #print(self.fit)

    def get_last_best(self):
        self.lastbestfit = max(self.fit)
        self.lastbestindex = self.fit.index(self.lastbestfit)
        self.lastbestind = self.pop[self.lastbestindex].copy()
        self.lastbestper = self.performance[self.lastbestindex].copy()
        #print(self.lastbestindex)

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

    def in_cross(self):
        for i in range(self.popsize):
            pos = random.randint(0, self.vn)
            self.pop[i, [0, 1], pos:] = self.pop[i, [1, 0], pos:]

    def hybrid(self):
        for i in range(0, self.popsize-1, 2):
            if (random.random() < self.hybridrate):
                if (random.random() < 0.5):
                    self.pop[[i, i+1], 0, :] = self.pop[[i+1, i], 0, :]
                else:
                    self.pop[[i, i+1], [0, 1], :] = self.pop[[i+1, i], [1, 0], :]

    def mutation(self):
        for i in range(self.popsize):
            for j in range(self.vn):
                for k in range(2):
                    if (random.random() < self.mutationrate):
                        self.pop[i, k, j] = 1 - self.pop[i, k, j]

    def elitism(self):
        worstfit = min(self.fit)
        worstindex = self.fit.index(worstfit)
        self.pop[worstindex] = self.lastbestind.copy()
        self.fit[worstindex] = self.lastbestfit
        self.performance[worstindex] = self.lastbestper.copy()

    def run(self):
        self.get_performance()
        self.get_fit()
        self.get_last_best()
        
        for i in range(self.genaration):
            print('generation:', i+1)
            self.selection()
            self.in_cross()
            self.hybrid()
            self.mutation()
            self.get_performance()
            self.get_fit()
            self.elitism()
            self.get_last_best()

            print('now best:', self.lastbestfit)
            print(self.lastbestper)
            print('----------------------')
            if (self.lastbestfit == self.cn):
                break

s = new_sat(30, 129, 'sat1.txt')
s.run()