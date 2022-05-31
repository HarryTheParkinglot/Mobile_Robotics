#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b,node) tmp = node[a], node[a] = node[b],node[b] = tmp;

int g_route[20],route[20],min,check[20] = {0},seet_check[12] = {0},visit[20] = {0},detectPuck = 0,detectSeet = 0,block_detect = 0;
int map[20];
int input[20] = {
	0,0,0,0,
	0,0,0,0,
	4,0,0,0,
	2,0,3,0,
	0,0,0,0
};

int seet_map[12][2];
int seetInput[12][2];


int link[20][4];
int points[6];
int seet_points[4]; //dir dist 

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

int GetVerticalPuckCount(int p, int d){
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
int GetVerticalSeetCount(int p){
	int count = 0;
	
	if(p / 4 < 2) return 0;
	// 0 1 
	// 2 3
	memset(seet_points,-1,sizeof(seet_points));

	if(p % 4) seet_points[2] = (p/4)*3  + p % 4 - 7; 
	if(p >= 0 && seet_points[2] / 4) seet_points[0] = seet_points[2] - 3;
	
	if(p % 4 < 3) seet_points[3] = (p/4)*3  + p % 4 - 6,count++; 
	if(p >= 0 && seet_points[3] / 4) seet_points[1] = seet_points[3] - 3; 

	for(int i = 0; i < 4; i++){
		if(seet_check[seet_points[i]]) seet_points[i] = -1;
		if(seet_points[i] < 0) continue;
		count++;
	}
	return count;
	
} 

int GetPuckTotal(int p){
	int tot = 0;
	for(int i = 0 ; i < 4 ; i++)
		tot += GetVerticalPuckCount(p,i);
	
	return tot;
}

int NextPos(int p, int len){
	if(len < 0) return -1;
	
	for(int i = 0 ; i < 4; i++){
		int np = link[p][i];
		if(np < 0 || map[np]) continue; //퍽이 있거나 방문한경우 
		
		if(detectSeet >= 3 && !GetPuckTotal(np)) continue;
		else if(detectPuck >= 3 && !GetVerticalSeetCount(np)) continue;
		else if(!GetPuckTotal(np) && !GetVerticalSeetCount(np)) continue;
		else if(!check[np]) return np;
		
		int val = NextPos(np,len-1); 
		if(val >= 0) return val;
	}
	return -1;
}


void ApplySeetScan(int p) {
	check[p]  = 1;
	for(int i = 0 ; i < 4 ; i++){
		if(seet_points[i] < 0) continue;
		printf("SEET SCAN %d %d\n",i,seet_points[i]);
		//map[seet_points[i]] = ScanSeetByDist(i);
		seet_map[seet_points[i]][0] = seetInput[seet_points[i]][0];
		seet_map[seet_points[i]][1] = seetInput[seet_points[i]][1];
		seet_check[seet_points[i]] = 1;
		if(seet_map[seet_points[i]][0] > 0) detectSeet ++;
	}
}

void ApplyScan(int p, int count){
	check[p]  = 1;
	for(int i = 1 ; i <= count ; i++){
		printf("SCAN %d\n",points[i]);
		
		//ScanByDist(p,list[i]);
		if(!check[points[i]] && (i == 1 || block_detect || (input[points[i]] && input[points[i]] < 4 ))){ //바로 앞이거나 퍽인 경우 
			map[points[i]] = input[points[i]];
			if(map[points[i]]){
				detectPuck ++;
				if(map[points[i]] == 4) block_detect = 1;
				check[points[i]] = 1;
				if(detectPuck == 3)
					for(int j = 0 ; j < 20 ; j++)
						if(map[j] < 0)  map[j] = 0;
				
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
	system("pause"); 
	system("cls"); 
}


int main(){
	for(int i = 0 ; i < 20 ; i++)	Link(i,link[i]);
	//printf("%d\n",link[19][1]);
	memset(map,-1,sizeof(map));
	
	seetInput[1][0] = 2;
	seetInput[1][1] = 0;
	
	seetInput[5][0] = 3;
	seetInput[5][1] = 2;
	
	seetInput[7][0] = 1;
	seetInput[7][1] = 1;
	
	
	for(int i = 16 ; i < 20 ; i++)	map[i] = 0;
	for(int i = 9 ; i < 12 ; i++) {
		seet_check[i] = 1;
		seet_map[i][0] = seetInput[i][0];
		seet_map[i][1] = seetInput[i][1];
	}
	
	display();

	int nowPos = 16,nowDir = 0;
	detectPuck = detectSeet = 0;
	while(1){
		for(int j = 0 ; j < 4 ; j++){
			nowDir = j;
			
			int cnt = GetVerticalPuckCount(nowPos,j);
			int cnt2 = (j == 0)? GetVerticalSeetCount(nowPos) : 0;
			if(!cnt && !cnt2)	continue;
			printf("applyScan\n POS %d DIR %d\n\n",nowPos,nowDir);
			if(cnt) ApplyScan(nowPos,cnt);
			if(cnt2) ApplySeetScan(nowPos);
			display();
			
		}

		if(detectPuck + detectSeet == 6) break;
		
		int np;
		for(int j = 0 ; j < 10 ; j++){
			np = NextPos(nowPos,j);
			if(np >= 0) break;
		}
		printf("MOVE %d -> %d\n",nowPos,np);
		nowPos = np;
	}



}

