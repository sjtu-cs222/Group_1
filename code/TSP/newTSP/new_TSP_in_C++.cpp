#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include "math.h"
#include "time.h"
#include <unistd.h>
#include <bits/stdc++.h>
#define CITY_NUM 38     //城市数，城市编号是0~CITY_NUM-1
#define POPSIZE 300        //种群个体数
#define MAXVALUE 10000000   //路径最大值上限
#define N 100000//需要根据实际求得的路径值修正
#include <iostream>
using namespace std;

unsigned seed=(unsigned)time(0);
double Hash[CITY_NUM+1];
double Hash1[CITY_NUM+1];
double Hash2[CITY_NUM+1];
double Hash3[CITY_NUM+1];
double Hash4[CITY_NUM+1];
typedef struct CityPosition
{
    double x;
    double y;
}CityPosition;

CityPosition CityPos[38]={
    {11003.611100,42102.500000},{11108.611100,42373.888900},{11133.333300,42885.833300},{11155.833300,42712.500000},{11183.333300,42933.333300},{11297.500000,42853.333300},{11310.277800,42929.444400},{11416.666700,42983.333300},{11423.888900,43000.277800},{11438.333300,42057.222200},{11461.111100,43252.777800},{11485.555600,43187.222200},{11503.055600,42855.277800},{11511.388900,42106.388900},{11522.222200,42841.944400},{11569.444400,43136.666700},{11583.333300,43150.000000},{11595.000000,43148.055600},{11600.000000,43150.000000},{11690.555600,42686.666700},{11715.833300,41836.111100},{11751.111100,42814.444400},{11770.277800,42651.944400},{11785.277800,42884.444400},{11822.777800,42673.611100},{11846.944400,42660.555600},{11963.055600,43290.555600},{11973.055600,43026.111100},{12058.333300,42195.555600},{12149.444400,42477.500000},{12286.944400,43355.555600},{12300.000000,42433.333300},{12355.833300,43156.388900},{12363.333300,43189.166700},{12372.777800,42711.388900},{12386.666700,43334.722200},{12421.666700,42895.555600},{12645.000000,42973.333300}
};

double CityDistance[CITY_NUM][CITY_NUM];//城市距离词典

typedef struct{
    int colony[POPSIZE][CITY_NUM+1];//城市种群,默认出发城市编号为0，则城市编号的最后一个城市还应该为0，表达的路径
    int part1[POPSIZE][CITY_NUM+1];//第一条城市链
    int part2[POPSIZE][CITY_NUM+1];//第二条城市链
    double fitness[POPSIZE];// 每个个体的适应值，即1/Distance[POPSIZE]
    double Distance[POPSIZE];//每个个体的总路径
    int BestRooting[CITY_NUM+1];//最优城市路径序列
    double BestFitness;//最优路径适应值
    double BestValue;//最优路径长度
    int BestNum;
}TSP,*PTSP;

/*计算城市距离词典CityDistance[i][j]*/
void CalculatDist()
{
    int i,j;
    double temp1,temp2;
    for(i=0;i<CITY_NUM;i++){
        for(j=0;j<=CITY_NUM;j++){//最后一个城市还应该返回到出发节点
            temp1=CityPos[j].x-CityPos[i].x;
            temp2=CityPos[j].y-CityPos[i].y;
            CityDistance[i][j]=sqrt(temp1*temp1+temp2*temp2);
        }
    }
}
/*数组复制*/
void copy(int a[],int b[])
{
    int i=0;
    for(i=0;i<CITY_NUM+1;i++)
    {
        a[i]=b[i];
    }
}

/*用来检查新生成的节点是否在当前群体中，0号节点是默认出发节点和终止节点*/
bool checkpart1(TSP &city,int pop,int num,int k)
{
    int i;
    for(i=0;i<=num;i++){
        if(k==city.part1[pop][i])
            return true;//新生成节点存在于已经生成的路径中
    }
    return false;//新生成节点没有存在于已经生成的路径中
}

bool checkpart2(TSP &city,int pop,int num,int k)
{
    int i;
    for(i=0;i<=num;i++){
        if(k==city.part2[pop][i])
            return true;//新生成节点存在于已经生成的路径中
    }
    return false;//新生成节点没有存在于已经生成的路径中
}

