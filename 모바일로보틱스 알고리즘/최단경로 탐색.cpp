#include <stdio.h>
#include <windows.h>


int map[25] = {
	0,1,0,0,0,
	0,1,1,1,1,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0
};
int chk[25] = {0}; //�湮 ���θ� ǥ���ϴ� �迭  

int route[25] = {0}; //���� ��ġ���� �κ��� ��������� �̵� ��� ���� 
int g_route[25] = {0}; //���ݱ��� ���� �κ��� �ִܰŸ� �̵���� ���� 

int min = 20;


void display(int p, int *arr){
	int save = map[p];
	map[p] = 5; //�κ��� ��ġ ǥ��
	printf("===map �迭====\n");  
	for(int i = 0 ; i < 25; i ++){
		printf("%-3d",map[i]); 
		if(i % 5 == 4) printf("\n");
	}
	map[p] = save;
	
	printf("\n===chk �迭 ===\n");
	for(int i = 0 ; i < 25;  i++){
		printf("%-3d",chk[i]);
		if(i % 5 == 4) printf("\n");
	}
	
} 

void DFS(int p, int goal, int dp){ 
	//p : �κ��� ���� ��ǥ 
	//goal : �κ��� �����ϰ��� �ϴ� ��ġ
	//dp : �κ��� �̵��� Ƚ��(�Ÿ�) 

	if(dp >= min) return;
	//�ִܰŸ��� ���ϴ� ���� �����̹Ƿ� dp(�̵��Ÿ�)�� ���ݱ��� ���� 
	//�ּ� �Ÿ����� �� ��� �ش� ���� ���� 
	
	route[dp] = p; //route�� dp��° �ڸ���  
	chk[p] = 1;
	
	
	
	display(p,map);
	system("pause");
	system("cls");
	if(p == goal){  //���� return ���ǹ��� ��ȭ�ϰ�,
	//�� �����ߴٸ� ������  �ִܰŸ��̹Ƿ� g_route�� ���� ��� ������ 
		min = dp;
	
		for(int i = 0 ; i <= dp ; i++)
		g_route[i] = route[i];
		
		printf("����! ���� ����� ���� �б�� �ǵ��ư��ϴ�\n");		
		printf("min : %d\n",min);
	}
	
	
	
	
	int x = p%5;
	int y = p/5;
	
	int dx[4] = {1,0,-1,0}; //x�̵� �ݰ� 
	int dy[4] = {0,1,0,-1}; //y�̵� �ݰ� 
	
	for(int i = 0 ; i < 4 ; i++){
		int nx = x + dx[i];
		int ny = y + dy[i]; 
		int np = nx + (ny * 5);
	
		if(nx < 0 || ny < 0 || nx >= 5 || ny >= 5) continue;  //���� ���� ���
		if(chk[np]) continue;//�̹� �湮�� ���� ���  
		if(map[np]) continue; //���� ���� ���(��ֹ�) 
		DFS(np,goal,dp+1); 
	}
	chk[p] = 0; 
	//�ش� �б⸦ ��� Ž���ϸ� �ٸ� �б⿡���� Ž���� ���� chk���� �ʱ�ȭ�� 
}

int main(){
	DFS(0,24,0);
	printf("\n=====�ִܰ�� : \n");
	
	for(int i = 0 ; i <= min ; i++){
		printf("%d ",g_route[i]);
	}
	
	/*S : START G : GOAL 
	S 0 0 0 0
	0 0 0 0 0
	0 0 0 0 0
	0 0 0 0 0
	0 0 0 0 G
	*/
}
