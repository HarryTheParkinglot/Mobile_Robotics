#include <stdio.h>
#include <memory.h>
#include <windows.h>
int map[8] = {
	1,1,
	2,2,
	3,3,
	0,0
};
int mapGoal[8] = {
	0,0,
	3,3,
	2,2,
	1,1
};

int fix[8] = {0};
int tmp;


#define swap(a,b) tmp = a, a = b, b = tmp;
#define M 60

int flg = 0, null[2],g_route[M],route[M],min,dfs_min;
int link[8][4],check[8];
int hist[M][8];
int dfs_route[5],g_dfs_route[5];
int stage = 0,nowPos;

int InHist(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 8 ; j++){
			if(node[j] && node[j] != hist[i][j]) break;
			if(j == 7) return 1;
		}
	}
	return 0;
}


int Sorted(int *node){
	for(int i = 0 ; i < 8 ; i++){
		if(!mapGoal[i]) continue;
		if(mapGoal[i] != node[i]) return 0;
	}
	return 1;
}

void Link(int p){
	memset(link[p],-1,sizeof(link[p]));
	if(p/2) link[p][0] = p-2;
	if(p%2) link[p][1] = p-1;
	if(p/2 < 3) link[p][2] = p+2;
	if(p%2 == 0) link[p][3] = p+1;
}

int call=  0;

int SideCheck(int p, int *node){
	int check = 0;
	
	for(int i = 0 ; i <= 6 ; i += 6){
		for(int j = 0; j < 2 ; j++){
			if(fix[i + j]) continue;
			if(node[i+j] != mapGoal[i+j] || p == i + j)	break;
			
			fix[i + j] = 1;
			check ++;
		}
	}
	return (check > 0);
}
void Sort(int lp, int p, int dp, int *node){
	call++;
	if(flg || dp >= min) return;

	if(InHist(dp,node)) return;
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	route[dp] = p;
	
	if(Sorted(node)){
		flg= 1;
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	for(int i = 0 ; i < 4 && !flg; i++){
		int np = link[p][i];
		if(np < 0 || np == lp || fix[np]) continue;
		
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}

int main(){
	for(int i = 0,cnt = 0 ; i < 8 ; i++){
		Link(i);
		if(!map[i]) {
			null[cnt++] = i;
			map[i] = -cnt;
		}
	}
	
	for(stage = 0 ; stage < 2 ; stage++){
		for(int i = 0; i < M ; i+=2){
			min = i;
			flg = 0;
			if(stage) nowPos = g_route[min-1];
			else nowPos = -1;
			
			for(int j = 0 ; j < 2 ; j++){
				Sort(-1,nowPos,0,map);		
				if(flg) goto exit;
			}	
		}
		
		exit:
		for(int i = 0 ; i < min ; i++){
			//printf("%d - > %d\n",g_route[i][0],g_route[i][1]);
			swap(map[g_route[i]],map[g_route[i+1]]);
		}
		
		printf("%d%d\n%d%d\n%d%d\n%d%d\n",map[0],map[1],map[2],map[3],map[4],map[5],map[6],map[7]);
		//printf("nowPos");
		printf("flg %d \nmin%d\n",flg,min);
		printf("CALL %d\n",call);
	}		
}
