//�ǳ�ġ ����
#include <stdio.h>
int cnt, a=0, b=1, c=1;

int fact(int i){
    if(i<3) return 1;
    return fact(i-1)+fact(i-2);
}

int main(){
    scanf("%d",&cnt);
    printf("�Ǻ���ġ ������ %d��° ��: %d\n",cnt, fact(cnt));
    for(int i=0; i<cnt; i++){
        a=b;
        b=c;
        c=a+b;

        printf("%d ",a);
    }
}
