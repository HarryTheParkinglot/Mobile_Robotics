#include <stdio.h>
#include <memory.h>
#include <windows.h>
int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
#define M 50

int node[8];
int map[8] = {
   1,1,
   2,0,
   3,2,
   0,3
};

int mapGoal[8] = {
   3,3,
   2,2,
   1,1,
   0,0
};

int g_route[M],route[M],min,flg = 0;
int dfs_route[8],dfs_g_route[8],dfs_min = 8,check[8] = {0};
int link[8][4]; 
long hist[3000];
char inform[3000][2]; //pos dp
int call = 0;

int detect = 0;

long ArrayToLong(int *array){
	long val = 0;
	for(int i = 0 ; i < 8 ; i++)
		val |= (map[i] << (7-i) * 2);
	
	return val;
}

int Hist(int p, int dp, long val){
	for(int i = 0 ; i < detect ; i++){
		if(val == hist[i] && inform[i][0] == p) {
			if(inform[i][1] < dp) return 1;
			else{
				inform[i][0] = dp;
				return 0;
			}
		}
	}
	
	
	if(detect < 3000){
		hist[detect] = val;
		inform[detect][0] = p;
		inform[detect++][1] = dp;
	}
	
	return 0;
}

void Link(int p, int *str){
   memset(str,-1,sizeof(int) * 4);
   
   if(p / 2  > 0) str[0] = p-2;
   if(p % 2  > 0) str[1] = p-1;
   if(p / 2  < 3) str[2] = p+2;
   if(p % 2 == 0) str[3] = p+1;
}

int Sorted(int *node){
   for(int i = 0 ; i < 8 ; i++){
      if(node[i] <= 0) continue;
      if(node[i] != mapGoal[i]) return 0;
   }
   return 1;
}

void Sort(int lp, int p, int dp ,int *node){
   call++;
   if(dp >= min || flg) return;
   if(Hist(p,dp,ArrayToLong(node))) return;
   
   route[dp] = p;

   if(Sorted(node)){
      min = dp;
      flg= 1;
      memcpy(g_route,route,sizeof(int) * (min + 1));
      return;
   }
   
   	for(int i = 0 ; i < 4 && !flg; i++){
	    int np = link[p][i];
	    if(np < 0 || np == lp) continue;
	    
	    swap(node[p],node[np]);
	    Sort(p,np,dp+1,node);
	    swap(node[p],node[np]);
   	}
}

int main(){
   	int null[2]= {0},val = 0,cnt = 0;
   
   	for(int i = 0 ; i < 8 ; i++){
      	Link(i,link[i]);
		if(!map[i]) null[cnt++] = i;
	}
   
   	for(int i = 0 ; i < M ; i++){
   		for(int j = 0 ; j < 2; j++){
			min = i;
			flg = 0;
			Sort(-1,null[j],0,map);
			if(flg) goto exit;
		}
   	}
   	exit:
   
   	printf("cal %d\n",call);
   	printf("%d %d\n",min,flg);
	printf("det %d\n",detect);	   
}