bool checkcolony(TSP &city,int pop,int num,int k)
{
    int i;
    for(i=0;i<=num;i++){
        if(k==city.colony[pop][i])
            return true;//新生成节点存在于已经生成的路径中
    }
    return false;//新生成节点没有存在于已经生成的路径中
}

/****************种群初始化，即为city.part1[i][j],city.part2[i][j]赋值，根据上两条得到表征路径city.colony[i][j]****************/
void InitColony(TSP &city)
{
    int i,j,r,sum,n;
    for(i=0;i<POPSIZE;i++){
        city.part1[i][0]=0;
        city.part1[i][CITY_NUM]=0;
        city.part2[i][0]=0;
        city.part2[i][CITY_NUM]=0;
        city.colony[i][0]=0;
        city.colony[i][CITY_NUM]=0;
        city.BestValue=MAXVALUE;
        city.BestFitness=0;//适应值越大越好
    }

    for(i=0;i<POPSIZE;i++)                    //链1
    {
        for(j=1;j<CITY_NUM;j++)
        {
            r=rand()%(CITY_NUM-1)+1;//产生1～CITY_NUM-1之间的随机数
            while(checkpart1(city,i,j,r))//随机产生城市序号，即为city.colony[i][j]赋值
            {
                r=rand()%(CITY_NUM-1)+1;
            }

            city.part1[i][j]=r;
        }
    }

    for(i=0;i<POPSIZE;i++)                   //链2
    {
        for(j=1;j<CITY_NUM;j++)
        {
            r=rand()%(CITY_NUM-1)+1;//产生1～CITY_NUM-1之间的随机数
            while(checkpart2(city,i,j,r))//随机产生城市序号，即为city.colony[i][j]赋值
            {
                r=rand()%(CITY_NUM-1)+1;
            }

            city.part2[i][j]=r;
        }
    }

    for(i=0;i<POPSIZE;i++){
        int tmp[CITY_NUM];
        int flag[CITY_NUM+1];
        for(int i=0;i<=CITY_NUM;i++){
            flag[i]=0;
        }
        flag[0]=1;
        sum=0;                                                     //表现链
        for(j=1;j<CITY_NUM;j++){
            r=min(city.part1[i][j],city.part2[i][j]);
            if(checkcolony(city,i,j,r)){
                int k=max(city.part1[i][j],city.part2[i][j]);
                if(checkcolony(city,i,j,k)){
                    tmp[sum]=j;
                    sum+=1;
                }
                else{
                    city.colony[i][j]=k;
                    flag[k]=1;
                }
            }
            else{
                city.colony[i][j]=r;
                flag[r]=1;
            }
        }
        for(int m=0;m<sum;m++){
            n=0;
            while(flag[n]!=0){
                n+=1;
            }
            city.colony[i][tmp[m]]=n;
            flag[n]=1;
        }
    }
}

/*计算适应值,考虑应该在这里面把最优选出来*/
void CalFitness(TSP &city)
{
    int i,j;
    int start,end;
    int Best=0;
    for(i=0;i<POPSIZE;i++){//求每个个体的总路径，适应值
        city.Distance[i]=0;
        for(j=1;j<=CITY_NUM;j++){
            start=city.colony[i][j-1];end=city.colony[i][j];
            city.Distance[i]=city.Distance[i]+CityDistance[start][end];//city.Distance[i]每个个体的总路径
        }
        city.fitness[i]=N/city.Distance[i];
        if(city.fitness[i]>city.fitness[Best])//选出最大的适应值，即选出所有个体中的最短路径
            Best=i;
    }
    for(j=0;j<=CITY_NUM;j++){
        cout<<city.colony[Best][j]<<"  ";
    }
    cout<<endl;
    copy(city.BestRooting,city.colony[Best]);//将最优个体拷贝给city.BestRooting
    city.BestFitness=city.fitness[Best];
    city.BestValue=city.Distance[Best];
    city.BestNum=Best;
}


