#include <stdio.h>
#include <math.h>
#include <windows.h>
int temp = 0;

#define swap(a,b) temp = a; a = b; b = temp;

//선분의 기울기의 역수 / --> dx / dy 

int map[25] = {
   0,0,0,0,0,
   0,0,0,0,0,
   0,0,2,0,0,
   0,0,1,0,0,
   0,0,0,0,0
};
int min = 0;
int g_displayMap[25] = {0};
int array[25] = {0};
int colorPos[3][3] = {0};
int g_colorPos[3][3] = {0};
int territory[25] = {0};
int f_flg = 0;//b / r이 모두 1이면 f_flg = 1 

int Finish(int *node, int *nodeStack){
   for(int i = 0 ; i < 25 ; i++){
      if(!node[i]) continue;
      if(nodeStack[i] != node[i]) return 0;
   }
   return 1;
}

int setTerritory(int *points, int *output) {
   int cpPoints[3];
   
   memcpy(cpPoints,points,sizeof(int) * 3);
   memset(output,0,25 * sizeof(int));

   int tmp = 0;
 
   
   for(int j =  0 ; j < 2 ; j++){
      for(int i = 0  ; i < 2 ; i++){
         if(cpPoints[i] % 5 > cpPoints[i+1] % 5) {
            swap(cpPoints[i],cpPoints[i+1]);
         }
      }
   }
   
   //점들을 x좌표 순으로 정렬  
   int p[3][2] = {0};
   float grad[3] = {0}; // 0 : 0 - 1 | 1 : 1 - 2 | 2 : 2 - 3 
   

   for(int i = 0 ; i < 3 ; i++){
      p[i][0] = cpPoints[i]/5; //포인트의 y좌표    
      p[i][1] = cpPoints[i]%5; //포인트의 x좌표 
      output[p[i][0]*5 + p[i][1]] = 2;   
   }
   
   int dx[3],dy[3];
   
   for(int i = 0 ; i < 3 ; i++){
      dx[i] = p[i][1] - p[(i+1)%3][1];
      dy[i] = p[i][0] - p[(i+1)%3][0];
      
      grad[i] = (dy[i])? (float)dx[i]/dy[i] : 99;
   }
   
     for(int i = 0 ; i < 3 ; i++){
       if(grad[i] == 99 && abs(dx[i]) > 1){ 
           int s = (dx[i] > 0)? p[(i+1)%3][1] : p[i][1];
           int e = (dx[i] > 0)? p[i][1] : p[(i+1)%3][1];
         
           for(int j = s + 1 ; j < e ; j++)   output[p[i][0] * 5 + j] = 2;
        }
      
       else{
           int s = p[i][0];
           int e = p[(i+1)%3][0];
            
           int sx = p[i][1];
          
          if(s > e){
            swap(s,e);
            sx = p[(i+1)%3][1];
         }
       
            for(int j = s + 1 ; j < e ; j++){
               float nx = sx + ((j-s) * grad[i]);
               
               if(int(nx) < nx){
                  nx = (nx > p[1][1])? (int)ceil(nx) : (int)floor(nx);
                  output[j * 5 + (int)nx]= 1;
               }   
               else   output[j * 5 + (int)nx]= 2;            
            }
       }
   }
   int width = 0;
   
   for(int i = 0 ; i < 5 ; i++){
      int cnt = 0,sx = -1;
      
      for(int j = 0 ; j < 5 ; j++){
         if(output[i*5 + j]) {
            cnt ++;
            sx = (sx == -1)? j : sx;
         }
      }
      if(cnt == 2){
         for(int j = sx + 1 ; !output[i*5 + j] ; j++){       
          output[i*5 + j] = 3;     
          width++;
      }
      }
   }
   return width;
}

