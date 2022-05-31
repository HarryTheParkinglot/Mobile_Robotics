#include <stdio.h>
#include <memory.h>
#include <windows.h>
int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int link[18][4];
int map[18] = {0};
int mapGoal[18] = {0};
int node[18];

bool block[18] = {0},check[18] = {0}; 

int route[10],g_route[10],min,flg = 0;
int sort_min;
int sort_route[20][3],type[20];
int g_sort_route[20][3],g_type[20]; //0 : (default) --> GetIn / 1 : getPuck(특정 포인트로 이동) / 2 : putPuck  / 3 : Strike Puck  
int call = 0;

bool InList(int val, int *list, int len){
	for(int i = 0 ; i < len ; i++)
		if(val == list[i]) return 1;
	return 0;
}

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 3 > 0) str[0] = p - 3; 
	if(p % 3 > 0) str[1] = p - 1; 
	if(p / 3 < 5) str[2] = p + 3; 
	if(p % 3 < 2) str[3] = p + 1; 
	if(p % 3 == 1) str[0] = str[2] = -1;
}

void DFS(int p, int wp, int dp, int *node){
	if(dp >= min) return;
	route[dp] = p;
	
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || (node[np] && np != wp) || check[np] || block[np]) continue;
		DFS(np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

bool CheckDFS(int p, int wp, int *node){
	memset(check,0,sizeof(check));
	min = 10;
	DFS(p,wp,0,node);
	return min != 10;
}

bool Sorted(int *node){
	for(int i = 0 ; i < 18 ; i++){
		if(!node[i]) continue;
		if(node[i] != mapGoal[i]) return 0;
	}
	return 1;
}
void display(int *node, int *gNode){
	for(int i = 0 ; i < 18 ; i++){
		printf("%-3d",node[i]);
		if(i % 3 == 2) printf("\n");
	}
	
	printf("\n\n");
	
	for(int i = 0 ; i < 18 ; i++){
		printf("%-3d",gNode[i]);
		if(i % 3 == 2) printf("\n");
	}

	
	system("pause");
	system("cls");
}

void SortDFS(int dp, int change, int *node){
	call++;
	if(dp >= sort_min) return;
	if(Sorted(node)){
		sort_min = dp;
		memcpy(g_type,type,sizeof(type));
		
		for(int j = 0 ; j < sort_min ; j++)
			for(int i = 0 ;i < 3 ; i++)
				g_sort_route[j][i] = sort_route[j][i];
		
		flg = 1;
		return;
	}
	
	for(int i = 0 ; i < 6 ; i++){
		if(!node[i*3 + 1]) continue;
		if(node[i*3] || node[i*3+2]) return; //퍽이 있는 상태에서 가로막히면 해당 경우 버림  
	}
	
	for(int i = 0 ; i < 18 && !flg ; i++){
		if(!node[i] || node[i] == mapGoal[i]) continue;
		
		for(int j = 0 ; j < 36 && !flg; j++){
			if(node[j%18] || !mapGoal[j%18] || !CheckDFS(i,j%18,node)|| (i/3 != (j%18)/3 && node[((j%18)/3*3)+1]))continue;
			if(j / 18 == 0 && mapGoal[j%18] != node[i]) continue;
			if(j / 18 && mapGoal[j%18] == node[i]) continue;
			if(node[i] != mapGoal[j%18] && change >= 3) continue;
			
			int start = -1,plus = (mapGoal[j%18] != node[i]);
			
			if(i % 3 == 1)
				start = (InList(i-1,g_route,min))? i + 1 : i - 1;				
			
			else{
				if(i >= 3 && !node[i-3]) start = i-3;
				if(i <= 12 && !node[i+3]) start = i+3;	
			}
			
			if(start < 0) continue;
			
			sort_route[dp][0] = start; //Robot In
			sort_route[dp][1] = i; //Real Puck Pos 
			sort_route[dp][2] = j%18; //Put Pos
			type[dp] = (block[i]); 
				
			swap(node[i],node[j%18]);
			
			SortDFS(dp+1,change + plus,node);
			swap(node[i],node[j%18]);
			
		}
	}
	
	return;
} 


int main(){
	for(int i = 0 ; i < 18 ; i++)	Link(i,link[i]);
	
	mapGoal[3] = 1;
	mapGoal[6] = 1;
	mapGoal[9] = 1;
	mapGoal[12] = 2;
	mapGoal[15] = 2;
	
	mapGoal[2] = 3;
	mapGoal[5] = 0;
	mapGoal[8] = 0;
	mapGoal[11] = 2;
	mapGoal[14] = 3;
	
	map[1] = 2; block[1] = 1;
	map[4] = 3;
	map[7] = 3; block[4] = 1;
	map[10] = 2;
	map[13] = 1; block[10] = 1;
	map[16] = 1;
	

	//GoPoint(2);
	//StrikePuck(0); --> 퍽을 치고 반대쪽으로 가서 먹는거까지 보장  
	//display(map,mapGoal);
	

	memcpy(node,map,sizeof(node));
	

	flg = 0;
	sort_min = 10;
	SortDFS(0,0,node);
	
	printf("flg : %d\n",flg);
	
	display(map,mapGoal);
	
	for(int i = 0 ; i < sort_min ; i++){
		printf("%d %d %d type : %d\n",g_sort_route[i][0],g_sort_route[i][1],g_sort_route[i][2],type[i]);
		CheckDFS(g_sort_route[i][1],g_sort_route[i][2],map);
		
		for(int j = 0 ; j <= min ; j++){
			printf("%d->",g_route[j]);
		}
		printf("\n\n");
		if(type[i]) printf("STRIKE \n");
		swap(map[g_sort_route[i][1]],map[g_sort_route[i][2]]);
		display(map,mapGoal);
	}
	
	printf("\ncnt : %d",call);
	printf("\n%d",sort_min);
	
}
