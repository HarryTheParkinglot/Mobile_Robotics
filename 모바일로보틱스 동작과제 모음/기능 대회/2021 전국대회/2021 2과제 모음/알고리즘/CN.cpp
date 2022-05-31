#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>
 
int tmp;
#define swap(a,b) tmp = node[a],node[a] = node[b],node[b] = tmp;
#define M 40

int dirList[6] = {30,90,150,210,270,330};

int node[10];
int map[10] = {
   1,3,2,
   2,3,
   3,0,0,
   1,0
}; 
int hole = 0,block = 6;

int mapGoal[10] = {
   1,3,2,
   2,3,
   3,1,2,
   1,2
};
int pList[10];
int dfs_route[10],g_dfs_route[10],dfs_min = 10,check[10];
int g_route[M],route[M],min,flg = 0,null[3],nullCnt = 0;
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

int Sorted(){
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
void display(){
   printf("%-4d %-4d %-4d\n",node[0],node[1],node[2]);
   printf("   %-4d %-4d\n",node[3],node[4]);
   printf("%-4d %-4d %-4d\n",node[5],node[6],node[7]);
   printf("   %-4d %-4d\n",node[8],node[9]);
   system("pause");
   system("cls");
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
int GetVisitable(int p, int *list){
	int cnt = 0;
	for(int i = 0 ; i < 10 ; i++)
		if(CheckDFS(p,i) && i != hole && i != block)  list[cnt++] = i;
	return cnt;
}



void Sort(int lp, int p, int dp ,int *node){
   if(dp >= min || flg) return;
   
   if(InHistory(node,dp) && lp != -1) return;
   
   //display();
      
   	if(Sorted()){
      	min = dp;
      	flg= 1;
      	memcpy(g_route,route,sizeof(int) * (min + 1));
      	return;
	}
   
   	char cnt = GetVisitable(p,pList);		
   	
   	for(int i = 0 ; i < cnt && !flg; i++){
   		GetVisitable(p,pList);		
      	int np = pList[i];
      	if(node[np] <= 0 || np == lp) continue;
      
      	char cnt2 = GetVisitable(np,pList);
      	
      	for(int j = 0 ; j < cnt2 && !flg ; j++){
      		GetVisitable(np,pList);
			int nnp = pList[j];
			CheckDFS(np,nnp);
			
			if(node[nnp] > 0) continue;
			
			route[dp] = np;
			route[dp+1] = nnp;
			
			swap(np,nnp);
      		Sort(nnp,g_dfs_route[min-1],dp+2,node);
      		swap(np,nnp);
		}
   }
   
    for(int i = 0 ; i < nullCnt  && !flg; i++){
		int np = null[i];
		if(np == p || InList(np,link[p],6)) continue;
		route[dp+1] = -1;
		Sort(-1,np,dp+2,node);
   }
}

int main(){
   int val = 0;
   
   
   for(int i = 0 ; i < 10 ; i++){   
      if(!map[i] && i != hole && i != block) {
         null[nullCnt++] = i;
         map[i] = val--;
      }
   }

   if(map[hole]){
      memcpy(node,map,sizeof(node));
      
      int min_dist = 10;
      int min_pos = -1;
      int way[10];
      
	  for(int i = 0 ; i < nullCnt ; i++){
         if(!CheckDFS(null[i],hole) || dfs_min >= min_dist) continue;
         min_pos = null[i];
         min_dist = dfs_min;
         memcpy(way,g_dfs_route,sizeof(way));
      }
      
      //GetIn(way[0]);
      printf("min%d pos : %d\n",min_dist,min_pos);
      
      for(int i = 0 ; i < min_dist ; i++){
         display();
         swap(way[i],way[i+1]);
         //if(node[g_route[i+1] <= 0]) JustMove(g_route[i],g_route[i+1]); //∑Œ∫ø ¿ßƒ°∏∏ ∫Ø∞Ê(∫Ûƒ≠¿∏∑Œ ¿Ãµø) 
         //else SwapMove(g_route[i],g_route[i+1]); //Ω«¡¶∑Œ ∆‹ ∫Ø∞Ê
      }
      display();
      memcpy(map,node,sizeof(node));
      
      nullCnt = 0;
      for(int i = 0 ; i < 10 ; i++)
         if(map[i] <= 0 && i != block && i != hole)
            null[nullCnt++] = i;   
   }
   
   printf("Real Sort\n\n");
   
   for(int i = 0 ; i < M ; i+=2){
   		for(int j = 0 ; j < nullCnt; j++){
			memcpy(node,map,sizeof(map));
			min = i;
			flg = 0;
			
			Sort(-1,null[j],0,node);
			if(flg) goto exit;
		}
   }
   
   exit:
   printf("%d %d\n",min,flg);
   
   
   //mode == 0¿Ãπ«∑Œ 
   //if(!mode) GetIn(null);
   
   for(int j = 0 ; j < min ; j+=2){
      display();
      //if(node[g_route[j+1] <= 0]) JustMove(g_route[j],g_route[j+1]); //∑Œ∫ø ¿ßƒ°∏∏ ∫Ø∞Ê(∫Ûƒ≠¿∏∑Œ ¿Ãµø) 
      //else SwapMove(g_route[j],g_route[j+1]); //Ω«¡¶∑Œ ∆‹ ∫Ø∞Ê 
     	
		 if(g_route[j] >= 0){
	      	swap(g_route[j],g_route[j+1]);   //∫Ûƒ≠¿ª -1 / 0¿∏∑Œ «•±‚«œπ«∑Œ ∫Ûƒ≠µµ πËø≠ ªÛø°º≠ ¿Ãµø¿Ã « ø‰«‘ / swap¿∏∑Œ πŸ≤„¡‹ 
      	}
    	else printf("Convert");
   }
   display();
   
}

