#include <stdio.h>
#include <memory.h>
#include <windows.h>
int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

//0 1 2 3 
//4 5 6(B R Y)

int aMap[20] = {
	0,3,0,5,
	0,3,0,0,
	3,0,0,3,
	0,0,3,0,
	0,0,3,4
};



int bMap[12] = {
	0,0,0,0,
	1,1,2,2,
	1,1,0,1
};
int node[12];

int aLink[20][8] = {0};
int bLink[12][8] = {0};

int aMapGoal[20] = {0};
int route[15],g_route[15],min,pos,dir;
int sort_min,sort_route[30],g_sort_route[30]; 
bool check[20],visit[20],mark[20],flg = 0;
int needPerColor[4] = {0};

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
	printf("\n\n");
	for(int i = 0 ; i < 12 ; i++){
		printf("%-3d",bMap[i]);
		if(i % 4 == 3) printf("\n");
	}
	printf("\n\n");
	
	system("pause");
	system("cls");
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
		int np = p + d;
		if(p / 4 >= 4) break;
		if(dir == 1 && (p % 4 >= 3) ) break;
		if(dir == -1 && p % 4 <= 0) break;
		
		aLink[p][d] = np;
		aLink[np][(d + 4) % 8] = p; 
		
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

int GetDir(int p, int wp, int link[][8]){
	for(int i = 0 ; i < 8 ; i++)
		if(link[p][i] == wp) return i;
}


void SwapMove(int p, int wp, int *node){
	swap(node[p],node[wp]);
	dir  = (dir + 2) % 4;
	//Movement
}

void MovePtoP(int p, int np, int *node, int link[][8]){
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


void DFS(int p, int wp, int dp, int *node, int link[][8]){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		memcpy(g_route,route,sizeof(route));
		min = dp;
		return;
	}
	
	check[p] = true;
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || check[np] || (np != wp && (node[np] && node[np] <= 3))) continue;
		DFS(np,wp,dp+1,node,link);
	}
	
	check[p] = false;
}

//void Scan(int p)


bool CheckDFS(int p, int wp, int *node, int link[][8]){
	memset(check,0,sizeof(check));
	min = 12;
	
	DFS(p,wp,0,node,link);
	return min != 12;
}
  
bool InList(int val, int *str, int len){
	for(int i = 0; i < len ; i++)
		if(val == str[i]) return 1;
	return 0;	
}


int FindPutPlace(int gate, int p, int *node, int link[][8], int len){
	for(int i = len-1  ;i >= 0 ; i--){
		if(i == gate || node[i] || !CheckDFS(p,i,node,link)) continue;
		return i;
	}
	return -1;
}


int GetColorCnt(int color, int *node){
	int cnt = 0;
	for(int i = 0 ;i < 20 ; i++)
		cnt += (node[i] == color);
	
	return cnt;
}


void dis2(int *node){
	for(int i = 0 ; i < 12 ; i++){
		printf("%-3d",node[i]);
		if(i % 4 == 3)printf("\n");
	}
	printf("\n\n");
	
	system("pause");
	system("cls");
}
void GoPos(int from, int to, int type, int *node, int link[][8]){ 
	//type 0 : GetPuck
	//type 1 : PutPuck 
	CheckDFS(from,to,node,link);	
	
	
	if(type == 1){
		swap(node[from],node[to]);
		pos = g_route[min-1];
	}
	else pos = to;
}

void CurrentSort(int *node){
	for(int i = 0 ; i < 20 ; i++){
		if(mark[i] != 1 || node[i] == aMapGoal[i] || !node[i]) continue;
		for(int j = 0 ; j < 20 ; j++){
			if(aMapGoal[j] != node[i] || node[j] || !CheckDFS(pos,i,aMap,aLink) || !CheckDFS(i,j,aMap,aLink)) continue;			
			printf("\n %d -> %d - > %d\n",pos,i,j);
			GoPos(pos,i,0,aMap,aLink);
			GoPos(i,j,1,aMap,aLink);
			dis();
			break;
		}	
	}
}

int CanGetWantColor(int priorColor, int cnt, int *colors, int *node){
	int col[4] = {0},val = 0,can = 0;

	for(int i = 0 ; i < 12 ; i++){
		if(!node[i] || col[node[i]] == colors[node[i]] ||  !CheckDFS(1,i,node,bLink)) continue;
		if(node[i] == priorColor) can = 1;
		val++;
		col[node[i]]++;
	}
	return (val >= cnt && can);
}

