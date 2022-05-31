#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <windows.h>
int tmp;
#define swap(a,b,node) tmp = node[a], node[a] = node[b], node[b] = tmp;

int goalArray[9] = {
	1,2,3,  
	3,1,2,
	2,3,1
};

int test[9],max = 0;

int B_Input[16] = {
	1,0,0,1,
	0,0,0,0,
	2,0,0,0,
	3,3,3,1
};

int A_Map[9] = {
	1,0,0,
	0,0,2,
	0,0,2
};
int B_Map[16] = {};
 


int route[25],g_route[25],min = 10,check[25] = {0},visit[25] = {0},flg = 0; //
int lastPos,lastDir;

int A_Link[9][4],B_Link[16][4];

int prior[2][3][3] = {
	{ 
	//Horizon
		{0,1,2},
		{3,4,5},
		{6,7,8}
	},{
	//Vertical
		{0,3,6},
		{1,4,7},
		{2,5,8}
	}
};

int No_Line[16][4] = {
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,1,0},{0,0,0,0},{0,0,1,0},{0,0,0,0},
	{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
};


void GenLink(int p, int size, int *str);
int GetDir(int p, int wp, int link[][4]);
int GetAng(int d, int wd);
void RotateArray(int *arr);

int GetMatch(int *Arr, int *gArr);
	
void DFS(int p, int wp, int dp, int *node, int link[][4]);
bool JudgeDFS(int p, int wp, int dp, int *node, int link[][4]);
bool CanGetPos(int pos, int sortDir, int nowLine);
bool NotFinished();
bool VisitAll(int p);
int FindBlock(int startPos);
void display(int p, int d);
void display2();
int Search(int lp, int p, int d, int *node, int *sc_visit, int link[][4]);

int main(){
	for(int i = 0 ; i < 16 ; i++){ //Generate Goal 
		if(i < 9) 
		GenLink(i,3,A_Link[i]); //Link Of A_Map 
		GenLink(i,4,B_Link[i]); //Link Of B_Map
	} 
	
	memcpy(test,goalArray,sizeof(test)); //Goal Array Copy
	
	if(!A_Map[4]){
		int prior[8] = {1,3,5,7,0,2,6,8};
		
		for(int i = 0 ; i < 8 ; i++){
			if(!A_Map[prior[i]]) continue;
			
			swap(prior[i],4,A_Map);
			//MovePuck(i,4);
			break;
		}
	}

	for(int i = 0 ; i < 9 ; i++) {
		printf("%-3d",A_Map[i]);
		if(i % 3 == 2) printf("\n");
	}

	if(A_Map[4] != goalArray[4]){ //일치하지 않는 경우 
		for(int i = 0 ; i < 9 ; i++){
			if(test[i] == A_Map[4])	test[i] = goalArray[4];
			else if(test[i] == goalArray[4])	test[i] = A_Map[4]; //Goal Array Center와 같은 색들을 A_Map Center와 교환 
		}
	}

	for(int j = 0 ;  j < 4 ; j++){ // Rotate Array 4 Times
		int value = GetMatch(A_Map,test);
		
		if(max < value){
			max = value;
			memcpy(goalArray,test,sizeof(test));	
		}
		RotateArray(test);		
	}
	
	for(int i = 0 ; i < 3 - max ; i++){
		for(int j = 0 ; j < 9 ; j++){
			if(!A_Map[j] || goalArray[j] == A_Map[j]) continue;
			
			int dist = 10,target = -1;
			
			for(int k = 0 ; k < 9 ; k++){
				if(A_Map[j] != goalArray[k] || A_Map[k]) continue; //채워져있거나 못가면 
				
				min = 10;
				DFS(j,k,0,A_Map,A_Link);
				
				if(min < dist) dist = min,target = k;
			}
			printf("\nFROM : %d TARGET : %d\n",j,target);
			swap(j,target,A_Map);
			//Move(j,target);
		}
	}
	for(int i = 0 ; i < 9 ; i++) {
		printf("%-3d",A_Map[i]);
		if(i % 3 == 2) printf("\n");
	}
	
	for(int i = 0 ; i < 9 ; i++) {
		printf("%-3d",goalArray[i]);
		if(i % 3 == 2) printf("\n");
	}
	
	int line_prior[3] = {0,2,1}; //0번,2번,1번 순으로 완성;
	int sortDir = 0; //정렬 방향 || 0 : Horizon / 1 : Vertical 
	int nowLine = 0;
	int startPos = 1,startDir = 2;
	
	//Bgate Solving

	//GoSearch(now_pos,End);
	//now_pos = last_pos;
	//now_dir = last_dir;
	
	for(int i = 0 ; i < 8 ; i++){
		B_Map[i] = B_Input[i]; //8개 기본 스캔 
		if(B_Map[i]) visit[i]  = 2;
	}
	system("pause");
	system("cls");
	
	Search(-1,1,0,B_Map,visit,B_Link);
	
	while(nowLine < 3){	//스도쿠 완성 전까지 동작  
		int putCnt = 0;
		
		for(int j = 0 ; j < 3 ; j++){
			for(int i = 0 ; i < 16 ; i++){
				if(!B_Map[i] || !CanGetPos(i,sortDir,nowLine) || !JudgeDFS(startPos,i,0,B_Map,B_Link))	continue; //가져갈 퍽이 없거나 / 필요한 퍽이 아니거나, 방문 불가능할때  // 
				
				//GetPuck(i);
				//PutPuck(prior[sortDir][now_line][j]);  //실제  
				A_Map[prior[sortDir][nowLine][j]] = B_Map[i];
				B_Map[i] = 0;
				putCnt ++;				
				display(-1,-1);
				break;
			}
		}
		
		/*
		if(!putCnt){//더이상 퍽을 옮기지 못하는 경우 
			int pos = FindBlock(startPos);
			int notFlg = 0;
			for(int i = 0 ; i < 16 ; i++){
				if(B_Map[i] || visit[i] || JudgeDFS(pos,i,0,B_Map,B_Link))	continue;
				
				for(int j = 0 ; j <= min ; j++)
					if(g_route[j] == i)	notFlg = 1;
				
				//GetPuck(i);
				//PutPuck(j);
				
				if(!notFlg){
					B_Map[i] = B_Map[pos]; //퍽 가져다두기 
					B_Map[pos] = 0; 

					display(-1,-1);
					
					Search(-1,pos,0,B_Map,visit,B_Link);
					break;
				}
			}
		}
		*/
		
		//다음 라인 넘어가기  
		
		for(int i = 0 ; i < 3 ; i++){
			int pos = prior[sortDir][nowLine][i];
			if(A_Map[pos] != goalArray[pos]) break;
			if(i == 2) nowLine ++; 
		} 
	}	
	
}


