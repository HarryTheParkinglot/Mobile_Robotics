#include <stdio.h>
#include <memory.h>
#include <math.h>

int tmp;
#define swap(a,b)  tmp = a,a = b,b = tmp;
#define M 50

int map[8] = {
	1,0,
	2,1,
	3,2,
	0,3
};
int mapGoal[8] = {
	3,0,
	2,3,
	1,2,
	0,1
};

int flg = 0, min = 0, sort_min = 0;
int route[10],g_route[10],check[8];
int sort_route[M][2],g_sort_route[M][2];
int stack = 0,nullCnt = 0,null[2];
int link[8][4],nowPos = -1,last = -1,call = 0;
int hist[M][8],stage = 0,total = 0;

int GetDist(int *node){
	int dist[2] = {0},mark[8] = {0};
	
	for(int i = 0 ; i < 8 ; i++)
		mark[i] = (node[i] == mapGoal[i]);
		
	for(int i = 0 ; i < 8 ; i++){ //골대  
		if(!mapGoal[i] || node[i] == mapGoal[i]) continue;
		
		int min_dist = 100,min_pos = -1;
		
		for(int j = 0 ; j < 8 ; j++){
			if(!node[j] || mark[j] || node[j] != mapGoal[i]) continue;
			
			if(abs(i/2 - j/2) + abs(i%2 - j%2) < min_dist) {
				//printf("%d\n",i);
				min_pos = j;
				min_dist = abs(i/2 - j/2) + abs(i%2 - j%2);
			}
		}
		
		mark[min_pos] = 1; //이미 선점함 
		dist[(i/2)%3] += min_dist;
	}
	
	return dist[0] * 5;
}

int InHist(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 8 ; j++){
			if(node[j] != hist[i][j]) break;
			if(j == 7) return 1;
		}
	}
	return 0;
}

int Sorted(int *node){
	for(int i = 0 ; i < 8 ; i++)
		if(node[i] != mapGoal[i]) return 0;
	return 1;
}

void DFS(int p, int wp, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
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
	min = 5;
	memset(check,0,sizeof(check));
	DFS(p,wp,0,node);
	
	return min != 5;
}
	
void Sort(int lp, int p, int dp, int *node){
	call++;
	total++;
	
	if(dp >= sort_min || flg) return;
	
	if(call > 100000) {
		flg = 2;
		return;
	}
	
	if(Sorted(node) || (stage == 0 && GetDist(map) <= stack)){
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		return;
	}
	
	for(int i = 0 ; i < 8 && !flg; i++){
		if(node[i] <= 0 || i == lp || !CheckDFS(p,i,node)) continue;
		
		for(int j = 0 ; j < 8 && !flg; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			swap(node[i],node[j]);
			Sort(j,g_route[min-1],dp+1,node);
			swap(node[i],node[j]);
		}
	}
}

int Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	if(p / 2) str[0] = p-2;
	if(p % 2) str[1] = p-1;
	if(p / 2 < 3) str[2] = p+2;
	if(p % 2 == 0) str[3] = p+1;
}

int main(){
	
	for(int i = 0,cnt = 0 ; i < 8 ; i++){
		Link(i,link[i]);
		if(!map[i]) null[nullCnt++] = i;
	}
	
	if(CheckDFS(null[0],null[1],map)) nullCnt--;
	
	stack = GetDist(map);
	
	for(stage = 1 ; stage <= 1 ; stage++){
		if(stack > 4) stage = 0;
		rt:
		flg = 0;
		call =  0;
		
		for(int i = 0 ; i < M; i ++){
			sort_min = i;
					
			nowPos = (nowPos < 0)? null[0] : nowPos;
			Sort(last,nowPos,0,map); 			
			
			if(flg == 2) {stack--; stage = 0; goto rt;}
			if(flg == 1) {
				if(!stage) stack -= (stack > 0);
				goto exit;
			}
		}

		exit:
		
		for(int i = 0 ; i < sort_min && flg == 1; i++){
			swap(map[g_sort_route[i][0]],map[g_sort_route[i][1]]);
			CheckDFS(g_sort_route[i][0],g_sort_route[i][1],map);
			nowPos= g_route[min-1];
		}
		last = g_sort_route[sort_min][1];
		
		printf("stage %d\n",stage);
		printf("stack %d\n",stack);
		printf("FLG %d",flg);
		printf("call : %d min %d\n",call,sort_min);
		
		for(int i = 0 ; i < 8 ; i++){
			printf("%-3d",map[i]);
			if(i % 2) printf("\n");
		}
	}
	printf("TT%d",total);
}
