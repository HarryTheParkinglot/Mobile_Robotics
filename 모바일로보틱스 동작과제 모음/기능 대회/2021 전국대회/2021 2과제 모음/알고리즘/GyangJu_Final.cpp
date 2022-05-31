#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a,a = b,b = tmp;
int aMap[16] = {
   0,0,0,0,
   0,0,0,0,
   4,0,0,0,
   3,2,1,0
};
int block = 8;
int bMap[8] = {
   2,3,1,0,
   0,1,2,3
};

int aMapGoal[16] = {
   0,2,0,0,
   3,0,1,0
};
int bMapGoal[8] = {0};

int dfs_route[4][10],g_dfs_route[4][10],dfs_min[4],convert_min = 10,check[16] = {0};
int g_route[15],route[15],min,flg = 0;
int aLink[16][8];
int bLink[8][4];
int wayList[4];
int minWayList[4];
int minOrder[3];
int colorPos[4] = {0};
int colorGPos[4] = {0};

void ALink(int p) {
   memset(aLink[p],-1,sizeof(int) * 8);

   if(p / 4 > 0) aLink[p][0] = p - 4;
   if(p % 4 > 0) aLink[p][2] = p - 1;
   if(p / 4 < 3) aLink[p][4] = p + 4;
   if(p % 4 < 3) aLink[p][6] = p + 1;

   if(aLink[p][0] >= 0 && aLink[p][2] >= 0) aLink[p][1] = p - 5;
   if(aLink[p][4] >= 0 && aLink[p][2] >= 0) aLink[p][3] = p + 3;
   if(aLink[p][4] >= 0 && aLink[p][6] >= 0) aLink[p][5] = p + 5;
   if(aLink[p][0] >= 0 && aLink[p][6] >= 0) aLink[p][7] = p - 3;
}

void BLink(int p) {
   memset(bLink[p],-1,sizeof(int) * 4);

   if(p / 4 > 0) bLink[p][0] = p - 4;
   if(p % 4 > 0) bLink[p][1] = p - 1;
   if(p / 4 < 1) bLink[p][2] = p + 4;
   if(p % 4 < 3) bLink[p][3] = p + 1;
}

int Sorted(int *node, int *goal, int len) {
   for(int i = 0 ; i < len ; i++) {
      if(bMapGoal[i] < 0) continue;
      if(node[i] != goal[i]) return 0;
   }
   return 1;
}

void display() {
   for(int i = 0 ; i < 16 ; i++) {
      printf("%-4d",aMap[i]);
      if(i % 4 == 3) printf("\n");
   }

   printf("\n\n");

   for(int i = 0 ; i < 16 ; i++) {
      printf("%-4d",aMapGoal[i]);
      if(i % 4 == 3) printf("\n");
   }

   printf("\n\n");

   for(int i = 0 ; i < 8 ; i++) {
      printf("%-4d",bMap[i]);
      if(i % 4 == 3) printf("\n");
   }

   printf("\n\n");

   for(int i = 0 ; i < 8 ; i++) {
      printf("%-4d",bMapGoal[i]);
      if(i % 4 == 3) printf("\n");
   }

   system("pause");
   system("cls");
}

int GetWay(int p, int np) {
   if(abs(p - np) == 4) return 0; //Vertical
   if(abs(p - np) == 1) return 1; //Horizon
   return 2; //diagonal
}

int GetColorGate(int color, int *node){
	for(int i =  0 ; i < 3;  i++)
		if(color == node[i])  return i%4;
	return -1;
}

int GetColorWay(int color, int *node){
	for(int i = 4 ; i < 8 ; i++)
		if(color == node[i]) {
			return i%4;
		}
	return -1;
}

