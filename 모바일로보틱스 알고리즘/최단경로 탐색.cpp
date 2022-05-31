#include <stdio.h>
#include <windows.h>


int map[25] = {
	0,1,0,0,0,
	0,1,1,1,1,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0
};
int chk[25] = {0}; //방문 여부를 표시하는 배열  

int route[25] = {0}; //현재 위치까지 로봇의 오기까지의 이동 경로 저장 
int g_route[25] = {0}; //지금까지 구한 로봇의 최단거리 이동경로 저장 

int min = 20;


void display(int p, int *arr){
	int save = map[p];
	map[p] = 5; //로봇의 위치 표시
	printf("===map 배열====\n");  
	for(int i = 0 ; i < 25; i ++){
		printf("%-3d",map[i]); 
		if(i % 5 == 4) printf("\n");
	}
	map[p] = save;
	
	printf("\n===chk 배열 ===\n");
	for(int i = 0 ; i < 25;  i++){
		printf("%-3d",chk[i]);
		if(i % 5 == 4) printf("\n");
	}
	
} 

void DFS(int p, int goal, int dp){ 
	//p : 로봇의 현재 좌표 
	//goal : 로봇이 도착하고자 하는 위치
	//dp : 로봇이 이동한 횟수(거리) 

	if(dp >= min) return;
	//최단거리를 구하는 것이 목적이므로 dp(이동거리)가 지금까지 구한 
	//최소 거리보다 긴 경우 해당 경우는 버림 
	
	route[dp] = p; //route의 dp번째 자리에  
	chk[p] = 1;
	
	
	
	display(p,map);
	system("pause");
	system("cls");
	if(p == goal){  //위의 return 조건문을 통화하고,
	//골에 도착했다면 무조건  최단거리이므로 g_route에 현재 경로 넗어줌 
		min = dp;
	
		for(int i = 0 ; i <= dp ; i++)
		g_route[i] = route[i];
		
		printf("도착! 가장 가까운 이전 분기로 되돌아갑니다\n");		
		printf("min : %d\n",min);
	}
	
	
	
	
	int x = p%5;
	int y = p/5;
	
	int dx[4] = {1,0,-1,0}; //x이동 반경 
	int dy[4] = {0,1,0,-1}; //y이동 반경 
	
	for(int i = 0 ; i < 4 ; i++){
		int nx = x + dx[i];
		int ny = y + dy[i]; 
		int np = nx + (ny * 5);
	
		if(nx < 0 || ny < 0 || nx >= 5 || ny >= 5) continue;  //범위 밖인 경우
		if(chk[np]) continue;//이미 방문한 점인 경우  
		if(map[np]) continue; //막힌 곳인 경우(장애물) 
		DFS(np,goal,dp+1); 
	}
	chk[p] = 0; 
	//해당 분기를 모두 탐색하면 다른 분기에서의 탐색을 위해 chk값을 초기화함 
}

int main(){
	DFS(0,24,0);
	printf("\n=====최단경로 : \n");
	
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
