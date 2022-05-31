#include <stdio.h>
#include <memory.h>
#include <windows.h>
 
int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
#define M 11

int node[32];
int map[32] = {
   1,0,2,1,
   3,2,0,3,
   1,0,3,3,
   0,0,1,2
};

int mapGoal[32] = {0};
int seet[3] = {2,1,3};


int dfs_route[20],g_dfs_route[20],dfs_min = 20,check[32];
int g_route[M][2],route[M][2],len,flg = 0;

int link[32][4];

void Link(int p){
   memset(link[p],-1,sizeof(int) * 4);
   
   if(p / 4 > 0) link[p][0] = p - 4;
   if(p % 4 > 0) link[p][1] = p - 1;
   if(p / 4 < 7) link[p][2] = p + 4;
   if(p % 4 < 3) link[p][3] = p + 1;
}


int Sorted(){
   for(int i = 0 ; i < 32 ; i++){  
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}


void display(int *node){
   for(int i = 0 ; i < 32 ; i++){
      printf("%-4d",node[i]);
      if(i % 4 == 3) printf("\n");
   }
   
   printf("\n\n");
   
   for(int i = 0 ; i < 32 ; i++){
      printf("%-4d",mapGoal[i]);
      if(i % 4 == 3) printf("\n");
   }
   
   system("pause");
   system("cls");
}


bool CanPass(int color, int p,int np){
   if(color <= 0) return 1; //퍽을 먹지 않은 상태면 상관 없음 
   if(abs(np - p) != 4) return 1;
   
   if((np / 4 <= 3 && p / 4 <= 3 )|| (np / 4 >= 4 && p / 4 >= 4)) return 1;
   for(int i = 0; i < 3 ; i++)
      if(seet[i] == color && (p%4 == i || p%4 == i + 1)) return 1;
   
   return 0;
}


void DFS(int color, int p, int wp, int dp){ //색깔 보유시  
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
      if(np < 0 || check[np] || !CanPass(color,p,np) || (np != wp && node[np])) continue;
      DFS(color,np,wp,dp+1);
   }
   
   check[p] = 0;
}

bool CheckDFS(int color, int p, int wp){
   dfs_min = 15;
   memset(check,0,sizeof(check));
   DFS(color,p,wp,0);
   return dfs_min != 15;
}

void MirrorSort(int p, int dp, int *node){
   if(flg) return;
   
   //printf("p : %d dp : %d\n\n",p,dp);
   //display(node);
   
   
   if(Sorted()) {
      len = dp;
      memcpy(g_route,route,sizeof(g_route));
      flg = 1; 
      return;
   }
   
   //first Check
   
   for(int i = 31 ; i >= 16 && !flg; i--){
       if(!mapGoal[i] || node[i]) continue;
      if(!CheckDFS(0,p,i)) return; //정렬이 되지 않았는데 막혔으므로 버려야함  
   }
   
   for(int i = 31 ; i >= 16 && !flg; i--){
      if(!mapGoal[i] || node[i]) continue;
      if(!CheckDFS(0,p,i)) return; //정렬이 되지 않았는데 막혔으므로 버려야함  
      
      for(int j = 0 ; j < 16 && !flg; j++){
         if(!node[j] || node[j] != mapGoal[i] || !CheckDFS(mapGoal[i],i,j)) continue;
         swap(node[i],node[j]);
         
         route[dp][0] = j; 
         route[dp][1] = i;
         
         MirrorSort(g_dfs_route[dfs_min-1],dp+1,node);
         swap(node[i],node[j]);
      }
   }
}

int main(){
   for(int i = 0 ; i < 32 ; i++)   Link(i);
      
   //Mirror
   
   for(int i = 0 ; i < 16 ; i++){
      int p = 28 - (i / 4)*4 + (i % 4);
      mapGoal[28 - (i/4)*4 + (i % 4)] = map[i]; 
   }
   

   memcpy(node,map,sizeof(map));

   MirrorSort(21,0,node);
   
   printf("flg : %d\n",flg);
   
   memcpy(node,map,sizeof(map));
   
   display(node);
   
   int nowPos = 21;
   
   	for(int i = 0 ; i < len ; i++){
      //printf("Color : %d\n\n%d -> %d\n\n",node[g_route[i][0]],g_route[i][0],g_route[i][1]);
      
      	printf("\nGETPUCK %d -> %d\n",nowPos,g_route[i][0]);
	  	CheckDFS(0,nowPos,g_route[i][0]);
	  
		for(int j = 0 ; j <= dfs_min ; j++){
        	printf("%d -> ",g_dfs_route[j]);
        	//MovePtoP(g_dfs_route[j],g_dfs_route[j+1],g_dfs_route[j+2]);
    	}
	  
	  	printf("\nPUTPUCK %d -> %d\n",g_route[i][0],g_route[i][1]);
	  	CheckDFS(node[g_route[i][0]],g_route[i][0],g_route[i][1]);
      
	  	for(int j = 0 ; j <= dfs_min ; j++){
        	printf("%d -> ",g_dfs_route[j]);
        	//MovePtoP(g_dfs_route[j],g_dfs_route[j+1],g_dfs_route[j+2]);
    	}
      
	  	printf("\n\n\n");
      
      	node[g_route[i][1]] = node[g_route[i][0]];
      	node[g_route[i][0]] = 0;
      	nowPos = g_dfs_route[dfs_min-1];
      	display(node);
   	}
   
   printf("GO END POINT %d\n",CheckDFS(0,nowPos,13));
}