void DFS(int color, int pastWay, int p, int wp, int dp, int wayConvert, int *node) { //색깔 보유시
   if(wayConvert > convert_min || (wayConvert == convert_min && dp >= dfs_min[color])) return;

   dfs_route[color][dp] = p;
   	
   	if(p == wp && (!color || (pastWay == wayList[color]))){
		wayList[color] = pastWay;
    	dfs_min[color] = dp;
    	convert_min = wayConvert;
    	memcpy(g_dfs_route[color],dfs_route[color],sizeof(dfs_route[color]));
      	return;
   }

   check[p] = 1;

   for(int i = 0 ; i < 8 ; i++) {
      int np = aLink[p][i];

      if(np < 0 || check[np] || node[np]) continue;
      int nowWay = GetWay(p,np);
      if(p / 4 == 2 && nowWay == 1) continue; //두번째 라인에서는 좌우이동 불가능

      int plus = color && ((nowWay != pastWay) || (np / 4 == 2 && np % 4 != GetColorGate(color,bMap))); //색깔이 다른 경우 convert필요
      DFS(color,nowWay,np,wp,dp+1,wayConvert + plus,node);
   }
   check[p] = 0;
}

bool CheckDFS(int color, int pastWay, int p, int wp, int *node) {
   dfs_min[color] = 8;
   convert_min = 5;
   memset(check,0,sizeof(check));
   DFS(color,pastWay,p,wp,0,0,node);

   return dfs_min[color] != 8;
}

void Sort(int lp, int p, int dp, int *node) {
   if(dp >= min) return;

   route[dp] = p;

   if(Sorted(node,bMapGoal,8)) {
      flg = 1;
      min = dp;

      memcpy(g_route,route,sizeof(route));
      return;
   }

   for(int i = 0; i < 4 ; i++) {
      int np = bLink[p][i];

      if(np < 0 || np == lp) continue;
      swap(node[p],node[np]);
      Sort(p,np,dp+1,node);
      swap(node[p],node[np]);
   }
}

void SetGateWay(int color, int gate, int way) {
   int null;
   memset(bMapGoal,-1,sizeof(bMapGoal));

   if(gate >= 0)	bMapGoal[gate] = color;
   bMapGoal[way + 4] = color;

   for(int i = 0 ; i < 8 ; i++)
      if(!bMap[i])   null = i;

   for(int i = 0 ; i < 20 ; i++) {
      min = i;
      flg = 0;
      
      Sort(-1,null,0,bMap);
      
      if(flg) {
         return;
      }
   }
}

void SetFinalGateWay() {
   memset(bMapGoal,-1,sizeof(bMapGoal));
	
   	for(int i = 1 ; i <= 3 ; i++)
		bMapGoal[wayList[i] + 4] = i;
   	
   	for(int i = 1 ; i <= 3 ; i++){
   		int p = g_dfs_route[i][dfs_min[i] - 1] ;
	   	
		if(p / 4 == 2){
			int gate = p%4 - (p%4 > block);
	   		bMapGoal[gate] = i;
	   	}
	}

	int cnt = 0,val = 0,null[2] = {0};
   	
	for(int i = 0 ; i < 8 ; i++){
    	if(!bMap[i])   {
			null[cnt++]  = i;
			bMap[i] = val--;
		}
    }

   	for(int i = 0 ; i < 20 ; i++) {
		min = i;
      	flg = 0;
      	
      	for(int j = 0 ; j < 2 ; j++){  
      		Sort(-1,null[j],0,bMap);
    		if(flg) return;  		
		}
   	}
}

