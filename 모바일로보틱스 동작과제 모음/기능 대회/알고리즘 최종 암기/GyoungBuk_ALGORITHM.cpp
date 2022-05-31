#include <stdio.h>
#include <memory.h>
#include <windows.h>
#define M 30

int tmp;
#define swap(a,b)  tmp = a, a = b, b = tmp;
int map[8] = {
	3,0,
	3,1,
	2,0,
	1,2
};
int shufmap[8];
int mapGoal[8] = {
	0,0,
	1,1,
	2,2,
	3,3
};

int link[8][4];
int history[30][8] = {0};
int call= 0 ;
int null = 0,shuffle = 0,caseCnt = 0;
int sort_route[M],g_sort_route[M],sort_min;
int g_shuf_route[M],shuf_min;
int nowPos;
int TOTAL = 0;

int CheckSortAble(int p,int *node);

int Shuffled(int *now){ //셔플 검사
	int spot[4] = {0,1,6,7};
	
	int match = 0;
	int cnt;
	f
	or(int i = 0 ; i < caseCnt ; i++){
		cnt = 0;
		
		for(int j = 0 ; j < 8 ; j++)
			if(now[j] && now[j] == history[i][j]) cnt++;
		if(cnt >= 2) return 0;
		
	}
		
	memcpy(history[caseCnt],now,sizeof(history[caseCnt]));
	caseCnt++;		
	return 1;
}
void Link(int p){
	memset(link[p],-1,sizeof(link[p]));
	
	if(p/2) link[p][0] = p-2;
	if(p%2) link[p][1] = p-1;
	if(p/2 < 3) link[p][2] = p+2;
	if(p%2 == 0) link[p][3] = p+1;
}

int Sorted(int *node){
	for(int i = 0 ; i < 8 ; i++)
		if(node[i] != mapGoal[i]) return 0;
	return 1;
}

int Sort(int lp, int p, int dp, int *node){ //초기 데이터에서 셔플 // 	
	if(!shuffle)	call++;
	TOTAL ++;
	
	if(dp >= sort_min) return 0;
	if(call > 100000) return 0;
	
	sort_route[dp] = p;
	
	if(shuffle && Shuffled(node)){
		
		memcpy(g_shuf_route,sort_route,sizeof(sort_route));
		shuf_min = dp;
		
		shuffle = 0;
		printf("TOTAL %d\n\n",TOTAL);
		
		if(CheckSortAble(sort_route[shuf_min],node)){
			shuffle = 1;
			
			return 1;
		}
		call = 0;
		
		shuffle = 1;
		return 0;
	}
	else if(!shuffle && Sorted(node)){
		if(Sorted(node)){
			
			sort_min = dp;
			memcpy(g_sort_route,sort_route,sizeof(sort_route));
			return 1;	
		}
	}
	
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		
		swap(node[p],node[np]);
		int flg = Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
		
		if(flg) return 1;
	}	
	
	return 0;
}

int CheckSortAble(int p, int *node){
	call = 0;
	
	for(int i = shuffle*29 ; i < 30 ; i+=2){
		sort_min = i;
		if(Sort(-1,p,0,node))	return 1;
	}
	return 0;
}
int main(){
	
	memcpy(history[caseCnt++],map,sizeof(history[0]));
	caseCnt++;
	
	
	for(int i = 0 ; i < 8 ; i++) {//null 좌표 구하기 
		Link(i);
		if(!map[i])	 null = i;	
	}
	
	shuffle = 0;
	CheckSortAble(null,map);
	if(!CheckSortAble(null,map)){
		printf("SS");
		shuffle = 1;	
		CheckSortAble(null,map);
	}
	printf("TOTAL %d\n",TOTAL);
	
	if(shuffle){
		printf("SHUFFLE\n");
		for(int i = 0 ; i < shuf_min ; i++){
			printf("%d -> %d\n",g_shuf_route[i],g_shuf_route[i+1]);	
			swap(map[g_shuf_route[i]],map[g_shuf_route[i+1]]);
		}
	}	
	printf("NOW\n");
	for(int i = 0 ; i< 8 ; i++){
		printf("%d ",map[i]);
		if(i % 2) printf("\n");
	}
	printf("\n");
	
	printf("MAIN\n");
	for(int i = 0 ; i < sort_min ; i++){
		printf("%d -> %d\n",g_sort_route[i],g_sort_route[i+1]);
		swap(map[g_sort_route[i]],map[g_sort_route[i+1]]);
	}
	
	for(int i = 0 ; i< 8 ; i++){
		printf("%d ",map[i]);
		if(i % 2) printf("\n");
	}
		
	printf("TOTAL : %d\n",TOTAL);
		
} 