/****************选择算子：轮盘赌法****************/
void Select(TSP &city)
{
    int TempColony[POPSIZE][CITY_NUM+1];
    int Temppart1[POPSIZE][CITY_NUM+1];
    int Temppart2[POPSIZE][CITY_NUM+1];
    int i,j,t;
    double s;
    double GaiLv[POPSIZE];
    double SelectP[POPSIZE+1];
    double avg;
    double sum=0;
    for(i=0;i<POPSIZE;i++)
    {
        sum+=city.fitness[i];
    }
    for(i=0;i<POPSIZE;i++)
    {
        GaiLv[i]=city.fitness[i]/sum;
    }
    SelectP[0]=0;
    for(i=0;i<POPSIZE;i++)
    {
        SelectP[i+1]=SelectP[i]+GaiLv[i]*RAND_MAX;
    }
    memcpy(TempColony[0],city.colony[city.BestNum],sizeof(TempColony[0]));//void *memcpy(void *dest, const void *src, size_t n)从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
    memcpy(Temppart1[0],city.part1[city.BestNum],sizeof(Temppart1[0]));
    memcpy(Temppart2[0],city.part2[city.BestNum],sizeof(Temppart2[0]));
    for(t=1;t<POPSIZE;t++)
    {
        double ran = rand() % RAND_MAX + 1;
        s= (double) ran / 100.0;
        for(i=1;i<POPSIZE;i++)
        {
            if(SelectP[i]>=s)
                break;
        }
        memcpy(TempColony[t],city.colony[i-1],sizeof(TempColony[t]));
        memcpy(Temppart1[t],city.part1[i-1],sizeof(Temppart1[t]));
        memcpy(Temppart1[t],city.part2[i-1],sizeof(Temppart2[t]));
    }
    for(i=0;i<POPSIZE;i++)
    {
        memcpy(city.colony[i],TempColony[i],sizeof(TempColony[i]));
        memcpy(city.part1[i],Temppart1[i],sizeof(Temppart1[i]));
        memcpy(city.part2[i],Temppart2[i],sizeof(Temppart2[i]));
    }

}

