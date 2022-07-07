#include <iostream>
#include <map>
#include <queue>
#include <cstdio>

using namespace std;

#define MaxNameLen 3
#define MaxPeopleNum 10000

typedef struct People ElementType;
typedef int Position;
struct People {
	int T;
	int P;
	char Name[MaxNameLen + 1];
	Position Index;
} PQueue[MaxPeopleNum];

int main() 
{
	//freopen("E:in.txt", "r", stdin);
	map<string, int> Group;
	queue<ElementType> Que;
	char Name[MaxNameLen + 1];
	int N, M, L, i, j;
	
	scanf("%d %d", &N, &M);
	for (i = 0; i < M; i++) {
		scanf("%d", &L);
		for (j = 0; j < L; j++) {
			scanf("%s", Name);
			Group[Name] = i;
		}
	}
	for (i = 0; i < N; i++) {
		scanf("%s %d %d", &PQueue[i].Name, &PQueue[i].T, &PQueue[i].P);
		if (PQueue[i].P > 60) {
			PQueue[i].P = 60;
		} 
		PQueue[i].Index = i;
		Que.push(PQueue[i]);
	}
	
	int CurrentTime, SumWaitTime, WindowsTime, WaitTime;
	ElementType X;
	bool Visited[N] = {false};
		
	CurrentTime = SumWaitTime = WindowsTime = WaitTime = 0;
	while (!Que.empty()) {
		X = Que.front();
		Que.pop();
		if (!Visited[X.Index]) {  //���û�б�ɾ�� 
			Visited[X.Index] = true;
			printf("%s\n", X.Name);
			CurrentTime += WindowsTime; //�������һ������ 
			if (X.T <= CurrentTime) {   //����˿���ǰ�������ۼӵȴ�ʱ�� 
				WaitTime = CurrentTime - X.T;
				SumWaitTime += WaitTime;
			} else {
				CurrentTime = X.T;  //������µ�ǰʱ�䣬Ϊ�˿͵���ʱ�� 
			} 
			WindowsTime = X.P; //��ӵ�ǰ����
			for (i = X.Index + 1; i < N; i++) {
				if (!Visited[i] && Group.count(X.Name) && Group.count(PQueue[i].Name) && (Group[X.Name] == Group[PQueue[i].Name])) {  //�ж��Ƿ���ͬһ������Ȧ 
					if (CurrentTime + WindowsTime >= PQueue[i].T) {  //�����ǰ�������ǰ�������Ѿ��������������� 
						SumWaitTime += (CurrentTime + WindowsTime - PQueue[i].T);  //�ۼ����ѵȴ�ʱ�� 
						Visited[i] = true;
						printf("%s\n", PQueue[i].Name);
						WindowsTime += PQueue[i].P;
					}
				}
			}
		}		
	}
	
	printf("%.1f", (double)SumWaitTime / (double)N);
	
    //fclose(stdin);
    return 0;
}
