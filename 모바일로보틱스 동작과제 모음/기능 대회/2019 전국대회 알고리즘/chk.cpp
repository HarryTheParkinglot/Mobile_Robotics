#include <stdio.h>

int chk(int array[][4]){
	array[0][0] = 1;
}

int main(){
	int arr[4][4] = {0};
	printf("%d",arr[0][0]);
	chk(arr);
	printf("%d",arr[0][0]);

}
