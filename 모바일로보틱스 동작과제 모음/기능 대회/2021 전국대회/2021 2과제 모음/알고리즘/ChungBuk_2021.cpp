#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

//0 1 2 3 
//4 5 6(B R Y)
int aMap[20] = {
	0,3,3,0,
	0,3,5,0,
	1,6,0,1,
	0,0,2,1,
	0,0,3,0
};



int bMap[12];

int aLink[20][8];
int bLink[12][8];

int aMapGoal[20] = {0};
int route[15],g_route[15],min,blockMin,pos,dir;
bool check[20],visit[20],match[20];

void dis(){
	for(int i = 0 ; i < 20 ; i++){
		printf("%-3d",aMap[i]);
		if(i % 4 == 3) printf("\n");
	}
	printf("\n\n");
	for(int i = 0 ; i < 20 ; i++){
		printf("%-3d",aMapGoal[i]);
		if(i % 4 == 3) printf("\n");
	}
}

void ALink(int p, int *str){
	memset(str,-1,sizeof(int) * 8);
	
	if(p/4 > 0) str[0] = p - 4;
	if(p%4 > 0) str[2] = p - 1;
	if(p/4 < 4) str[4] = p + 4;
	if(p%4 < 3) str[6] = p + 1;
}

void SetDiagonalLink(int p, int dir){
	int d = (dir == 1)? 5 : 3;
	
	for(;;){
		int np = p + 4 + dir;
		aLink[p][d] = np;
		aLink[np][(d + 4) % 8] = p; 
		
		if(dir == 1 && p % 4 >= 2) break;
		if(dir == -1 && p % 4 <= 1) break;
		p = np;
	}
}

void SetGoal(int p, int color){
	for(int i = 0 ;i < 8 ; i += 2){
		int np = aLink[p][i];
		if(np < 0) continue;
		aMapGoal[np] += color;
	}
}

void BLink(int p, int *str){
	memset(str,-1,sizeof(int) * 8);
	
	if(p/4 > 0) str[0] = p - 4;
	if(p%4 > 0) str[2] = p - 1;
	if(p/4 < 2) str[4] = p + 4;
	if(p%4 < 3) str[6] = p + 1;
}

bool Sorted(int *node){
	for(int i = 0 ; i < 20 ; i++)
		if(node[i] != aMapGoal[i]) return 0;
	return 1;
}

int GetDir(int p, int wp, int link[][4]){
	for(int i = 0 ; i < 8 ; i++)
		if(link[p][i] == wp) return i;
}


void SwapMove(int p, int wp, int *node){
	swap(node[p],node[wp]);
	dir  = (dir + 2) % 4;
	//Movement
}

void MovePtoP(int p, int np, int *node, int link[][4]){
	int nDir = GetDir(p,np,link);	
	
	//At(GetAng(dir,nDir),60);
	dir = nDir;
	
	if(node[np])
		SwapMove(p,np,node);
	else {
		int nnp = link[np][dir];
		if(nnp < 0)	{
			int prior[4] = {0,2,6,4};
			
			for(int i = 0; i < 4;  i++) {
				nDir = (dir + prior[i])%8;
				
				if(link[np][nDir] < 0) continue;
				dir = nDir;
				break;
			}
		}
		//else JustMove // 그냥 한칸 이동 
	}
	pos = np;
}	


void DFS(int p, int wp, int dp, int blockTime, int *node, int link[][8]){
	if(blockTime > blockMin || (blockTime == blockMin && dp >= min)) return;
	
	route[dp] = p;
	
	if(p == wp){
		memcpy(g_route,route,sizeof(route));
		min = dp;
		blockMin = blockTime;
		return;
	}
	
	check[p] = true;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np]) continue;
		int block = (node[np] && node[np] <= 3);
		DFS(np,wp,dp+1,blockTime + block,node,link);
	}
	
	check[p] = false;
}

//void Scan(int p)


bool CheckDFS(int p, int wp, int *node, int link[][8], int *way){
	memset(check,0,sizeof(check));
	min = 12;
	blockMin = 3;
	
	DFS(p,wp,0,0,node,link);
	memcpy(way,g_route,sizeof(way));
	
	if(!blockMin) return 0; //Way Gained
	else return 1; //Need to Make Way
}
  
bool InList(int val, int *str, int len){
	for(int i = 0; i < len ; i++)
		if(val == str[i]) return 1;
	return 0;	
}

/*
int FindPutPlace(int p){
	for(int i = 0 ; i < 20 ; i++){
		int placeWay[20];
		if(aMap[p] || match[i] || !CheckDFS(p,17,aMap,aLink,placeWay) || InList(p,placeWay,min)) continue; 
		return p;
	}
	return -1;
}
*/


int main(){
	for(int i = 0 ; i < 20 ; i++)
		ALink(i,aLink[i]);
	for(int i = 0 ; i < 12 ; i++)
		BLink(i,bLink[i]);
	
	SetDiagonalLink(0,1);
	SetDiagonalLink(2,1);
	SetDiagonalLink(8,1);
	
	SetDiagonalLink(2,-1);
	SetDiagonalLink(7,-1);
	SetDiagonalLink(15,-1);
	
	
	/*
	for(int i = 0; i < 20 ; i++){
		for(int j = 0; j < 8 ; j++)
			printf("%-5d",aLink[i][j]);
		printf("\n");
	}
	*/
	
	for(int i = 0 ; i < 20 ; i++)
		if(aMap[i] >= 4)  SetGoal(i,aMap[i] - 3);
	
	for(int i = 0; i < 20 ; i++)
		if(aMapGoal[i] >= 3) aMapGoal[i] = 6 - aMapGoal[i];

	dis();			
	/*
	
	for(int i = 0 ; i < 20 ; i++){
		if(aMap[i] == aMapGoal[i]) {
			match[i]++;	continue;
		}
		
		if(node[i]){ //퍽 치워넣기 
			if(CheckDFS(pos,i,aMap,aLink,g_route){
				int wp = FindPutPlace(i); 
				int way[20];
				CheckDFS(i,wp,aMap,aLink,way);
				pos = way[min-1];
				
				swap(aMap[i],aMap[wp]);
			}
		}
		
		
		if(aMapGoal[i]) {
			for(int j = i+1 ; j < 20 ; j++){
				if(aMap[j] == aMapGoal[i] && CheckDFS(pos,j) )
			}	
		}
	}
	
	*/
	
	
	printf("VAL : %d\n",CheckDFS(0,3,aMap,aLink,g_route));
	for(int i = 0  ;i <= min ; i++)
		printf("%d ->",g_route[i]);
	
	printf("\nmin : %d block : %d",min,blockMin);
	
}

 
