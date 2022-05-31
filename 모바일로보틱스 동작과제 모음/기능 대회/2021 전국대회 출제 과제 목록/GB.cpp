#include <iostream>
#include <time.h>

using namespace std;
int map[8] = {
	1,0,
	2,1,
	0,2,
	3,3
};
int aMap[8] = {
	3,3,
	2,2,
	1,1,
	0,0
};
#define M 30
unsigned long puckMark[8];
///////////////////////////////////////////////////////
void Graph_Init(void);
int graph[8][8];
///////////////////////////////////////////////////////

int Slide_Puzzle_DFS(int lP, int cnt);
int dfsValidPos[2]; //�� ���� ���� �ִ� ��� �ڸ� (������ �˻� ���� ��ģ �ɷ��� ��)
int dfsValidPosCnt; //���� ������ ����
int dfsValidPosFront;

unsigned long dfsMask;
unsigned long dfsBuf;

int dfsAllValidPos[M][8][2];
int dfsAllValidPosCnt[M][8];
unsigned long dfsAllMovedMap[M][8][2];
int dfsAllHeuristic[M][8][2];
unsigned long dfsPuckMarkBackUp[M];

unsigned long stack[15];
unsigned long aStack[15];
unsigned long aMapB;

int minDepth = M;
int searchStat = 0;
int callCnt = 0;

///////////////////////////////////////////////////
int Search_BFS(int cP, long cMap, int *posQ);
int bfsAPosQ[4];
unsigned long bfsMask;
/////////////////////////////////////////////////////
int Heuristic(unsigned long map);
////////////////////////////////////////////////////
void Convert_Coordinate(void);
int path[M][2];
////////////////////////////////////////////////////


int main()
{
  //freopen("input.txt", "r", stdin);
    cout << "Input start :" << endl;

    for (int index = 0; index < 8; index++)
    {
        stack[0] |= (map[index] << (7 - index) * 2);
    }

    for (int index = 0; index < 8; index++)
    {
        aMapB |= (aMap[index] << (7 - index) * 2);
    }
    Graph_Init();
    callCnt = 0;
    Slide_Puzzle_DFS(-1, 0);

    Convert_Coordinate();
    
    cout << "result :" << endl << endl;
    for (int i = 0; i <= minDepth; i++)
    {
        cout << i + 1 << ". " << path[i][0] << "   " << path[i][1] << endl << endl;
    }
    cout << "CALLCNT : " << callCnt;

}

void Graph_Init(void)
{
    graph[0][2] = graph[2][0] = 1;
    
	graph[4][2] = graph[2][4] = 1;
    
	graph[6][4] = graph[4][6] = 1;
	
	graph[1][3] = graph[3][1] = 1;
    
	graph[3][5] = graph[5][3] = 1;
    
	graph[5][7] = graph[7][5] = 1;
    
	graph[0][1] = graph[1][0] = 1;
    graph[2][3] = graph[3][2] = 1;
    graph[4][5] = graph[5][4] = 1;
    graph[6][7] = graph[7][6] = 1;
}

void Convert_Coordinate()
{
    unsigned long mask;
    
    for (int idx = 0; idx <= minDepth; idx++)
    {
        mask = 0b1100000000000000;
        
        for (int cP = 0; cP < 8; cP++)
        {
            if ((aStack[idx] & mask) && !(aStack[idx + 1] & mask))
                path[idx][0] = cP;
            else if (!(aStack[idx] & mask) && (aStack[idx + 1] & mask))
                path[idx][1] = cP;
            
            mask >>= 2;
        }
    }
}