/****************Cross: Fetch and Rearrangement****************/
void Cross1(TSP &city,double pc)//交叉概率是pc
{
    int i,j,t,l,r;
    int a,b,x,y;
    int Temp1[CITY_NUM+1],Temp2[CITY_NUM+1],Temp3[CITY_NUM+1],Temp4[CITY_NUM+1];
    for(i=0;i<POPSIZE/2;i++){
        double s=((double)(rand()%RAND_MAX))/RAND_MAX;

        if(s<pc){
            x=rand()%POPSIZE;
            y=rand()%POPSIZE;
            if(x==city.BestNum||y==city.BestNum)//如果遇到最优则直接进行下次循环
                continue;

            l=rand()%19+1;  //1-19
            a=rand()%(CITY_NUM-l)+1; //1-37

            memset(Hash1,0,sizeof(Hash1));//void *memset(void *s, int ch, size_t n);将s中当前位置后面的n个字节 用 ch 替换并返回 s 。
            Temp1[0]=Temp1[CITY_NUM]=0;

            for(j=1;j<=l;j++)//打乱顺序即随机，选出来的通过Hash标记为1    //x链part2改变
            {
                Temp1[j]=city.part1[x][a+j-1]; //a+L=2~38 20~38
                Hash1[Temp1[j]]=1;
            }
            for(t=1;t<CITY_NUM;t++)
            {
                if(Hash1[city.part2[x][t]]==0)
                {
                    Temp1[j++]=city.part2[x][t];
                    Hash1[city.part2[x][t]]=1;
                }
            }

            memset(Hash2,0,sizeof(Hash2));//void *memset(void *s, int ch, size_t n);将s中当前位置后面的n个字节 用 ch 替换并返回 s 。
            Temp2[0]=Temp2[CITY_NUM]=0;

            for(j=1;j<=l;j++)//打乱顺序即随机，选出来的通过Hash标记为1    //x链part1改变
            {
                Temp2[j]=city.part2[x][a+j-1]; //a+L=2~38 20~38
                Hash2[Temp2[j]]=1;
            }
            for(t=1;t<CITY_NUM;t++)
            {
                if(Hash2[city.part1[x][t]]==0)
                {
                    Temp2[j++]=city.part1[x][t];
                    Hash2[city.part1[x][t]]=1;
                }
            }

            memset(Hash3,0,sizeof(Hash3));//void *memset(void *s, int ch, size_t n);将s中当前位置后面的n个字节 用 ch 替换并返回 s 。
            Temp3[0]=Temp3[CITY_NUM]=0;

            for(j=1;j<=l;j++)//打乱顺序即随机，选出来的通过Hash标记为1    //y链part2改变
            {
                Temp3[j]=city.part1[y][a+j-1]; //a+L=2~38 20~38
                Hash3[Temp3[j]]=1;
            }
            for(t=1;t<CITY_NUM;t++)
            {
                if(Hash3[city.part2[y][t]]==0)
                {
                    Temp3[j++]=city.part2[y][t];
                    Hash3[city.part2[y][t]]=1;
                }
            }

            memset(Hash4,0,sizeof(Hash4));//void *memset(void *s, int ch, size_t n);将s中当前位置后面的n个字节 用 ch 替换并返回 s 。
            Temp4[0]=Temp4[CITY_NUM]=0;

            for(j=1;j<=l;j++)//打乱顺序即随机，选出来的通过Hash标记为1    //y链part1改变
            {
                Temp4[j]=city.part2[y][a+j-1]; //a+L=2~38 20~38
                Hash4[Temp4[j]]=1;
            }
            for(t=1;t<CITY_NUM;t++)
            {
                if(Hash4[city.part1[y][t]]==0)
                {
                    Temp4[j++]=city.part1[y][t];
                    Hash4[city.part1[y][t]]=1;
                }
            }

            memcpy(city.part2[x],Temp3,sizeof(Temp3));
            memcpy(city.part1[x],Temp4,sizeof(Temp4));
            memcpy(city.part2[y],Temp1,sizeof(Temp1));
            memcpy(city.part1[y],Temp2,sizeof(Temp2));


            int tmp[CITY_NUM];                           //表现链x
            int flag[CITY_NUM+1];
            int r,sum,n;
            for(int i=0;i<=CITY_NUM;i++){
                flag[i]=0;
            }
            flag[0]=1;
            sum=0;
            for(j=1;j<CITY_NUM;j++){
                r=min(city.part1[x][j],city.part2[x][j]);
                if(checkcolony(city,x,j,r)){
                    int k=max(city.part1[x][j],city.part2[x][j]);
                    if(checkcolony(city,x,j,k)){
                        tmp[sum]=j;
                        sum+=1;
                    }
                    else{
                        city.colony[x][j]=k;
                        flag[k]=1;
                    }
                }
                else{
                    city.colony[x][j]=r;
                    flag[r]=1;
                }
            }
            for(int m=0;m<sum;m++){
                n=0;
                while(flag[n]!=0){
                    n+=1;
                }
                city.colony[x][tmp[m]]=n;
                flag[n]=1;
            }

            int tmp1[CITY_NUM];                           //表现链y
            int flag1[CITY_NUM+1];
            for(int i=0;i<=CITY_NUM;i++){
                flag1[i]=0;
            }
            flag1[0]=1;
            sum=0;
            for(j=1;j<CITY_NUM;j++){
                r=min(city.part1[y][j],city.part2[y][j]);
                if(checkcolony(city,y,j,r)){
                    int k=max(city.part1[y][j],city.part2[y][j]);
                    if(checkcolony(city,y,j,k)){
                        tmp1[sum]=j;
                        sum+=1;
                    }
                    else{
                        city.colony[y][j]=k;
                        flag1[k]=1;
                    }
                }
                else{
                    city.colony[y][j]=r;
                    flag1[r]=1;
                }
            }
            for(int m=0;m<sum;m++){
                n=0;
                while(flag1[n]!=0){
                    n+=1;
                }
                city.colony[y][tmp1[m]]=n;
                flag1[n]=1;
            }

        }
    }
}

