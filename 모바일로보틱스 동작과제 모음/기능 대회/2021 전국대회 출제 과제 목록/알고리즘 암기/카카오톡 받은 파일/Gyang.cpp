#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[16] = {
	0,0,0,0,
	0,0,0,0,
	0,0,4,0,
	2,3,0,1		
};

int mapGoal[16] = {
	0,2,0,0,
	3,0,1,0,
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

int min,route[20],g_route[20],check[16];
int sort_min,flg,sort_route[30],g_sort_route[30];
int indexToGate[4],gateToIndex[4],block = 9;  

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

void DFS(int way, int p, int wp, int dp, int *node){
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
		
		DFS(way,np,wp,dp+1,node);
	}
	
	check[p] = 0;
}

int CheckDFS(int way, int p, int wp, int *node){
	min = 12;
	memset(check,0,sizeof(check));
	DFS(way,p,wp,0,node);
	return min != 12;
}


int GetNextCount(int way, int cPos, int *node, int *list){
	int cnt = 0;

	for(int i = 0 ; i < 16 ; i++){
		if(way == 0 && (i % 4 != cPos % 4)) continue;
		if(way == 1 && (i / 4 != cPos / 4)) continue;
		if(way == 2 && ((abs(i/4 + i % 4) != abs(cPos / 4 + cPos % 4)) && (abs(i / 4 - i % 4) != abs(cPos / 4 - cPos % 4)))) continue;
		if(i == cPos || node[i] || !CheckDFS(way,cPos,i,node)) continue;
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

void MainSortDFS(int color, int dp, int *node){ //깊이별 색 위치 /  
	if(dp >= main_min || flg) return;
	
	if(!color && Sorted(node,mapGoal)){
		display(node);
		main_min = dp + 1;
		flg = 1;
		return;
	}

	if(dp == main_min || (dp && !MakeOrder(dp,order[dp],node))) return;
	
	if(color == 0)	MainSortDFS(1,dp + 1,node);
	
	else{
		
		for(int i = 0; i < 3 && !flg; i++){
			int pList[10];
			if(IsUsedWay(color,i,dp)) continue;
			if(dp && cWay[dp-1][color] == i) continue;
			
			cWay[dp][color] = i; //색깔 방향 
			
			int cnt = GetNextCount(i,cPos[dp][color],node,pList);
		
			for(int j = 0 ; j < cnt && !flg; j++){
				
				if(node[pList[j]]) continue; //이미 채워져있으면 버림 
				
				if(mapGoal[cPos[dp][color]] == color) 
					pList[j] = cPos[dp][color];
				//printf("color : %d np : %d dp : %d way : %d\n",color,pList[j],dp,cWay[dp][color]);
					
				cPos[dp+1][color] = pList[j];
				if(cPos[dp][color] / 4 >= 2 && cPos[dp+1][color] / 4 <= 2){
					
					if(i == 0) cGate[dp][color] = indexToGate[cPos[dp][color] % 4]; //사용하는 게이트  
					if(i == 1) continue; //횡으로 2에서 이동은 절대 안됨 
					if(i == 2) {
						if(cPos[dp][color] % 4 + cPos[dp][color] / 4 == cPos[dp+1][color] % 4 + cPos[dp+1][color] / 4){
							for(int k = 0 ; k < 4 ; k++)
								if(2 + k == cPos[dp][color] / 4 + cPos[dp][color] % 4) 
									cGate[dp][color] = indexToGate[k]; //사용하는 게이트  
						} 
						else{
							for(int k = 0 ; k < 4 ; k++)
								if(abs(2 - k) == abs(cPos[dp][color] / 4 - cPos[dp][color] % 4)) 
									cGate[dp][color] = indexToGate[k]; //사용하는 게이트
						}
					}
					if(IsUsedGate(color,cGate[dp][color],dp))  continue;
				}
				else cGate[dp][color] = -1;
				
				swap(node[cPos[dp][color]],node[pList[j]]);
				//display(map);
				
				MainSortDFS((color+1)%4,dp,node);
					
				swap(node[cPos[dp][color]],node[pList[j]]);
				
			}
		}
	}
}

int MakeOrder(int dp, int *list, int *node){ //이동 가능하게 만드는지 검사 
	int o[3],flg=0,test[20]; //order
	memcpy(test,node,sizeof(test));
	
	for(o[0] = 1 ; o[0] <= 3 ; o[0] ++){
		for(o[1] = 1 ; o[1] <= 3 ; o[1] ++){
			for(o[2] = 1 ; o[2] <= 3 ; o[2] ++){
				for(int k = 0 ; k < 3 ; k++){
					if(!CheckDFS(cWay[dp][o[k]],cPos[dp][o[k]],cPos[dp+1][o[k]],test)) continue;
					swap(test[cPos[dp][o[k]]],test[cPos[dp+1][o[k]]]);
					if(k == 2) {
						flg=1;
						memcpy(list,o,sizeof(o));
					}
				}
			}
		}
	}
	return flg;
}

int main(){
	for(int i = 0 ; i < 16 ; i++) {
		Link(i,link[i]);
		cPos[0][map[i]] = i;
	}
	for(int i = 0 ; i < 8 ; i++) SubLink(i,sub_link[i]);
	
	for(int i = 8,cnt = 0 ; i < 12 ; i++){
		if(i == block) {
			indexToGate[i%4] = -1;
			continue;
		}	
		indexToGate[i%4] = cnt;
		gateToIndex[cnt++] = i;
	}
	
	
	for(int i = 0; i <= 3 ; i++){
		main_min = i;
		MainSortDFS(1,0,map);
		if(flg) break;
	}
	printf("%d\n",main_min);
	for(int i = 0 ; i <= main_min ; i++){
		int color = order[i][j];
		for(int j = 0 ; j < 3 ; j++)	
			printf("c : %d %d\n",color,cPos[i][color],cWay[i][color],cGate[i][color]);
		
	}
	
} 


