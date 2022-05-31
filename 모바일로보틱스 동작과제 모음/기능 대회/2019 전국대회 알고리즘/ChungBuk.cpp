#include <stdio.h>
#include <windows.h>

int map[28] = {
   0,0,1,0,0,0,0,
   0,0,0,0,0,0,0,
   0,0,0,0,1,0,0,
   0,0,0,0,0,2,0
};

int gmap[28] = {0};
int order[3] = {1,3,2};

int node[28] = {0};
int link[28][4] = {0};
int visit[28] = {0},sc_visit[28] = {0},check[28] = {0};

int route[28],g_route[28],min,flg = 0,lastPos = 0,lastDir = 0;


int target[2] = {0,8};
int targetPos[2][4] = {0};
int answer[2][4] = {
	{0,3,1,2},
	{3,1,2,0}
};

void Link(int p, int *str){
    memset(str,-1,sizeof(int)*4);
   
	if(p / 7 > 0) str[0] = p - 7; 
	if(p % 7 > 0) str[1] = p - 1;
	if(p / 7 < 3) str[2] = p + 7;
	if(p % 7 < 6) str[3] = p + 1;
}

void DFS(int from, int to, int dp){
    if(dp >= min) return;
   
    route[dp] = from;
   
    if(from == to){
        memcpy(g_route,route,sizeof(g_route));
       	min = dp;
    }
   
   	else{
        visit[from] = 1;
        for(int i = 0 ; i < 4 ; i++){
	        int np = link[from][i];
	            
	        if(np < 0 || visit[np] || (np != to && node[np])) continue;
	        DFS(np,to,dp+1);
       	}
      
    	visit[from] = 0;
   	}
   	
	return;
}

int GetDir(int p, int wp){
    for(int i = 0 ; i < 4;  i++)
        if(link[p][i] == wp)    return i;
}

int GetAng(int d, int wd){
    int rd = wd - d;
    if(abs(rd) == 3) rd/=-3;

    return rd * 90;
    return -1;
}

int NotYet(int p){
	check[p] = 1;
   
    for(int i = 0 ; i < 4 ; i++){
        int np = link[p][i];
      
        if(np < 0 || check[np] || node[p])    continue;
        if(!sc_visit[np] || NotYet(np))   return 1;
    }
    return 0;
}
void display(int p, int d){
    for(int i = 0 ; i < 28 ; i++){
        if(p == i){
            if(d == 0) printf("%s","위");
            else if(d == 1) printf("%s","왼");
            else if(d == 2) printf("%s","밑");
        	else if(d == 3) printf("%s","오");
            else printf("err : %d",d);
        }
        
        else printf("%d ",node[i]);
        if(i % 7 == 6) printf("\n");
    }

   	printf("\n");
    for(int i = 0 ; i < 28 ; i++){
        printf("%d ",sc_visit[i]);
        if(i % 7 == 6) printf("\n");
   		}
   	
    system("pause");
    system("cls");

}

int Search(int lp, int p, int d){ 
    //lcd(1,8,"LP%d P%d",lp,p);
	//lcd(2,8,"d%d",d);
	display(p,d);
	sc_visit[p] = 1;
	       
    for(int i = 0 ; i < 4 && !flg ; i++){
      	int np = link[p][i]; //현재 바라보고있는 곳 부터 사방 검사 
       	if(np < 0 || sc_visit[np]) continue;//이미 방문했거나 인덱스 바깥인경우 
    		
        //if(i != d)	At(GetAng(d,i),100);  //각도 회전 
        if(i != d) {
			d = i;
        	display(p,d);
    	}
       	
		//if(T(101,3))	node[np] = Scan(147,155);  //노드 스캔  
		node[np] = map[np];
		
		if(node[np]){ 
			//lcs
			sc_visit[np] = 1;
			for(int i = 0 ; i < 4 ; i++){
				/*for(int j = 0 ; j < 7 ; j++){
					lcd(i,j,"%d",node[i * 7 + j]);
				}*/
			}

        }
      	else{
         	if(link[np][d] < 0){
				for(int j = 0 ; j < 8 ; j++){
					int nnp = link[np][j%4];
					if(nnp < 0 || (j / 4 == 0 && sc_visit[nnp])) continue; 
					
					//MLC();
					//MoveLine(10,0,15,40,50);
					//Ad(0,35,GetAng(d,j % 4),40,-50,100);
			
					d = j%4;
					break;
				}
			} 	
			else{
				//MLC();
				//MoveLine(1,0,1000,50,60);
				//MoveLine(10,0,13,50,-60);
			}
			d = Search(p,np,d);
			display(p,d); 
			//lcd(1,8,"LP%d P%d",lp,p);
			//lcd(2,8,"d%d",d);
		}
   	}
   
   for(int i = 0 ; i < 28 ; i++) check[i] = 0;
	
	printf("NOT YET : %d flg : %d\n",NotYet(p),flg);
	if(!NotYet(p) || flg){
      	if(!flg){
         	flg = 1;
         	lastPos = p;
         	lastDir = d;
      	}      
   	}
   
   else{
        int wd = GetDir(p,lp);
		display(p,d);
        if(wd != d)	//At(GetAng(d,wd),100);  //각도 회전 
        d = wd;
		
      	if(link[lp][d] < 0){
			for(int j = 0 ; j < 8 ; j++){
				int nnp = link[lp][j % 4];
				if(nnp < 0 || (j / 4 == 0 && sc_visit[nnp])) continue;
				
				//MLC();
				//MoveLine(10,0,15,40,50);
				//Ad(0,35,GetAng(d,j % 4),40,-50,100);
				
				d = j % 4; 
				break;
			}	
		}
		else{
			//MLC();
			//MoveLine(1,0,1000,50,60);
			//MoveLine(10,0,13,50,-60);
		}
  	}

   	return d;
}



//0(0,1,7,8) 1(1,2,8,9) 2(2,3,9,10)
//3(7,8,14,15) 4 5
//6 7 8 

//3 몫으로 * 7 / + 3 나머지  

void SetTarget(int area,int pixel, int *str){
	int value = pixel / 3 * 7 + pixel % 3;
	
	for(int i = 0 ; i < 4 ; i++)
		str[i] = value + i/2*7 + i%2 + (area * 3);
		
	printf("%d %d\n%d %d\n\n",str[0],str[1],str[2],str[3]);
}


int main(){
   	for(int i = 0; i < 28 ; i++)   Link(i,link[i]); //Link Setting
   	
    Search(-1,7,3);
    //printf("LLLS %d %d",lastPos,lastDir);
   
   	for(int i = 0  ; i < 2 ; i++){
   		SetTarget(i,target[i],targetPos[i]);
		   
		for(int j = 0 ; j < 4 ; j++)		
			map[targetPos[i][j]] = answer[i][j];
	}
	
	for(int i = 0 ; i < 28 ; i++){
		printf("%d ",map[i]);
		if(i % 7 == 6) printf("\n");
	}
	
	memset(check,0,sizeof(int) * 28);
	for(int i = 0 ; i < 28 ; i++) sc_visit[i] = 1;
	printf("%d",NotYet(7));
}

/*
0	1	2	3	4	5	6 
7 	8 	9 	10	11	12	13
14	15	16	17 	18	19	20
21	22	23	24	25	26	27

*/
