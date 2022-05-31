#include <stdio.h>
#include <windows.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[13] = {
	2,1,1,
	 0,0,
	3,3,1,
	 0,2,
	2,0,3
};
int node[13];
int bMap[6];

int link[13][8] = {
   {-1,-1,-1,-1,-1,3,1,-1},
   {-1,-1,0,3,-1,4,2,-1},
   {-1,-1,1,4,-1,-1,-1,-1},
   
   {-1,0,-1,5,-1,-1,-1,1},
   {-1,1,-1,-1,-1,7,-1,2},
   
   {-1,-1,-1,-1,-1,8,-1,3},
   {-1,-1,-1,-1,-1,-1,-1,-1},
   {-1,4,-1,9,-1,-1,-1,-1},
   
   {-1,5,-1,10,-1,11,-1,-1},
   {-1,-1,-1,11,-1,12,-1,7},
   
   {-1,-1,-1,-1,-1,-1,11,8},
   {-1,8,10,-1,-1,-1,12,9},
   {-1,9,11,-1,-1,-1,-1,-1}
};

int route[40]; //[0] : 회전 타입 / [1] : 회전 방향  [2] : 회전 횟수 
int g_route[40];

int seq_route[8],g_seq_route[8],seq_min;
int sort_route[40][2],g_sort_route[40][2];
int minMove = 30,flg = 0;
int fix[13] = {0};
int hist[40][13];
int curDp = 0;
int stage = 0;
int nowPos;
int call=  0;
int seq[8] = {1,4,7,9,11,8,5,3};

int InHist(int *node, int dp){
   for(int i = 0  ;i < dp ; i++){
      for(int j = 0 ;  j < 13 ; j++){
         if(node[j] != hist[i][j]) break;
         if(j == 12) return 1;
      }
   }
   return 0;
}



void dis(int *arr){
	printf("\n%3d %3d %3d\n",arr[0],arr[1],arr[2]);
	printf("  %3d %3d\n",arr[3],arr[4]);
	printf("%3d %3d %3d\n",arr[5],arr[6],arr[7]);
	printf("  %3d %3d\n",arr[8],arr[9]);
	printf("%3d %3d %3d\n",arr[10],arr[11],arr[12]);
	system("pause");
	system("cls");
}



int EdgeCheck(int *node){
   	if(node[0] > 0 && node[0] == node[2] && node[0] != node[6]){ 
      	fix[0] = fix[2] = 1;
      	return 1;
   	}
   	if(node[10] > 0 && node[10] == node[12] && node[10] != node[6]){ 
      	fix[10] = fix[12] = 1;
      	return 1;
   	}
   	return 0;
}

int Check(int *node){ //1차 정렬 확인 함수 
   	if(node[0]+node[2]+node[10]+node[12] != (6-map[6])*2) return 0;
   	if(node[0] <= 0 || node[2] <= 0 || node[10] <= 0 || node[12] <= 0) return 0;
   	if(node[0] != node[2]) return 0;
   
   	int pCnt = 0,pList[8];
   	for(int i = 0 ; i < 8 ; i++){
      	int col = node[seq[i]];
      	if((pCnt && col == pList[pCnt - 1]) || pCnt > 4) return 0;
      	if(col > 0) pList[pCnt++] = col;
   	}
   
   if(pList[pCnt-1] == pList[0]) return 0;
   
   return 1;
}

void Sort(int lp, int p,int dp, int *node){
	call ++;
	if(flg || dp >= curDp) return;
	if(InHist(node,dp)) return;
	
	route[dp] = p;
	
	memcpy(hist[dp],node,sizeof(hist[dp]));
	
	if(stage == 0 && EdgeCheck(node) || (stage == 1 && Check(node))){
		flg = 1;
		memcpy(g_route,route,sizeof(route));
		curDp= dp;
		return;
	}
	
	
	for(int i = 0 ; i < 8 ; i++){
		int np = link[p][i];
		if(np < 0 || np == lp) continue;
		if(fix[np]) continue;
		
		
		swap(node[p],node[np]);
		Sort(p,np,dp+1,node);
		swap(node[p],node[np]);
	}
}


