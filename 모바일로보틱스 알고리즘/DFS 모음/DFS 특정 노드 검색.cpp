#include <stdio.h>


#define M 10 //��� ���� 10�� ����

int i;

int check_node[M+1] ={0}; // �湮 ǥ�� �迭
int nodeLink[M+1][M-1]= {
   //��庰 ���� ��ũ�� �ִ� M-1��
   //Node ��ȣ�� 1���� �ο��ǹǷ� 0�� �׳� ����� / M+1���� ���� �ʿ���
   {0},//0 => �ǹ� ���� �� ����
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
   check_node[p] = 1; //���� ��ġ �ߺ��ؼ� �湮���� �ʵ��� �湮ǥ����

   printf("���� ���� %-2d | ���̴� %-2d\n",p,dp);
   for(int i = 0 ; i < M-1; i++){ //M-1���� ��尡 ������ �� �����Ƿ� �� ����ŭ �ݺ�
      int nextNode = nodeLink[p][i]; //p�� ���� ����� ��带 nextNode�� �־���
      if(nextNode == 0 || check_node[nextNode]) continue;//nextNode == 0(����X)�Ǵ� check_node[nextNode]�̹� �湮�� ����� ��� �н�
      DFS_nodeSearch(nextNode,dp+1); //�� ���� ��� dp�� +1�� �ش� ��� �湮
   }
   return;
}

int main(){
    printf("��� ��忡�� ��ġ�� �������� �����ֽÿ� : ");
    scanf("%d",&i);
   DFS_nodeSearch(i,0); //��� i�� �������� DFS_search ����

}
