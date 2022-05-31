#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[16] = {
	0,0,0,0,
	0,0,0,0,
	0,0,4,0,
	2,3,0,1
};

int mapGoal[16] = {
	0,2,0,0,
	3,0,1,0	
};

int link[16][8];
int bLink[8][4];

int bMap[8] = {
	2,3,1,0,
	2,3,1,0
};
int node[8];


int g_route[15],route[15],min,check[16],flg = 0;
int dfs_g_route[15][3],dfs_route[15][3],dfs_min[3];   //[0] : 점 이동 / [1] : 이동 방식 / [2] : 지나갈 수 있는 A2 노드  
int null[2] = {0},cnt = 0,val = 0;
int pass[4] = {0},block = 2;

void Link(int p, int *str){
	memset(str,-1,sizeof(link[0]));
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 < 3) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
	
	if(str[0] >= 0 && str[1] >= 0) str[4] = p - 5;
	if(str[2] >= 0 && str[1] >= 0) str[5] = p + 3;
	if(str[2] >= 0 && str[3] >= 0) str[6] = p + 5;
	if(str[0] >= 0 && str[3] >= 0) str[7] = p - 3;
}

void BLink(int p, int *str){
	memset(str,-1,sizeof(bLink[0]));
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 < 1) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
}

int Sorted(int *node){
	for(int i =  0 ; i < 8 ; i++){
		if(mapGoal[i] < 0) continue;
		if(node[i] != mapGoal[i]) return 0;
	}
	return 1;
}

void Sort(int lp, int p, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(Sorted(node)){
		flg = 1;
		min = dp;
		memcpy(g_route,route,sizeof(g_route));
		return;
	}
	
	for(int i = 0 ; i < 4 && !flg; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}

bool MatchRule(int p, int nextMode, int nextPass, int convertCnt){
	
	
}


void DFS(int p, int mode, int pass, int convertCnt int wp, int dp, int *node){
	//dfs_min 제한을 8로 두고 Goal 경우가 구해지지 않는 경우 버림처리 
	if(dp >= min) return;
	
	if(p == wp){
		min = dp;
		memcpy(g_dfs_route,dfs_route,sizeof(g_dfs_route));
		return;
	} 
	
	check[p] = 1;
	
	for(int i = 0 ; i < 8 ; i++){
		int nextPass = -1;
		bool needConvert = 0;
		
		int np = link[p][i];
		if(np < 0 || node[np] == 4) continue;
		if(p / 4 == 2 && (i == 1 || i == 3)) continue;
		if(np / 4 == 2)	{nextPass = (np % 4 < block)? np % 4 : np % 4 + 1; if(!node[bMap])}
		
		
	}
} 


int main(){
	
	for(int i = 0 ; i < 8 ; i++){
		BLink(i,bLink[i]);
		if(!bMap[i]) null[cnt++] = val--;
	}
	
	for(int i = 0 ; i < 16 ; i++)	Link(i,link[i]);
	
}