void GenLink(int p, int size, int *str){
	if(p / size > 0) str[0] = p - size;
	if(p % size > 0) str[1] = p - 1;
	if(p / size < size - 1) str[2] = p + size;
	if(p % size < size - 1) str[3] = p + 1;
}

int GetDir(int p, int wp, int link[][4]){
	for(int i = 0  ;i < 4 ; i++)
		if(link[p][i] == wp)	return i;
}


void RotateArray(int *arr){ //배열 회전 
	int cpArray[9] = {0};
	
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			cpArray[(2-j)*3+i] = arr[j+i*3];    
			
	memcpy(arr,cpArray,sizeof(cpArray));	
}

int GetMatch(int *Arr, int *gArr){ //일치 개수 리턴 
	int cnt = 0;
	for(int i = 0 ; i < 9 ; i++)
		if(Arr[i] && Arr[i] == gArr[i]) cnt ++;
	
	return cnt;
}
	
void DFS(int p, int wp, int dp, int *node, int link[][4]){ //가변 영역 DFS 
	if(dp >= min) return;
	
	route[dp] = p;
	
	if(p == wp){
		min = dp;
		memcpy(g_route,route,sizeof(int) * (min + 1));
	}
	
	check[p] = 1;
	
	for(int i = 0 ; i < 4 ; i++){
		int np = link[p][i];
		if(np < 0 || (np != wp && node[np]) || check[np]) continue;
		DFS(np,wp,dp+1,node,link);		
	}
	
	check[p] = 0;
}

bool JudgeDFS(int p, int wp, int dp, int *node, int link[][4]){
	min = 20;	
	memset(visit,0,sizeof(visit));
	DFS(p,wp,dp,node,link);
	return min != 20;
}

bool CanGetPos(int pos, int sortDir, int nowLine){ //해당 색을 먹을 수 있는가를 판단 
	
	for(int i = 0 ; i < 3 ; i++){
		int pos = prior[sortDir][nowLine][i];
		if(A_Map[pos] || goalArray[pos] != B_Map[pos]) continue; //이미 채워져있거나 필요한 색이 아닌경우 패스  
		return 1;
	}
	return 0;
}

bool NotFinished(){
	for(int i = 0 ; i < 9 ; i++)
		if(A_Map[i] != goalArray[i]) return 1;
	return 0;
}

