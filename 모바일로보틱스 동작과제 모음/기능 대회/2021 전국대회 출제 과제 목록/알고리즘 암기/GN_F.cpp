#include <stdio.h>
#include <windows.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;
#define M 50
int map[13] = {
	1,0,2,
	 3,3,
	1,1,0,
	 3,2,
	0,0,2
};

int node[13];
int bMap[6];

int link[13][8] = {
   {-1,-1,-1,-1,-1,3,1,-1},
   {-1,-1,0,3,-1,4,2,-1},
   {-1,-1,1,4,-1,-1,-1,-1},
   
   {-1,0,-1,5,-1,-1,-1,1},
   {-1,1,-1,-1,-1,7,-1,2},
   
   {-1,-1,-1,-1,-1,8,-1,3},
   {-1,-1,-1,-1,-1,-1,-1,-1},
   {-1,4,-1,9,-1,-1,-1,-1},
   
   {-1,5,-1,10,-1,11,-1,-1},
   {-1,-1,-1,11,-1,12,-1,7},
   
   {-1,-1,-1,-1,-1,-1,11,8},
   {-1,8,10,-1,-1,-1,12,9},
   {-1,9,11,-1,-1,-1,-1,-1}
};

int route[M],g_route[M]; //[0] : 회전 타입 / [1] : 회전 방향  [2] : 회전 횟수 
int sort_min,sort_route[10][2],g_sort_route[10][2],check[13];

int flg = 0,fix[13] = {0};
int hist[M][13];
int min = 0,stage = 0,nowPos;
int seq[8] = {1,4,7,9,11,8,5,3};
int call = 0;

int InHist(int *node, int dp){
   for(int i = 0  ;i < dp ; i++){
      for(int j = 0 ;  j < 13 ; j++){
         if((node[j] > 0 || node[j] == -3) && node[j] != hist[i][j]) break;
         if(j == 12) return 1;
      }
   }
   return 0;
}

void dis(int *arr){
	printf("\n%3d %3d %3d\n",arr[0],arr[1],arr[2]);
	printf("  %3d %3d\n",arr[3],arr[4]);
	printf("%3d %3d %3d\n",arr[5],arr[6],arr[7]);
	printf("  %3d %3d\n",arr[8],arr[9]);
	printf("%3d %3d %3d\n",arr[10],arr[11],arr[12]);
	system("pause");
	system("cls");
}

int EdgeCheck(int *node){
   	if(node[0] > 0 && node[0] == node[2] && node[0] != node[6]){ 
      	fix[0] = fix[2] = 1;
      	return 1;
   	}
   	if(node[10] > 0 && node[10] == node[12] && node[10] != node[6]){ 
      	fix[10] = fix[12] = 1;
      	return 1;
   	}
   	return 0;
}

int Check(int *node){ //1차 정렬 확인 함수 
   	if(node[0] <= 0 || node[2] <= 0 || node[10] <= 0 || node[12] <= 0) return 0;
	if(node[0]+node[2]+node[10]+node[12] != (6-map[6])*2) return 0;
   	if(node[0] != node[2]) return 0;
   
   	int pCnt = 0,pList[8];
   	for(int i = 0 ; i < 8 ; i++){
      	int col = node[seq[i]];
      	if(col <= 0) continue;
      	
      	if(pCnt && col == pList[pCnt-1]) return 0;
      	if(col > 0) pList[pCnt++] = col;
   	}
   
   if(pList[pCnt-1] == pList[0]) return 0;
   
   fix[0] = fix[2] = fix[10] = fix[12] = 1;
   return 1;
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
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(check[np] || np < 0 || (np != wp && node[np] > 0)) continue;
		DFS(np,wp,dp+1,node);
	}
	check[p] = 0;
}

int CheckDFS(int p, int wp, int *node){
	memset(check,0,sizeof(check));
	min = 10;
	
	DFS(p,wp,0,node);
	
	return min != 10;
}

int Sorted(int *node){
	if(node[0] != node[1]) return 0;
	if(node[10] != node[11]) return 0;
	if(node[5] != node[6] || node[6] != node[7]) return 0;
	return 1;
}

void Sort(int lp, int p,int dp, int *node){
	call ++;
	if(flg || dp >= sort_min) return;
	if(InHist(node,dp)) return;
	
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	if(stage == 0 && EdgeCheck(node) || (stage == 1 && Check(node)) || (stage == 2 && Sorted(node))){
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		return;
	}
	
	for(int i = 0 ; i < 13 ; i++){
		if(lp == i || node[i] <= 0|| fix[i] || !CheckDFS(p,i,node)) continue;
		
		for(int j = 0 ; j < 13 ; j++){
			if(node[j] > 0 || !CheckDFS(i,j,node)) continue;
			
			swap(node[i],node[j]);
			sort_route[dp][0] = i;
			sort_route[dp][1] = j;
			
			Sort(j,g_route[min-1],dp+1,node);
			swap(node[i],node[j]);
		}	
	}
}

int main(){
	stage = 0;
	for(int i = 0,val = 0; i < 13 ; i++){
		if(!map[i]) map[i] = val--;
	}
	
	for(int i = 10 ; i < 13 ; i++)
		if(map[i] <= 0) nowPos = i;
	
	printf("%d",nowPos);
	
	dis(map);
	for(int k = 0 ; k < 3 ; k++){
		stage = k;
		memset(hist,0,sizeof(hist));
		
		int last = (stage)? g_sort_route[sort_min-1][1] : -1;
		
		for(int i = 0 ; i < M ; i+=5){
			sort_min = i;
			flg = 0;
			Sort(last,nowPos,0,map);
			if(flg) break;
		}
		printf("flg : %d\n",flg);
		printf("min : %d\n",sort_min);
		printf("call %d\n",call);
		
		for(int i = 0 ; i < sort_min ; i++){	
			printf("%d -> %d\n",g_sort_route[i][0],g_sort_route[i][1]);	
			swap(map[g_sort_route[i][0]],map[g_sort_route[i][1]]);
			dis(map);
			CheckDFS(g_sort_route[i][0],g_sort_route[i][1],map);
			nowPos = g_route[min-1];
		}
	}
}

