#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxVertexNum 101 //��󶥵�����ע�ⶥ���±��1��ʼ 

typedef int Vertex;

typedef Vertex ElementType;
typedef Vertex SetName;
typedef ElementType SetType[MaxVertexNum]; //���鼯 

typedef int Cost;
typedef int Build;
typedef struct ENode *PtrToENode;   //�� 
struct ENode {
	Vertex V1;
	Vertex V2;
	Cost C;
	Build B;
};
typedef PtrToENode Edge;

typedef struct AdjVNode *PtrToAdjVNode; //�ٽӵ� 
struct AdjVNode {
	Vertex Adj;
	PtrToAdjVNode Next;
	Cost C;
	Build B;
};

typedef struct VNode {     //ͷ�ڵ� 
	PtrToAdjVNode FirstEdge;   
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode; //�ڽӱ� 
struct GNode {
	AdjList G;
	int Nv;  //������ 
	int Ne;  //���� 
};
typedef PtrToGNode LGraph;

LGraph CreateGraph(int VertexNum)  //����һ����ͼ 
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

void InsertEdge(LGraph Graph, Edge E)  //ͷ�巨����� 
{
	PtrToAdjVNode NewNode;
	
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->Adj = E->V2;
	NewNode->C = E->C;
	NewNode->B = E->B;
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;
	
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode)); //����ͼ����Ҫ�����<V2, V1> 
	NewNode->Adj = E->V1;
	NewNode->C = E->C;
	NewNode->B = E->B;
	NewNode->Next = Graph->G[E->V2].FirstEdge;
	Graph->G[E->V2].FirstEdge = NewNode;
	
	Graph->Ne++;
}

LGraph BuildGraph()  //��ȡ����, �ѱ߲���ͼ�� 
{
	int N;
	LGraph Graph;
	Edge E;
	
	scanf("%d", &N);
	Graph = CreateGraph(N);
	N = N * (N - 1) / 2;
	while (N--) {
		E = (Edge)malloc(sizeof(struct ENode));
		scanf("%d %d %d %d", &E->V1, &E->V2, &E->C, &E->B);
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
		if (Child != N - 1 && ESet[Child].C > ESet[Child + 1].C) { //����ҽ��Ȩ�ظ�С�������ӽڵ� 
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
				ESet[ECount].C = P->C;
				ESet[ECount++].B = P->B;	
			}
		}
	}
	for (ECount = Graph->Ne / 2 - 1; ECount >= 0; ECount--) { //������С�� 
		PercDown(ESet, ECount, Graph->Ne);
	}
}

void Swap(Edge E1, Edge E2)  //ע���ǽ������ߵ�ֵ
{
	Vertex V1, V2;
	Cost C;
	Build B;
	
	V1 = E1->V1, V2 = E1->V2, C = E1->C, B = E1->B;
	E1->V1 = E2->V1, E1->V2 = E2->V2, E1->C = E2->C, E1->B = E2->B;
	E2->V1 = V1, E2->V2 = V2, E2->C = C, E2->B = B;
}

int GetEdge(Edge ESet, int CurrentSize)
{
	Swap(&ESet[0], &ESet[CurrentSize - 1]); //����С�������һ���߽���
	PercDown(ESet, 0, CurrentSize - 1); //������
	
	return CurrentSize - 1; 
}

SetName Find(SetType S, ElementType X)  //Ѱ�Ҹ��ڵ㲢ѹ��·�� 
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

bool CheckCycle(SetType VSet, Vertex V1, Vertex V2) //����Ƿ񹹳ɻ�· 
{
	Vertex Root1, Root2;
	
	Root1 = Find(VSet, V1);
	Root2 = Find(VSet, V2);
	if (Root1 == Root2) {
		return false;
	} else {
		Union(VSet, Root1, Root2); //���ǵĻ�����V1, V2����һ������ 
		return true;
	}
}

void InitVSet(SetType VSet, int Nv) //��ʼ�����鼯��ע�����±��1��ʼ 
{
	int i;
	
	for (i = 1; i <= Nv; i++) {
		VSet[i] = -1;
	}
}
 
int CheckEdge(SetType VSet, Edge ESet, int Ne) //����ѽ��õĵ�·
{
	int ECount, i;
	
	ECount = 0;
	for (i = 0; i < Ne; i++) {
		if (ESet[i].B) {  //����Ѿ������˵�·
			if (CheckCycle(VSet, ESet[i].V1, ESet[i].V2)) { //��������ɻ�· 
				ECount++;	   //�ۼӱ���
			}
		}
	}
	
	return ECount;	
} 
 
int Kruskal(LGraph Graph) 
{
	Cost TotalCost;
	int ECount, NextEdge; 
	Edge ESet; 
	SetType VSet; 
	
	NextEdge = Graph->Ne, TotalCost = 0;
	//��ʼ����С�� 
	ESet = (Edge)malloc(sizeof(struct ENode) * Graph->Ne);
	InitESet(ESet, Graph);
	//��ʼ�����鼯 
	InitVSet(VSet, Graph->Nv);
	//ͳ���Ѿ��õ��ı��� 
	ECount = CheckEdge(VSet, ESet, Graph->Ne);
	while (ECount < Graph->Nv - 1) {  //��С�������ı������ڽ����-1 
		NextEdge = GetEdge(ESet, NextEdge);  //����С���еõ�Ȩ����С��һ���� 
		if (CheckCycle(VSet, ESet[NextEdge].V1, ESet[NextEdge].V2) == true) { //��������ɻ�· 
			TotalCost += ESet[NextEdge].C;   //�ۼ�Ȩ�� 
			ECount++;                        //�ۼӱ��� 
		}
	}
	free(ESet); //�ǵ��ͷ��ڴ� 
	
	return TotalCost;
}

void DestroyGraph(LGraph Graph) //�����ڽӵ㲢�ͷ��ڴ� 
{
	Vertex V;
	PtrToAdjVNode P, Temp;
	
	for (V = 1; V <= Graph->Nv; V++) {
		for (P = Graph->G[V].FirstEdge; P; P = Temp) {
			Temp = P->Next;
			free(P);
		}
	}
	free(Graph);
}

int main() 
{
	LGraph Graph; 
	int result;
	
	Graph = BuildGraph();
	printf("%d", Kruskal(Graph));
	DestroyGraph(Graph); //�ǵ��ͷ��ڴ� 

    return 0;
}
