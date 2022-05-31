#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;
 
void display(int p, int d);
bool NotYet(int p, int *node, int link[][4]);

int bMap[20]  = {
	0,0,0,0,
	0,0,0,0,
	4,0,0,0,
	0,0,0,0,
	0,0,0,0
};

int bMapInput[20] = {
	0,0,0,0,
	0,0,0,0,
	4,0,0,0,
	1,0,1,0,
	0,0,0,0
};

int bMapGoal[20];

int aMap[8] = {
	1,0,0,1,
	1,1,1,1
};
int node[8];

int aMapGoal[8];
int seetMap[12][2];

int seetInput[2][12] = {
	{	0,1,0,
		0,0,0,
		0,1,0,
		1,0,0
	},{
		0,15,0,
		0,0,0,
		0,15,0,
		10,0,0
	}
};

int aLink[8][4];
int bLink[20][4];

bool check[20]={0},visit[20]={0},seetVisit[12] = {0},flg = 0;
int g_route[10],route[10],min = 10,pos,dir,nowAng,colors[3];
int sort_route[20],g_sort_route[20],sort_min = 20,sort_flg = 0,aGate,bGate;


bool Sorted(int *arr, int *goalArr, int size){
	for(int i = 0 ; i < size ; i++){
		if(arr[i] <= 0) continue;
		if(arr[i] != goalArr[i]) return 0;
	}
	return 1;
}

int SubSorted(int fc, int gate, int *node){
	if(node[gate] > 0 || node[gate+1] > 0)  return 0;
	
	for(int i = 0 ; i < 4; i++){
		if(node[i] <= 0) continue;
		if(node[i] != node[i+4]) return 0;
 	}
 	if(node[0] == fc || node[3] == fc) return 0;
}

void ALink(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 4 > 0) str[0] = p-4;
	if(p % 4 > 0) str[1] = p-1;
	if(p / 4 < 1) str[2] = p+4;
	if(p % 4 < 3) str[3] = p+1;
}

void BLink(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 4 > 0) str[0] = p-4;
	if(p % 4 > 0) str[1] = p-1;
	if(p / 4 < 4) str[2] = p+4;
	if(p % 4 < 3) str[3] = p+1;
}



void DFS(int p, int wp, int dp, int *node, int link[][4]){
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	
	check[p] = 1;
	
	for(int i = 0 ;i< 4;  i++){
		int np = link[p][i];
		if(np < 0 || check[np] || (np != wp && node[np])) continue;
		DFS(np,wp,dp+1,node,link);
	}
	
	check[p] = 0;
}

bool CheckDFS(int p, int wp, int *node, int link[][4]){
	memset(check,0,sizeof(check));
	min = 13;
	DFS(p,wp,0,node,link);
	return min != 13;
}


void SetBMapGoal(int color, int index, int type){
	//type 1(10cm) : 가로 (horizon) 
	//type 2(15cm) : 세로 (vertical) 
	//type 3(20cm) : 대각선 (diagonal) 
	
	index = (index/3) + index%3;
		
	if(type == 1){
		int value = (bMapGoal[index] || bMapGoal[index+1]) * 2;	
		bMapGoal[index + value] = bMapGoal[index + value + 1] = color;
	}
	else if(type == 2){
		int value = (bMapGoal[index] || bMapGoal[index+4] || index+4 == bGate);	
		bMapGoal[index + value] = bMapGoal[index + value + 4] = color;
	}
	else{
		if(index + 5 != bGate && !bMapGoal[index] && !bMapGoal[index + 5]) bMapGoal[index] = bMapGoal[index + 5] = color;
		else bMapGoal[index + 1] = bMapGoal[index + 4] = color;
	}
}

int GetDir(int p, int wp, int link[][4]){
	for(int i = 0 ; i < 4; i++)
		if(link[p][i] == wp) return i;
}

int GetAng(int d, int wd){
	int rd = wd - d;
	
	if(abs(rd) == 3) rd/=-3;
	//if(abs(rd) == 2 && g_psd[2] < 20) rd = -rd;	
	return rd;
}

void ScanSeet(int index, int *str){
	int val;
	str[0] = seetInput[0][index];
	
	if(seetInput[1][index] == 10) val = 1;
	if(seetInput[1][index] == 15) val = 2;
	if(seetInput[1][index] == 20) val = 3;
	str[1] = val;
}