void Sort(int cnt, int priorColor, int *colors, int lp, int p, int dp, int *node){
	if(flg || dp >= sort_min) return;
	//dis2(node);
	
	sort_route[dp] = p;
	
	if(CanGetWantColor(priorColor,cnt,colors,node) && CheckDFS(p,1,node,bLink) && !node[1]){
		sort_min = dp;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		flg = 1;
		return;
	}
		
	for(int i = 0 ; i < 8 && !flg; i++){
		
		int np = bLink[p][i];
		if(np < 0 || np == lp) continue;
		//printf("np : %d\n",np);
		swap(node[p],node[np]);
		Sort(cnt,priorColor,colors,p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}

bool MakeSort(int cnt, int priorColor, int *colors,int *node){
	for(int i = 0 ; i < 30 ; i++){
		flg = 0;
		sort_min = i;
		memcpy(node,bMap,sizeof(node));
		Sort(cnt,priorColor,colors,-1,1,0,node);
		if(flg) return 1;
	}
}

int GetAble(int p, int color, int *node){
	for(int i = 0 ; i < 12 ; i++)
		if(node[i] == color && CheckDFS(p,i,node,bLink))
			return i;
	
}


int main(){
	for(int i = 0 ; i < 20 ; i++)
		ALink(i,aLink[i]);
	for(int i = 0 ; i < 12 ; i++){
		BLink(i,bLink[i]);
		//printf("%d %d %d %d\n",bLink[i][0],bLink[i][1],bLink[i][2],bLink[i][3]);
	}
	
	SetDiagonalLink(0,1);
	SetDiagonalLink(2,1);
	SetDiagonalLink(8,1);
	
	SetDiagonalLink(2,-1);
	SetDiagonalLink(7,-1);
	SetDiagonalLink(15,-1);
	for(int i = 0 ; i < 20 ; i++)
		if(aMap[i] >= 4)  {
			SetGoal(i,aMap[i] - 3);
		}
	
	for(int i = 0; i < 20 ; i++){
		if(aMapGoal[i] >= 3) aMapGoal[i] = 6 - aMapGoal[i]; 
		if(aMapGoal[i]) needPerColor[aMapGoal[i]]++; //색깔당 필요한 개수 할당 
	}
	//Filling

	
	printf("PULLDOWN\n");
	for(int i = 8 ; i < 12 ; i++)
		if(!bMap[i] && bMap[i-4]){
			GoPos(pos,i-4,0,bMap,bLink);
			GoPos(i-4,i,1,bMap,bLink);
			pos = i-4;
			dis();
		}
	
	
	//GoPos(pos,1);
	//GoA();

	int colors[4] = {0},cnt = 0,cnt2 = 0,laterClenaPos[2] = {0},needFill = 0;
	
	
	for(int i = 0  ; i < 20 ; i++){
		if(aMap[i] && aMap[i] <= 3 && needPerColor[aMap[i]]){
			colors[aMap[i]]++;
			mark[i] = 1;
			cnt++;
			printf("makr : %d\n",i);
		}
		if(cnt == 2) break;
	}
	
	int later[2];
	
	if(cnt < 2){
		for(int i = 0 ; i < 20 ; i++){
			if(aMap[i] && aMap[i] <= 3  && !mark[i]) {
				mark[i] = 2; //임시 Stack  
				later[cnt2++] = i;
				printf("sc makr : %d\n",i);
			}
			if((cnt + cnt2) == 2) break;
		}
	}
	
	pos = 17;

	printf("SWEEP\n");
	
	//|| !CheckDFS(pos,i,aMap,aLink) || !CheckDFS(i,17,aMap,aLink)) 
	while(GetColorCnt(1,aMap) + GetColorCnt(2,aMap) + GetColorCnt(3,aMap) > 2){ //남은 퍽 개수가 2개가 될때까지 
		for(int i = 19 ; i >= 0 ; i--){
			if(!aMap[i] || aMap[i] > 3 || mark[i])continue;			
			GoPos(pos,i,0,aMap,aLink);
			GoPos(i,17,0,aMap,aLink);
			//GoB();
			swap(aMap[i],bMap[1]);
			GoPos(1,FindPutPlace(1,1,bMap,bLink,12),1,bMap,bLink);
			dis();
			//GoA();
			pos = 17;
		}
	}
	printf("SORT\n");
	
	if(cnt)
		CurrentSort(aMap);
	dis();
	
	printf("AFTER SWEEP\n");
	
	
	
	//MakeSort(1,1,colors,bMap);
	
	
	if(cnt2){
		for(int i = 0 ; i < cnt2 ; i++){
			GoPos(pos,later[i],0,aMap,aLink);
			GoPos(later[i],FindPutPlace(17,i,aMap,aLink,20),1,aMap,aLink);
			dis();
		}
	}
	
	
	int needColors[4] = {0};
	for(int i = 1 ; i <= 3 ; i++){
		needColors[i] = GetColorCnt(i,aMapGoal) - colors[i];
		needFill += needColors[i];
	}
	
	
	for(int i = 0 ; i < 20 ; i++){
		if(aMap[i] == aMapGoal[i] || (aMap[i]  > 3)) continue;
		
		if(aMap[i]){ //쓰레기 치우기 
			//GoA();
			printf("Garbage Sweep\n");
			GoPos(pos,i,0,aMap,aLink); 
			GoPos(i,17,0,aMap,aLink);
			//GoB
			swap(aMap[i],bMap[1]);
			pos = 1;
			GoPos(1,FindPutPlace(1,1,bMap,bLink,12),1,bMap,bLink);
			GoPos(pos,1,0,bMap,bLink);
			//GoA();
			pos = 17;
			dis();
		}
		
		if(aMapGoal[i]){
			
			GoPos(pos,17,0,aMap,aLink);
			//GoB(0);
			
			pos = 1;
			
			if(MakeSort((needFill >=3)? 2 : needFill,aMapGoal[i],needColors,bMap)){
				printf("sort_min : %d\n",sort_min);
				if(sort_min){
					printf("BSORT %d\n",needFill);
					for(int j = 0 ; j < sort_min ; j++){
					
						MovePtoP(g_sort_route[j],g_sort_route[j+1],bMap,bLink);
						dis();
					}
				}
			}
			
			int ind = GetAble(1,aMapGoal[i],bMap);
			printf("ind : %d\n",ind);
			GoPos(pos,ind,0,bMap,bLink);
			GoPos(pos,1,0,bMap,bLink);
			
			//GoA();
			pos = 17;
			GoPos(pos,i,1,aMap,aLink);
			swap(bMap[ind],aMap[i]);
			needColors[aMapGoal[i]]--;
			needFill--;
			dis();

		}
	}
	
	
	
	
}

 
