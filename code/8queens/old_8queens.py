#-------------------------------------------------------------------------------
# Name:        模块1
# Purpose:
#
# Author:      DELL
#
# Created:     22/12/2018
# Copyright:   (c) DELL 2018
# Licence:     <your licence>
#-------------------------------------------------------------------------------

def main():
    pass

if __name__ == '__main__':
    main()

import random

import matplotlib.pyplot as plt

a=[]

b=[]

c=[]

d=[]

def initState(a):

    while len(a)<8:

        a.append(random.randint(1,8))

    return a

def chooseParent(a,b,c,d):

    parent=[]

    stack=[]

    stack.append(a)

    stack.append(b)

    stack.append(c)

    stack.append(d)

    fitnessValue=fitnessFunc(stack)

    sort=sorted(fitnessValue.items(),key=lambda e:e[1],reverse=True)

    if sort[0][0]=='a':

        firstFather=a

    elif sort[0][0]=='b':

        firstFather=b

    elif sort[0][0]=='c':

        firstFather=c

    else:firstFather=d



    parent.append(firstFather)



    if sort[1][0]=='a':

        firstMother=a

    elif sort[1][0]=='b':

        firstMother=b

    elif sort[1][0]=='c':

        firstMother=c

    else:firstMother=d



    parent.append(firstMother)



    secondFather=firstFather

    parent.append(secondFather)



    if sort[2][0]=='a':

        secondMother=a

    elif sort[2][0]=='b':

        secondMother=b

    elif sort[2][0]=='c':

        secondMother=c

    else:secondMother=d



    parent.append(secondMother)



    return parent





def fitnessFunc(myList):

    fitnessIndex=[28,28,28,28]

    for c in range(0,4):

        for i in range(0,7):

            for n in range(i+1,8):

                if myList[c][i]==myList[c][n]:

                    fitnessIndex[c]-=1

                if myList[c][i]+n-i==myList[c][n] or myList[c][i]-n+i==myList[c][n]:

                    fitnessIndex[c]-=1

    fitnessDict={'a':fitnessIndex[0],'b':fitnessIndex[1],'c':fitnessIndex[2],'d':fitnessIndex[3]}

    return fitnessDict







def crossOver(myList):

    newList=[]

    crossPoint=random.randint(1,7)

    ff=myList[0]

    fm=myList[1]

    sf=myList[2]

    sm=myList[3]



    fchild1=ff[0:crossPoint]

    fchild1.extend(fm[crossPoint:])



    fchild2=fm[0:crossPoint]

    fchild2.extend(ff[crossPoint:])



    schild1=sf[0:crossPoint]

    schild1.extend(sm[crossPoint:])



    schild2=sm[0:crossPoint]

    schild2.extend(sf[crossPoint:])



    newList.append(fchild1)

    newList.append(fchild2)

    newList.append(schild1)

    newList.append(schild2)

    return newList

def mutationPoint():

    mPoint=random.randint(0,8)

    return mPoint

def mutationNumber():

    mNumber=random.randint(1,8)

    return mNumber

def mutation(myList):



    childMutationList=[]

    fc1_mPoint=mutationPoint()

    fc1_mNumber=mutationNumber()

    fchild1=myList[0]

    if fc1_mPoint==0:

        fchild1_m=fchild1

    else:

        fchild1_m=fchild1

        fchild1_m[fc1_mPoint-1]=fc1_mNumber



    fc2_mPoint=mutationPoint()

    fc2_mNumber=mutationNumber()

    fchild2=myList[1]

    if fc2_mPoint==0:

        fchild2_m=fchild2

    else:

        fchild2_m=fchild2

        fchild2_m[fc2_mPoint-1]=fc2_mNumber



    sc1_mPoint=mutationPoint()

    sc1_mNumber=mutationNumber()

    schild1=myList[2]

    if sc1_mPoint==0:

        schild1_m=schild1

    else:

        schild1_m=schild1

        schild1_m[sc1_mPoint-1]=sc1_mNumber



    sc2_mPoint=mutationPoint()

    sc2_mNumber=mutationNumber()

    schild2=myList[3]

    if sc2_mPoint==0:

        schild2_m=schild2

    else:

        schild2_m=schild2

        schild2_m[sc2_mPoint-1]=sc2_mNumber



    childMutationList.append(fchild1_m)

    childMutationList.append(fchild2_m)

    childMutationList.append(schild1_m)

    childMutationList.append(schild2_m)

    return childMutationList







print(initState(a))

print(initState(b))

print(initState(c))

print(initState(d))

parent=chooseParent(a,b,c,d)

# print(parent)

answer=[]

asumeCount=0

while(True):

    crossParent=crossOver(parent)

    mutationParent=mutation(crossParent)

    newFitDict=fitnessFunc(mutationParent)

    asumeCount+=1

    if newFitDict['a']==28 or newFitDict['b']==28 or newFitDict['c']==28 or newFitDict['d']==28:

        if newFitDict['a']==28:

            answer.append(mutationParent[0])

        if newFitDict['b']==28:

            answer.append(mutationParent[1])

        if newFitDict['c']==28:

            answer.append(mutationParent[2])

        if newFitDict['d']==28:

            answer.append(mutationParent[3])

        print("I find it! The answer is {0}".format(answer))

        print("I did {0} cycle to find it".format(asumeCount))

        for i in answer:

            pointA=i.index(1)+1

            pointB=i.index(2)+1

            pointC=i.index(3)+1

            pointD=i.index(4)+1

            pointE=i.index(5)+1

            pointF=i.index(6)+1

            pointG=i.index(7)+1

            pointH=i.index(8)+1

            x=[1,2,3,4,5,6,7,8]

            y=[pointA,pointB,pointC,pointD,pointE,pointF,pointG,pointH]

            plt.axis([0,9,0,9])

            plt.plot(x,y, '*')

            plt.show()



        break

    else:parent=mutationParent
