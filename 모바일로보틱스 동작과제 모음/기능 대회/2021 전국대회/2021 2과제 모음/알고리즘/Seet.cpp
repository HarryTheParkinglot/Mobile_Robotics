#include <stdio.h>
#include <memory.h>

int link[20][4];
//int points[6];
int seet_points[4]; //dir dist 

void Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	
	if(p / 4) str[0] = p - 4;
	if(p % 4) str[1] = p - 1;
	if(p / 4 < 4) str[2] = p + 4;
	if(p % 4 < 3) str[3] = p + 1;
}

int GetSeetCount(int p, int d){
	int plus[4] = {-3,-1,3,1},count = 0;
	
	int std = (p/4-1)*3 + p%4;
	
	int points[4] = {std,std-1,std+2,std+3};
	int view_points[2] = {points[(d+1)%4] + plus[d],points[d] + plus[d]};
	if(link[p][(d+1)%4] < 0) view_points[0] = -1;
	if(link[p][(d+3)%4] < 0) view_points[1] = -1;
	if(link[p][d] < 0 || link[link[p][d]][d] < 0) memset(view_points,-1,sizeof(view_points));
	
	for(int i = 0 ; i < 2 ; i ++)
		if(view_points[i] >= 0) count ++;
	
	printf("%d %d\n",view_points[0],view_points[1]);
	return count;
}

int main(){
	for(int i = 0 ;i < 20 ; i++) Link(i,link[i]);
	GetSeetCount(3,0);	
	GetSeetCount(3,1);	
	GetSeetCount(3,2);	
	GetSeetCount(3,3);	
}
