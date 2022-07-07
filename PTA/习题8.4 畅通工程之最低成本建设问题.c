#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxAdjNum 1001 

typedef int Vertex;

typedef Vertex ElementType;
typedef Vertex SetName;
typedef ElementType SetType[MaxAdjNum]; //���岢�鼯 

typedef int Cost;
typedef struct ENode *PtrToENode;   //�� 
struct ENode {
	Vertex V1;
	Vertex V2;
	Cost C;
};
typedef PtrToENode Edge;

typedef struct AdjVNode *PtrToAdjVNode; //�ٽӵ� 
struct AdjVNode {
	Vertex Adj;
	PtrToAdjVNode Next;
	Cost C;
};

typedef struct VNode {     //ͷ�ڵ� 
	PtrToAdjVNode FirstEdge;   
} AdjList[MaxAdjNum];

typedef struct GNode *PtrToGNode; //�ڽӱ� 
struct GNode {
	AdjList G;
	int Nv;  //������ 
	int Ne;  //���� 
};
typedef PtrToGNode LGraph;

LGraph CreateGraph(int VertexNum)  //���ݶ���������һ����ͼ 
{
	LGraph Graph;
	Vertex V;
	
	Graph = (LGraph)malloc(sizeof(struct GNode));
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	for (V = 1; V <= VertexNum; V++) {
		Graph->G[V].FirstEdge = NULL;
	}
	
	return Graph;
}

void InsertEdge(LGraph Graph, Edge E)  //����ߣ�ͷ�巨 
{
	PtrToAdjVNode NewNode;
	
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->Adj = E->V2;
	NewNode->C = E->C;
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;
	
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->Adj = E->V1;
	NewNode->C = E->C;
	NewNode->Next = Graph->G[E->V2].FirstEdge;
	Graph->G[E->V2].FirstEdge = NewNode;
}

LGraph BuildGraph()  //��ȡ����, �ѱ߲���ͼ�� 
{
	int N, M;
	LGraph Graph;
	Edge E;
	
	scanf("%d %d", &N, &M);
	Graph = CreateGraph(N);
	Graph->Ne = M; //�趨ͼ�ı���
	while (M--) {
		E = (Edge)malloc(sizeof(struct ENode));
		scanf("%d %d %d", &E->V1, &E->V2, &E->C);
		InsertEdge(Graph, E);
	}
	
	return Graph; 
}

void PercDown(Edge ESet, int P, int N)  //������С�Ѳ������ӵ�һ����Ҷ���Ľ�㿪ʼ 
{
	int Parent, Child;
	struct ENode X;
	
	X = ESet[P];
	for (Parent = P; Parent * 2 + 1 < N; Parent = Child) {
		Child = Parent * 2 + 1;
		if (Child != N - 1 && ESet[Child].C > ESet[Child + 1].C) {
			Child++;
		}
		if (X.C > ESet[Child].C) {
			ESet[Parent] = ESet[Child];
		} else {
			break;
		}
	}
	ESet[Parent] = X;
}

void InitESet(Edge ESet, LGraph Graph)
{
	Vertex V;
	PtrToAdjVNode P;
	int ECount;
	
	ECount = 0;           
	for (V = 1; V <= Graph->Nv; V++) {  //�ѱߴ����С�� 
		for (P = Graph->G[V].FirstEdge; P; P = P->Next) {
			if (V < P->Adj) { //����ͼ�ı���˫��ģ�Ϊ�����ظ�¼�룬ֻ��ȡV1<V2�ı� 
				ESet[ECount].V1 = V;
				ESet[ECount].V2 = P->Adj;
				ESet[ECount++].C = P->C;	
			}
			
		}
	}
	for (ECount = Graph->Ne / 2 - 1; ECount >= 0; ECount--) {
		PercDown(ESet, ECount, Graph->Ne);
	}
}

void Swap(Edge E1, Edge E2)  //���������� 
{
	Vertex V1, V2;
	Cost C;
	
	V1 = E1->V1, V2 = E1->V2, C = E1->C;
	E1->V1 = E2->V1, E1->V2 = E2->V2, E1->C = E2->C;
	E2->V1 = V1, E2->V2 = V2, E2->C = C; 
}

int GetEdge(Edge ESet, int CurrentSize)
{
	Swap(&ESet[0], &ESet[CurrentSize - 1]); //����С�������һ���߽���
	PercDown(ESet, 0, CurrentSize - 1); //������
	
	return CurrentSize - 1; 
}

void InitVSet(SetType VSet, int Nv)
{
	int i;
	
	for (i = 1; i <= Nv; i++) {
		VSet[i] = -1;
	}
}

SetName Find(SetType S, ElementType X)  //Ѱ�Ҹ��ڵ㲢·��ѹ�� 
{
	if (S[X] < 0) {
		return X;
	}
	return S[X] = Find(S, S[X]);
}

void Union(SetType S, ElementType Root1, ElementType Root2)  //�ϲ� 
{ 
	if (S[Root1] < S[Root2]) {
		S[Root1] += S[Root2]; 
		S[Root2] = Root1;
	} else {
		S[Root2] += S[Root1]; 
		S[Root1] = Root2;
	}
}

bool CheckCycle(SetType VSet, Vertex V1, Vertex V2)
{
	Vertex Root1, Root2;
	
	Root1 = Find(VSet, V1);
	Root2 = Find(VSet, V2);
	if (Root1 == Root2) {
		return false;
	} else {
		Union(VSet, Root1, Root2);
		return true;
	}
}
 
int Kruskal(LGraph Graph, LGraph MST) //MSTΪ��С�����������ڽӱ�洢 
{
	Cost TotalCost;
	int ECount, NextEdge; 
	Edge ESet; //�ߵ���С�� 
	SetType VSet; //���㲢�鼯 
	
	ESet = (Edge)malloc(sizeof(struct ENode) * Graph->Ne);
	InitESet(ESet, Graph); 
	InitVSet(VSet, Graph->Nv);
	MST = CreateGraph(Graph->Nv);  
	TotalCost = ECount = 0;
	NextEdge = Graph->Ne;
	while (ECount < Graph->Nv - 1) {
		NextEdge = GetEdge(ESet, NextEdge);
		if (NextEdge < 0) {
			break;
		}
		if (CheckCycle(VSet, ESet[NextEdge].V1, ESet[NextEdge].V2) == true) { //��������ɻ�·��������С�������У��ۼӻ��� 
			InsertEdge(MST, ESet + NextEdge); //ָ������
			TotalCost += ESet[NextEdge].C;
			ECount++; 
		}
	}
	if (ECount < Graph->Nv - 1) {
		TotalCost = -1;
	}
	
	return TotalCost;
}

int main() 
{
	//freopen("E:in.txt", "r", stdin);
	LGraph Graph, MST;
	int result;
	
	Graph = BuildGraph();
	result = Kruskal(Graph, MST);
	if (result == -1) {
		printf("Impossible");
	} else {
		printf("%d", result);
	}
	//fclose(stdin);
    return 0;
}
 