int Slide_Puzzle_DFS(int lP, int cnt)
{
    dfsMask = 0b1100000000000000;
    dfsBuf = 0;
    
    callCnt++;
    
    for (int cP = 0; cP < 8; cP++) //8���� ���� �����̴� ��� ��츦 ���� ���� for��
    {
        dfsAllValidPosCnt[cnt][cP] = 0;
        
        if (cP == lP) //�ٷ� ���� �������� ���� �ȿ�����
        {
            dfsMask >>= 2;
            continue;
        }
        
        if (stack[cnt] & dfsMask) //cP�ڸ��� ���� �ִ°��
        {
            dfsValidPosCnt = Search_BFS(cP, stack[cnt], dfsValidPos); //dfsPosQ �迭�� cP�ڸ��� �ִ� ���� �����ϼ� �ִ� ��� ��ǥ ���� // dfsQCnt���� ��ǥ�� ���� ����
            dfsValidPosFront = 0;
            
            for (int i = 0; i < dfsValidPosCnt; i++) //cp�ڸ��� �ִ� ���� ������ �� �ִ� ��� ��ǥ�� ������ŭ �ݺ�
            {
                dfsBuf = stack[cnt] & dfsMask; // cP�� �ִ� ���� dfsBuf�� ����
                
                if (dfsValidPos[i] - cP > 0)            //�ű� ��ġ�� dfsBuf ����Ʈ
                    dfsBuf >>= (dfsValidPos[i] - cP) * 2;
                else
                    dfsBuf <<= (cP - dfsValidPos[i]) * 2;
                
                dfsAllMovedMap[cnt][cP][dfsValidPosFront] = stack[cnt] | dfsBuf; //���� �� + dfsBuf�� ���� �ʿ� ����
                dfsAllMovedMap[cnt][cP][dfsValidPosFront] &= ~dfsMask;           //�ű����� �����ڸ� ����
                
                for (int j = 0; j <= cnt; j++) //���� �� ���°� ���� ���¿��� �������� �ִ��� üŷ
                {
                    if (stack[j] == dfsAllMovedMap[cnt][cP][dfsValidPosFront]) //�ִٸ� Ż��
                        break;
                    
                    if(j == cnt) //���ٸ� �Ʒ� ����
                    {
                        dfsAllValidPos[cnt][cP][dfsValidPosFront] = dfsValidPos[i]; //��ǥ ����
                        
                        dfsAllHeuristic[cnt][cP][dfsValidPosFront] = Heuristic(dfsAllMovedMap[cnt][cP][dfsValidPosFront]); //�޸���ƽ ����
                        
                        if (!dfsAllHeuristic[cnt][cP][dfsValidPosFront]) //���� üŷ
                        {
                            for (int k = 0; k <= cnt; k++) //aStack�� ��������� �ʻ��� ��ȭ ����
                            {
                                aStack[k] = stack[k];
                            }
                            
                            aStack[cnt + 1] = dfsAllMovedMap[cnt][cP][dfsValidPosFront]; //���� ���� �߰�
                            
                            minDepth = cnt; // �ּ� �̵� Ƚ�� ����
                            
                            searchStat = 1; //������ �ѹ��̶� ã�� ��츦 �˱� ���� (�˰��� ���۰��� �����)
                            
                            return 1;
                        }
                        dfsValidPosFront++;
                    }
                }
            }
            dfsAllValidPosCnt[cnt][cP] = dfsValidPosFront; //�� �ɷ����� cP�ڸ��� ���� �̵��Ҽ� �ִ� ����� �� ����
        }
        dfsMask >>= 2; //����ũ ����Ʈ
    }
    
    if (cnt >= minDepth - 1) //�ּ� �̵����� �������� ��� Ž�� �ߴ�
        return 0;
    
    for (int hCnt = 1; hCnt <= 6; hCnt++) //���̶� ���������� Ž�� //�޸���ƽ ���� ����� �켱���� Ž���� for��
    {
        for (int cP = 0; cP < 8; cP++) //�޸���ƽ ���� ������ Ž�� ���� (ũ�� ���� ������������ �˰� ����)
        {
            for (int i = 0; i < dfsAllValidPosCnt[cnt][cP] ; i++)
            {
                if (hCnt == dfsAllHeuristic[cnt][cP][i])
                {
                    dfsPuckMarkBackUp[cnt] = puckMark[cP]; //cP �ڸ��� ���� ���� ������ ���
                    puckMark[dfsAllValidPos[cnt][cP][i]] = puckMark[cP]; //�� ���� ������ �̵��� �ڸ��� �ű��
                    
                    for (int area = 0; area < dfsAllValidPosCnt[cnt][cP]; area++) //���� �ܰ��� �� ���� ������ ���
                    {
                        puckMark[dfsAllValidPos[cnt][cP][i]] |= (0b1100000000000000 >> (dfsAllValidPos[cnt][cP][area] * 2));
                    }
                    
                    puckMark[dfsAllValidPos[cnt][cP][i]] &= ~(0b1100000000000000 >> (cP * 2)); //�� ���� �����Ϳ��� ���� ��� ����
                    
                    stack[cnt + 1] = dfsAllMovedMap[cnt][cP][i]; //�� ���¸� ���
                    
                    if (Slide_Puzzle_DFS(dfsAllValidPos[cnt][cP][i], cnt + 1)) //���� �ܰ迡�� ���� ã�� ��� �ٷ� �����ܰ迡�� �� Ž���� �ʿ� ����
                    {
                        puckMark[cP] = dfsPuckMarkBackUp[cnt]; //����ص� �� ���� ������ ����
                        
                        return 0;
                    }
                    
                    puckMark[cP] = dfsPuckMarkBackUp[cnt]; //����ص� �� ���� ������ ����
                }
            }
        }
    }
    
    return 0;
}

int Search_BFS(int cP, long cMap, int *posQ) //cP���� �ѹ��� �� �� �ִ� ��ǥ Ž�� (posQ �迭�� ���� (������) )
{
    int r = 0;
    int f = 1;
    int f2 = 0;
    
    bfsAPosQ[r] = cP; //����� cP
    
    while (r < f)
    {
        bfsMask = 0b1100000000000000;
        
        for (int tP = 0; tP < 8; tP++)
        {
            if (graph[bfsAPosQ[r]][tP] && !(bfsMask & cMap)) //r���� tP�� �׷����� ����Ǿ� ���� && tP�� �������
            {
                for (int i = 0; i < f; i++) //bfs ť �ߺ��˻縦 ���� for��
                {
                    if (bfsAPosQ[i] == tP) //tP�� �̹� ť�� ����Ǿ� ������� Ż�� (�ߺ�üũ)
                        break;
                    
                    if (i == f - 1) //�ߺ��˻縦 ��� ����� ���
                    {
                        if (!(puckMark[cP] & bfsMask)) //������ �˻� (���ô� ���̸� �Ȱ�)
                            posQ[f2++] = tP; //�˻� ����� posQ�� ���� (�� �Լ��� ���� �����)
                        
                        bfsAPosQ[f++] = tP;
                        break;
                    }
                }
            }
            bfsMask >>= 2;
        }
        r++;
    }
    
    return f2;
}

int Heuristic(unsigned long map) //���Լ� // �ڸ��� Ÿ�ٰ� �ٸ���� +1
{
    bfsMask = 0b1100000000000000;
    
    int hSum = 0;
	    
    for (int cP = 0; cP < 8; cP++)
    {
        if (((map ^ aMapB) & bfsMask) && (bfsMask & map)) //Ÿ�ٰ� �ش��ڸ��� ���� �ٸ� && �ش��ڸ��� ���� ����
            hSum++;
        
        bfsMask >>= 2;
    }
    
    return hSum;
}

