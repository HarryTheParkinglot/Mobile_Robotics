#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>
 
int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
#define M 30

int dirList[6] = {30,90,150,210,270,330};

int node[10];
int map[10] = {
   1,2,0,
   2,3,
   3,0,2,
   0,1
}; 
int hole = 1,block = 2;

int mapGoal[10] = {
   1,3,2,
   2,3,
   3,1,2,
   1,2
};

int dfs_route[10],g_dfs_route[10],dfs_min = 10,check[10];
int g_route[M],route[M],min,flg = 0,null[3],cnt = 0;
int link[10][6] = {
   {-1,-1,-1,3,1,-1},
   {-1,0,3,4,2,-1},
   {-1,1,4,-1,-1,-1},
   
   {0,-1,5,6,4,1},
   {1,3,6,7,-1,2},
   
   {-1,-1,-1,8,6,3},
   {3,5,8,9,7,4},
   {4,6,9,-1,-1,-1},
   
   {5,-1,-1,-1,9,6},
   {6,8,-1,-1,-1,7}
};

int history[M][10];

int Sorted(int *node){
   for(int i = 0 ; i < 10 ; i++){  
      if(node[i] <= 0) continue;
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}

bool InHistory(int *list,int dp){
   for(int i = 0 ; i < dp ; i++){
      for(int j = 0 ; j < 10 ; j++){
         if(node[j] != history[i][j]) break;
         if(j == 9) return 1;
      }
   }
   return 0;
}

void DFS(int p, int wp, int dp){
   if(dp >= dfs_min) return;
   
   dfs_route[dp] = p;
   
   if(p == wp){
      dfs_min = dp;
      memcpy(g_dfs_route,dfs_route,sizeof(dfs_route));
      return;
   }
   
   check[p] = 1;
   
   for(int i = 0 ; i < 6 ; i++){
      int np = link[p][i];
      if(np < 0 || check[np] || np == block) continue;
      DFS(np,wp,dp+1);
   }
   
   check[p] = 0;
}

void display(){
	printf("\n%d %d %d\n",map[0],map[1],map[2]);
	printf(" %d %d\n",map[3],map[4]);
	printf("%d %d %d\n",map[5],map[6],map[7]);
	printf(" %d %d\n",map[8],map[9]);
	system("pause");
	system("cls");
}

bool InList(int p, int *list, int len){
	for(int i = 0 ; i < len ; i++)
		if(p == list[i]) return 1;
	return 0;
}


bool CheckDFS(int p, int wp){
   dfs_min = 10;
   memset(check,0,sizeof(check));
   DFS(p,wp,0);
   return dfs_min != 10;
}

void Sort(int lp, int p, int dp ,int *node){
   	if(dp >= min || flg) return;
   
   	if(InHistory(node,dp) && lp != -1) return;
   
   	route[dp] = p;
   
   	//display();
      
   	if(Sorted(node)){
      	min = dp;
      	flg= 1;
	    memcpy(g_route,route,sizeof(int) * (min + 1));
	    return;
   	}
   
   	for(int i = 0 ; i < 6 && !flg; i++){
      	int np = link[p][i];
      	if(np < 0 || np == lp || np == hole || np == block) continue;
      
      	swap(node[p],node[np]);
      	Sort(p,np,dp+1,node);
      	swap(node[p],node[np]);
   	}
   	
   	for(int i = 0 ; i < cnt  && !flg; i++){
		int np = null[i];
		if(np == p || np == lp || InList(np,link[p],6)) continue;
		route[dp+1] = -1;
		Sort(p,np,dp+2,node);  //다른 노드로 이동하기  
   	}
}

int main(){
   	int val = 0;
   
   
   	for(int i = 0 ; i < 10 ; i++){   
    	if(!map[i] && i != hole && i != block) {
        	null[cnt++] = i;
         	map[i] = val--;
      	}
   	}
   	if(map[hole] > 0 && !Sorted(map)){
    	//memcpy(node,map,sizeof(node));
    	memset(node,0,sizeof(node));
      	
		int min_dist = 10;
      	int min_pos = -1;
      	int way[10];
      
		for(int i = 0 ; i < cnt ; i++){
         	if(!CheckDFS(null[i],hole) || dfs_min >= min_dist) continue;
         	min_pos = null[i];
         	min_dist = dfs_min;
         	memcpy(way,g_dfs_route,sizeof(way));
      	}
      	
      	//GetIn(way[0]);
      	
      	printf("min%d pos : %d\n",min_dist,min_pos);
      	for(int i = 0 ; i < min_dist ; i++){
         	display();
         	swap(map[way[i]],map[way[i+1]]);
         	//if(node[g_route[i+1] <= 0]) JustMove(g_route[i],g_route[i+1]); //로봇 위치만 변경(빈칸으로 이동) 
         	//else SwapMove(g_route[i],g_route[i+1]); //실제로 퍽 변경 	
      	}
      	display();
      	//memcpy(map,node,sizeof(node));
      
      	cnt = 0;
      	for(int i = 0 ; i < 10 ; i++)
	        if(map[i] <= 0 && i != block && i != hole)
	            null[cnt++] = i;   
   }
   
   printf("Real Sort\n\n");
   
   for(int i = 0 ; i < M ; i++){
   		for(int j = 0 ; j < cnt; j++){
			memcpy(node,map,sizeof(map));
			min = i;
			flg = 0;
			
			Sort(-1,null[j],0,node);
			if(flg) goto exit;
		}
   }
   
   exit:
   printf("%d %d\n",min,flg);
   
   
   //mode == 0이므로 
   //if(!mode) GetIn(null);
   
   	for(int j = 0 ; j < min ; j++){
      	display();
      	//if(node[g_route[j+1] <= 0]) JustMove(g_route[j],g_route[j+1]); //로봇 위치만 변경(빈칸으로 이동) 
      	//else SwapMove(g_route[j],g_route[j+1]); //실제로 퍽 변경 
     	if(g_route[j+1] >= 0){
     		printf("%d -> %d\n",g_route[j],g_route[j+1]);
	      	swap(map[g_route[j]],map[g_route[j+1]]);   //빈칸을 -1 / 0으로 표기하므로 빈칸도 배열 상에서 이동이 필요함 / swap으로 바꿔줌 
      	}
    	else {
			printf("Convert to %d",g_route[j+2]);
			//GetIn(g_route[j+2]);
			j++;
		}
   }
   printf("FINISH \n\n");
   display();
}

