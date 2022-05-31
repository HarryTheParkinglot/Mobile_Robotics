#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <math.h>
#include <time.h>

char tmp;
char min;
#define M 20  // 들어갈수 있는 최대 깊이  
#define move(a,b,str) str[b] = str[a],str[a] = 0;

//퍽들에 번호
//각 퍽들을 for문을 돌려 한번씩 이동시켜가며 함수 실행 

void Sort(char lp, char dp, char *node); //정렬함수
char GetScore(char *node);
void CreateLink(int p,char *str);
char GetVisitable(int p, char *str ,char *node); //현재 위치에서 방문 가능한 노드의 개수를 리턴

char flg = 0; //정렬완료 flg 
char route[M] = {0};
char g_route[M] = {0};
char nodeHistory[M][12] = {0}; //중복 제거용 

char map[12] = {
   1,2,2,3,
   1,3,0,2,
   0,1,0,3
};


char node[12];
//-1 : 홀 / -2 : 장애물
//9 	//6
char mapGoal[12] = {
 1,6,2,3,
 1,3,0,2,
 0,1,9,3
};

char link[12][4];


int main(){
 clock_t start = clock(); // 시작 시간 저장

   for(int i = 0 ; i < 12;  i++)
      CreateLink(i,link[i]);
   char str[12];
   

   
   
   for(int i = 2 ; i < M * 2 ; i+=2){ //최대 21번 이동 가능 
      min = i;
      memcpy(node,map,sizeof(char)*12);
      Sort(-1,0,node);
      
      if(flg) break;
   }
   
   int startPoint;
   if(flg){
   		for(int i = 0 ; i < 4 ; i++){
			int point = g_route[0];
			int p = link[point][i];
			if(p < 0) continue;
			if(!node[p]) startPoint = p;
		}
		
		
      printf("dp : %d\n",min);
      printf("startPoint : %d\n",startPoint);
      for(int i = 0 ; i < min; i+=2)
         printf("%d -> %d\n",g_route[i],g_route[i+1]);
         
   }
   double cal = (double)(clock() - start)/CLOCKS_PER_SEC;
   
      
   printf("\ncal : %lf",cal);
   
}

void display(char dp, char *node){
   system("cls");
   
   for(int i = 0 ; i < 12 ; i++){
      printf("%-2d",node[i]);
      if(i % 4 == 3) printf("\n");
   }
   printf("\ndp : %d\n\n\n",dp);
   
   system("Pause");
   
}

char GetVisitable(int p, char *str, char *node){
   char q[12],f=0,r=0,chk[12] = {0};
   memset(q,-1,sizeof(char)*12);
   
   q[r++] = p,chk[p] = 1;
   
   while(q[f] >= 0){
      
      int pp = q[f++];
      
      //방문 가능한 점의 좌표 / 방문하지 않은 점 / 현재 위치에서 퍽이 없어야함 (가로막힘) / 장애물이 아님 
      //홀의 경우 알고리즘에서 판단   
      for(int i = 0 ; i < 4; i++) {
      
         int np = link[pp][i];
         if(np < 0 || chk[np]) continue;
         if(node[p] && node[np]) continue;
         if(!node[p] && (node[pp] || mapGoal[np] == 6)) continue;  //
         //printf("p : %d",np);
         q[r++] = np,chk[np] = 1;
      //   printf("pp : %d np : %d\n",pp,np);
      }
   }
   for(int i = 0 ; i < f ; i++) str[i] = q[i];
   return f;
}


char GetScore(char *node){
   char match[12] = {0};
   char score = 0;
   
   for(int i = 0 ; i < 12;  i++)
      if(node[i] == mapGoal[i]) match[i] = 1;
   
   for(int i = 0 ; i < 12 ; i++){
      char min = 100,minPos = -1;
      if(!node[i] || node[i] == mapGoal[i] || mapGoal[i] == 6) continue;
      
      for(int j = 0 ; j < 12 ; j++){
         if(mapGoal[j] != node[i] || match[j] ) continue;
         
         int dis = abs(i/4-j/4) +abs(i%4-j%4);
         if(dis < min){ min = dis,minPos = j;}
      }
      match[minPos] = 1;
      score += min; 
   
   }
   
   
   return score;
   
}


///////////////////////////

void CreateLink(int p , char *str){
   int cnt = 0;
   memset(str,-1,sizeof(char)*4);
   if(p%4 - 1 >= 0) {str[cnt++] = p-1;}
   if(p%4 + 1 < 4) {str[cnt++] = p+1;}
   if(p/4 - 1 >= 0) {str[cnt++] = p-4;}
   if(p/4 + 1 < 3) {str[cnt++] = p+4;}   
}


void Sort(char lp, char dp, char *node){ //lp : 이전에 옮겼던 퍽 / dp : 현 탐색깊이 : node : 현 배열
   
   if(dp > min || flg) return; // 지정한 min 깊이보다 더 들어가면 종료  
   
   if(!GetScore(node)) { //score가 0이면 정렬완료 
      flg = 1;
      min = dp;
   //   printf("dp : %d",dp);
      for(int i = 0 ; i < dp ; i++)
         g_route[i] = route[i];  //현재까지의 경로 집어넣어줌 
   }
   
   for(int i = 0 ; i < dp/2 ; i++){
      for(int j = 0 ; j < 12 ; j++){
         if(node[j] != nodeHistory[i][j]) break;
         if(j == 11) return; //index 11까지 다 같다면 중복 / 제거 필요  
         
      }
   }
   
   for(int i = 0 ; i < 12 ; i++)
      nodeHistory[dp/2][i] = node[i];
   

   char visitables[12][12] = {0};;
   int scoreAfterMove[12][12];
   
   for(int i = 0 ; i < 12*12 ; i++) scoreAfterMove[i/12][i%12] = 999;   
    
   for(char i = 0 ; i < 12 && !flg ; i++){ //for문 돌려가며 각 위치별 옮겼을 때의 점수를 구함  
      if(i == lp || !node[i] || mapGoal[i] == 6) continue; //퍽이 없는곳은 건너뜀 
      
      char n = GetVisitable(i,visitables[i],node);   
      
      for(int j = 0 ; j < n && !flg; j++){
          char np = visitables[i][j];
         if(np < 0 || node[np] || mapGoal[np] == 6 || mapGoal[np] == 9 || i == np) continue; //퍽이 있는 경우 해당위치에 퍽을 가져다 둘 수 없음 
         move(i,np,node); //퍽 이동
         scoreAfterMove[i][j] = GetScore(node); // i위치의 퍽을 link[i][j]위치로 이동했을 때의 점수 저장 
         move(np,i,node); //다시 원위치로         
      } 
   }

   for(char s = 0 ; s < 45 && !flg; s++){ //걸러야 할 점수 범위를 잘 모르겠어서 일단 수민이형 소스 참고 
      for(char  i = 0 ; i < 12 && !flg; i++){
         for(char j = 0 ; j < 12 && !flg; j++){
            if(scoreAfterMove[i][j] == s) { //점수가 가장 낮은 애부터 옮겨봄 
               char np = visitables[i][j];
               move(i,np,node);
               
               route[dp] = i; //퍽을 먹은 위치  
               route[dp + 1] = np; //퍽을 둘 위치 
               
               Sort(np,dp+2,node);
               move(np,i,node);
            }
         }
      }
   }
   
   return;
}
