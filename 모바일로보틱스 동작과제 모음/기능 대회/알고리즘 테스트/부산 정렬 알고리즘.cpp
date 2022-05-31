#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <memory.h>

#define M 20  // 들어갈수 있는 최대 깊이  
#define move(a,b) tmp = node[a]; node[a] = node[b]; node[b] = tmp;
int tmp = 0,flg = 0;
int sortMin,min,sortRoute[M],g_sortRoute[M],route[M],g_route[M],visit[9]= {0};
int visitableNodes[9] = {0};

int map[9] = {
   2,0,1,
   3,2,0,
   1,1,0
};

int mapGoal[9] = {
   2,1,2,
   3,0,0,
   1,4,1
};
int node[9],link[9][4];

void CreateLink(int p,int *str);
void Sort(int lp, int p, int dp,int moveCnt ,int *node);
void DFS(int p, int g, int dp, int *node);
int GetVisitable(int p, int *str, int *node);

int main(){
   clock_t start = clock(); // 시작 시간 저장
   
   for(int i = 0 ; i < 9 ; i++)
      CreateLink(i,link[i]);
      
   	for(int i = 0 ; i < M*2 ; i+=2){
      sortMin = i;
      
    	memcpy(node,map,sizeof(int)*9);      
    	
	    for(int j = 3 ; j < 9; j++){
		  	if(node[j]) continue;
		  	sortRoute[0] = j; 
			Sort(-1,j,1,0,node);
		  	
			if(flg) goto fin;
		}	
   }   
   fin:
   if(flg){
      printf("sortMin : %d\n\n",sortMin);
      
      for(int i = 0 ; i < sortMin ; i++)
         printf("%d ->",g_sortRoute[i]);      
      
      double cal = (double)(clock() - start)/CLOCKS_PER_SEC;
      printf("\n\nTIME : %lf\n\n",cal);
   }
}

void CreateLink(int p , int *str){
   int cnt = 0;
   memset(str,-1,sizeof(int) * 4);
   if(p%3 - 1 >= 0) {str[1] = p-1;}
   if(p%3 + 1 < 3) {str[3] = p+1;}
   if(p/3 - 1 >= 0) {str[0] = p-3;}
   if(p/3 + 1 < 3) {str[2] = p+3;}   
}

int Sorted(int *node){
	for(int i = 0 ;i < 9 ; i++){
		if(mapGoal[i] == 4) continue;
		if(mapGoal[i] != node[i]) return 0;
	}
	return 1;
}

void DFS(int p, int g, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == g){
		min = dp;
		memcpy(g_route,route,sizeof(int) * (dp + 1));
		return;
	}	
	visit[p] = 1;
	for(int i = 0; i < 4 ; i++){
		int np = link[p][i];	
		if(np < 0 || (node[np] && np != g) || visit[np]) continue;
		DFS(np,g,dp+1,node);
	}
	visit[p] = 0;
}

void display(int *node){
	for(int i= 0;i< 9; i++){
		printf("%-3d",node[i]);
		if( i % 3 == 2) printf("\n"); 
	}
	system("pause");
	system("cls");
}

void Sort(int lp, int p, int dp, int moveCnt, int *node){
   	if(dp >= sortMin || flg) return;

   	if(Sorted(node)){
   		flg = 1,sortMin = dp;
   		memcpy(g_sortRoute,sortRoute,sizeof(int) * (sortMin+1));
		return;
   	}
   	int visitables[9];
	int nodeCnt = GetVisitable(p,visitables,node); 
	
	if(node[p]){
		for(int i = 0; i < nodeCnt; i++){
			int np = visitables[i];
			if(node[np] || np == p || mapGoal[np] == 4)continue; //이동 불가 / 퍽이 있는 곳 / 퍽을 둘 수 없는 곳 
			
			min = 8;
			DFS(p,np,0,node);
			move(np,p);
			
			sortRoute[dp] = np;
			Sort(np,g_route[min-1],dp+1,moveCnt+1,node);
			move(np,p);
		}
	}
	
	else if(!node[p]){
		for(int i = 0; i < nodeCnt; i++){
			int np = visitables[i];
			if(!node[np] || np == lp || node[np] == 3) continue; //이동 불가 / 퍽이 없는 곳 / 장애
			
			sortRoute[dp] = np;
			Sort(lp,np,dp+1,moveCnt,node);
		}
	}
	 
	if(moveCnt && !node[p]){ //퍽을 최소 한번이라도 옮겼다면 나가볼 수 있음  
		for(int i = 0 ; i < 9 ; i++){
			visitableNodes[i] = i;
		 	for(int j = 0 ; j < nodeCnt ; j++)
				if(i == visitables[j]) visitableNodes[i] = -1;
		}	
		
		for(int i = 0 ; i < 9 ; i++){
			if(visitableNodes[i] < 0 || node[i] || i == lp)	 continue; //무조건 퍽이없는 곳으로 들어감  
			sortRoute[dp] = i;
			Sort(lp,i,dp+1,0,node);
		}	
	}
	return;
}

int GetVisitable(int p, int *str, int *node){
	int cnt = 0;
	
  	for(int i = 0 ; i < 9 ; i++){
	  	if(node[i] == 3 || i == p) continue;
	  	min = 6;
	  	DFS(p,i,0,node);
	  	if(min != 6) str[cnt++] = i;
	}
	return cnt;
}
