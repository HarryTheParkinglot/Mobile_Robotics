#include <stdio.h>
#include <memory.h>
#include <windows.h>

int tmp;
#define swap(a,b) tmp = a, a = b, b = tmp;

bool IsBetween(double t, double a, double b) {
   double maxV = a > b ? a : b;
   double minV = a < b ? a : b;

   return minV <= t && t <= maxV;
}

int IsCrossLine(int px, int py, double lineX1, double lineY1, double lineX2, double lineY2) {
   // y = slope * x + b
    if (!IsBetween(py, lineY1, lineY2)) return 0;
    
   	double slope = (lineY1 - lineY2) / (lineX1 - lineX2); //
   	double b = lineY1 - lineX1 * slope; // y = ax + b, b = y - ax;
   	double ty = slope * px + b;
   	if (slope * px + b == py	|| (lineX1 == lineX2 && px == lineX1))	return -1;
	if (IsBetween(ty, lineY1, lineY2) && ty > py) return 1;
	
   	return 0;
}

bool IsInsideOfTriangle(int px, int py, double* xarr, double *yarr) {
   int cnt = 0;
   	for (int i = 0; i < 3; i++) {
      	int val = IsCrossLine(px, py, xarr[i], yarr[i], xarr[(i + 1) % 3], yarr[(i + 1) % 3]);
      	if(val < 0) return 0;
		
	}
   return cnt == 3;
}

int main() {
	int ps[3] = {24,21,10};
    
    double x[3],y[3];
    for(int j = 0 ; j < 4 ; j++) 
		x[j] = ps[j]%5,y[j] = ps[j]/5;
    
	printf("XX%d\n",IsInsideOfTriangle(16%5,16/5,x,y));
}



