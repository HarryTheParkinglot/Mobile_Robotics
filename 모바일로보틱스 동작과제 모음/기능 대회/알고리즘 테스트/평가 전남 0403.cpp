#include <stdio.h>
#include <memory.h>
int tmp = 0;
#define swap(a,b) tmp = node[a],node[a] = node[b],node[b] = tmp;
#define M 50
int map[9] = {
	1,1,1,
	2,2,2,
	3,0,3
};


int mapGoal[9] = {
	1,1,1,
	2,2,0,
	3,3,2
};

int node[9];
int link[9][4] = {0};
int g_route[M],route[M],min,flg=  0;

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 3 - 1 >= 0) str[0] = p - 3;
	if(p % 3 - 1 >= 0) str[1] = p - 1;
	if(p / 3 + 1 < 3) str[2] = p + 3;
	if(p % 3 + 1 < 3) str[3] = p + 1;
}

int Sorted(){
	for(int i = 0 ; i < 9 ; i++)
		if(node[i] != mapGoal[i]) return 0;
	return 1;
}

void Sort(int lp, int p, int dp, int *node){
	if(dp >= min || flg) return;
	
	route[dp] = p;
	
	if(Sorted()){
		flg=  1;
		min = dp;
		
		memcpy(g_route,route,sizeof(int) * (min + 1));
		return;
	}
	
	for(int i = 0 ; i < 4;  i++){
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
		Link(i,link[i]);
 	}
 	
 	for(int i = 0 ; i <= M ; i++){
	 	min = i;
	 	
	 	memcpy(node,map,sizeof(int) * 9);
	 	
	 	Sort(-1,bp,0,node);
	 	if(flg) break;
	 }
	 
	 printf("%d",min);
}


