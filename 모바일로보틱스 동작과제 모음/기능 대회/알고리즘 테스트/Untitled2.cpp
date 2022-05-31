#include <stdio.h>
#include <memory.h>
#define M 40

int  tmp = 0;
#define swap(a,b) tmp = node[a]; node[a] = node[b]; node[b] = tmp;

int node[9],link[9][4];
int map[9] = {
	2,1,1,
	0,2,2,
	3,3,3
};
int mapGoal[9] = {
	1,0,1,
	2,2,2,
	3,3,3
};
int g_route[M]  = {0};
int route[M]  = {0};

int min,flg = 0;
int null = 0;

void CreateLink(int p, int *str){
	memset(str,-1,sizeof(int)*4);
	
	if(p / 3 - 1 >= 0) str[p] = p - 3; 
	if(p % 3 + 1 >= 0) str[p] = p - 1;
	if(p / 3 - 1 < 3) str[p] = p + 3;
	if(p % 3 + 1 < 3) str[p] = p + 1;
}


int Finish(){
	for(int i = 0; i < 9 ; i++)
		if(node[i] != mapGoal[i]) return 0;
		
	return 1;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp >= min) return;
	
	route[dp] = p;
	
	
	if(Finish()){
		flg =1;
		min = dp;
		
		for(int i = 0 ; i <= min ; i++)
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
	for(int i = 0 ; i < 9 ; i++){
		CreateLink(i,link[i]);
		if(!map[i]) null = i;
	}	
	
	for(int i = 0; i <= M ; i++){
		min = i;
		memcpy(node,map,sizeof(int) * 9);
		Sort(-1,null,0,node);
		if(flg) break;
	}
	
	printf("%d",min);
}
