#include <stdio.h>
#include <memory.h>
#define M 50

int tmp;
#define swap(a,b) tmp = node[a],node[a] = node[b],node[b] = tmp;

int node[9] = {0};
int map[9] = {
	1,0,2,
	3,3,3,
	2,1,1
};
int mapGoal[9] = {
	2,0,1,
	3,3,3,
	2,1,1
};
int min,flg = 0,link[9][4];
int route[M],g_route[M];
int nul;

int Finish(){
	for(int i = 0 ; i < 9 ; i++)
		if(node[i] != mapGoal[i]){
				return 0;
	}
	return 1;
}

void CreateLink(int p, int *str){
	memset(str,-1,sizeof(int)*4);
	
	if(p /3 - 1 >= 0) str[0] = p-3;
	if(p %3 - 1 >= 0) str[1] = p-1;
	if(p /3 + 1 < 3) str[2] = p+3;
	if(p %3 + 1 < 3) str[3] = p+1;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp > min) 	return;
	
	route[dp] = p;
	
	if(Finish()){
		flg = 1;
		min = dp;
		
		for(int i = 0 ; i <= dp ; i++)
			g_route[i] = route[i];
		
		return;
	}
	
	for(int i = 0 ; i < 4 && !flg ; i++){
		int np = link[p][i];
		
		if(np < 0 || np == lp) continue;
		
		swap(p,np);
		Sort(p,np,dp+1,node);
		swap(p,np);
	}
}

int main(){
	for(int i = 0 ; i < 9 ; i++){
		CreateLink(i,link[i]);
		if(!map[i]) nul = i;
	}
	
	for(int i = 0 ; i <= 30 ; i++){
		min = i;
		memcpy(node,map,sizeof(int)* 9);
		Sort(-1,nul,0,node);
		if(flg) break;
	}
	
	printf("%d",min);
}
