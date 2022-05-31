#include <stdio.h>
#include <memory.h>
#define M 30
int tmp = 0;


int map[9] = {
	1,0,1,
	2,1,2,
	3,2,3
};
int node[9] = {
	0
};
int mapGoal[9] = {
	1,1,1,
	2,2,2,
	3,3,0
};

int flg = 0,min,route[M],g_route[M],link[9][4];
#define swap(a,b) tmp = node[a],node[a] = node[b],node[b] = tmp;

int Finish(){
	for(int i = 0 ; i < 9 ; i++)
		if(node[i] != mapGoal[i])	return 0;
	return 1;
}

void Create(int p, int *str){
	memset(str,-1,sizeof(int)  * 4);
	
	if(p / 3 - 1 >= 0) str[0] = p - 3;
	if(p % 3 - 1 >= 0) str[1] = p - 1;
	if(p / 3 + 1 < 3) str[2] = p + 3;
	if(p % 3 + 1 < 3) str[3] = p + 1;
}

void Sort(int lp, int p, int dp, int *node){	
	if(flg || dp >= min) return;
	
	route[dp] = p;
	
	if(Finish()){
		flg =1 ;
		min = dp;
		memcpy(g_route,route,sizeof(int) * (min + 1));
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
	int null ;
	for(int i = 0 ; i < 9 ; i++){
		if(!map[i]) null = i;
		Create(i,link[i]);
	}
	
	for(int i=  0; i <= M ; i++){
		min = i;
		
		memcpy(node,map,sizeof(int) * 9);
		Sort(-1,null,0,node);
		
		if(flg) break;
	}
	
	printf("\n\nmin : %d\n",min);
	
	for(int i = 0 ; i <= min ; i++)
		printf("%d->",g_route[i]);
}
