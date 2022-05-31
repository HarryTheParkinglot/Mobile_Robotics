#include <stdio.h>
#include <memory.h>
#include <windows.h>
#define M 40

int  tmp = 0;
#define swap(a,b) tmp = node[a]; node[a] = node[b]; node[b] = tmp;

int node[9],link[9][4] = {0};
int map[9] = {
	2,1,1,
	0,2,2,
	3,3,3
};
int mapGoal[9] = {
	2,1,1,
	3,2,2,
	3,0,3
};
int g_route[M + 1]  = {0};
int route[M + 1]  = {0};

int min,flg = 0;
int nul = 0;

void CreateLink(int p, int *str){
	memset(str,-1,sizeof(int)*4);
	
	if(p / 3 - 1 >= 0) str[0] = p - 3; 
	if(p % 3 - 1 >= 0) str[1] = p - 1;
	if(p / 3 + 1 < 3) str[2] = p + 3;
	if(p % 3 + 1 < 3) str[3] = p + 1;
}


int Finish(){
	for(int i = 0; i < 9 ; i++)
		if(node[i] != mapGoal[i]) 
			return 0;
	return 1;
}

void Sort(int lp, int p, int dp, int *node){
	if(flg || dp > min) return;
	
/*	printf("lp %d p %d dp %d\n",lp,p,dp);
	for(int i = 0 ; i < 9 ; i++){
		printf("%d ",node[i]);
		if(i % 3 == 2) printf("\n");
	}
	system("pause");	
	system("cls");
*/	
	route[dp] = p;
	
	if(Finish()){
		flg =1;
		min = dp;
		
		for(int i = 0 ; i <= min ; i++)
			g_route[i] = route[i];
			
		return;
	}
	for(int i = 0 ; i < 4 && !flg; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		swap(p,np);
		Sort(p,np,dp+1,node);
		swap(p,np);
	}
}


int main(){
	for(int i = 0 ; i < 9 ; i++){
		CreateLink(i,link[i]);
		if(!map[i]) nul = i;
	}	
	
	
	printf("%d\n",nul);
	for(int i = 0; i <= M ; i++){
		min = i;
		memcpy(node,map,sizeof(int) * 9);
		Sort(-1,nul,0,node);
		if(flg) break;
	}
	
	printf("%d",min);
}
