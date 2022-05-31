#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>
int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[16] = {
	0,0,0,0,
	0,0,0,0,
	4,0,0,0,
	0,3,1,2		
};

int mapGoal[8] = {
	0,0,3,2,
	0,0,0,1,
};
int main_route[4],main_g_route[4],main_min;
int cWay[4][4],cPos[4][4]; //cWay[dp / color]

int link[16][8],sub_link[8][4];
int sub_map[8] = {
	2,3,1,0,
	2,3,1,0
};
int cGate[4][4],order[4][3];
int subGoal[8];

int min,route[20],g_route[20],check[16],test[16];
int sort_min,flg,sort_route[30],g_sort_route[30];
int indexToGate[4],gateToIndex[4],block = 8;  
int pList[4][4][12];  //dp / color / np


void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 8);
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[2] = p - 1;
	if(p / 4 < 3) str[4] = p + 4;
	if(p % 4 < 3) str[6] = p + 1;
	
	
	if(str[0] >= 0 && str[2] >= 0) str[1] = p - 5;
	if(str[4] >= 0 && str[2] >= 0) str[3] = p + 3;
	if(str[4] >= 0 && str[6] >= 0) str[5] = p + 5;
	if(str[0] >= 0 && str[6] >= 0) str[7] = p - 3;
	
}

void SubLink(int p, int *str){
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 == 0) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
}

int GetWay(int p, int np){
	if(abs(p - np) == 4) return 0; //Verical 
	if(abs(p - np) == 1) return 1; //Horizontal
	return 2;	//diagonal(대각선)
}

int Sorted(int *node, int *goal){
	for(int i = 0 ; i < 8 ; i++){
		if(goal[i] < 0) continue;
		if(goal[i] != node[i]) return 0;
	}
	return 1;
}
void Sort(int lp, int p, int dp, int *node){
	if(dp >= sort_min || flg) return;
	
	sort_route[dp] = p;
	
	if(Sorted(node,subGoal)){
		sort_min = dp;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		flg = 1;
		return;
	}
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || lp == np) continue;
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);	
		swap(node[p],node[np]);
	}
}

