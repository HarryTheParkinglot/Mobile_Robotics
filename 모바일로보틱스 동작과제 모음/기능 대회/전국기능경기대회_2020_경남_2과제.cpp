#include <stdio.h>

int arr[2][4]={0,}; // A ���� �迭
int trash[100]={0,}; //������ �� ����
int line[4]; //C ���� �� Ȯ��
int block[4][2]; //��� �� ����

int c1=0,c2=0, c3=0, c4=0; //�� �� ������ ����
int t1=0; // �����Ⱚ ������ ����
//���� 1, �Ķ� 2, ��� 3
//�� 1, �� 2
int main()
{
    //�� Ž��
    printf("�� ���� �����ֽÿ�(��=1, ��=2)\n");
    for(int i=0;i<4;i++)
        scanf("%d", &line[i]);
    //�¿�Ž��
    printf("�¿� �� ���� �����ֽÿ�(����=1, �Ķ�=2, ���=3)\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            scanf("%d", &block[i][j]);
        }
    }
    //arr 0�� �̵�
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[0]==2&&block[i][j]==1){
                trash[t1]=1;
                t1++;
                block[i][j]=0;
                c1++;
            }
            else if(block[i][j]==1){
                arr[c1][0]=1;
                c1++;
                block[i][j]=0;
            }
             if(c1==2){i=4;
			 break;}
        }
    }

    //arr 1�� �̵�
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[1]==2&&block[i][j]==2){
                trash[t1]=2;
                t1++;
                block[i][j]=0;
                c2++;
            }
            else if(block[i][j]==2){ 
                arr[c2][1]=2;
                c2++;
                block[i][j]=0;
            }
            if(c2==2){i=4;
			 break;}
    	}
    }

    //arr 2�� �̵�
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[2]==2&&block[i][j]==3){
                trash[t1]=3;
                t1++;
                block[i][j]=0;
                c3++;
            }
            else if(block[i][j]==3){ 
                arr[c3][2]=3;
                c3++;
                block[i][j]=0;
            }
            if(c3==2){i=4;
			 break;}
        }
    }

//arr 3�� �̵�
    for(int i=0; i<4; i++){
        for(int j=0; j<2; j++){
            if(line[3]==2&&block[i][j]==2){
                trash[t1]=2;
                t1++;
                block[i][j]=0;
                c4++;
            }
            else if(block[i][j]==2){ 
                arr[c4][3]=2;
                c4++;
                block[i][j]=0;
            }
            if(c4==2){i=4;
			 break;}
        }
    }
    printf("A ���� �� ��ġ ���(0=��ĭ, 1=����, 2=�Ķ�, 3=���)\n");
    for(int i=0; i<2; i++){
        for(int j=0; j<4; j++){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
    printf("�����Ⱚ ���\n");
    for(int i=0; i<t1; i++)
        printf("%d",trash[i]);

    printf("\n====���� ���====\n");
    printf("c1=%d, c2=%d, c3=%d, c4=%d, t1=%d\n",c1,c2,c3,c4,t1);

    return 0;
}
