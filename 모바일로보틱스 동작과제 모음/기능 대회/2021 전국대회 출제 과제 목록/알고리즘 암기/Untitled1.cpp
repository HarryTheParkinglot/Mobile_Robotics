#include <stdio.h>
#include <memory.h>
#include <windows.h>


int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;
#define M 50

int map[13] = {
	1,2,3,
	 3,2,
	0,1,0,
	 1,3,
	2,0,0
};

int flg,min,g_route[M],route[M],fix[13];
int sort_min,sort_route[10][2],g_sort_route[10][2],check[13] = {0};
int seq[8] = {1,4,7,9,11,8,5,3};
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

int hist[M][13],nowPos,stage = 0;

int InHist(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] != hist[i][j]) break;
			if(j == 12) return 1;
		}
	}
	return 0;
}


int EdgeCheck(int *node){
	if(node[0] > 0 && node[0] == node[2] && node[6] != node[0]){
		fix[0] = fix[2]= 1;
		return 1;
	}
	if(node[10] > 0 && node[10] == node[12] && node[6] != node[10]){
		fix[10] = fix[12]= 1;
		return 1;
	}
	return 0;
}

int Check(int *node){
	
	if(node[0] <= 0 || node[2] <= 0 || node[10] <= 0 || node[12] <= 0) return 0;
	if(node[0] != node[2] || node[10] != node[12]) return 0;	
	if(node[6] == node[0] || node[6] == node[12]) return 0;
	
	int pCnt = 0,pList[8] = {0};
	
	for(int i = 0 ; i < 8 ; i++){
		int col = node[seq[i]];	
		if(col == pList[pCnt-1]) return 0;
		if(col > 0) pList[pCnt++] = col;
		
	}
	
	if(pList[0] == pList[pCnt-1]) return 0;
	
	return 1;
}

int GetNext(int check, int p, int lp){
	int cnt = 0;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp || fix[np]) continue;
		if(check == 1) return np;
		cnt++;
	}
	return cnt;
}

void Sort(int lp, int p, int dp, int *node){
	if(dp >= min || flg || InHist(dp,node))  return;
	
	route[dp] = p;
	
	if((stage == 0 && EdgeCheck(node)) || (stage == 1 && Check(node))){
		flg = 1;
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}		
	
	for(int i = 0 ; i < 8 && !flg ; i++){
		int np = link[p][i];
		int nnp = -1;
		
		if(np < 0 || np == lp || fix[np]) continue;
		
		if(GetNext(0,np,p) == 1)	nnp = GetNext(1,np,p);
		
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
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np] || (node[np] > 0 && np != wp)) continue;
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
	if(node[0] == node[1] && node[5] == node[7] && node[5] == node[6] && node[11] == node[10]) return 1;
	return 0;
}

void Final(int lp, int p, int dp, int *node){
	if(dp >= sort_min || flg) return;
	
	if(Sorted(node)){
		flg = 1;
		sort_min = dp;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		return;
	}
	
	for(int i = 0; i < 13 ; i++){
		if(lp == i || node[i]  <= 0 || fix[i] || !CheckDFS(p,i,node)) continue;
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			swap(node[i],node[j]);
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			
			Final(j,g_route[min-1],dp+1,node);
			swap(node[i],node[j]);
		}
	}
}

int main(){
	for(int i = 0,val = 0 ; i < 13 ; i++) if(!map[i]) map[i] = val--;
	for(int i = 10 ; i < 13 ; i++)	
		if(map[i] <= 0) nowPos = i;
		
	//printf("%d\n",nowPos);
	
	
	for(int t = 0 ; t < 2 ; t++){
		stage = t;
		memset(hist,0,sizeof(hist));
		
		for(int i = 0 ; i < M ; i+=5){
			min = i;
			flg = 0;
			
			Sort(-1,nowPos,0,map);
			if(flg) break;
		}
		
		for(int i = 0 ; i < min ; i++){
			swap(map[g_route[i]],map[g_route[i+1]]);
		}
		
		printf("flg  %d \nmin : %d\n",flg,min);
		printf("%d %d %d\n",map[0],map[1],map[2]);
		printf(" %d %d\n",map[3],map[4]);
		printf("%d %d %d\n",map[5],map[6],map[7]);
		printf(" %d %d\n",map[8],map[9]);
		printf("%d %d %d\n",map[10],map[11],map[12]);
		nowPos = g_route[min];
	}
	
	fix[0] = fix[2] = fix[10] = fix[12] = 1;
	
	for(int i = 0 ; i < 10 ; i++){
		flg = 0;
		sort_min = i;
		Final(-1,nowPos,0,map);
		if(flg) break;
	}
	
	for(int i = 0; i < sort_min ; i++)
		swap(map[g_sort_route[i][0]],map[g_sort_route[i][1]]);
		
	printf("\n\nflg  %d \nmin : %d %d\n",flg,sort_min,nowPos);
	printf("%d %d %d\n",map[0],map[1],map[2]);
	printf(" %d %d\n",map[3],map[4]);
	printf("%d %d %d\n",map[5],map[6],map[7]);
	printf(" %d %d\n",map[8],map[9]);
	printf("%d %d %d\n",map[10],map[11],map[12]);
}

