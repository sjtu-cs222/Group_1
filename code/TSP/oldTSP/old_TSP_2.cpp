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
struct Point { //����
    string name;
    double x, y;
    int i; //���
};
vector<Point> p;
double d[maxn][maxn]; //�������
int n;
double sum = 0; //��ǰ���·������

double dist(Point a, Point b) { //�����������
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double get_sum(vector<Point> a) { //����·������
    double sum = 0;
    for (int i = 1; i < a.size(); i++) {
        sum += d[a[i].i][a[i - 1].i];
    }
    sum += d[a[0].i][a[a.size()-1].i];
    return sum;
}

void init() {                    //��ʼ��
    srand((unsigned)time(NULL)); //�������������
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

void show() { //��ʾ��ǰ���
    cout << "·������: " << sum << endl;
    cout << "·��:";
    for (int i = 0; i < n; i++)
        cout << ' ' << p[i].name;
    puts("");
}

int w = 100;                  //�޶���Ⱥֻ�ܻ�100������
vector<vector<Point> > group; //��Ⱥ��Ҳ����Ⱦɫ���б�

void Improve_Circle() { //����Ȧ���õ���ʼ����
    vector<Point> cur = p;
    for (int t = 0; t < w; t++) {     //�ظ�50��
        for (int i = 0; i < n; i++) { //�������˳��
            int j = rand() % n;
            swap(cur[i], cur[j]);
        }
        int flag = 1;
        while (flag) {
            flag = 0;
            //����ѡȡuv�Ӵ������Ե���uv�Ӵ���˳�����Ƿ���ţ������������
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

vector<int> get_randPerm(int n) { //����һ���������
    vector<int> c;
    for (int i = 0; i < n; i++) {
        c.push_back(i);
    }
    for (int i = 0; i < n; i++) {
        swap(c[i], c[rand() % n]);
    }
    return c;
}

//����ʱ�õ��ıȽϺ���
bool cmp(vector<Point> a, vector<Point> b) { return get_sum(a) < get_sum(b); }

int dai = 200; //һ������200���Ľ���ѡ��
int c[maxn];
double bylv = 0.1; //������

void genetic_algorithm() { //�Ŵ��㷨
    vector<vector<Point> > A = group, B, C;
    // A����ǰ������Ⱥ  B:����������Ӵ�  C������������Ӵ�
    for (int t = 0; t < dai; t++) {
        B = A;
        vector<int> c = get_randPerm(A.size());
        for (int i = 0; i + 1 < c.size(); i += 2) {
            int F = rand() % n; //���򻮷ַֽ��
            int u=c[i],v=c[i+1];
            for (int j = F; j < n;
                 j++) { //�������ѡ��2������Ļ�����Σ�Ҳ���ǽ���
                swap(B[u][j], B[v][j]);
            }
            //��������ܷ�����ͻ����Ҫ�����ͻ
            //����Fǰ��Ĳ��ֲ��䣬F����Ĳ����г�ͻ�򽻻�
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
            //���ڱ���ĸ���,ȡ3����u<v<w,���Ӵ�[u,v]�嵽w����
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
        //�ϲ�A��B��C
        for (int i = 0; i < B.size(); i++) {
            A.push_back(B[i]);
        }
        for (int i = 0; i < C.size(); i++) {
            A.push_back(C[i]);
        }
        sort(A.begin(), A.end(), cmp); //��С��������
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
    cout << "��ʼ";
    show();
    cout << "����Ȧ��";
    Improve_Circle();
    show();
    cout << "�Ŵ��㷨";
    genetic_algorithm();
    show();
    return 0;
}
