#include <stdio.h>
#include <memory.h>
#include <windows.h>
#define M 30

int tmp;
#define swap(a,b)  tmp = a, a = b, b = tmp;
int map[8] = {
	2,2,
	3,3,
	1,1,
	0,0
};
int shufmap[8];
int mapGoal[8] = {
	2,2,
	3,3,
	1,1,
	0,0
};

int link[8][4];
int history[30][8] = {0};
int call= 0 ;
int null = 0,shuffle = 0,caseCnt = 0;
int sort_route[M],g_sort_route[M],sort_min;
int g_shuf_route[M],shuf_min;
int nowPos,flg = 0;
int stage = 0;
int TOTAL = 0;

int Shuffled(int *ori, int *now){ //셔플 검사
	int spot[4] = {0,1,6,7};
	
	int match = 0;
	int cnt;
	for(int i = 0 ; i < caseCnt ; i++){
		cnt = 0;
		
		for(int j = 0 ; j < 4 ; j++)
			if(now[j] && now[j] == history[i][j]) cnt++;
		
		if(cnt >= 1) return 0;
	}
	
	for(int i = 0 ; i < 8 ; i++){
		if(mapGoal[i] == now[i]) match++;
	}
		
	if(match >= 5){
		memcpy(history[caseCnt],now,sizeof(history[caseCnt]));
		caseCnt++;
		
		return 1;
	}
	return 0;
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

void Sort(int lp, int p, int dp, int *node){ //초기 데이터에서 셔플 // 	
	call++;
	TOTAL ++;
	if(dp >= sort_min || flg) return;
	
	sort_route[dp] = p;
	
	if(!shuffle && call > 100000) {
		flg = 2;
		return;
	}
	
	if((!shuffle && Sorted(node)) || (shuffle && Shuffled(map,node))){ //셔플 되었거나  
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		return;
	}
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}	
}

int CheckSortAble(int p, int *node){
	flg = 0;
	call = 0;
	
	for(int i = 0 ; i < 30 ; i+= 2){
		sort_min = i;
		Sort(-1,p,0,node);
		if(flg) break;
	}
	
	return flg == 1;
}
int main(){
	
	memcpy(history[caseCnt++],map,sizeof(history[0]));
	caseCnt++;
	
	for(int i = 0 ; i < 8 ; i++) {//null 좌표 구하기 
		Link(i);
		if(!map[i])	 null = i;	
	}
	
	
	while(1){
		if(!shuffle && CheckSortAble(null,map)){
			goto sort;
		}
		else{
			printf("TRY : %d\n",caseCnt);
			
			
			shuffle = 1;
			memcpy(shufmap,map,sizeof(map));
			CheckSortAble(null,shufmap);
			
			shuf_min = sort_min;
			memcpy(g_shuf_route,g_sort_route,sizeof(g_sort_route));
			
			for(int i = 0; i < shuf_min ; i++){
				swap(shufmap[g_shuf_route[i]],shufmap[g_shuf_route[i+1]]);
			}
			
			for(int i = 0 ; i< 8 ; i++)
				printf("%d\n",shufmap[i]);
			system("pause");
			system("cls");
			int pos = g_shuf_route[sort_min];
			
		
			shuffle = 0;
			//printf("case Cnt %d\n",caseCnt);
			if(CheckSortAble(pos,shufmap))	{
				shuffle = 1;
				goto sort;
			}
			shuffle =  1;
		}
	}
	
	sort:
	printf("FLG %d SUF %d\n",flg,shuffle);
	
	
	if(shuffle){
		
		for(int i = 0 ; i < shuf_min ; i++){
			printf("%d -> %d\n",g_shuf_route[i],g_shuf_route[i+1]);
			
			swap(map[g_shuf_route[i]],map[g_shuf_route[i+1]]);
		}
	}	
	for(int i = 0 ; i< 8 ; i++)
		printf("%d\n",map[i]);
	printf("\n");
	for(int i = 0 ; i < sort_min ; i++){
		printf("%d -> %d\n",g_sort_route[i],g_sort_route[i+1]);
		swap(map[g_sort_route[i]],map[g_sort_route[i+1]]);
	}
	
	for(int i = 0 ; i< 8 ; i++)
		printf("%d\n",map[i]);
		
	printf("TOTAL : %d\n",TOTAL);
		
} 
