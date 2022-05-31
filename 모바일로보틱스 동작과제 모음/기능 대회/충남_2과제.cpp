#include <stdio.h>
#include <windows.h>
int A[2][4] = {
   {3,1,2,0},
   {2,0,3,1}
};
int B[6] = {1,3,2,3,2,1};
int BRY[4] = {0,1,2,1};
int xp[] = {-1,0,1,0};
int yp[] = {0,-1,0,1};
int m_route[20] = {0};
int chk[2][4] = {0};      
int route[20] = {0};
int min = 20;
char C[3][6] = {
   {3,0,0,0,0,1},
   {0,3,2,2,1,0},
   {0,0,2,0,2,2}
};
void C4(int col);
void C3(int col);
void CC(int num);
void swap(int y1, int x1, int y2, int x2);
void f(int x, int y, int dp);
void print(){
   for(int i = 0 ; i < 3 ; i++){
      for(int j = 0 ; j < 6 ; j++){
         if(i==1){
            printf("%d", C[0][j]);
         }else{
            if(C[1][j]&&C[2][j]==i)   printf("%d", C[1][j]);
            else               printf(" ");
         }
      }
      printf("\n");
   }
   printf("\n");
   system("pause");
   system("cls");
}
main(){
   if(C[0][5]!=B[5])   C4(B[5]);
   if(C[0][0]!=B[0])   C3(B[0]);
   for(int i = 1; i < 4; i++)
      if(C[1][i] != B[i])   CC(i);
   f(1,1,0);
   for(int i = 0 ; i <= min ; i++)
      printf("%d %d\n", m_route[i]/10, m_route[i]%10);
}
void f(int x, int y, int dp){
   if(dp >= min)   return;
   route[dp] = y*10+x;
   if(!BRY[1]&&!BRY[2]&&!BRY[3]){
      for(int i = 0 ; i <= (min = dp) ; i++)
         m_route[i] = route[i];
      return;
   }
   for(int i = 0 ; i < 4 ; i++){
      int n = x+xp[i];
      int m = y+yp[i];
      int o = A[m][n];
      if(n<0||n>3||m<0||m>1)   continue;
      chk[m][n] = 1;
      BRY[o]-=1;         
      A[m][n] = 0;
      f(n, m, dp+1);
      A[m][n] = o;
      BRY[o]+=1;         
      chk[m][n] = 0;
   }
}
void swap(int y1, int x1, int y2, int x2){
   C[y1][x1] = C[y2][x2];
   C[y2][x2] = 0;
}
void C4(int col){
   int a = 1;
   for(int i = 4 ; i >=0 ; i--){
      if(!i)   a=0;
      if(C[a][i]!=col)   continue;
      switch(i){
         case 4:case 3:case 2:
            swap(0,i-1,0,5);
            swap(0,5,1,i);
            swap(1,i,0,i-1);
            break;
         case 1:case 0:
            swap(0,4,1,3);
            swap(1,3,i,i);
            swap(i,i,0,4);
            swap(0,2,0,5);
            swap(0,5,1,3);
            swap(1,3,0,2);
      }
      break;
   }
}
void C3(int col){
   for(int i = 1 ; i < 5 ; i++){
      if(C[1][i]!=col)   continue;
      switch(i){
         case 1:case 2:case 3:
            swap(0,i+1,0,0);
            swap(0,0,1,i);
            swap(1,i,0,i+1);
            break;
         case 4:
            swap(0,1,1,2);
            swap(1,2,1,4);
            swap(1,4,0,1);
            swap(0,3,1,2);
            swap(1,2,0,0);
            swap(0,0,0,3);
      }
      break;
   }
}

void CC(int num){
   int l = 0;
   rt:
   for(int i = num ; i <= 4 ; i++){
      if(C[1][i]!=B[num])   continue;
      if(num==1&&i==4){
         swap(0,1,1,4);
         swap(1,4,1,2);
         swap(1,2,0,1);
         swap(1,2,0,1);
         goto rt;
      }
      if(num==1)   l = i+1;
      else      l = num-1;
      swap(0,l,1,num);
      swap(1,num,1,i);
      swap(1,i,0,l);
      break;
   }
   return;
}