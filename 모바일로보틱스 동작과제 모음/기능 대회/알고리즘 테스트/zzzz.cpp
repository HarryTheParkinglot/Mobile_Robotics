#include <stdio.h>
#include <memory.h>
#include <windows.h>
#define M 40

int tmp;
#define swap(a,b) tmp = node[a],node[a] = node[b],node[b] = tmp;


int flg,min;
int link[9][4],g_route[M],route[M];

int map[9] = {
	1,1,1,
	2,2,2,
	3,3,0
};
int node[9];
int mapGoal[9] = {
	1,1,0,
	2,2,1,
	3,2,3 
};


int Finish(){
	for(int i = 0; i < 9 ; i++)
		if(node[i] != mapGoal[i]) return 0;
		
	return 1;
}

void CreateLink(int p, int *str){
	memset(str,-1,sizeof(int)*4);
	if(p / 3 - 1 >= 0) str[0] = p-3;
	if(p % 3 - 1 >= 0) str[1] = p-1;
	if(p / 3 + 1 < 3) str[2] = p+3;
	if(p % 3 + 1 < 3) str[3] = p+1;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp >= min) return;
	
	

	route[dp] = p;
	if(Finish()){
		flg =1;
		min = dp;
		
		for(int i = 0; i <= dp ; i++)
			g_route[i] = route[i];
			
		return;
	}
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		swap(p,np);
		Sort(p,np,dp+1,node);
		swap(p,np);
	}
}

int main(){
	int null;
	for(int i = 0; i < 9 ; i++){
		if(!map[i]) null = i;
		CreateLink(i,link[i]);
	}
	
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			printf("%d ",link[i][j]);		
		}
		printf("\n");
	}	
		
	for(int i = 0 ; i <= M;  i++){
		memcpy(node,map,sizeof(int) * 9);
		min = i;
		
		Sort(-1,null,0,node);
		if(flg) break;
	}
	
	printf("%d",min);
}
