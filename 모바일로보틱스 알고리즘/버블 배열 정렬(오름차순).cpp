#include <stdio.h>
void arrange(int,int*); 


int main(){
	int Arr[5] = {2,4,3,2,1};
	
	arrange(1,Arr);
	
}

void arrange(int type, int *arr){ // *이 붙은 경우 배열을 함수의 매개변수로 받을 수 있음 (포인터 : 주소(변수가 저장되어 있는 위치)와 같은 개념 ) 
int index = 0,tmp;
	
	for(int i = 0 ; i < 5 ; i++){
		index = i;
		for(int j = i + 1;  j < 5 ; j++)
			if((type == -1 && arr[index] < arr[j]) || (type == 1 && arr[index] > arr[j]))
				index = j;	
		
		tmp = arr[index];
		arr[index] = arr[i];
		arr[i] = tmp;
	}
	for(int i = 0 ; i < 5;  i++){
		printf("%d ",arr[i]);
	}
} 