int MakeSeqRoute(int from, int to, int *node){
	seq_min = 8;
	
	seq_route[0] = from;
	
 	for(int i = 1 ; i < 8 ; i++){
		seq_route[i] = (from + i) % 8; //Real Idx Type 
		
		if(seq_route[i] != to && node[seq[seq_route[i]]]) break;
		if(seq_route[i] == to) {
			seq_min = i;
			memcpy(g_seq_route,seq_route,sizeof(seq_route));
		}
	}
	
	for(int i = 1 ; i < 8 ; i++){
		seq_route[i] = (from < i)? from - i + 8 : from - i;
		
		if(seq_route[i] != to && node[seq[seq_route[i]]]) break;
		if(seq_route[i] == to) {
			seq_min = i;
			memcpy(g_seq_route,seq_route,sizeof(seq_route));
		}
	}
	return (seq_min == 8)? -1 : seq_min;
}

int SeqCheck(int *node){
	if(!node[1] || !node[5] || !node[7] || !node[11]) return 0;
	if(node[1] != node[0] || node[11] != node[10] || node[5] != node[7])  return 0;
	return 1;
}
/*
//Seq 인덱스로 연산 
void SeqSortDFS(int lp, int p, int dp, int *node){
	if(dp >= curDp || flg) return;
	
	if(InHist(node,dp)) return;
   	memcpy(hist[dp],node,sizeof(hist[dp]));
   	
	if(SeqCheck(node)){
		flg = 1;
		curDp = dp;
		memcpy(g_route,route,sizeof(route));
		return;
	}
	for(int i = 0 ; i < 8 && !flg; i++){
		if(i == p || i == lp || !node[seq[i]] || MakeSeqRoute(p,i,node) < 0) continue;	
		
		route[dp] = i;
			
		for(int j = 0 ; j < 8 ; j++){
			
			if(node[seq[j]] || MakeSeqRoute(i,j,node) < 0) continue;
			//printf("%d %d %d %d\n",i,j,MakeSeqRoute(i,j,node),dp);
			
			swap(node[seq[i]],node[seq[j]]);
			
			route[dp][1] = j;	
			int np = g_seq_route[seq_min-1];
			
			//printf("%d -> %d -> %d || %d dp : %d\n",p,i,j,np,dp);
			//dis(node);
			SeqSortDFS(j,np,dp+1,node);
			swap(node[seq[i]],node[seq[j]]);
		}
	}
}
*/

int main(){
	stage = 0;
	for(int i = 0,val = 0; i < 13 ; i++){
		if(!map[i]) map[i] = val--;
	}
	
	for(int i = 10 ; i < 12 ; i++)
		if(map[i] <= 0) nowPos = i;
		
	
	printf("%d",nowPos);
	
	for(int k = 0 ; k < 2 ; k++){
		stage = k;
		memset(hist,0,sizeof(hist));
		
		for(int i = 0 ; i < 40 ; i+=3){
			curDp = i;
			flg = 0;
			Sort(-1,nowPos,0,map);
			if(flg) break;
		}
		printf("flg : %d\n",flg);
		printf("min : %d\n",curDp);
		dis(map);
		
		for(int i = 0 ; i < curDp ; i++){
			//printf("%d -> %d",g_route[i],g_route[i+1]);
			swap(map[g_route[i]],map[g_route[i+1]]);
			dis(map);
		}
		nowPos = g_route[curDp];
	}
	
	printf("CALL : %d",call);
/*		
	for(int i = 0 ; i < 30 ; i++){
	 	curDp = i;
	 	memcpy(node,map,sizeof(map));
	 	flg = 0;
	 	
	 	SeqSortDFS(-1,GetIdx(nowPos,seq,8),0,node);
	 	if(flg)		break;	
	}
	
	for(int i = 0 ; i < curDp ; i++){
		MakeSeqRoute(GetIdx(nowPos,seq,8),g_route[i][0],map);
		
		for(int j = 0 ; j < seq_min ; j++){
			//wait
			//MovePtoP(0,seq[g_seq_route[j]],seq[g_seq_route[j+1]]);
		}
		
		MakeSeqRoute(g_route[i][0],g_route[i][1],map);
		
			
		for(int j = 0 ; j < seq_min ; j++){
			//wait
			//MovePtoP(j == seq_min -1,seq[g_seq_route[j]],seq[g_seq_route[j+1]]);
		}
	
		swap(map[seq[g_route[i][0]]],map[seq[g_route[i][1]]]);
		dis(map);
	}
*/
}