void DFS(int way, int rock, int p, int wp, int dp, int *node){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(way == 0 && (i != 0 && i != 4)) continue;
		if(way == 1 && (i != 2 && i != 6)) continue;
		if(way == 2 && i%2 == 0) continue;
		if(way == 3 && np != wp  && node[np]) continue;
		if(np < 0 || check[np] || node[np] == 4) continue; 
		if(rock && node[np]) continue;
		
		DFS(way,rock,np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int way, int rock, int p, int wp, int *node){
	min = 12;
	memset(check,0,sizeof(check));
	DFS(way,rock,p,wp,0,node);
	return min != 12;
}


int GetNextCount(int way, int cPos, int *node, int *list){
	int cnt = 0;

	for(int i = 0 ; i < 16 ; i++){
		if(way == 0 && (i % 4 != cPos % 4)) continue;
		if(way == 1 && (i / 4 != cPos / 4)) continue;
		if(way == 2 && ((abs(i/4 + i % 4) != abs(cPos / 4 + cPos % 4)) && (abs(i / 4 - i % 4) != abs(cPos / 4 - cPos % 4)))) continue;
		if(i == cPos || node[i] || !CheckDFS(way,0,cPos,i,node)) continue;
		
		list[cnt++] = i;
	}
	
	return cnt;
}



void display(int *node){
	for(int i = 0; i < 16 ; i++){
		printf("%d ",node[i]);
		if(i % 4 == 3) printf("\n");
	}
	system("pause");
	system("cls");
}

int IsUsedWay(int color, int way, int dp){
	for(int i = 1 ; i < color ; i++)
		if(cWay[dp][i] == way) return 1;
	
	return 0;
}
int IsUsedGate(int color, int gate, int dp){
	for(int i = 1 ; i < color ; i++)
		if(cGate[dp][i] == gate) return 1;
	
	return 0;
}
int MakeOrder(int dp, int *list, int *node){ //이동 가능하게 만드는지 검사 
	int o[3]; //order
	
	memcpy(test,node,sizeof(test));
	
	for(int k = 3; k >= 1 ; k--){
		swap(test[cPos[dp][k]],test[cPos[dp+1][k]]);
	}
	
	//display(test);
	for(o[0] = 1 ; o[0] <= 3 ; o[0] ++){
		for(o[1] = 1 ; o[1] <= 3 ; o[1] ++){
			if(o[0] == o[1]) continue;
			for(o[2] = 1 ; o[2] <= 3 ; o[2] ++){
				if(o[2] == o[1] || o[2] == o[0]) continue;
				
				//display(test);
				int moved[3] = {0};
				
				for(int k = 0; k < 3 ; k++){
					if(!CheckDFS(cWay[dp][o[k]],1,cPos[dp][o[k]],cPos[dp+1][o[k]],test)) break;
					if(cPos[dp+1][o[k]] < 0) continue;
					moved[k] = 1;
					swap(test[cPos[dp][o[k]]],test[cPos[dp+1][o[k]]]);
					
					if(k == 2) {
						memcpy(list,o,sizeof(o));
						return 1;
					}	
				}
				
				for(int k = 0; k < 3 ; k++)	{
					if(!moved[k]) continue;
					swap(test[cPos[dp][o[k]]],test[cPos[dp+1][o[k]]]);
				}
			}
		}
	}
	return 0;
}
void MainSortDFS(int color, int dp, int *node){ //깊이별 색 위치 /  
	if(dp > main_min || flg) return;		
	
	if(!color && !MakeOrder(dp,order[dp],node)) return;
	
	if(!color && Sorted(node,mapGoal)){
		main_min = dp + 1;
			
		flg = 1;
		return;
	}
	
	if(dp == main_min) return;
	if(color == 0)	MainSortDFS(1,dp + 1,node);
	
	else{
		int p = cPos[dp][color],cnt = 1;
		
		for(int i = 0; i < 3 && !flg; i++){
			
			
			if(IsUsedWay(color,i,dp)) continue;
			if(p < 8 && mapGoal[p] == color){ 
				if(i) continue;
				pList[dp][color][0] = -1;
				cWay[dp][color] = -1;
			}
			else cnt = GetNextCount(i,p,node,pList[dp][color]);
			
			if(dp && cWay[dp-1][color] == i) continue;  
			
			cWay[dp][color] = i; //색깔 방향 
		
			for(int j = 0 ; j < cnt && !flg; j++){	
				int np = cPos[dp+1][color] = pList[dp][color][j];
				if(node[np]) continue; //이미 채워져있으면 버림 
				
				cGate[dp][color] = -1;
				if(i != 1 && np >= 0 && p / 4 >= 2 && np / 4 <= 2){
					
					if(i == 0) cGate[dp][color] = indexToGate[p % 4]; //사용하는 게이트  
					
					else if(i == 2) {
						if(p % 4 + p / 4 == np % 4 + np / 4){
							for(int k = 0 ; k < 4 ; k++)
								if(2 + k == p / 4 + p % 4) 
									cGate[dp][color] = indexToGate[k]; //사용하는 게이트  
						} 
						else{
							for(int k = 0 ; k < 4 ; k++)
								if(abs(2 - k) == abs(p / 4 - p % 4)) 
									cGate[dp][color] = indexToGate[k]; //사용하는 게이트
						}
					}

					if(IsUsedGate(color,cGate[dp][color],dp))  continue;
				}
				
				if(pList[j] >= 0){
					swap(node[p],node[np]);
				}
				//display(map);
				MainSortDFS((color+1)%4,dp,node);
				
				if(pList[j] >= 0){
					swap(node[p],node[np]);
				}
			}
		}
	}
}

int main(){
	clock_t start = clock(); // 시작 시간 저장
	for(int i = 0 ; i < 16 ; i++) {
		Link(i,link[i]);
		cPos[0][map[i]] = i;
	}
	for(int i = 0 ; i < 8 ; i++) SubLink(i,sub_link[i]);
	
	for(int i = 8,cnt = 0 ; i < 12 ; i++){
		if(map[i] == 4) {
			block = i;
			indexToGate[i%4] = -1;
			continue;
		}	
		indexToGate[i%4] = cnt;
		gateToIndex[cnt++] = i;
	}
	
	for(int i = 1; i <= 4 ; i++){
		main_min = i;
		MainSortDFS(1,0,map);
		if(flg) break;
	}
	
	double cal = (double)(clock() - start)/CLOCKS_PER_SEC;
	printf("time %lf\n",cal);
	printf("flg : %d\n",flg);
	for(int i = 0 ; i < main_min ; i++){
		for(int j = 0 ; j < 3 ; j++){
			int color = order[i][j];
			printf("c:%d pos : %d -> nPos : %d | way : %d gate : %d\n",color,cPos[i][color],cPos[i+1][color],cWay[i][color],cGate[i][color]);
		}
		
	}
	
} 


