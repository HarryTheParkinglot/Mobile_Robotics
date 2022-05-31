#include <stdio.h>
#include <math.h>
#include <windows.h>


//선분의 기울기의 역수 / --> dx / dy 

int map[36] = {
   0,0,0,0,0,0,
   0,0,2,3,3,0,
   0,0,2,3,3,0,
   0,1,0,0,0,0,
   0,1,1,2,2,0,
   0,0,0,0,0,0
};

int store[3];
int g_displayMap[36] = {0};
int colorCnt[4] = {0};


int colorAndPos[12][2] = {0};
int f_flg = 0;//b / r이 모두 1이면 f_flg = 1 

int setTerritory(int *points, int *output) {
   int cpPoints[3];
   memcpy(cpPoints,points,sizeof(int) * 3);
   
   for(int i = 0 ; i < 36 ; i++)
      output[i] = 0;

   int tmp = 0;
   
   
   for(int j =  0 ; j < 2 ; j++){
      for(int i = 0  ; i < 2 ; i++){
         if(cpPoints[i] % 6 > cpPoints[i+1] % 6) {
            tmp = cpPoints[i];
            cpPoints[i] = cpPoints[i+1];
            cpPoints[i+1] = tmp;
         }
      }
   }
   
   
   //점들을 x좌표 순으로 정렬  
   int p[3][2] = {0};
   float grad[3] = {0}; // 0 : 0 - 1 | 1 : 1 - 2 | 2 : 2 - 3 
   
   for(int i = 0 ; i < 3 ; i++){
      p[i][0] = cpPoints[i]/6; //포인트의 y좌표    
      p[i][1] = cpPoints[i]%6; //포인트의 x좌표 
      output[p[i][0]*6 + p[i][1]] = 2;   
   }
   
   int dx[3],dy[3];
   
   for(int i = 0 ; i < 3 ; i++){
      dx[i] = p[i][1] - p[(i+1)%3][1];
      dy[i] = p[i][0] - p[(i+1)%3][0];
      
      grad[i] = (dy[i])? (float)dx[i]/dy[i] : 99;
   //   printf("\ngrad %.1f\n",grad[i]);
   }
   
   //printf("\n %.1f \n",grad[0]);
//   printf("\n %.1f \n",grad[1]);
   
   for(int i = 0 ; i < 3 ; i++){
      if(grad[i] == 99 && abs(dx[i]) > 1){ 
         int s = (dx[i] > 0)? p[(i+1)%3][1] : p[i][1];
         int e = (dx[i] > 0)? p[i][1] : p[(i+1)%3][1];
         
         for(int j = s + 1 ; j < e ; j++)
            output[p[i][0] * 6 + j] = 2;
      }
      
      else{
         int s = p[i][0];
         int e = p[(i+1)%3][0];
         
         int sx = p[i][1];
         
         
         if(s < e){
            for(int j = s + 1 ; j < e ; j++){
               float nx = sx + ((j-s) * grad[i]); 
      //         printf("nx : %\n",nx);
               
               if(int(nx) < nx){
                  nx = (nx > p[1][1])? ceil(nx) : floor(nx);
                  output[j * 6 + (int)nx]= 1;
               }   
               else{
                  output[j * 6 + (int)nx]= 2;
               }            
            
            }
         }
         else{
            for(int j = s-1 ;  j > e ; j--){
               float nx = sx + (j-s) * grad[i]; 
            //   printf("nx : %\n",nx);
               
               if(int(nx) < nx){
                  nx = (nx > p[1][1])? ceil(nx) : floor(nx);
                  output[j * 6 + (int)nx]= 1;
               }   
               else{
                  output[j * 6 + (int)nx]= 2;
               }            
            }
         }
      }
   }
   int width = 0;
   for(int i = 0 ; i < 6 ; i++){
      int cnt = 0,sx = -1;
      
      for(int j = 0 ; j < 6 ; j++){
         if(output[i*6 + j]) {
            cnt ++;
            if(sx == -1)
               sx = j;
         }
      }
      if(cnt == 2){
         for(int j = sx + 1 ; !output[i*6 + j] ; j++){
            output[i*6 + j] = 3;
            width++;
         }
      }
      
   }
   
   return width;
}

