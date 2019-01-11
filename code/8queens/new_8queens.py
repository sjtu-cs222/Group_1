import numpy as np
from numpy import random

class nQueens():
    def __init__(self, prosize):
        self.prosize = prosize
        self.popsize = 4
        self.hybridrate = 0.9
        self.mutationrate = 0.1
        self.pop = []
        self.geneTable = []
        self.performance = []
        self.fit = []

        for i in range(self.prosize):
            geneRela = [i for i in range(self.prosize)]
            np.random.shuffle(geneRela)
            self.geneTable.append(geneRela)

        print(self.geneTable)
        
        self.pop = random.randint(0, prosize, size = (self.popsize, 2, self.prosize))
        self.pop = list(self.pop)
        #print(self.pop)
    
    def get_performance(self):
        self.performance = []
        for i in range(self.popsize):
            ind_per = []
            for j in range(self.prosize):
                if (self.geneTable[j][self.pop[i][0][j]] < self.geneTable[j][self.pop[i][1][j]]):
                    ind_per.append(self.pop[i][0][j])
                else:
                    ind_per.append(self.pop[i][1][j])
            self.performance.append(list(ind_per))

        #print(self.performance)

    def get_fit(self):
        self.fit = []

        max_col = self.prosize*(self.prosize-1)/2
        for i in range(self.popsize):
            col = 0
            for j in range(self.prosize):
                for k in range(j+1, self.prosize):
                    if (self.performance[i][j] == self.performance[i][k] or abs(j-k) == abs(self.performance[i][j] - self.performance[i][k])):
                        col += 1
            self.fit.append(max_col-col)
    
    def get_last_best(self):
        self.lastbestfit = max(self.fit)
        self.lastbestindex = self.fit.index(self.lastbestfit)
        self.lastbestind = list(self.pop[self.lastbestindex])

    def selection(self):
        new_fitvalue = []
        totalfit = sum(self.fit)
        accumulator = 0.0
        for val in self.fit:
            new_val =(val*1.0/totalfit)            
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
                newpop.append(list(self.pop[fitin]))
                newin = newin + 1
            else:
                fitin = fitin + 1
        self.pop = list(newpop)



    def hybrid(self):
        for i in range(0, self.popsize-1, 2):
            if (random.random()<self.hybridrate):
                point = random.randint(0, self.prosize)
                tmp1 = list(self.pop[i][0][0:point]) + list(self.pop[i][1][point:self.prosize])
                tmp2 = list(self.pop[i][1][0:point]) + list(self.pop[i][0][point:self.prosize])
                
                point = random.randint(0, self.prosize)
                tmp3 = list(self.pop[i+1][0][0:point]) + list(self.pop[i+1][1][point:self.prosize])
                tmp4 = list(self.pop[i+1][1][0:point]) + list(self.pop[i+1][0][point:self.prosize])

                self.pop[i][0] = list(tmp1)
                self.pop[i+1][0] = list(tmp2)

                if (random.random()<0.5):
                    self.pop[i][1] = list(tmp3)
                    self.pop[i+1][1] = list(tmp4)
                else:
                    self.pop[i][1] = list(tmp4)
                    self.pop[i+1][1] = list(tmp3)

    def mutation(self):
        for i in range(self.popsize):
            for j in range(2):
                if (random.random()< self.mutationrate):
                    point = random.randint(0, self.prosize)
                    changeto = random.randint(0, self.prosize)
                    self.pop[i][j][point] = changeto
    
    def elitism(self):
        self.worstfit = min(self.fit)
        self.worstindex = self.fit.index(self.worstfit)
        if (max(self.fit) < self.lastbestfit):
            self.pop[self.worstindex] = list(self.lastbestind)
            self.get_performance()
            self.get_fit()

    def run(self):
        self.get_performance()
        self.get_fit()
        self.get_last_best()

        i = 0
        while(True):
            print ('generation:', i)
            self.selection()
            self.hybrid()
            self.mutation()
            self.get_performance()
            self.get_fit()
            self.elitism()
            self.get_last_best()
            #print (self.pop)
            #print (self.fit)
            print('now best', self.lastbestfit)
            print(self.lastbestind)

            max_col = self.prosize*(self.prosize-1)/2
            if (max_col - self.lastbestfit <= 0):
                break
            else:
                i += 1




problem = nQueens(8)
problem.run()