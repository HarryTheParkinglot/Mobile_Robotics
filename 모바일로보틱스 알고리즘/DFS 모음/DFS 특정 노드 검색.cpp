#include <stdio.h>


#define M 10 //노드 개수 10개 세팅

int i;

int check_node[M+1] ={0}; // 방문 표시 배열
int nodeLink[M+1][M-1]= {
   //노드별 가능 링크는 최대 M-1개
   //Node 번호가 1부터 부여되므로 0은 그냥 빈공간 / M+1개의 열이 필요함
   {0},//0 => 의미 없는 빈 공간
   {2,3,4},//1
   {1,5,9},//2
   {1,6},//3
   {1,7},//4
   {2,8,9},//5
   {3},//6
   {4,10},//7
   {5},//8
   {2,5},//9
   {7}//10
};

void DFS_nodeSearch(int p, int dp){
   check_node[p] = 1; //현재 위치 중복해서 방문하지 않도록 방문표시함

   printf("현재 노드는 %-2d | 깊이는 %-2d\n",p,dp);
   for(int i = 0 ; i < M-1; i++){ //M-1개의 노드가 존재할 수 있으므로 그 수만큼 반복
      int nextNode = nodeLink[p][i]; //p번 노드와 연결된 노드를 nextNode에 넣어줌
      if(nextNode == 0 || check_node[nextNode]) continue;//nextNode == 0(연결X)또는 check_node[nextNode]이미 방문한 노드인 경우 패스
      DFS_nodeSearch(nextNode,dp+1); //그 외의 경우 dp를 +1해 해당 노드 방문
   }
   return;
}

int main(){
    printf("몇번 노드에서 서치를 시작할지 정해주시요 : ");
    scanf("%d",&i);
   DFS_nodeSearch(i,0); //노드 i를 기준으로 DFS_search 진행

}