void MakeMinWayList(){
	int min = 10,min2 = 20;
	
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			if(i == j) continue;
			for(int k = 0 ; k < 3 ; k++){
				if(k == j || k == i) continue;
				wayList[1] = i;
				wayList[2] = j;
				wayList[3] = k;
				
				int order[3];
				for(order[0] = 1 ; order[0] <= 3 ; order[0]++){
					for(order[1] = 1 ; order[1] <= 3 ; order[1]++){
						if(order[1] == order[0]) break;
						for(order[2] = 1 ; order[2] <= 3 ; order[2]++){
							if(order[1] == order[2] || order[0] == order[2]) break;
							
							int node[16],bNode[8];
							memcpy(node,aMap,sizeof(node));
							memcpy(bNode,bMap,sizeof(bNode));
							
							int cnt = 0,cnt2 = 0;
							
							for(int t = 0 ; t < 3 ; t++){
								int color = order[t];
								if(!CheckDFS(color,(t == 0)? GetColorGate(color,bNode): -1,colorPos[color],colorGPos[color],node)) break; 
								
								cnt += convert_min;
								cnt2 += dfs_min[t];
								swap(node[colorPos[color]],node[g_dfs_route[color][dfs_min[color]-1]]);
								if(t == 2){
									if(cnt < min || (cnt == min && cnt2 < min2)) {
										memcpy(minWayList,wayList,sizeof(wayList));
										memcpy(minOrder,order,sizeof(order));
									}
								}
							}			
						}
					}
				}
			}	
		}
	}
}

int CalGate(int pos, int block){
	return pos%4 - (pos % 4 > block%4);
} 


int main() {
   	int val = 0,cnt = 0;
   	for(int i = 0 ; i < 16 ; i++)  {
	   	ALink(i);
	   	if(aMapGoal[i])	colorGPos[aMapGoal[i]] = i;
	   	if(aMap[i])	colorPos[aMap[i]] = i;
	}

   	for(int i = 0 ; i < 8 ; i++){   
      	BLink(i);
      	if(!bMap[i]) bMap[i] = val--;
   	}
   
   	MakeMinWayList();
   
	display();
	for(int i = 0 ; i < 3 ; i++){
		int color = minOrder[i];
		printf("CHECK %d\n",CheckDFS(color,(color == 1)? GetColorGate(color,bMap) : -1,colorPos[color],colorGPos[color],aMap));
		
		for(int w = 0 ; w < dfs_min[color] - 1; w++) {
			
			int lp = (w)? g_dfs_route[color][w-1] : -1;
			int p = g_dfs_route[color][w];
			int np = g_dfs_route[color][w+1];
			printf("%d -> %d\n\n",p,np);
			
			int goalWay = GetWay(p,np);
			int goalGate = -1;
			//몇번째 게이트냐  
			if(p / 4 == 2) goalGate = p % 4 - (p%4 > block%4);
			if(np / 4 == 2) goalGate = np % 4 - (np%4 > block%4); 

            if((goalGate >= 0 && GetColorGate(color,bMap) != goalGate) || (goalWay != GetColorWay(color,bMap))){
               	SetGateWay(color,goalGate,goalWay);
               	printf("nw %d gw%d ng%d wg%d\n\n",GetColorWay(color,bMap),goalWay,GetColorGate(color,bMap),goalGate);
               	cnt++;   	
				if(min) {
					
	               	printf("ChangeBMap\n\n");
					for(int t = 0 ; t < min ; t++){
						swap(bMap[g_route[t]],bMap[g_route[t+1]]);   
						display();
					}
				}
            }
			swap(aMap[g_dfs_route[color][w]],aMap[g_dfs_route[color][w+1]]);	
			display();
		}
	}
	
   	SetFinalGateWay();
   	display();
   
   	printf("Final cnt : %d\n\n",cnt);
   	//GetInB(g_route[0]);
   	for(int w = 0 ; w < min ; w++) {
      	int p = g_route[w];
      	int np = g_route[w+1];

	    //if(bMap[np]) BChangeMove(p,np);
      	//else BJustMove(p,np);

      	swap(bMap[p],bMap[np]);
      	display();
   	}
   	//GetOutB(g_route[min]);   
   
   	int check[4] = {0};
   	
	for(int j = 0  ; j < 3 ; j++){
	   	for(int i = 1 ; i <= 3 ; i++){
			int m = dfs_min[i];
			int p = g_dfs_route[i][m-1];
			int np = g_dfs_route[i][m];
			if(aMap[np]) continue;
			
			//GetInA(g_dfs_route[color][m-1]);
			printf("PutPuck %d -> %d\n",p,np);
			//PutPuck(p,np);
			swap(aMap[p],aMap[np]);
			display();
	   	}
	}
}

