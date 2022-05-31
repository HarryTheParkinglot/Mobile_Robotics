#include <stdio.h>
#include <memory.h>

int tmp;
#define swap(a,b) tmp = a, a=  b, b = tmp;


//좌표계  
int map[25] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,3,2,0,
	0,0,1,1,0,
	0,0,0,0,0
};
 
int posList[16],posCnt = 0;
int seetList[4],seetCnt = 0; 

//알고리즘 내부 사용 변수 
int flg= 0,min=0,dir=0,rTIme = 0;
int spot[5][3],g_spot[5][3];
int insideCnt= 0,remainCnt= 0;
int maxPos,mPos = 0;
int call = 0;

int ccw(int p1, int p2, int p3){
	double d1[2] = {p1 % 5, p1/5};
	double d2[2] = {p2 % 5, p2/5};
	double d3[2] = {p3 % 5, p3/5};
	//2131

	return ((d2[0] - d1[0]) * (d3[1] - d1[1])) - ((d2[1] - d1[1]) * (d3[0] - d1[0]));
}

int IsInsideOfTriangle(int p, int *tri){
	int c[3];
	
	for(int i = 0; i < 3 ; i++){
		c[i] = ccw(tri[i],tri[(i+1)%3],p);
		if(!c[i]) return 0;
	}
	
	return c[0] < 0 == c[1] < 0 && c[0] < 0 == c[2] < 0;
}




