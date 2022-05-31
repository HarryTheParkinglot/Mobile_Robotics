#include <iostream>
#include <time.h>

using namespace std;

int map[8];
int aMap[8];

unsigned long puckMark[8];
///////////////////////////////////////////////////////
void Graph_Init(void);
int graph[8][8];
///////////////////////////////////////////////////////
int Slide_Puzzle_DFS(int lP, int cnt);
int dfsValidPos[2]; //한 퍽이 갈수 있는 모든 자리 (퍽추적 검사 까지 마친 걸러진 값)
int dfsValidPosCnt; //위에 데이터 갯수
int dfsValidPosFront;

unsigned long dfsMask;
unsigned long dfsBuf;

int dfsAllValidPos[10][8][2];
int dfsAllValidPosCnt[10][8];
unsigned long dfsAllMovedMap[10][8][2];
int dfsAllHeuristic[10][8][2];
unsigned long dfsPuckMarkBackUp[10];

unsigned long stack[15];
unsigned long aStack[15];
unsigned long aMapB;

int minDepth = 8;
int searchStat = 0;
///////////////////////////////////////////////////
int Search_BFS(int cP, long cMap, int *posQ);
int bfsAPosQ[4];
unsigned long bfsMask;
/////////////////////////////////////////////////////
int Heuristic(unsigned long map);
////////////////////////////////////////////////////
void Convert_Coordinate(void);
int path[10][2];
////////////////////////////////////////////////////
int callCnt = 0;
clock_t sT, eT;

int main()
{
    //freopen("input.txt", "r", stdin);
//    cout << "Input start :" << endl;
//
//    for (int index = 0; index < 8; index++)
//    {
//        cin >> map[index];
//        stack[0] |= (map[index] << (7 - index) * 2);
//    }
//
//    for (int index = 0; index < 8; index++)
//    {
//            cin >> aMap[index];
//            aMapB |= (aMap[index] << (7 - index) * 2);
//    }
//    stack[0] = 35316;
//    aMapB = 23262;
//    //cout << Heuristic(22707);
//    Graph_Init();
//    callCnt = 0;
//    Slide_Puzzle_DFS(-1, 0);
//
//    Convert_Coordinate();
//
//    cout << "result :" << endl << endl;
//    for (int i = 0; i <= minDepth; i++)
//    {
//        cout << i + 1 << ". " << path[i][0] << "   " << path[i][1] << endl << endl;
//    }
//    cout << callCnt;
    
    freopen("All Cases_2 Pucks Each Color.txt", "r", stdin);

    //freopen("OUTPUT_All Cases_2 Pucks Each Color.txt", "w", stdout);


    Graph_Init();

    int count = 1;

    for(int j = 0; j < 3; j++)
    {
        if(j == 0) aMapB = 23231;
        if(j == 1) aMapB = 22207;
        if(j == 2) aMapB = 22191;

        for (int i = 0; i < 2520; i++)
        {
            stack[0] = 0;

            for (int index = 0; index < 8; index++)
            {
                cin >> map[index];
                stack[0] |= (map[index] << (7 - index) * 2);
            }

            for(int j = 0; j < 8; j++)
                puckMark[j] = 0;

            callCnt = 0;

            minDepth = 6;

            if(Heuristic(stack[0]))
                Slide_Puzzle_DFS(-1, 0);
            else
                minDepth = -1;

            cout << count << ".  " << stack[0] << "   " << minDepth << "   " << callCnt << endl << endl;

            count++;
        }
    }
}

