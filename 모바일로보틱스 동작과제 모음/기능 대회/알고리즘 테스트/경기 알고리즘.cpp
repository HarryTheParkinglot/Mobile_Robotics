#include <stdio.h>
#include <math.h>
#include <windows.h>
//25

int originArea[36] = {
   0,0,0,0,0,0,
   0,1,1,1,1,0,
   0,1,1,1,1,0,
   0,1,1,1,1,0,
   0,1,1,1,1,0,
   0,0,0,0,0,0
};

int setMap[36] = {0};
int solveSet[36] = {0}; //답 배치  

void Horse(int p, int  *array){
   int px = p % 6;
   int py = p / 6;

   int dx[4] = {1,2,-1,-2};
   int dy[4] = {1,2,-1,-2};
   
   for(int i = 0 ; i < 4 ; i++){
      for(int j = 0 ; j < 4 ; j++){
         int nx = px + dx[i];   
         int ny = py + dy[j];
      
         if(abs(dx[i]) + abs(dy[j])== 4) continue; //2 2칸인 경우 제외
         
         if(nx < 0 || nx >= 6 || ny < 0 || ny >= 6) continue;   //범위 밖인경우 제외   
         array[nx + ny * 6] = 1; //이외의 좌표에 영역처리 
      }
   }
}

void Bishop(int p, int *array){
   int px = p % 6;
   int py = p / 6;
   
   int dx[4] = {1,1,-1,-1};
   int dy[4] = {1,-1,1,-1};
   
   for(int i = 0 ; i < 4 ; i++){
      for(int j = 1 ; j <= 6 ; j++){
         int nx = px + (dx[i] * j);
         int ny = py + (dy[i] * j);
         if(nx >= 6 || nx < 0 || ny >= 6 || ny < 0) continue;
         
         array[nx + ny * 6] =1;
      }
   }
} 

void Queen(int p, int *array){
   Bishop(p,array);
   
   int px = p%6;
   int py = p/6;
   
   int dx[4] = {1,0,-1,0};
   int dy[4] = {0,1,0,-1};
   
   for(int i = 0 ; i < 4 ; i++){
      for(int j = 1 ; j <= 6 ; j++){
         int nx = px + (dx[i] * j);
         int ny = py + (dy[i] * j);
      
         if(nx < 0 || ny < 0 || nx >= 6 || ny >= 6) continue;
         array[nx + ny * 6] = 1;
      }
   }
}

int areaCheck(int *array, int *area){
   
   
   for(int i = 0 ; i  < 36 ; i++){
      if(originArea[i]) area[i] = 1;
      if(array[i] == 1) Horse(i,area);
      if(array[i] == 2) Queen(i,area);
      if(array[i] == 3) Bishop(i,area);
   }
   
   for(int i = 0 ; i < 36 ; i++)
      if(area[i] && array[i]) return 0;
   
   return 1;
}
void display(int pos, int *array, int *area){
   system("cls");
   
   
   for(int i = 0 ; i < 36 ; i++){
      printf("%-3d", array[i]);
      if(i % 6 == 5) printf("\n");
   }
   
   printf("\n====\n");
   
   for(int i = 0 ; i < 36 ; i++){
      printf("%-3d", area[i]);
      if(i % 6 == 5) printf("\n");
   }
   printf("pos : %d\n",pos);
//   system("pause");
}


int fin = 0; 

void SetPuck(int b, int r, int y, int pos, int *array){
   int areaMap[36] = {0};
   if(!areaCheck(array,areaMap) || fin || pos == -1)   return; //불가능한 경우 리턴 
   
//   display(pos,array,areaMap);

   if(b + r + y == 0) { //전부 다 배치한경우 
      fin = 1;
      
      for(int i = 0 ; i < 36 ; i++)
         solveSet[i] = array[i];
      
      return;
   }
   
   if(b){
      array[pos] = 1;
      SetPuck(b-1,r,y,pos-1,array);
      array[pos] = 0;
   }
   if(r){
      array[pos] = 2;
      SetPuck(b,r-1,y,pos-1,array);
      array[pos] = 0;
   }
   if(y){
      array[pos] = 3;
      SetPuck(b,r,y-1,pos-1,array);
      array[pos] = 0;
   }
   SetPuck(b,r,y,pos-1,array);
   
   return;
} 



int main(){
   SetPuck(2,2,2,35,setMap); 
   //청 적 황 퍽 개수
   //2 3 2

   if(fin){
      for(int i = 0 ; i < 36 ; i++){
         printf("%-3d",solveSet[i]);
         if(i % 6 == 5) printf("\n");
      }
   }
   else
      printf("불가능한 경우");
   
   
      
}
