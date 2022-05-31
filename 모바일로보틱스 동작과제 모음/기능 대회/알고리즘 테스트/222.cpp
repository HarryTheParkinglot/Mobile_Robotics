#include <stdio.h>
#include <memory.h>

int main(){
	int arr[10] = {0,1,2};
	int arr2[10];
	memcpy(arr2,arr,sizeof(int)*10);
	
	for(int i =0 ; i < 10 ; i++)
		printf("%d ",arr2[i]);
}
