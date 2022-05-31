#include <stdio.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;
#define M 50
int cnt = 0;
int map[13] = {
	1,1,1,
	 0,0,
	2,2,3,
	 0,3,
	2,0,3
};

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

int fix[13] = {0},flg = 0,min,route[M],g_route[M],stage = 0;
int sort_min,sort_route[10][2],g_sort_route[10][2];
int seq[8] ={1,4,7,9,11,8,5,3},check[13] = {0};
int nowPos,hist[M][13];


int EdgeCheck(int *node){
	if(node[0] > 0 && node[0] == node[2] && node[6] != node[0]){
		fix[0] = fix[2] = 1;
		return 1;
	}
	if(node[10] > 0 && node[10] == node[12] && node[6] != node[10]){
		fix[10] = fix[12] = 1;
		return 1;
	}
}

int Check(int *node){
	if(node[0] <= 0 || node[2] <= 0 || node[10] <= 0 || node[12] <= 0) return 0;
	if(node[0] != node[2] || node[10] != node[12]) return 0;
	if(node[0] == node[6] || node[10] == node[6]) return 0;
	
	int pCnt,pList[8];
	
	for(int i = 0 ; i < 8 ; i++){
		int col = node[seq[i]];
		
		if(col <= 0) continue;
		if(pCnt && col == pList[pCnt-1]) return 0;
		pList[pCnt ++] = col;
	}
	
	if(pList[pCnt-1] == pList[0]) return 0;
	
	return 1;
}

int GetNext(int check, int p, int lp){
	int cnt = 0;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || fix[np] || np == lp ) continue;
		cnt++;
		
		if(check) return np;
	}
	return cnt;
}

int InList(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] != hist[i][j]) break;
			if(j == 12) return 1;
		}
	}
	return 0;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp >= min) return;
	cnt++;
	route[dp] = p;
	
	if((stage == 0 && EdgeCheck(node))|| (stage == 1 && Check(node))){
		flg = 1;
		memcpy(g_route,route,sizeof(route));
		min = dp;
	}
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		int nnp = -1;
		
		if(np < 0 || fix[np] || np == lp )	continue;
		if(GetNext(0,np,p) == 1)	nnp = GetNext(1,np,p);	
		
		if(nnp == lp) continue;
		
		swap(node[p],node[np]);
		
		if(nnp >= 0){
			swap(node[np],node[nnp]);
			route[dp+1] = np;
			Sort(np,nnp,dp+2,node);
		}	
		else	Sort(p,np,dp+1,node);
		
		if(nnp >= 0){
			swap(node[np],node[nnp]);
		}
		
		swap(node[p],node[np]);
	}
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
	
	for(int i = 0; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np] || (np != wp && node[np] > 0)) continue;
		DFS(np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int p, int wp, int *node){
	memset(check,0,sizeof(check));
	min = 10;
	DFS(p,wp,0,node);
	
	return min != 10;
}

int Sorted(int *node){
	return (node[0] == node[1] && node[10] == node[11] && node[5] == node[6] && node[6] == node[7]);
}

void Final(int lp, int p, int dp, int *node){
	if(flg || dp >= sort_min) return;
	
	if(Sorted(node)){
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;	
		return;
	}
	
	for(int i = 0 ; i < 13 && !flg ; i++){
		if(lp == i || node[i] <= 0 || fix[i] || !CheckDFS(p,i,node)) continue;
		
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			
			swap(node[i],node[j]);
				
			Final(j,g_route[min-1],dp+1,node);

			swap(node[i],node[j]);
		}
	}
}

int main(){
	for(int i = 0 , val = 0; i < 13 ; i++){
		if(!map[i])	map[i] = val --;
		
		if(i >= 10 && map[i] <= 0) nowPos = i; 
	}
	
	for(int t = 0 ; t < 2 ; t++){
		stage = t;
		flg = 0;
		cnt = 0;
		
		for(int i = 0 ; i < M ; i++){
			memset(hist,0,sizeof(hist));
			min = i;
			Sort(-1,nowPos,0,map);
			if(flg) break;
		}
		printf("min : %d\ncnt %d\n",min,cnt);
		
		for(int i = 0; i < min ; i++){
			swap(map[g_route[i]],map[g_route[i+1]]);
		}
		
		nowPos = g_route[min];
	}
	
	fix[0] = fix[2] = fix[10] = fix[12] = 1;
	
	flg = 0;
	
	printf("BP : %d\n",nowPos);
	
	
	
	for(int i = 0 ; i < 12 ; i++){	
		sort_min = i;
		Final(-1,nowPos,0,map);
		if(flg) break;
	}
	
	
	printf("flg %d\n",flg);		
	
	for(int i = 0 ; i < sort_min ; i++){
		swap(map[g_sort_route[i][0]],map[g_sort_route[i][1]]);	
	}
	printf("%d %d %d\n%d %d\n%d %d %d\n%d %d\n%d %d %d",map[0],map[1],map[2],map[3],map[4],map[5],map[6],map[7],map[8],map[9],map[10],map[11],map[12]);
	
	
}
