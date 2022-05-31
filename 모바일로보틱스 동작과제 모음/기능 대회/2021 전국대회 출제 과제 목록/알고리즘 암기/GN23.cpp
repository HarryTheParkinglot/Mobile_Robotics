#include <stdio.h>
#include <memory.h>

int tmp;
#define M 20
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[13] = {
	1,0,2,
	 3,3,
	2,1,3,
	 0,0,
	2,1,0
};

int fix[13] = {0},flg = 0,sort_min;
int sort_route[M][2],g_sort_route[M][2];
int route[10],g_route[10],min,check[13] = {0};
int hist[M][13];
int nowPos,stage = 0;

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
	if(node[0] != node[2] || node[10] != node[12] || node[0] == node[6] || node[10] == node[6]) return 0;
	
	int pCnt,pList[6] = {0};
	
	for(int i = 0 ; i < 8;  i++){
		int col = node[seq[i]];
		
		if(col > 0 && pCnt) 
			if(col == pList[pCnt-1]) return 0;
		if(col > 0) pList[pCnt++] = col;
	}
	
	if(pList[pCnt-1] == pList[0]) return 0;
	
	fix[0] = fix[2] = fix[10] = fix[12] = 1;
	
	return 1;
}

int Sorted(int *node){
	if(node[0] != node[1]) return 0;
	if(node[10] != node[11]) return 0;
	if(node[5] != node[6] || node[6] != node[7]) return 0;
	return 1;
}


int InHist(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 13 ; j++){
			if((node[j] > 0 || node[j] == -3) && node[j] != hist[i][j]) break;
			if(j == 12) return 1;
		}
	}
	return 0;
}


void DFS(int p, int wp , int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0; i < 8 ; i++){
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

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp >= sort_min) return;
	if(InHist(dp,node)) return;
	
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	if((stage == 0 && SideCheck(node)) || (stage == 1 && Check(node)) || (stage == 2 && Sorted(node))){
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));	
		sort_min = dp;

		return;
	}
	
	for(int i = 0 ; i < 13 ; i++){
		if(lp == i || node[i] <= 0 || fix[i] || !CheckDFS(p,i,node)) continue;
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			swap(node[i],node[j]);
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			
			Sort(j,g_route[min-1],dp+1,node);
			swap(node[i],node[j]);
		}
	}
}

int main(){
	for(int i = 0,val = 0 ; i < 13 ; i++){
		if(!map[i]) map[i] = val--;
		if(map[i] <= 0 && i >= 10) nowPos = i;
	}
	
	for(int t = 0 ;  t < 3 ; t ++){
		memset(hist,0,sizeof(hist));
		flg = 0;
		stage = t;
			
		for(int i = 0; i < M ; i+=3){
			sort_min = i;
			Sort(-1,nowPos,0,map);
			if(flg) break;	
		}
		
		for(int i = 0 ; i < sort_min ; i++){
			
			swap(map[g_sort_route[i][0]],map[g_sort_route[i][1]]);
			CheckDFS(g_sort_route[i][0],g_sort_route[i][1],map);
		}
		nowPos = g_route[min-1];
		printf("NP %d",nowPos);
		printf("FLG %d\n%d %d %d\n %d %d\n%d %d %d\n%d %d\n%d %d %d\n\n",flg,map[0],map[1],map[2],map[3],map[4],map[5],map[6],map[7],map[8],map[9],map[10],map[11],map[12]);
	}
	
}

