#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <math.h>
#include <time.h>

char tmp;
char min;
#define M 20  // ���� �ִ� �ִ� ����  
#define move(a,b,str) str[b] = str[a],str[a] = 0;

//�ܵ鿡 ��ȣ
//�� �ܵ��� for���� ���� �ѹ��� �̵����Ѱ��� �Լ� ���� 

void Sort(char lp, char dp, char *node); //�����Լ�
char GetScore(char *node);
void CreateLink(int p,char *str);
char GetVisitable(int p, char *str ,char *node); //���� ��ġ���� �湮 ������ ����� ������ ����

char flg = 0; //���ĿϷ� flg 
char route[M] = {0};
char g_route[M] = {0};
char nodeHistory[M][12] = {0}; //�ߺ� ���ſ� 

char map[12] = {
   1,2,2,3,
   1,3,0,2,
   0,1,0,3
};


char node[12];
//-1 : Ȧ / -2 : ��ֹ�
//9 	//6
char mapGoal[12] = {
 1,6,2,3,
 1,3,0,2,
 0,1,9,3
};

char link[12][4];


int main(){
 clock_t start = clock(); // ���� �ð� ����

   for(int i = 0 ; i < 12;  i++)
      CreateLink(i,link[i]);
   char str[12];
   

   
   
   for(int i = 2 ; i < M * 2 ; i+=2){ //�ִ� 21�� �̵� ���� 
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
      
      //�湮 ������ ���� ��ǥ / �湮���� ���� �� / ���� ��ġ���� ���� ������� (���θ���) / ��ֹ��� �ƴ� 
      //Ȧ�� ��� �˰��򿡼� �Ǵ�   
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


void Sort(char lp, char dp, char *node){ //lp : ������ �Ű�� �� / dp : �� Ž������ : node : �� �迭
   
   if(dp > min || flg) return; // ������ min ���̺��� �� ���� ����  
   
   if(!GetScore(node)) { //score�� 0�̸� ���ĿϷ� 
      flg = 1;
      min = dp;
   //   printf("dp : %d",dp);
      for(int i = 0 ; i < dp ; i++)
         g_route[i] = route[i];  //��������� ��� ����־��� 
   }
   
   for(int i = 0 ; i < dp/2 ; i++){
      for(int j = 0 ; j < 12 ; j++){
         if(node[j] != nodeHistory[i][j]) break;
         if(j == 11) return; //index 11���� �� ���ٸ� �ߺ� / ���� �ʿ�  
         
      }
   }
   
   for(int i = 0 ; i < 12 ; i++)
      nodeHistory[dp/2][i] = node[i];
   

   char visitables[12][12] = {0};;
   int scoreAfterMove[12][12];
   
   for(int i = 0 ; i < 12*12 ; i++) scoreAfterMove[i/12][i%12] = 999;   
    
   for(char i = 0 ; i < 12 && !flg ; i++){ //for�� �������� �� ��ġ�� �Ű��� ���� ������ ����  
      if(i == lp || !node[i] || mapGoal[i] == 6) continue; //���� ���°��� �ǳʶ� 
      
      char n = GetVisitable(i,visitables[i],node);   
      
      for(int j = 0 ; j < n && !flg; j++){
          char np = visitables[i][j];
         if(np < 0 || node[np] || mapGoal[np] == 6 || mapGoal[np] == 9 || i == np) continue; //���� �ִ� ��� �ش���ġ�� ���� ������ �� �� ���� 
         move(i,np,node); //�� �̵�
         scoreAfterMove[i][j] = GetScore(node); // i��ġ�� ���� link[i][j]��ġ�� �̵����� ���� ���� ���� 
         move(np,i,node); //�ٽ� ����ġ��         
      } 
   }

   for(char s = 0 ; s < 45 && !flg; s++){ //�ɷ��� �� ���� ������ �� �𸣰ھ �ϴ� �������� �ҽ� ���� 
      for(char  i = 0 ; i < 12 && !flg; i++){
         for(char j = 0 ; j < 12 && !flg; j++){
            if(scoreAfterMove[i][j] == s) { //������ ���� ���� �ֺ��� �Űܺ� 
               char np = visitables[i][j];
               move(i,np,node);
               
               route[dp] = i; //���� ���� ��ġ  
               route[dp + 1] = np; //���� �� ��ġ 
               
               Sort(np,dp+2,node);
               move(np,i,node);
            }
         }
      }
   }
   
   return;
}
