 #include <stdio.h>
#include <math.h>
#include <windows.h>
int tmp;

#define swap(a,b) tmp = j_node[a]; j_node[a] = j_node[b]; j_node[b] = tmp;


int j_node[20] = {
   1,0,0,0,0,
   1,3,0,0,0,
   2,0,0,0,0,
   2,0,0,0,0
};
int g_Area[4] = {8,9,13,14};

int horse_Xmove[8] = {2,1,-1,-2,-2,-1,1,2};
int horse_Ymove[8] = {-1,-2,-2,-1,1,2,2,1};

int check[20] = {0};

int horse_Route[20] = {0};
int cannon_Route[20] = {0};
int janggi_Route[30] = {0};

int cannon_g_Route[20] = {0};
int janggi_g_Route[30] = {0};

int horse_min = 0;

int cannon_min_touch = 19;
int cannon_min_move = 10;

int janggi_min_touch = 20; //퍽 터지 
int janggi_min_move = 20;


/*
00000
00000
00000
00000
*/
//동작 방향 (프로토콜)을 저장함 

//g_route[0] --> 퍽의 종류
//g_route[1] --> 해당 퍽의 위치 
//g_route[n] n>1--> 해당 퍽의 동작 방향  

void display(int p){
   for(int i = 0 ; i < 20 ; i++){
      printf("%d ", j_node[i]);
      if(i % 5 == 4) printf("\n");
   }
   printf("\n\n");
   for(int i = 0 ; i < 20 ; i++){
      printf("%d ", check[i]);
      if(i % 5 == 4) printf("\n");
   }
      
   system("pause");
   system("cls");
}

int g_check(int p){
   for(int i = 0 ; i < 4 ; i++)
      if(p == g_Area[i])   return 1;
   return 0;
}

int Finish(int *goals, int *j_node){
   for(int i = 0 ; i < 4;  i++)
      if(!j_node[goals[i]]) return 0; 
      
   return 1;
}

void HorseDFS(int p, int g, int dp, int *str, int index){
   if(dp >= horse_min) return;
   
//   display(p);
   if(dp == 2) str[index + 1] = p;
   if(p == g){  
      horse_min = dp;
      
      str[index] = -1; // 프로토콜 -1
      
      for(int i = 2 ; i < dp ; i++)
         str[index + i] = horse_Route[i];
   }
   
   int x = p % 5;
   int y = p / 5;
   
   check[p] = 1;
   
   for(int i = 0 ; i < 8 ; i++){ 
         
      int nx = x + horse_Xmove[i];
      int ny = y + horse_Ymove[i];
      int np = nx + (ny*5);
      
      if(nx < 0 || ny < 0 || ny >= 4 || nx >= 5) continue;
      
      if(abs(horse_Xmove[i]) == 2 && j_node[p + horse_Xmove[i]/2]) continue;
      if(abs(horse_Ymove[i]) == 2 && j_node[p + (horse_Ymove[i]/2)*5]) continue;
      if(j_node[np] || check[np]) continue;
      horse_Route[dp] = np; 
      HorseDFS(np,g,dp+1,str,index);
   }
   check[p] = 0;
}

void CannonDFS(int lp, int p, int y, int *goals, int *j_node, int dp, int touch_cnt, int *str, int index){
   if(touch_cnt > cannon_min_touch)return;

   if(g_check(p) && (dp < cannon_min_move || touch_cnt < cannon_min_touch)){
      for(int i = 0 ; i < dp ; i++)
         str[index + i] = cannon_Route[i];
         
      cannon_min_move = dp;
      cannon_min_touch = touch_cnt;
      return;
   }
   
   check[p] = 1;
   
   if(j_node[p + 1] % 2){ //1 또는 3 
      for(int i = 2 ; i <= 4 ; i++){
          if((p % 5) + i >= 5 || j_node[p+i] || check[p + i]) break; //범위바깥  //더이상 진행이 불가한 위치일경우 
          
          if(lp == 2)
             cannon_Route[dp] = p+i; //다음 위치로 그냥 넘어가면 됨 
          else{
             cannon_Route[dp] = -2;
             cannon_Route[dp+1] = p;   
             cannon_Route[dp+2] = p+i;
         }
            
          swap(p,p+i);
          CannonDFS(2,p+i,y,goals,j_node,dp + ((lp != 2)? 3 : 1),touch_cnt + (lp != 2),str,index);
          swap(p,p+i);
      }
   }
   
   int dir = (p/5<2)? 1 : -1;
   
   if(!j_node[p + (5*dir)]){
      for(int i = 0 ; i < 20 ; i++){
         if(j_node[i] != 1 || g_check(i)) continue;
         horse_min = 10;
         
         HorseDFS(i,p+5*dir,2,cannon_Route,dp);
         if(horse_min == 10) continue;
         swap(i,p+5*dir);
         
         CannonDFS(1,p,y,goals,j_node,dp+horse_min,touch_cnt+1,str,index);      
         swap(i,p+5*dir);
      }
   }   
   
   else if(j_node[p + (5*dir)] != 2){
      for(int i = 2 ; i <= 3 ; i++){  //최대 3칸까지 이동 가능 
         int np = p + (5*i*dir);
         if(np >= 20 || np < 0 || j_node[np] ||check[np]) break;
         
          if(lp == 2)
             cannon_Route[dp] = np; //다음 위치로 그냥 넘어가면 됨 
          else{
             cannon_Route[dp] = -2;
             cannon_Route[dp+1] = p;   
             cannon_Route[dp+2] = np;
         }

          swap(p,np);
          CannonDFS(2,np,y,goals,j_node,dp + ((lp != 2)? 3 : 1),touch_cnt + (lp != 2),str,index);
          swap(p,np);
      }      
   }

   if(p % 5 == 0){ //처음 위치  stepOne
      if(y / 5 != p / 5){
         cannon_Route[dp] = -3;
         cannon_Route[dp+1] = y;
         cannon_Route[dp+2] = p+1;
         swap(y,p+1);
         CannonDFS(3,p,p+1,goals,j_node,dp+3,touch_cnt+1,str,index);
         swap(y,p+1);
      }
   }
   check[p] = 0;
}



