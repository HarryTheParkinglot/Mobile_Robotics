//DFS 모든 경로 출력

#include <stdio.h>

#define M 5

int link[M+1][M-1] = {
   {0},
   {2,3,4,5},
   {1,3,4,5},
   {1,2,4,5},
   {1,2,3,5},
   {1,2,3,4}
};

int route[M] = {0};
int chk[M + 1] = {0};

void DFS(int p, int g, int dp){
   route[dp] = p;

   if(p == g){
      for(int i = 0 ; i <= dp ; i++)
         printf("%d ",route[i]);
      printf("\n");
      
      return;
   }
   
   chk[p] = 1;
   
   for(int i = 0 ; i < M ; i++){
      int np = link[p][i];
      if(np <= 0 || chk[np]) continue;
      DFS(np,g,dp+1);
   }
   chk[p] = 0;
   return;
}

int main(){
   DFS(1,4,0);
}