void Graph_Init(void)
{
    graph[0][1] = 1; graph[1][0] = 1;
    graph[0][3] = 1; graph[3][0] = 1;
    graph[1][2] = 1; graph[2][1] = 1;
    graph[1][3] = 1; graph[3][1] = 1;
    graph[1][4] = 1; graph[4][1] = 1;
    graph[2][4] = 1; graph[4][2] = 1;
    graph[3][4] = 1; graph[4][3] = 1;
    graph[3][5] = 1; graph[5][3] = 1;
    graph[3][6] = 1; graph[6][3] = 1;
    graph[4][6] = 1; graph[6][4] = 1;
    graph[4][7] = 1; graph[7][4] = 1;
    graph[5][6] = 1; graph[6][5] = 1;
    graph[6][7] = 1; graph[7][6] = 1;
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
    
    for (int cP = 0; cP < 8; cP++) //8개의 퍽이 움직이는 모든 경우를 보기 위한 for문
    {
        dfsAllValidPosCnt[cnt][cP] = 0;
        
        if (cP == lP) //바로 전에 움직였던 퍽은 안움직임
        {
            dfsMask >>= 2;
            continue;
        }
        
        if (stack[cnt] & dfsMask) //cP자리에 퍽이 있는경우
        {
            dfsValidPosCnt = Search_BFS(cP, stack[cnt], dfsValidPos); //dfsPosQ 배열에 cP자리에 있는 퍽이 움직일수 있는 모든 좌표 저장 // dfsQCnt에는 좌표의 갯수 저장
            dfsValidPosFront = 0;
            
            for (int i = 0; i < dfsValidPosCnt; i++) //cp자리에 있는 퍽이 움직일 수 있는 모든 좌표의 갯수만큼 반복
            {
                dfsBuf = stack[cnt] & dfsMask; // cP에 있는 퍽을 dfsBuf에 저장
                
                if (dfsValidPos[i] - cP > 0)            //옮길 위치로 dfsBuf 시프트
                    dfsBuf >>= (dfsValidPos[i] - cP) * 2;
                else
                    dfsBuf <<= (cP - dfsValidPos[i]) * 2;
                
                dfsAllMovedMap[cnt][cP][dfsValidPosFront] = stack[cnt] | dfsBuf; //현재 맵 + dfsBuf를 다음 맵에 저장 //
                dfsAllMovedMap[cnt][cP][dfsValidPosFront] &= ~dfsMask;           //옮긴퍽의 원래자리 삭제
                
                for (int j = 0; j <= cnt; j++) //현재 맵 상태가 이전 상태에서 나온적이 있는지 체킹
                {
                    if (stack[j] == dfsAllMovedMap[cnt][cP][dfsValidPosFront]) //있다면 탈출
                        break;
                    
                    if(j == cnt) //없다면 아래 실행
                    {
                        dfsAllValidPos[cnt][cP][dfsValidPosFront] = dfsValidPos[i]; //좌표 저장
                        
                        dfsAllHeuristic[cnt][cP][dfsValidPosFront] = Heuristic(dfsAllMovedMap[cnt][cP][dfsValidPosFront]); //휴리스틱 저장
                        
                        if (!dfsAllHeuristic[cnt][cP][dfsValidPosFront]) //정답 체킹
                        {
                            for (int k = 0; k <= cnt; k++) //aStack에 현재까지의 맵상태 변화 복사
                            {
                                aStack[k] = stack[k];
                            }
                            
                            aStack[cnt + 1] = dfsAllMovedMap[cnt][cP][dfsValidPosFront]; //현재 상태 추가
                            
                            minDepth = cnt; // 최소 이동 횟수 갱신
                            
                            searchStat = 1; //정답을 한번이라도 찾은 경우를 알기 위해 (알고리즘 동작과는 무상관)
                            
                            return 1;
                        }
                        dfsValidPosFront++;
                    }
                }
            }
            dfsAllValidPosCnt[cnt][cP] = dfsValidPosFront; //다 걸러낸후 cP자리의 퍽이 이동할수 있는 경우의 총 갯수
        }
        dfsMask >>= 2; //마스크 시프트
    }
    
    if (cnt >= minDepth - 1) //최소 이동보다 많아지는 경우 탐색 중단
        return 0;
    
    for (int hCnt = 1; hCnt <= 6; hCnt++) //답이랑 가까운경우부터 탐색 //휴리스틱 값에 기반한 우선순위 탐색용 for문
    {
        for (int cP = 0; cP < 8; cP++) //휴리스틱 값이 같을때 탐색 순위 (크게 영향 없었던것으로 알고 있음)
        {
            for (int i = 0; i < dfsAllValidPosCnt[cnt][cP] ; i++)
            {
                if (hCnt == dfsAllHeuristic[cnt][cP][i])
                {
                    dfsPuckMarkBackUp[cnt] = puckMark[cP]; //cP 자리의 퍽의 추적 데이터 백업
                    puckMark[dfsAllValidPos[cnt][cP][i]] = puckMark[cP]; //퍽 추적 데이터 이동한 자리에 옮기기
                    
                    for (int area = 0; area < dfsAllValidPosCnt[cnt][cP]; area++) //현재 단계의 퍽 추적 데이터 기록
                    {
                        puckMark[dfsAllValidPos[cnt][cP][i]] |= (0b1100000000000000 >> (dfsAllValidPos[cnt][cP][area] * 2));
                    }
                    
                    puckMark[dfsAllValidPos[cnt][cP][i]] &= ~(0b1100000000000000 >> (cP * 2)); //퍽 추적 데이터에서 현재 기록 삭제
                    
                    stack[cnt + 1] = dfsAllMovedMap[cnt][cP][i]; //현 상태를 기록
                    
                    if (Slide_Puzzle_DFS(dfsAllValidPos[cnt][cP][i], cnt + 1)) //다음 단계에서 답을 찾은 경우 바로 이전단계에서 더 탐색할 필요 없음
                    {
                        puckMark[cP] = dfsPuckMarkBackUp[cnt]; //백업해둔 퍽 추적 데이터 복원
                        
                        return 0;
                    }
                    
                    puckMark[cP] = dfsPuckMarkBackUp[cnt]; //백업해둔 퍽 추적 데이터 복원
                }
            }
        }
    }
    
    return 0;
}

int Search_BFS(int cP, long cMap, int *posQ) //cP에서 한번에 갈 수 있는 좌표 탐색 (posQ 배열에 저장 (포인터) )
{
    int r = 0;
    int f = 1;
    int f2 = 0;
    
    bfsAPosQ[r] = cP; //리어는 cP
    
    while (r < f)
    {
        bfsMask = 0b1100000000000000;
        
        for (int tP = 0; tP < 8; tP++)
        {
            if (graph[bfsAPosQ[r]][tP] && !(bfsMask & cMap)) //r에서 tP가 그래프상에 연결되어 있음 && tP가 비어있음
            {
                for (int i = 0; i < f; i++) //bfs 큐 중복검사를 위한 for문
                {
                    if (bfsAPosQ[i] == tP) //tP가 이미 큐에 저장되어 있을경우 탈출 (중복체크)
                        break;
                    
                    if (i == f - 1) //중복검사를 모두 통과한 경우
                    {
                        if (!(puckMark[cP] & bfsMask)) //퍽추적 검사 (가봤던 곳이면 안감)
                            posQ[f2++] = tP; //검사 통과시 posQ에 저장 (이 함수의 최종 결과물)
                        
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

int Heuristic(unsigned long map) //평가함수 // 자리가 타겟과 다를경우 +1
{
    bfsMask = 0b1100000000000000;
    
    int hSum = 0;
    
    for (int cP = 0; cP < 8; cP++)
    {
        if (((map ^ aMapB) & bfsMask) && (bfsMask & map)) //타겟과 해당자리의 퍽이 다름 && 해당자리에 퍽이 있음
            hSum++;
        
        bfsMask >>= 2;
    }
    
    return hSum;
}