void JanggiDFS(int *goals, int *j_node, int dp, int touch_cnt){   
   int pos[4] = {0},yPos;
   
   int cnt = 0;

   if(touch_cnt > janggi_min_touch) return;
   else if(touch_cnt == janggi_min_touch && dp >= janggi_min_move) return;
   
   if(Finish(goals,j_node)){
      janggi_min_touch = touch_cnt;
      janggi_min_move = dp;
      
      for(int i = 0 ; i < dp ; i++){
         janggi_g_Route[i] = janggi_Route[i];
      }         
      for(int i = 0 ; i < dp ; i++){
         if(janggi_g_Route[i] < 0) printf("\n");
         printf("%d ",janggi_g_Route[i]);
      }
      
      printf("\n");
      display(0);
   
      return;
   }
   
   for(int i = 0 ; i < 20 ; i++){
      if(g_check(i)) continue;
      if(j_node[i] == 1 || j_node[i] == 2 ) pos[cnt++] = i;
      if(j_node[i] == 3) yPos = i;
   }
   
   for(int i=  0 ; i < cnt ; i++){
      memset(check,0,sizeof(int) * 20);
      
      if(j_node[pos[i]] == 1){
         for(int j = 0 ; j < 4 ; j++){
            
            if(j_node[goals[j]]) continue;
            
            horse_min = 12;
            HorseDFS(pos[i],goals[j],2,janggi_Route,dp);
            if(horse_min == 12) continue;
            
            swap(pos[i],goals[j]); 
            
            JanggiDFS(goals,j_node,dp+horse_min,touch_cnt+1);
            
            swap(pos[i],goals[j]); 
         }
      }
      
      if(j_node[pos[i]] == 2){
         for(int j = 0 ; j < 4 ; j++){
            if(j_node[goals[j]]) continue;
            
            cannon_min_touch = 6;
            cannon_min_move = 25;
            CannonDFS(-1,pos[i],yPos,goals,j_node,0,0,janggi_Route,dp);
            if(cannon_min_move == 25) continue;
            // -1 0 7 10 -2 10 15
            
            int forSave = cannon_min_move;
            for(int i = 0 ; i < cannon_min_move-1 ; i++){
               
            
               if(janggi_Route[dp+i] < 0 || janggi_Route[dp+i+1] < 0) continue;
               swap(janggi_Route[dp + i],janggi_Route[dp+ i + 1]);               
            }
            
            JanggiDFS(goals,j_node,dp+cannon_min_move,touch_cnt+cannon_min_touch);
            
            for(int i = forSave-1 ; i > 1 ; i--){
               
               if(janggi_Route[dp+i] < 0 || janggi_Route[dp+i-1] < 0) continue;
               swap(janggi_Route[dp+ i],janggi_Route[dp+ i-1]);               
            }
         }
      }
   }
}

int main(){
   janggi_min_touch = 10;
   janggi_min_move = 40;

   JanggiDFS(g_Area,j_node,0,0);
   
   printf("janggi_min %d\n", janggi_min_move);
   //printf("min : %d",cannon_min_move);
   
   for(int i = 0 ; i < janggi_min_move ; i++){
      if(janggi_g_Route[i] < 0) printf("\n");
      printf("%d ",janggi_g_Route[i]);
   }
   printf("\n");
   

}
   
