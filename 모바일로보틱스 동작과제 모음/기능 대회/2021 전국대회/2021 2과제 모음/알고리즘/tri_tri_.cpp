#include <stdio.h>

int IsInside(int p, int *points) {
	int cn = 0; // the crossing number counter
	int py[3],px[3];
	
	for(int i = 0 ; i < 3; i ++)
		px[i] = points[i] % 5, py[i] = points[i] / 5;
		
	// loop through all edges of the polygon
	for (int i = 0; i < 3; i++) { // edge from V[i] to V[i+1]
		int nextIndex = (i + 1)%3;
		if (((py[i] <= p / 5) && (py[nextIndex] > p / 5)) // an upward crossing
		|| ((py[i] > p / 5) && (py[nextIndex] <= p / 5))) { // a downward crossing
		
			double vt = ((p/5) - py[i]) / (py[nextIndex] - py[i]);
			if (p%5 < px[i] + vt * (px[nextIndex] - px[i])) // P.x < intersect
				++cn; // a valid crossing of y=P.y right of P.x
		}
	}
	return cn % 2;
}



int main(){
	int arr[3] = {0,4,20};
	for(int i = 0 ; i < 25 ; i++)
		printf("%d : %d\n",i,IsInside(i,arr));
}