bool VisitAll(int p){
	check[p] = 1;
	for(int i = 0 ; i < 4 ; i++){
		int np = B_Link[p][i];
		if(np < 0 || check[np] || B_Map[np]) continue;
		if(!visit[np] || !VisitAll(np)) return false; 
	}
	return true;
}
	
int FindBlock(int startPos){
	for(int i = 0 ; i < 16 ; i++){
		int notVisit = 0;
		
		if(!B_Map[i] || !JudgeDFS(startPos,i,0,B_Map,B_Link)) continue;  //방문 불가능한 점인경우 Continue 
		
		for(int j = 0 ; j < 4 ; j++){
			int p = B_Link[i][j];
			if(p >= 0 && !visit[p]) notVisit = 1;
		}
		
		if(notVisit)	return i;
	}
}	

void display(int p, int d){
    for(int i = 0 ; i < 9 ; i++){   
	    printf("%d ",A_Map[i]);
	    if(i % 3 == 2) printf("\n");
	}

	for(int i = 0 ; i < 16 ; i++){
        if(p == i){
            if(d == 0) printf("%s","↑");
            else if(d == 1) printf("%s","←");
            else if(d == 2) printf("%s","↓");
        	else if(d == 3) printf("%s","→");
            else printf("err : %d",d);
        }
        
        else printf("%d ",B_Map[i]);
        if(i % 4 == 3) printf("\n");
    }

   	printf("\n");
    for(int i = 0 ; i < 16 ; i++){
        printf("%d ",visit[i]);
        if(i % 4 == 3) printf("\n");
   		}
   	
   	printf("\n\nflg : %d\n\n",flg); 
    system("pause");
    system("cls");

}


	
int Search(int lp, int p, int d, int *node, int *sc_visit, int link[][4]){ 
	sc_visit[p]++;
	display(p,d);
	
    for(int i = 0 ; i < 4 && !flg ; i++){
      	int np = link[p][i]; //현재 바라보고있는 곳 부터 사방 검사 
       	if(np < 0 || sc_visit[np]) continue;//이미 방문했거나 인덱스 바깥인경우 
    		
        if(i != d) {
        	//At(GetAng(d,i),100);  //각도 회전 
			d = i;
        	display(p,d);
    	}
       	/*
		if(T(101,3)){
			node[np] = Scan(147,155);  //노드 스캔  
			node[np] = map[np];	
		}*/
		
		node[np] = B_Input[np];
		
		if(node[np]){ 
			sc_visit[np]++;
        }
    
    	else if(No_Line[p][d]){ //나중에 if(IR_1 + IR_2 + IR_3 == 0) //라인 없음 
			printf("NOLINE\n\n");
			display(p,d);
			
			link[p][d] = -1;
		}
      	
		else{
         	if(link[np][d] < 0){
				int md = -1,m = 9;
				
				for(int j = 0 ; j < 4 ; j++){
					int nnp = link[np][j];
					if(nnp < 0) continue;
					
					if(visit[nnp] >= m) continue;
					
					m = visit[nnp];
					md = j;
				}
				
				//MLC();
				//MoveLine(10,0,15,40,50);
				//Ad(0,35,GetAng(d,md),40,-50,100);
				d = md;
			} 	
			else{
				//MLC();
				//MoveLine(1,0,1000,50,60);
				//MoveLine(10,0,13,50,-60);
			}
			
			d = Search(p,np,d,B_Map,visit,B_Link);
			display(p,d); 
			//lcd(1,8,"LP%d P%d",lp,p);
			//lcd(2,8,"d%d",d);
		}
   	}
   
   	memset(check,0,sizeof(check));
	
	if(VisitAll(p) || flg){
      	if(!flg){
         	flg = 1;
         	lastPos = p;
         	lastDir = d;
      	}      
   	}
   
   	else{
        sc_visit[lp]++;
		int wd = GetDir(p,lp,B_Link);
		
		display(p,d);
		if(wd != d){
			//At(GetAng(d,wd),100);  //각도 회전 
        	d = wd;
        	display(p,d);
    	}
		
      	if(link[lp][d] < 0){ 
      		int md = -1,m = 9;
			
			for(int j = 0 ; j < 4 ; j++){
				int nnp = link[lp][j];
				if(nnp < 0) continue;
				
				if(visit[nnp] >= m) continue;
				
				m = visit[nnp];
				md = j;
			}
				
			//MLC();
			//MoveLine(10,0,15,40,50);
			//Ad(0,35,GetAng(d,md),40,-50,100);
			d = md;
		}	
		
		else{
			//MLC();
			//MoveLine(1,0,1000,50,60);
			//MoveLine(10,0,13,50,-60);
		}
  	}

   	return d;
}

