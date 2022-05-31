#include <stdio.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a,a = b, b = tmp;

int map[8] = {
	1,1,
	2,2,
	3,3,
	0,0
};

int mapGoal[8] = {
	2,2,
	1,1,
	3,3,
	0,0
};

int flg,min,link[8][4],null[2];
int route[40],g_route[40];

void Link(int p){
	memset(link[p],-1,sizeof(link[p]));
	
	if(p / 2) link[p][0] = p - 2;
	if(p % 2) link[p][1] = p - 1;
	if(p / 2 < 3) link[p][2] = p + 2;
	if(p / 2== 0) link[p][3] = p + 1;
}

int Sorted(int *node){
	for(int i = 0 ; i < 8 ; i++){
		if(!mapGoal[i]) continue;
		if(mapGoal[i] != node[i]) return 0;
		
	}
	
	return 1;
}

void Sort(int lp, int p, int dp, int *node){
	if(dp >= min || flg) return;
	
	route[dp] = p;
	
	if(Sorted(node)){
		min = dp;
		flg = 1;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	for(int i = 0;  i < 4 ; i++){
		int np = link[p][i];
		if(np == lp || np < 0) continue;
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}

int main(){
	for(int i = 0 , val = 0;  i < 8 ; i++){
		Link(i);
		if(!map[i]) null[val++] = i;
	}
	
	for(int i = 0 ; i < 40 ; i++){
		min = i;
		flg = 0;
		
		for(int j = 0 ; j < 2 ; j++){
			Sort(-1,null[j],0,map);
			if(flg) goto exit;
		}
	}
	
	exit:
		
	printf("%d",min);
}
