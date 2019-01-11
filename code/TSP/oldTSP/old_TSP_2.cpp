#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <time.h>
using namespace std;
typedef long long LL;
const int maxn = 1e2 + 7;
const int INF = 0x7fffffff;
const double PI = acos(-1);
struct Point { //点类
    string name;
    double x, y;
    int i; //编号
};
vector<Point> p;
double d[maxn][maxn]; //距离矩阵
int n;
double sum = 0; //当前最短路径长度

double dist(Point a, Point b) { //计算两点距离
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double get_sum(vector<Point> a) { //返回路径长度
    double sum = 0;
    for (int i = 1; i < a.size(); i++) {
        sum += d[a[i].i][a[i - 1].i];
    }
    sum += d[a[0].i][a[a.size()-1].i];
    return sum;
}

void init() {                    //初始化
    srand((unsigned)time(NULL)); //设置随机数种子
    cin >> n;
    p.clear();
    for (int i = 0; i < n; i++) {
        Point t;
        cin >> t.name >> t.x >> t.y;
        t.i = i;
        p.push_back(t);
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            d[i][j] = d[j][i] = dist(p[i], p[j]);
        }
    }
    sum = get_sum(p);
}

void show() { //显示当前结果
    cout << "路径长度: " << sum << endl;
    cout << "路径:";
    for (int i = 0; i < n; i++)
        cout << ' ' << p[i].name;
    puts("");
}

int w = 100;                  //限定种群只能活100个个体
vector<vector<Point> > group; //种群，也就是染色体列表

void Improve_Circle() { //改良圈法得到初始序列
    vector<Point> cur = p;
    for (int t = 0; t < w; t++) {     //重复50次
        for (int i = 0; i < n; i++) { //构造随机顺序
            int j = rand() % n;
            swap(cur[i], cur[j]);
        }
        int flag = 1;
        while (flag) {
            flag = 0;
            //不断选取uv子串，尝试倒置uv子串的顺序后解是否更优，如果更优则变更
            for (int u = 1; u < n - 2; u++) {
                for (int v = u + 1; v < n - 1; v++) {
                    if (d[cur[u].i][cur[v + 1].i] + d[cur[u - 1].i][cur[v].i] <
                        d[cur[u].i][cur[u - 1].i] + d[cur[v].i][cur[v + 1].i]) {
                        for (int k = u; k <= (u + v) / 2; k++) {
                            swap(cur[k], cur[v - (k - u)]);
                            flag = 1;
                        }
                    }
                }
            }
        }
        group.push_back(cur);
        double cur_sum = get_sum(cur);
        if (cur_sum < sum) {
            sum = cur_sum;
            p = cur;
        }
    }
}

vector<int> get_randPerm(int n) { //返回一个随机序列
    vector<int> c;
    for (int i = 0; i < n; i++) {
        c.push_back(i);
    }
    for (int i = 0; i < n; i++) {
        swap(c[i], c[rand() % n]);
    }
    return c;
}

//排序时用到的比较函数
bool cmp(vector<Point> a, vector<Point> b) { return get_sum(a) < get_sum(b); }

int dai = 200; //一共进行200代的进化选择
int c[maxn];
double bylv = 0.1; //变异率

void genetic_algorithm() { //遗传算法
    vector<vector<Point> > A = group, B, C;
    // A：当前代的种群  B:交配产生的子代  C：变异产生的子代
    for (int t = 0; t < dai; t++) {
        B = A;
        vector<int> c = get_randPerm(A.size());
        for (int i = 0; i + 1 < c.size(); i += 2) {
            int F = rand() % n; //基因划分分界点
            int u=c[i],v=c[i+1];
            for (int j = F; j < n;
                 j++) { //交换随机选的2个个体的基因后半段，也就是交配
                swap(B[u][j], B[v][j]);
            }
            //交换后可能发生冲突，需要解除冲突
            //保留F前面的部分不变，F后面的部分有冲突则交换
            int num1[1000]={0},num2[1000]={0};
            for(int j=0;j<n;j++){
                num1[B[u][j].i]++;
                num2[B[v][j].i]++;
            }
            vector<Point> v1;
            vector<Point> v2;
            for(int j=0;j<n;j++){
                if(num1[B[u][j].i]==2){
                    v1.push_back(B[u][j]);
                }
            }
            for(int j=0;j<n;j++){
                if(num2[B[v][j].i]==2){
                    v2.push_back(B[v][j]);
                }
            }
            int p1=0,p2=0;
            for(int j=F;j<n;j++){
                if(num1[B[u][j].i]==2){
                    B[u][j]=v2[p2++];
                }
                if(num2[B[v][j].i]==2){
                    B[v][j]=v1[p1++];
                }
            }

        }
        C.clear();
        int flag=1;
        for (int i = 0; i < A.size(); i++) {
            if (rand() % 100 >= bylv * 100)
                continue;
            //对于变异的个体,取3个点u<v<w,把子串[u,v]插到w后面
            int u, v, w;
            u = rand() % n;
            do {
                v = rand() % n;
            } while (u == v);
            do {
                w = rand() % n;
            } while (w == u || w == v);
            if (u > v)
                swap(u, v);
            if (v > w)
                swap(v, w);
            if (u > v)
                swap(u, v);

            vector<Point> vec;
            for (int j = 0; j < u; j++)
                vec.push_back(A[i][j]);
            for (int j = v; j < w; j++)
                vec.push_back(A[i][j]);
            for (int j = u; j < v; j++)
                vec.push_back(A[i][j]);
            for (int j = w; j < n; j++)
                vec.push_back(A[i][j]);
            C.push_back(vec);
        }
        //合并A，B，C
        for (int i = 0; i < B.size(); i++) {
            A.push_back(B[i]);
        }
        for (int i = 0; i < C.size(); i++) {
            A.push_back(C[i]);
        }
        sort(A.begin(), A.end(), cmp); //从小到大排序
        vector<vector<Point> > new_A;
        for (int i = 0; i < w; i++) {
            new_A.push_back(A[i]);
        }
        A = new_A;
    }
    group = A;
    sum = get_sum(group[0]);
    p = group[0];
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
#endif
    init();
    cout << "初始";
    show();
    cout << "改良圈法";
    Improve_Circle();
    show();
    cout << "遗传算法";
    genetic_algorithm();
    show();
    return 0;
}
