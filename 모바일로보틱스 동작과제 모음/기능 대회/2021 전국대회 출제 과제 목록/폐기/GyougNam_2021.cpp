#include <stdio.h>
#include <windows.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

int map[13] = {
   1,3,2,
    2,0,
   1,1,2,
    0,3,
   0,3,0
};
int node[13];

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

int route[20][3]; //[0] : 회전 타입 / [1] : 회전 방향  [2] : 회전 횟수 
int g_route[20][3];
int seq_route[8],g_seq_route[8],seq_min;
int minMove = 30,flg = 0;
int fix[4] = {0,0,0,0};
int curDp = 0;
int move_activation = 0;

int tri[4][3] = {
   {0,1,3},
   {1,2,4},
   {8,11,10},
   {9,12,11},
};

int seq[8] = {1,4,7,9,11,8,5,3};
int hist[20][13],stage = 0;

int nowPos = -1;
int dist[2],way[2][10];
int check[8] = {0};

int GetIdx(int p, int *str, int len){
   for(int i = 0 ; i < len ; i++)
      if(p == str[i]) return i;
      
   return -1;
}

int InHist(int *node, int dp){
   for(int i = 0  ;i < dp ; i++){
      for(int j = 0 ;  j < 13 ; j++){
         if(node[j] != hist[i][j]) break;
         if(j == 12) return 1;
      }
   }
   return 0;
}

int GetTriSum(int t_idx, int *node){
   return node[tri[t_idx][0]] + node[tri[t_idx][1]] + node[tri[t_idx][2]];
}

//삼각형 선택 방향으로 회전 후 좌표 return  
// 로봇의 이동방향_ r_dir -1 : 시계 / r_dir 1 : 반시계  
int RotateTri(int t_idx, int p, int dir, int r_time, int *node){ 
   	int r_way[4] = {p,};
   	//mode 0 : 동작 안함 / mode 1 : 동작함 
   	//회전 후 	
   	if(!GetTriSum(t_idx,node)) return -1;
   
   	for(int i = 1  ; i <= r_time ; i++) {
      	int p_idx = GetIdx(p,tri[t_idx],3);
      	r_way[i] = tri[t_idx][(p_idx+dir) >= 0? (p_idx+dir)% 3 : 2];
      	p = r_way[i];
   	}
   	if(r_time == 1 && !node[r_way[0]] && !node[r_way[1]]) return -1;
   	if(GetIdx(r_way[r_time],seq,8) < 0) return -1; //회전 가능 여부 검사  
   
   	for(int i = 0 ; i < r_time ; i++){
   		if(move_activation){
			//MovePtoP(int r_way[i],r_way[i+1]);   	
		}
      	swap(node[r_way[i]],node[r_way[i+1]])
   	}
   
   	return r_way[r_time];
}

int RotateSeq(int p, int dir,int r_time, int *node){ 
   	int r_way[10] = {p,};
   //회전 후 
      
   	for(int i = 1  ; i <= r_time ; i++) {
      	int p_idx = GetIdx(p,seq,8);
      	r_way[i] = seq[(p_idx+dir) >= 0? (p_idx+dir)% 8 : 7];
		p = r_way[i];
   	}
   
   	if(stage < 2 && (r_way[r_time] == 5 || r_way[r_time] == 7)) return -1;
   
   	for(int i = 0 ; i < r_time ; i++){
   		if(move_activation){
   			//MovePtoP(int r_way[i],r_way[i+1]);   	
   		}
      	swap(node[r_way[i]],node[r_way[i+1]])
   	}
   
   	return r_way[r_time];
}

int EdgeCheck(int *node){
	
   	if(node[0] && node[0] == node[2] && node[0] != node[6]){ 
      	fix[0] = fix[1] = 1;
      	return 1;
   	}
   	if(node[10] && node[10] == node[12] && node[10] != node[6]){ 
      	fix[2] = fix[3] = 1;
      	return 1;
   	}
   	return 0;
}

int Check(int *node){ //1차 정렬 확인 함수 
   	if(node[0]+node[2]+node[10]+node[12] != (6-map[6])*2) return 0;
   	if(node[0] != node[2]) return 0;
   
   	int pCnt = 0,pList[8];
   
   	for(int i = 0 ; i < 8 ; i++){
      	int col = node[seq[i]];
      	if((pCnt && col == pList[pCnt - 1]) || pCnt > 4) return 0;
      	if(col) pList[pCnt++] = col;
   	}
   
   if(pList[pCnt-1] == pList[0]) return 0;
   return 1;
}

void dis(int *node){
	printf("\n\n%d %d %d\n",node[0],node[1],node[2]);
	printf(" %d %d\n",node[3],node[4]);
	printf("%d %d %d\n",node[5],node[6],node[7]);
	printf(" %d %d\n",node[8],node[9]);
	printf("%d %d %d\n",node[10],node[11],node[12]);
	
	system("pause");
	system("cls");
}