void Cross(TSP &city,double pc)//交叉概率是pc
{
    int i,j,t,l;
    int a,b,ca,cb;
    int Temp1[CITY_NUM+1],Temp2[CITY_NUM+1];
    for(i=0;i<POPSIZE;i++)
    {
        double s=((double)(rand()%RAND_MAX))/RAND_MAX;
        if(s<pc){
            cb=rand()%POPSIZE;
            ca=cb;
            if(ca==city.BestNum||cb==city.BestNum)//如果遇到最优则直接进行下次循环
                continue;

            l=rand()%19+1;  //1-19
            a=rand()%(CITY_NUM-l)+1; //1-37

            memset(Hash,0,sizeof(Hash));//void *memset(void *s, int ch, size_t n);将s中当前位置后面的n个字节 用 ch 替换并返回 s 。
            Temp1[0]=Temp1[CITY_NUM]=0;
            for(j=1;j<=l;j++)//打乱顺序即随机，选出来的通过Hash标记为1
            {
                Temp1[j]=city.colony[cb][a+j-1]; //a+L=2~38 20~38
                Hash[Temp1[j]]=1;
            }
            for(t=1;t<CITY_NUM;t++)
            {
                if(Hash[city.colony[ca][t]]==0)
                {
                    Temp1[j++]=city.colony[ca][t];
                    Hash[city.colony[ca][t]]=1;
                }
            }
            memcpy(city.colony[ca],Temp1,sizeof(Temp1));
        }
    }
}

/****************变异****************/
double GetFittness(int a[CITY_NUM+1])
{
    int i,start,end;
    double Distance=0;
    for(i=0;i<CITY_NUM;i++)
    {
        start=a[i];   end=a[i+1];
        Distance+=CityDistance[start][end];
    }
    return N/Distance;
}
/*对换变异*/
void Mutation(TSP &city,double pm)//变异概率是pm
{
    int i,k,m;
    int Temp[CITY_NUM+1];
    int Temp1[CITY_NUM+1];
    int Temp2[CITY_NUM+1];
    for(k=0;k<POPSIZE;k++)
    {
        double s=((double)(rand()%RAND_MAX))/RAND_MAX;//随机产生概率0~1间
        i=rand()%POPSIZE;//随机产生0~POPSIZE之间的数
        if(s<pm&&i!=city.BestNum)//i!=city.BestNum，即保证最优的个体不变异
        {
            int a,b,t;
            a=(rand()%(CITY_NUM-1))+1;
            b=(rand()%(CITY_NUM-1))+1;
            copy(Temp,city.colony[i]);
            copy(Temp1,city.part1[i]);
            copy(Temp2,city.part2[i]);
            if(a>b)//保证让b>=a
            {
                t=a;
                a=b;
                b=t;
            }

            for(m=a;m<(a+b)/2;m++)
            {
                t=Temp[m];
                Temp[m]=Temp[a+b-m];
                Temp[a+b-m]=t;
            }
            for(m=a;m<(a+b)/2;m++)
            {
                t=Temp1[m];
                Temp1[m]=Temp1[a+b-m];
                Temp1[a+b-m]=t;
            }
            for(m=a;m<(a+b)/2;m++)
            {
                t=Temp2[m];
                Temp2[m]=Temp2[a+b-m];
                Temp2[a+b-m]=t;
            }

            memcpy(city.colony[i],Temp,sizeof(Temp));
            memcpy(Temp1,city.part1[i],sizeof(Temp1));
            memcpy(Temp2,city.part2[i],sizeof(Temp2));
        }
    }
}

void OutPut(TSP &city)
{
    int i,j;
    printf("最佳路径为:\n");
    for(i=0;i<=CITY_NUM;i++)
    printf("%5d",city.BestRooting[i]);
    printf("\n最佳路径值为：%f\n",(city.BestValue));
}


int main()
{
        TSP city;
        double pcross,pmutation;//交叉概率和变异概率
        int MaxEpoc;//最大迭代次数
        int i;
        srand(seed);
        MaxEpoc=10000;
        pcross=0.5; pmutation=0.2;
        CalculatDist();//计算城市距离词典
        InitColony(city);//生成初始种群
        CalFitness(city);//计算适应值,考虑应该在这里面把最优选出来
        OutPut(city);
        for(i=0;i<MaxEpoc;i++)
        {
            Select(city);//选择(复制)：轮盘赌法
            Cross1(city,pcross);//交叉
            //CalFitness(city);//计算适应值
            //printf("第%3d次",i);
            //OutPut(city);
            Mutation(city,pmutation);//变异
            CalFitness(city);//计算适应值
            printf("第%3d次",i);
            OutPut(city);
        }

       OutPut(city);//输出
}