void display(int color, int *colorPos, int *a, int index){
   int dmap[25] = {0};
   
   for(int i = 0 ; i < 3;  i++)
      dmap[colorPos[i]] = color;   
   
      for(int i = 0 ; i < 25 ; i++){
         printf("%-3d",dmap[i]);
         if(i % 5 == 4) printf("\n");
      }

   printf("\n\n");
   
   for(int i = 0 ; i< 25 ; i++){
      printf("%-3d",a[i]);
      if(i % 5 == 4) printf("\n");
   }
   printf("\n\n");
   
   
   system("pause");
   system("cls");
}

void TriangleDFS(int posIndex, int countIndex, int tri_cnt){
   if(f_flg || posIndex == 25 || tri_cnt > min) return;

   if(tri_cnt == min){
      /*printf("\n\n");
      
      for(int i = 0 ; i < 3 ; i++)   printf("%d %d %d\n",colorPos[i][0],colorPos[i][1],colorPos[i][2]);
   
      display(2,colorPos[tri_cnt],territory,0);
      */
   
      if(Finish(map,territory))   {
         f_flg = 1;
         for(int i = 0 ; i < min ; i++){
            memcpy(g_colorPos[i],colorPos[i],sizeof(int) * 3);
            printf("%d %d %d\n",colorPos[i][0],colorPos[i][1],colorPos[i][2]);
         }   
      }
      return;
   }
   
   
   if(!f_flg){
      if(countIndex < 3){
         for(int i = 0 ; i < 2 ; i++){
            colorPos[tri_cnt][countIndex] = posIndex;
            TriangleDFS(posIndex + 1,countIndex + i,tri_cnt);
         }
      }
      
      else{
         if(colorPos[tri_cnt][0] % 5 == colorPos[tri_cnt][1] % 5 && colorPos[tri_cnt][1] % 5 == colorPos[tri_cnt][2] % 5) return;
         if(colorPos[tri_cnt][0] / 5 == colorPos[tri_cnt][1] / 5 && colorPos[tri_cnt][1] / 5 == colorPos[tri_cnt][2] / 5) return;
         if(setTerritory(colorPos[tri_cnt],array) <= 1) return;
         
         if(tri_cnt){
            for(int i = 0 ; i < 3 ; i++){
               for(int j = 0 ; j < 3 ; j++){
                  if(colorPos[i][j] != colorPos[(i+1)%3][j]) break;
                  if(j == 2) return;
               }
            }
         }
         
         for(int i = 0 ; i < 25 ; i++)
            if(array[i] == 3)   territory[i] ++;
         /*
         printf("\n====\ndp : %d\n",tri_cnt);   
         for(int i = 0 ; i < 25 ; i++){
            printf("%d ",array[i]);
            if(i % 5 == 5) printf("\n");
         }
         printf("\n\n");
         printf("\n====\n");   
         for(int i = 0 ; i < 25 ; i++){
            printf("%d ",territory[i]);
            if(i % 5 == 5) printf("\n");
         }
         printf("\n\n");
         
         display(2,colorPos[tri_cnt],territory,0);
         */
         TriangleDFS(0,0,tri_cnt+1);
         
         for(int i = 0 ; i < 25 ; i++)
            if(array[i] == 3)   territory[i]--;
      }
   }
   

}

int main(){

/*

0  1  2  3  4  
5  6  7  8  9
10 11 12 13 14
15 16 17 18 19
20 21 22 23 24
  
*/   

   for(int i = 1 ; i <= 3 && !f_flg;  i++){
      min = i;
      TriangleDFS(0,0,0);
   }
   
   
   printf("%d\n\n",min);    
   
   for(int i = 0 ; i < min ; i++){
      printf("\n%d %d %d\n\n",g_colorPos[i][0],g_colorPos[i][1],g_colorPos[i][2]);
      
      int array[25] = {0};
      setTerritory(g_colorPos[i],array);
      
      for(int j = 0; j < 25 ; j++){   
         printf("%d ",array[j]);
         if(j % 5 == 4)printf("\n");
      }
   }
}
