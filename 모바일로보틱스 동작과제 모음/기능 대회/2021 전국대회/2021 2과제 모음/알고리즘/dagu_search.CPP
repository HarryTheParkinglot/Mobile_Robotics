#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b,node) tmp = node[a], node[a] = node[b],node[b] = tmp;

int g_route[20],route[20],min,check[20] = {0},seet_check[12] = {0},visit[20] = {0},detect_puck = 0,detect_seet = 0,detect_block = 0;
int map[20];
int input[20] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	2,4,3,0,
	0,0,0,0
};

int seet_map[12][2];
int seetInput[12][2];

int link[20][4];
int points[6];
int seet_points[2]; //dir dist 

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 < 4) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
}


void DFS(int p, int wp, int dp, int *node){ //길찾기 DFS 하나  
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(int) * (min + 1));
		return;
	} 
	
	visit[p] = 1;
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || visit[np] || (np != wp && node[np])) continue;
		
		DFS(np,wp,dp+1,node);
	}
	
	visit[p] = 0;
} 

int CheckDFS(int p, int np, int *node){
	memset(visit,0,sizeof(visit));
	min = 12;
	DFS(p,np,0,node);
	return min != 12;
}

int GetPuckCnt(int p, int d){
	int count = 0;
	
	memset(points,-1,sizeof(points));
	points[0] = p;
	
	for(int i = 1 ; i <= 3 ; i++){
		points[i] = link[points[i-1]][d];
		
		if(points[i] < 0) break; //퍽이 있으면 어차피 다음꺼 스캔 힘듦  
		if(!map[points[i]] || check[points[i]]) continue;
		count ++;
	}
	return count;
} 

int GetSeetCnt(int p, int d){
	int plus[4] = {-3,-1,3,1},count = 0;
	
	int std = (p/4-1)*3 + p%4;
	
	int points[4] = {std,std-1,std+2,std+3};
	int view_points[2] = {points[(d+1)%4] + plus[d],points[d] + plus[d]};
	if(link[p][(d+1)%4] < 0) view_points[0] = -1;
	if(link[p][(d+3)%4] < 0) view_points[1] = -1;
	if(link[p][d] < 0 || link[link[p][d]][d] < 0) memset(view_points,-1,sizeof(view_points));
	
	for(int i = 0 ; i < 2 ; i ++){
		seet_points[i] = view_points[i];
		if(seet_points[i] < 0 || seet_check[seet_points[i]]) seet_points[i] = -1;		
		else count ++;
	}
	//printf("%d %d\n",view_points[0],view_points[1]);
	return count;
}

int GetSeetTotal(int p){
	int tot = 0;
	for(int i = 0 ; i < 4 ; i++)
		tot += GetSeetCnt(p,i);
	return tot;
}

int GetPuckTotal(int p){
	int tot = 0;
	for(int i = 0 ; i < 4 ; i++)
		tot += GetPuckCnt(p,i);
	
	return tot;
}

int NextPos(int p, int len){
	if(len < 0) return -1;
	
	
	for(int i = 0 ; i < 4; i++){
		int np = link[p][i];
		if(np < 0 || map[np]) continue; //퍽이 있거나 방문한경우 
		
		if(!check[np] &&	(detect_seet < 3 && GetSeetTotal(np)) || (detect_puck < 3 && GetPuckTotal(np)))	return np;
		int val = NextPos(np,len-1); 
		if(val >= 0) return val;
	}
	return -1;
}


void ApplySeetScan(int p) {
	check[p]  = 1;
	for(int i = 0 ; i < 2 ; i++){
		if(seet_points[i] < 0) continue;
		printf("SEET SCAN %d %d\n",i,seet_points[i]);
		//map[seet_points[i]] = ScanSeetByDist(i);
		seet_map[seet_points[i]][0] = seetInput[seet_points[i]][0];
		seet_map[seet_points[i]][1] = seetInput[seet_points[i]][1];
		seet_check[seet_points[i]] = 1;
		if(seet_map[seet_points[i]][0] > 0) detect_seet ++;
	}
}

void ApplyScan(int p){
	check[p]  = 1;
	for(int i = 1 ; i <= 3 ; i++){
		if(points[i] < 0) continue;
		printf("SCAN %d\n",points[i]);
		
		//ScanByDist(p,list[i]);
		if(!check[points[i]] && (i == 1 || detect_block || (input[points[i]] && input[points[i]] < 4 ))){ //바로 앞이거나 퍽인 경우 
			map[points[i]] = input[points[i]];
			if(map[points[i]]){
				detect_puck ++;
				if(map[points[i]] == 4) detect_block = 1;
				check[points[i]] = 1;
				if(detect_puck == 3)
					for(int j = 0 ; j < 20 ; j++)
						if(map[j] <= 0)  map[j] = 0;
				
			}
		}
	}
}

void display(){
	for(int i = 0 ; i < 20 ; i++){
		printf("%-3d",map[i]);
		if(i % 4 == 3) printf("\n");
	}
	
	printf("\n\n\n");
	
	for(int i = 0 ; i < 20 ; i++){
		printf("%-3d",check[i]);
		if(i % 4 == 3) printf("\n");
	}
	
	printf("\n\n\n");
	
	for(int i = 0 ; i < 12 ; i++){
		printf("%-3d",seet_map[i][0]);
		if(i % 3 == 2) printf("\n");
	}
	printf("\n\n\n");
	
	for(int i = 0 ; i < 12 ; i++){
		printf("%-3d",seet_check[i]);
		if(i % 3 == 2) printf("\n");
	}
	system("pause"); 
	system("cls"); 
}


int main(){
	for(int i = 0 ; i < 20 ; i++)	Link(i,link[i]);
	
	printf("min %d",min);
	//printf("%d\n",link[19][1]);
	memset(map,-1,sizeof(map));
	
	seetInput[5][0] = 2;
	seetInput[5][1] = 0;
	
	seetInput[6][0] = 3;
	seetInput[6][1] = 2;
	
	seetInput[7][0] = 1;
	seetInput[7][1] = 1;
	
	
memset(map,-1,sizeof(map));

map[16] = map[17] = map[18] = map[19] = 0;
int nowPos = 16,nowDir = 0;

	nowPos = 19,nowDir = 0;
	detect_puck = detect_seet = 0;

	
	display();

	
	detect_puck = detect_seet = 0;
	while(1){
		int sd = nowDir;
		for(int j = 0 ; j < 4 ; j++){
			int wd = (sd+j)%4;
			int cnt = (detect_puck < 3)? GetPuckCnt(nowPos,wd) : 0;
			int cnt2 = (detect_seet < 3)? GetSeetCnt(nowPos,wd) : 0;
			if(cnt + cnt2 == 0)	continue;
			nowDir = wd;
			printf("applyScan\n POS %d DIR %d\n\n",nowPos,wd);
			if(cnt) ApplyScan(nowPos);
			if(cnt2) ApplySeetScan(nowPos);
			display();
			
		}

		if(detect_puck + detect_seet == 6) break;
		
		int np;
		for(int j = 0 ; j < 10 ; j++){
			np = NextPos(nowPos,j);
			if(np >= 0) break;
		}
		printf("MOVE %d -> %d\n",nowPos,np);
		nowPos = np;
	}



}

