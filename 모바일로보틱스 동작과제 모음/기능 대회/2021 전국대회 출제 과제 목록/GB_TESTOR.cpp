#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <windows.h>
int tmp;
#define swap(a,b)  tmp = a,a = b,b = tmp;
#define M 50
int map[8] = {
	0,1,2,1,2,0,3,3
};
int mapGoal[8] = {
	2,3,1,0,2,0,1,3
};
int set[8],tot = 0;
int flg = 0, min = 0, sort_min = 0;
int route[10],g_route[10],check[8];
int sort_route[M],g_sort_route[M];
int stack = 0,nullCnt = 0,null[2];
int link[8][4],nowPos = -1,last = -1,call = 0;
int hist[M][8],stage = 0,count = 0;
clock_t start,end;
int GetDist(int p, int *node){
	int dist = {0},mark[8] = {0};
	
	for(int i = 0 ; i < 8 ; i++)
		mark[i] = (node[i] == mapGoal[i]);
		
	for(int i = 0 ; i < 8 ; i++){ //골대  
		if(mark[i] || i == p || node[i] == mapGoal[i]) continue;
		
		int min_dist = 100,min_pos = -1;
		
		for(int j = 0 ; j < 8 ; j++){
			if(mark[j] || node[j] != mapGoal[i]) continue;
			
			if(abs(i/2 - j/2) + abs(i%2 - j%2) < min_dist) {
				//printf("%d\n",i);
				
				min_pos = j;
				min_dist = abs(i/2 - j/2) + abs(i%2 - j%2);
			}
		}
		
		mark[min_pos] = 1; //이미 선점함 
		dist += min_dist;
	}
	return dist;
}

int InHist(int dp, int *node){
	for(int i = 0 ; i < dp ; i++){
		for(int j = 0 ; j < 8 ; j++){
			if(node[j] != hist[i][j]) break;
			if(j == 7) return 1;
		}
	}
	return 0;
}

int Sorted(int *node){
	for(int i = 0 ; i < 8 ; i++)
		if(node[i] != mapGoal[i]) return 0;
	return 1;
}

void Sort(int lp, int p, int dp, int *node){
	call++;
	count++;
	if(dp >= sort_min || flg) return;
	sort_route[dp] = p;
	
	if(stage && call > 100000) {
		flg = 2;
		return;
	}
	
	if(Sorted(node) || (stage == 0 && GetDist(p,map) <= stack)){
		flg = 1;
		memcpy(g_sort_route,sort_route,sizeof(sort_route));
		sort_min = dp;
		return;
	}
	for(int i = 0 ; i< 4 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}

int Link(int p, int *str){
	memset(str,-1,sizeof(int) * 4);
	if(p / 2) str[0] = p-2;
	if(p % 2) str[1] = p-1;
	if(p / 2 < 3) str[2] = p+2;
	if(p % 2 == 0) str[3] = p+1;
}

void Sorting(){
	count = 0;
	nullCnt = 0;
	int chk = 0;
	nowPos = -1;
	last = -1;
	start = clock();
		memcpy(map,set,sizeof(map));
		/*for(int i = 0 ; i < 8 ; i++)
				printf("%d",set[i]);
		system("pause");*/
	
	
	nullCnt = 0;
	
	
	for(int i = 0,cnt = 0 ; i < 8 ; i++){
		Link(i,link[i]);
		if(!map[i]) null[nullCnt++] = i;
	}
	
	if(abs(null[0]%2-null[1]%2) + abs(null[0]/2-null[1]/2) <= 2) nullCnt = 1;
	stack = GetDist(null[0],map);

		
		for(stage = 1 ; stage <= 1 ; stage++){
			rt:
			if(chk > 20) goto brea;
			flg = 0;
			call =  0;
			
			if(stack >= 3) stage = 0;
			chk++;	
			for(int i = 0 ; i < M; i +=2){
				sort_min = i;
				for(int j = 0 ; j < nullCnt ; j++){
					
					
					nowPos = (nowPos < 0)? null[j] : nowPos;
					Sort(last,nowPos,0,map); 			
					
					if(flg == 2) {stage = 0; goto rt;}
					if(flg == 1) goto exit;
				}
			}
	
			exit:
			
			
			for(int i = 0 ; i < sort_min && flg == 1; i++){
				swap(map[g_sort_route[i]],map[g_sort_route[i+1]]);
			}
			last = g_sort_route[sort_min-1];
			nowPos = g_sort_route[sort_min];
			
			if(!stage) stack -= (GetDist(nowPos,map) > 0);
			
			//printf("stack %d\n",stack);
			//printf("FLG %d",flg);
			//printf("call : %d min %d\n",call,sort_min);
			
			/*for(int i = 0 ; i < 8 ; i++){
				printf("%-3d",map[i]);
				if(i % 2) printf("\n");
			}*/
		}
		//printf("TT%d",total);
		end = clock();
		
		if(!Sorted(map) || (end-start)/1000.0 > 0.06 || count >= 300000) {
			if(0){
				brea:
				printf("NEED FIX ");
				for(int i = 0 ; i < 8 ; i++)
					printf("%d,",set[i]);
				printf("\n");
				
				for(int i = 0 ; i < 8 ; i++)
					printf("%d,",mapGoal[i]);
				printf("\n");
				system("Pause");
				//printf("%d%d%d%d\n",val[0],val[1],val[2],val[3]);
			}
			
			for(int i = 0 ; i < 8 ; i++)
				printf("%d,",set[i]);
			printf("\n");
			
			for(int i = 0 ; i < 8 ; i++)
				printf("%d,",mapGoal[i]);
			printf("\n");
			
			printf("time : %lf\n",(end-start)/1000.0);
			printf("call : %d\n\n",count);
			//system("pause");
		}
}
int val[4] = {2,2,2,2};
int val2[4] = {2,2,2,2};
void CaseMaker(int dp){
	if(dp == 16){	
		int total = 0;
		for(int i = 0 ; i < 8 ; i++)
			total += map[i] + mapGoal[i];
		if(total != 24) return;
		tot++;
		Sorting();
		/*for(int i = 0 ; i < 8 ; i++)
				printf("%d",map[i]);
			printf("\n");
			
			for(int i = 0 ; i < 8 ; i++)
				printf("%d",mapGoal[i]);
			printf("\n");
		*/
		
		if(tot % 10000 == 0){
			printf("CHECK %d\n",tot);
		}
		return;
	}
	
	for(int i = 0 ; i < 4 ; i++){
		if(val[i] > 0 && dp / 8 == 0) set[dp%8] = i;
		else if(val2[i] > 0 && dp / 8 == 1) mapGoal[dp%8] = i;
		else continue;
		
		if(dp/8) val2[i]--;
		else val[i]--;
		
		CaseMaker(dp+1);
		
		if(dp/8) val2[i]++;
		else val[i]++;
	}
}

int main(){
	CaseMaker(0);
	printf("FINISH");
}