void dis(int *node){
	for(int i = 0 ; i < 8 ; i++){
		printf("%-3d",node[i]);
		if(i % 4 == 3) printf("\n");
	}
	printf("\n\n");
	
	system("pause");
	system("cls");
}

void Sort(int lp, int p, int dp, int *node){
	if(dp >= sort_min) return;
	//printf("dp : %d\n",dp);
	//dis();
	
	sort_route[dp] = p;
	
	if(SubSorted(fc,aGate,node){
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		sort_flg = 1;
	}
	
	for(int i = 0 ; i < 4 && !sort_flg ; i++){
		int np = aLink[p][i];
		if(np < 0 || np == lp) continue;
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}

int main(){
	
	aGate =2,bGate= 16;
	int val = 0,cnt = 0,fill = 0;
	
	for(int i = 0 ; i < 20 ; i++)	
		BLink(i,bLink[i]);
	
	for(int i = 0 ; i < 8 ; i++){	
		if(!aMap[i]) aMap[i] = val--;
		ALink(i,aLink[i]);
	}	
	
	for(int i = 11 ; i >= 0 ; i--){
		//printf("%d\n",i + i/3);
		if(seetMap[i][0] <= 0) continue;
		SetBMapGoal(seetMap[i][0],i+i/3,seetMap[i][1]);
	}
	
	
	printf("pos : %d dir : %d\n",pos,dir);	

	for(int i = 0; i < 2 ; i++){
		for(int j =  0 ; j < 20 ; j++){	
			if(bMap[j] == 4 || !bMap[j] || bMap[j] == bMapGoal[j]) continue;
			for(int k = 0 ; k < 20 ; k++){
				if(!bMap[k] && bMapGoal[k] == bMap[j]){
				
					//GetPuck(j);
					//PutPuck(k);
					printf("pos %d\n",pos);
					printf("CheckDFS() %d",CheckDFS(pos,j,bMap,bLink));
					pos = g_route[min - 1];
					swap(bMap[j],bMap[k]);
					printf("from %d get %d to %d\n",pos,j,k);
					display(pos,0);
					break;
				}
			}
		}
	}
	
	cnt = 0;
	
	for(int i = 0 ; i < 12 ; i++){
		if(!seetMap[i][0])	 continue;
			
		for(int j = 0 ;j < 4 ; j++)
			if(bMap[seetLink[i][j]]) fill++;
		
		if(fill == 2) colors[2] = seetMap[i][0];
		else colors[cnt++] = seetMap[i][0];
	}

	
	for(int i = 0 ; i < 20 ; i++){
		sort_min = i;
		sort_flg = 0;
		
		memcpy(node,aMap,sizeof(aMap));
		Sort(-1,aGate,0,node);
		if(sort_flg) break;
	}
	
	printf("flg ; %d min : %d\n",sort_flg,sort_min);
	//GetInA(g_route[0]);
	for(int i = 0 ; i < sort_min ; i++){
		//if(g_route[i+1] > 0)  SwapMove(g_route[i],g_route[i+1]);
		//else JustMove(g_route[i],g_route[i+1]);
		swap(aMap[g_sort_route[i]],aMap[g_sort_route[i + 1]]);
		dis(aMap);
	}
	//GetOutA(g_route[min]);
	
	//GoPos(mode, from, to) 
	// mode 0 : GoPos
	// mode 1 : PutPuck
	
	for(int i = 0 ;i < 20 ; i++){
		if(!bMapGoal[i] || bMap[i]) continue;
		
		for(int j = 0 ; j < 8 ; j++){
			if(!aMap[j] || aMap[j] != bMapGoal[i] || !CheckDFS(aGate,j,aMap,aLink))  continue;
			//GetInA(bGate);		 
			//GoPosA(0,bGate,i); //get
			//GoPosA(0,i,bGate); //getout
			//GetOutA(bGate);
			
			//GetInB(bGate);
			//GoPosB(1,bGate,i); //put
			//GoPosB(0,i,bGate); //get
			//GetOutB(bGate);
			
			swap(aMap[j],bMap[i]);
			printf("FROM A %d To B %d\n",j,i);
			display(-1,-1);
			break;
		}
	}
}
