#include <stdio.h>
#include <memory.h>
#include <windows.h>

int fix[8] = {0};
int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;
#define M 60


int map[8] = {
	1,1,
	2,2,
	3,3,
	0,0
};
int mapGoal[8] = {
	3,3,
	2,2,
	1,1,
	0,0
};
int hist[M][8];

int InHist(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 8 ; j++){
			if(hist[i][j] != node[j]) break;
			if(j == 7) return 1;
		}
	}
	return 0;
}


int flg = 0, null[2],g_route[M][2],route[M][2],min,dfs_min;
int link[8][4],check[8];
int dfs_route[5],g_dfs_route[5];
int stage = 0;
int nowPos;
int stack = 0;

int call = 0, total = 0;

int GetCnt(int p, int *node){
	int spot[4] = {0,2,6,7};
	int cnt = 0;
	
	
	for(int i = 0 ; i < 4 ; i++){
		if(p == spot[i]) continue;
		cnt += !(mapGoal[spot[i]] > 0 && mapGoal[spot[i]] == node[spot[i]]);
	}
	return cnt;
}

int Sorted(int *node){
	for(int i = 0 ; i < 8 ; i++){
		if(mapGoal[i] < 0) continue; //상관없음표시  
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

void DFS(int p, int wp, int dp, int *node){
	if(dp >= dfs_min) return;
	
	dfs_route[dp] = p;
	
	if(p == wp){
		dfs_min = dp;
		memcpy(g_dfs_route,dfs_route,sizeof(dfs_route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np] || (np != wp && node[np])) continue;
		DFS(np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int p, int wp, int *node){
	dfs_min = 5;
	memset(check,0,sizeof(check));
	DFS(p,wp,0,node);
	return dfs_min != 5;
}

void Sort(int lp, int p, int dp, int *node){
	call++;
	total++;
	if(flg || dp >= min) return;
	
	if(InHist(dp,node)) return;
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	if(Sorted(node) || (stage == 0 && GetCnt(p,node) < stack)){
		flg = 1;
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	
	for(int i = 0 ; i < 8 ; i++){
		if(i == lp || !node[i] || !CheckDFS(p,i,node)) continue; //고정되어 있는건 먹으면 안됨 
		
		for(int j = 0 ; j < 8 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			swap(node[i],node[j]);
			route[dp][0] = i;
			route[dp][1] = j;
			Sort(j,g_dfs_route[dfs_min-1],dp+1,node);
			swap(node[i],node[j]);
		}
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
	
	stage = 0;		
	stack = GetCnt(null[0],map);
	nowPos = -1;
	
	while(!Sorted(map)){
		call = 0;	
		
		stage = !stack;
		
		for(int i = 0 ; i < M ; i++){
			min = i;
			flg =  0;
			Sort(-1,(nowPos < 0)? null[0] : nowPos,0,map);
			if(flg == 1) break;
		}
		
		printf("stage : %d\n",stage);	
		
		if(nowPos < 0) nowPos = null[0];
		if(stack) stack = GetCnt(nowPos,map);				
		printf("\n\n%d%d\n%d%d\n%d%d\n%d%d\n",map[0],map[1],map[2],map[3],map[4],map[5],map[6],map[7]);	
		for(int i = 0 ; i < min ; i++){
			swap(map[g_route[i][0]],map[g_route[i][1]]);
			CheckDFS(g_route[i][0],g_route[i][1],map);
			nowPos = g_dfs_route[dfs_min-1];
		}
	}
	
	
	
	
}
