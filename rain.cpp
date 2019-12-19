#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include<stdio.h>


using namespace std;
const int MaxPoint = 2705;
const double Pi = 3.1415926535897932384626;
int Point_num, Edge_num;
int X[MaxPoint], Y[MaxPoint], H[MaxPoint], H_final[MaxPoint];

bool Boundary[MaxPoint], Visited[MaxPoint];
string Point_Name[MaxPoint];
char in[2];
map<string, int> PointSet;
map<pair<int, int>, bool> Edge_all;
vector< pair<double, int> > e[MaxPoint];
vector<int > Height, Lake;
int Union_H[MaxPoint], Union_H_final[MaxPoint], Bind[MaxPoint], Union_num;

int Union_e[MaxPoint][MaxPoint];
bool Union_Boundary[MaxPoint];
void AddEdge(int a, int b) {                                       //给点添加从该端点出发的边
	Edge_all.insert(make_pair(make_pair(a, b), true));
	double deg = atan2(Y[b] - Y[a], X[b] - X[a]);
	e[a].push_back(make_pair(deg, b));
}
void FloodFill(int x, int p) {                                //使用FloodFill算法进行缩点
	if (Visited[x]) return;
	Visited[x] = true;
	Bind[x] = p;
	for (int i = 0; i < e[x].size(); i++) {
		if (H[e[x][i].second] == H[x])
			FloodFill(e[x][i].second, p);
	}
}
void Init() {                                             //初始化相关数据
	Height.clear();
	PointSet.clear();
	for (int i = 1; i <= Point_num; i++) {
		scanf("\n%s%d%d%d", &in, &X[i], &Y[i], &H[i]);
		Point_Name[i].clear();
		Point_Name[i].push_back(in[0]);
		Point_Name[i].push_back(in[1]);
		Height.push_back(H[i]);
		PointSet[Point_Name[i]] = i;
		e[i].clear();
	}

	sort(Height.begin(), Height.end());
	Height.erase(unique(Height.begin(), Height.end()), Height.end());

	Edge_all.clear();
	for (int i = 1; i <= Edge_num; i++) {
		string u, v;
		u.clear();
		v.clear();
		scanf("\n%s", &in);
		u.push_back(in[0]);
		u.push_back(in[1]);
		scanf("%s", &in);
		v.push_back(in[0]);
		v.push_back(in[1]);
		AddEdge(PointSet[u], PointSet[v]);
		AddEdge(PointSet[v], PointSet[u]);
	}

	for (int i = 1; i <= Point_num; i++) {
		Boundary[i] = false;
		sort(e[i].begin(), e[i].end());
		for (int a = 0; a + 1 < e[i].size(); a++) {
			if ((e[i][a + 1].first - e[i][a].first) >= Pi)
				Boundary[i] = true;
		}
		if (e[i][0].first + 2 * Pi - e[i][e[i].size() - 1].first >= Pi)
			Boundary[i] = true;
		for (int a = 0; a < e[i].size(); a++) {
			int b = (a + 1) % e[i].size();
			if (Edge_all.find(make_pair(e[i][a].second, e[i][b].second)) != Edge_all.end());
			else Boundary[i] = true;
		}
	}
	Union_num = 0;
	for (int i = 1; i <= Point_num; i++) {
		Visited[i] = false;
	}
	for (int i = 1; i <= Point_num; i++) {
		if (!Visited[i])
			FloodFill(i, ++Union_num);
	}
	for (int i = 1; i <= Union_num; i++) {
		Union_Boundary[i] = false;
	}
	for (int i = 1; i <= Union_num; i++) {
		Union_e[i][0] = 0;
	}
	for (int i = 1; i <= Point_num; i++) {
		Union_H[Bind[i]] = H[i];
	}
	for (int i = 1; i <= Point_num; i++) {
		if (Boundary[i])
			Union_Boundary[Bind[i]] = true;
		for (int j = 0; j < e[i].size(); j++) {
			if (Bind[i] != Bind[e[i][j].second])
				Union_e[Bind[i]][++Union_e[Bind[i]][0]] = Bind[e[i][j].second];
		}
	}
}
void U_edge_delete(int x, int hi) {                                            //删除与此缩点相连接的高度低于Hi的缩点
	if (Visited[x])
		return;
	Visited[x] = true;
	for (int i = 1; i <= Union_e[x][0]; i++) 
		if (Union_H[Union_e[x][i]] < hi)  
			if (Visited[Union_e[x][i]]) {
				swap(Union_e[x][i], Union_e[x][Union_e[x][0]]);
				Union_e[x][Union_e[x][0]] = 0;
				Union_e[x][0]--;
				i--;
			}else U_edge_delete(Union_e[x][i], hi);
			
		
	
}
void Handle() {                                                       //处理点与边的数据
	for (int i = 1; i <= Union_num; i++) {
		Union_H_final[i] = 0;
	}
	for (int i = 0; i < Height.size(); i++) {

		for (int j = 1; j <= Union_num; j++) {
			Visited[j] = false;
		}
		for (int j = 1; j <= Union_num; j++) {
			if (Union_Boundary[j] && Union_H[j] < Height[i]) {
				U_edge_delete(j, Height[i]);
			}
		}
		for (int j = 1; j <= Union_num; j++) {  
			if (!Visited[j]){
				Union_H_final[j] = Height[i];
				

			}

		}
	}
	for (int i = 1; i <= Point_num; i++) {
		H_final[i] = Union_H_final[Bind[i]];
	}
}
void GetLake(int x) {                                              //寻找该点附近同属的一个湖泊的点                                                
	if (Visited[x])
		return;
	Visited[x] = true;
	for (int i = 0; i < e[x].size(); i++) {
		if (H[e[x][i].second] < H_final[e[x][i].second] && H_final[x] == H_final[e[x][i].second])
			GetLake(e[x][i].second);
	}
}
void Output() {                                        //输出结果
	Lake.clear();
	for (int i = 1; i <= Point_num; i++) {
		
		Visited[i] = false;
	}
	for (int i = 1; i <= Point_num; i++) {

		if (!Visited[i] && H[i] < H_final[i]) {
			Lake.push_back(H_final[i]);
			GetLake(i);
		}
	}
	if (Lake.empty())
		printf("  0\n");
	else {
		sort(Lake.begin(), Lake.end());
		for (int i = 0; i < Lake.size(); i++) {
			printf("  %d\n", Lake[i]);
		}
	}
}

int main() {
	int Case = 0;
	

	while (2 == scanf("%d%d", &Point_num, &Edge_num) && (Point_num || Edge_num)) {                       //读取点与边的数目，为0则终止程序
		Init();                                
		Handle();
		printf("Case %d:\n", ++Case);
		Output();
	}
	//system("pause");
}


