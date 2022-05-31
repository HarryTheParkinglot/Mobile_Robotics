#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <memory.h>

#define M 40  // 들어갈수 있는 최대 깊이  
int tmp;
#define swap(a,b,str) tmp = str[a]; str[a] = str[b]; str[b] = tmp;

void Sort(int lp,int p,int dp,int *node);
int min;
int flg = 0;
void CreateLink(int p, int *str);
int link[9][4] = {0};

int g_route[M];
int route[M];

int map[9] = {
	2,0,1,
	2,3,2,
	2,1,3
};
int node[9];
int mapGoal[9] = {
   3,1,2,
   0,3,2,
   2,1,2
};

int main(){

   for(int i = 0 ; i < 9 ; i++)
      CreateLink(i,link[i]);

   for(int i = 0 ; i < M ; i++){
      int bang = 0;
      min = i;
      memcpy(node,map,sizeof(int) * 9);
      
      for(int j = 0 ; j < 9 ; j++)   
         if(!node[j]) bang = j;
      
      Sort(-1,bang,0,node);
      
      if(flg) break;
   }
   
   printf("%d",min);
}

void CreateLink(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p/3 - 1 >= 0)str[0] = p-3;
   if(p%3 - 1 >= 0)str[1] = p-1;
   if(p/3 + 1 < 3) str[2] = p+3;
   if(p%3 + 1 < 3) str[3] = p+1; 
}

int Sorted(int *nodes){
   	for(int i = 0 ; i < 9 ; i++){
      if(nodes[i] != mapGoal[i])   
	  	return 0;
	}
    return 1;
}

void Sort(int lp, int p, int dp, int *node){
   if(dp >= min || flg) return;
   
   route[dp] = p;
   
   if(Sorted(node)){
	  min = dp;
      flg = 1;
      	
      for(int i = 0 ; i <= min ; i++)
         g_route[i] = route[i];
      
	  return;
   }
	   
    for(int i = 0 ; i < 4 ; i++){      
        int np = link[p][i];
		
		if(np < 0 || np == lp) continue; 
		
		swap(p,np,node);
		Sort(p,np,dp+1,node);
        swap(p,np,node);
    }
}