void display(int color, int *colorPos, int *a, int index){
   int dmap[36] = {0};
   
   for(int i = 0 ; i < 3;  i++)
      dmap[colorPos[i]] = color;   
      for(int i = 0 ; i < 36 ; i++){
         printf("%-3d",dmap[i]);
         if(i % 6 == 5) printf("\n");
      }
   
   printf("\n\n");
   
   for(int i = 0 ; i< 36 ; i++){
      printf("%-3d",a[i]);
      if(i % 6 == 5) printf("\n");
   }
   printf("\n\n");
}



void TriangleDFS(int color, int *colorPos, int index, int point){ 
   
   if(f_flg || point == 36) return;

   int savePos[3] = {0};
   
   for(int i = 0 ; i < 3 ; i++)
      savePos[i] = colorPos[i];
   
   int flg = 0;
   int area[36] = {0};
   
   if(index == 3){   
      if(colorPos[0] % 6 == colorPos[1] % 6 && colorPos[1] % 6 == colorPos[2] % 6 ) return;
      if(colorPos[0] / 6 == colorPos[1] / 6 && colorPos[1] / 6 == colorPos[2] / 6 ) return;
      
      
      int cnt = 0;
      
      int width = setTerritory(colorPos,area);
      if(width < colorCnt[color]) return; //수용 공간이 부족하면 종료  
      
   //   display(color,colorPos,area,index);
      
      //system("pause");
   //   system("cls");
      
      
      for(int i = 0 ; i < 36 ; i++){
         if(area[i] == 3 && map[i] && map[i] != color) return; //영역 안에 다른 색이 있으면 종료 
         if(area[i] == 2 && map[i]) return;  //테두리에 색이 있으면 종료 
         if(area[i] == 3 && map[i] == color) cnt ++; //영역 안에 들어있는 색의 개수 카운트 
      }
      if(cnt < colorCnt[color]) return; //개수가 부족한 경우 
      
      flg = 1;
      
      display(color,colorPos,area,index);
      printf("\n%d %d %d\n",colorPos[0],colorPos[1],colorPos[2]);
      system("pause");
      system("cls");
   }
   
   if(flg){
      for(int i = 0 ; i < 3 ; i++)
         g_displayMap[colorPos[i]] = color;
      
      int cnt = 0;
      for(int i = 0 ; i < 36 ; i++){
         if(area[i] == 3 && cnt < 3 && !map[i])  {
            g_displayMap[i] = color;
            cnt++;
         }
      }
      
      if(color == 1){
         for(int i = 0 ; i < 3 ; i++) colorPos[i] = 0; 
         TriangleDFS(3,colorPos,0,0); //다음 색으로 넘어감  
         
         if(!f_flg){
            for(int i = 0 ; i < 36 ; i++)   g_displayMap[i] = 0;
            for(int i = 0 ; i < 3 ; i++) colorPos[i] = savePos[i];
            for(int i = 0 ; i < 12;  i++){
               colorAndPos[i][0] = 0;
               colorAndPos[i][1] = 0;
            }
         }
      }
      
      else{
         int pCnt = 0;
         for(int i = 0 ; i < 36 ; i++){
            if(g_displayMap[i] && !map[i]){
               colorAndPos[pCnt][0] = g_displayMap[i];
               colorAndPos[pCnt++][1] = i;
            }
         }
         f_flg = 1;
         
         return;
         
      }
   }
   
   if(index < 3 && !map[point] && !g_displayMap[point]){
      colorPos[index] = point;
      TriangleDFS(color,colorPos,index+1,point+1);
      for(int i = 0 ; i < 3;  i++) colorPos[i] = savePos[i];
   }
   TriangleDFS(color,colorPos,index,point+1);
   for(int i = 0 ; i < 3;  i++) colorPos[i] = savePos[i];
   
   
   return;
}

int main(){

/*
0  1  2  3  4  5
6  7  8  9  10 11
12 13 14 15 16 17
18 19 20 21 22 23
24 25 26 27 28 29
30 31 32 33 34 35   
*/

   

   for(int i = 0 ; i  <36  ;i++)
      colorCnt[map[i]]++;
//   printf("%d %d",colorCnt[1],colorCnt[3]);

   TriangleDFS(1,store,0,0);
   
   printf("%d\n",f_flg);
   
   for(int i = 0 ; i < 12 ; i++){
      if(colorAndPos[i][0]){   
         printf("%d %d\n",colorAndPos[i][0],colorAndPos[i][1]);
      }
   }
   
   for(int i = 0 ; i < 36 ; i++){
      
      printf("%-3d",g_displayMap[i]);
      if(i % 6 == 5) printf("\n");
   }
   
}