// last 0 1 2 3 tri 4 seq
void RotateSort(int last ,int p, int dp, int move, int *node){
   	if(flg || move > minMove) return;
   
   //dis(node);
   
   	if((stage == 1 && Check(node)) || (!stage && EdgeCheck(node))){
		flg = 1;
		curDp = dp;
		minMove = move;
      	memcpy(g_route,route,sizeof(route));
   	}
   	
   	if(InHist(node,dp)) return;
   	
   	memcpy(hist[dp],node,sizeof(hist[dp]));
   //dis(node);
      
   	for(int i = 0; i < 4 && !flg ; i++){
      	if(fix[i] || last == i || GetIdx(p,tri[i],3) <  0 || !GetTriSum(i,node)) continue;
      	for(int j = 2 ; j <= 3 ; j++){ //3번돌면 똑같아짐 
         	int np = (RotateTri(i,p,(j%2)? 1 : -1,j/2+1,node));
         	if(np < 0) continue;
         
         	//printf("p %d type%d\n",p,i);
         
         	route[dp][0] = i;
         	route[dp][1] = (j%2)? 1 : -1; //방향  
         	route[dp][2] = j/2 +1; //횟수 
         
         //printf("%d %d %d\n",route[dp][0],route[dp][1],route[dp][2]);
         	RotateSort(i,np,dp+1,move + j/2+1,node);
         	RotateTri(i,np,(j%2)? -1 : 1,j/2+1,node);
	    }
   	}
   	if(last == 4 || flg) return;
   
   	for(int j = 2 ; j < 16 ; j++){
      	int np = (RotateSeq(p,(j%2)? 1 : -1,j/2+1,node));
      	if(np < 0) continue;
      
      	route[dp][0] = 4;
      	route[dp][1] = (j%2)? 1 : -1; //방향  
      	route[dp][2] = j/2 +1; //횟수 
      	RotateSort(4,np,dp+1,move + j/2+1,node);
      
      	RotateSeq(np,(j%2)? -1 : 1,j/2+1,node);
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
		
		route[dp][0] = i;
			
		for(int j = 0 ; j < 8 ; j++){
			//printf("%d %d %d %d\n",i,j,MakeSeqRoute(i,j),dp);
			if(node[seq[j]] || MakeSeqRoute(i,j,node) < 0) continue;
			
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


int main(){  
   	
   	nowPos = 10;
   	dis(map);
   	printf("stg 0\n");
   	stage = 0;
   	for(int i = 0 ; i < 40 ; i+=3){
      	minMove = i;
      	flg = 0;
      	memcpy(node,map,sizeof(map));
      	RotateSort(-1,nowPos,0,0,node);
      	if(flg) break;
   	}
   
   	move_activation = 1;
   	
   	if(flg)
   		printf("MIN : %d\n",curDp);
	for(int i = 0 ; i < curDp ; i++){
      	printf("POS : %d type : %d dir : %d time : %d\n",nowPos,g_route[i][0],g_route[i][1],g_route[i][2]);
      	if(g_route[i][0] < 4) nowPos = RotateTri(g_route[i][0],nowPos,g_route[i][1],g_route[i][2],map);
      	else nowPos = RotateSeq(nowPos,g_route[i][1],g_route[i][2],map);
      	dis(map);
   	}
   	stage = 1;
   	move_activation = 0;

   	for(int i = 0 ; i < 40 ; i+=3){
      	minMove = i;
      	flg = 0;
      	memcpy(node,map,sizeof(map));
      	RotateSort(-1,nowPos,0,0,node);
      	if(flg) break;
   	}
   
   	dis(map);
   if(flg)
   		printf("MIN : %d\n",curDp);
   	move_activation = 1;
   	for(int i = 0 ; i < curDp ; i++){
      	printf("POS : %d type : %d dir : %d time : %d\n",nowPos,g_route[i][0],g_route[i][1],g_route[i][2]);
      	if(g_route[i][0] < 4) nowPos = RotateTri(g_route[i][0],nowPos,g_route[i][1],g_route[i][2],map);
      	else nowPos = RotateSeq(nowPos,g_route[i][1],g_route[i][2],map);
      	dis(map);
   	}
   	move_activation = 0;

	printf("final %d\n",nowPos);
 	for(int i = 5 ; i < 20 ; i++){
	 	curDp = i;
	 	memcpy(node,map,sizeof(map));
	 	flg = 0;
	 	SeqSortDFS(-1,GetIdx(nowPos,seq,8),0,node);
	 	if(flg) break;
	}
	if(flg) 
		printf("***min : %d\n",curDp);
	
	for(int i = 0; i < curDp ; i++)	{
		printf("%d - > %d\n",seq[g_route[i][0]],seq[g_route[i][1]]);
		swap(map[seq[g_route[i][0]]],map[seq[g_route[i][1]]]);
	}
	dis(map);
 	
}
