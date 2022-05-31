#include <stdio.h>
#include <memory.h>
#define M 40


int tmp = 0;
#define swap(a,b) tmp = node[a], node[a] = node[b],node[b] = tmp;


int map[9] = {
	1,1,1,
	2,2,0,
	3,3,2
};
int mapGoal[9] = {
	1,1,1,
	2,2,2,
	3,0,3
};
int node[9];

int g_route[M],route[M],min,flg = 0;
int link[9][4] = {0};

void CreateLink(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 3 - 1 >= 0) str[0] = p - 3;
	if(p % 3 - 1 >= 0) str[1] = p - 1;
	if(p / 3 + 1 < 3) str[2] = p + 3;
	if(p % 3 + 1 < 3) str[3] = p + 1;
}

int Sorted(){
	for(int i = 0 ; i < 9 ; i++)
		if(node[i] != mapGoal[i])	
			return 0;
	return 1;
}


void Sort(int lp, int p, int dp, int *node){
	if(dp >= min || flg) return;
	

	route[dp] = p;
	
	if(Sorted()){
		min = dp;
		memcpy(g_route,route,sizeof(int) * (dp + 1));
		flg = 1;
		return;
	}
	
	for(int i = 0 ; i < 4 && !flg ; i++ ){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		
		swap(p,np);
		Sort(p,np,dp+1,node);
		swap(p,np);
	}
	return;
}

int main(){
	int bp = 0;
	for(int i = 0 ; i < 9 ; i++){
		if(!map[i]) bp = i;
		CreateLink(i,link[i]);
	}
	
	for(int i = 0 ; i <= M ; i++){
		min = i;
		memcpy(node,map,sizeof(int) * 9);
		Sort(-1,bp,0,node);
		if(flg) break;
	}
	
	printf("%d",min);
}
