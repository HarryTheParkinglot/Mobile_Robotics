#include <stdio.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;
#define M 20

int map[13] = {
	1,0,2,
	 3,3,
	1,1,2,
	 3,2,
	0,0,0
};

int fix[13] = {0},check[13];
int min,sort_min,flg;
int route[10],g_route[10];
int sort_route[M][2],g_sort_route[M][2];
int stage,hist[M][13],nowPos;
int link[13][8] = {
	{-1,-1,-1,-1,-1,3,1,-1},
	{-1,-1,0,3,-1,4,2,-1},
	{-1,-1,1,4,-1,-1,-1,-1},
	
	{-1,0,-1,5,-1,-1,-1,1},
	{-1,1,-1,-1,-1,7,-1,2},
	
	{-1,-1,-1,-1,-1,8,-1,3},
	{-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,4,-1,9,-1,-1,-1,-1},
	
	{-1,5,-1,10,-1,11,-1,-1},
	{-1,-1,-1,11,-1,12,-1,7},
	
	{-1,-1,-1,-1,-1,-1,11,8},
	{-1,8,10,-1,-1,-1,12,9},
	{-1,9,11,-1,-1,-1,-1,-1}
};

int SideCheck(int *node){
	if(node[0] > 0 && node[0] == node[2] && node[0] != node[6]){
		fix[0] = fix[2] = 1;
		return 1;
	}
	if(node[10] > 0 && node[10] == node[12] && node[10] != node[6]){
		fix[10] = fix[12] = 1;
		return 1;
	}
	return 0;
}

int seq[8] = {1,4,7,9,11,8,5,3};

int Check(int *node){
	if(node[0] <= 0 || node[2] <= 0 || node[10] <= 0 || node[12] <= 0) return 0;
	if(node[0] != node[2] || node[10] != node[12] || node[6] == node[0] || node[6] == node[10]) return 0;
	
	int pCnt,pList[4];
	
	for(int i = 0 ; i < 8 ; i++){
		int col = node[seq[i]];
		
		if(col <= 0) continue;
		
		if(pCnt) if(pList[pCnt-1] == col) return 0;
		pList[pCnt++] = col;
	}
	if(pList[pCnt-1] == pList[0]) return 0;
	fix[0] = fix[2] = fix[10] = fix[12] = 1;
	return 1;
}

int Sorted(int *node){
	return (node[0] == node[1] && node[5] == node[6] && node[6] == node[7] && node[10] == node[11]);
}

void DFS(int p, int wp, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np] || (np != wp && node[np] > 0)) continue;
		DFS(np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int p, int wp, int *node){
	min = 10;
	memset(check,0,sizeof(check));
	DFS(p,wp,0,node);
	
	return min != 10;
}

int InList(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j  < 13 ; j++){
			if(hist[i][j] != node[j]) break;
			if(j == 12) return 1;
		}
	}
	return 0;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp >= sort_min) return;
	
	if((stage == 0 && SideCheck(node)) || ( stage == 1 && Check(node)) || (stage == 2 && Sorted(node))){
		flg =1;
		sort_min = dp;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		return;
	}
	
	for(int i = 0 ; i <13 ; i++){
		if(fix[i] || node[i] <= 0 || i == lp || !CheckDFS(p,i,node)) continue;
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)){
				sort_route[dp][0] = i;
				sort_route[dp][1] = j;
				swap(node[i],node[j]);
				Sort(j,g_route[min-1],dp+1,node);
				swap(node[i],node[j]);
			}
		}
	}
}


int main(){
	for(int i = 10 ; i < 13 ; i++)	if(!map[i]) nowPos = i;
	printf("%d",nowPos);
	
	for(stage = 0 ; stage <3 ; stage ++){
		
		flg = 0;
		int next = (stage)? g_sort_route[sort_min][1] : -1;
		for(int i = 0 ; i < M ; i++){
			sort_min = i;
			
			Sort(next,nowPos,0,map);
			if(flg) break;
		}
		printf("FLG: %d\n",flg);
	}
}
