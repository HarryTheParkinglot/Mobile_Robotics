#include <stdio.h>
#include <memory.h>
#include <windows.h>
int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
#define M 30

int node[8];
int map[8] = {
   2,3,
   0,3,
   1,0,
   1,2
};

int mapGoal[8] = {
   3,3,
   1,0,
   2,1,
   2,0
};

int g_sort_route[M][2],sort_route[M][2],sort_min,flg = 0;
int route[8],g_route[8],min = 8,check[8] = {0},fix[8] = {0};

int call= 0;
int link[8][4]; 
int hist[M][8];

void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 2  > 0) str[0] = p-2;
   if(p % 2  > 0) str[1] = p-1;
   if(p / 2  < 3) str[2] = p+2;
   if(p % 2 == 0) str[3] = p+1;
}

int Sorted(int *node){
   for(int i = 0 ; i < 8 ; i++){
      if(!mapGoal[i]) continue;
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}

int GetScore(int *node){
	int score = 0;
	
	for(int i = 0  ; i < 8 ; i++)
		if((mapGoal[i] == 0 && node[i] <= 0) || mapGoal[i] == node[i]) score++;
	return score;
}

int InHist(int *node ,int dp){
   for(int i = 0 ; i < dp ; i++){
      for(int j = 0 ; j < 8 ; j++){
         if(node[j] > 0 && node[j] != hist[i][j]) break;
         if(j == 7) return 1;
      }
   }
   return 0;
}


void DFS(int p, int wp, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(check[np] || np < 0 || (np != wp && node[np] > 0)) continue;
		DFS(np,wp,dp+1,node);
	}
	check[p] = 0;
}

int CheckDFS(int p, int wp, int *node){
	memset(check,0,sizeof(check));
	min = 3;
	
	DFS(p,wp,0,node);
	
	return min != 3;
}


void Sort(int lp, int p,int dp, int sc, int *node){
	call ++;
	if(flg || dp >= sort_min) return;
	
	if(InHist(node,dp)) return;
	
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	if(Sorted(node)){
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		return;
	}
	
	for(int i = 0,count = 0 ; i < 8 && !flg ; i++){
		if(lp == i || node[i] <= 0|| !CheckDFS(p,i,node)) continue;
		if(min > 1) continue;
		for(int j = 0 ; j < 8 && !flg ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			swap(node[i],node[j]);
	
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			
			Sort(j,g_route[min-1],dp+1,GetScore(node),node);
			swap(node[i],node[j]);
		}
	}
	return; 
}

int GetDir(int p, int np){
	for(int i = 0 ;i < 4; i++)
		if(link[p][i] == np) return i;
}

int main(){
   	int null[2]= {0},val = 0,cnt = 0;
   
   	for(int i = 0,cnt = 0 ; i < 8 ; i++){
      	Link(i,link[i]);
      	
		if(!map[i]) {
	        null[cnt] = i;
    	    map[i] = -(cnt++);
	    }
	}
   	
   	printf("cnt : %d",cnt);
   
   	for(int i = 0 ; i < M ; i+=3){
	   	flg = 0;
   		sort_min = i;	
   		
		for(int j = 0 ; j < 2; j++){
			Sort(-1,null[j],0,10,map);
			if(flg) goto exit;
		}
   }
   
   exit:
   
   printf("%d flg %d\n",sort_min,flg);
   printf("call %d",call);
}

