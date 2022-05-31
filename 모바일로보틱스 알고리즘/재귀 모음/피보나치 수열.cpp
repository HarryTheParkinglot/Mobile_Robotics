//피나치 수열
#include <stdio.h>
int cnt, a=0, b=1, c=1;

int fact(int i){
    if(i<3) return 1;
    return fact(i-1)+fact(i-2);
}

int main(){
    scanf("%d",&cnt);
    printf("피보나치 수열의 %d번째 수: %d\n",cnt, fact(cnt));
    for(int i=0; i<cnt; i++){
        a=b;
        b=c;
        c=a+b;

        printf("%d ",a);
    }
}
