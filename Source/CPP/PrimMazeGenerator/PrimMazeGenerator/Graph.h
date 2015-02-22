//Cody Childers
//cchilder@ucsc.edu
//Kyle Sullivan
//kyrsulli@ucsc.edu

#include "List.h"
#include <climits>
#if !defined(_Graph_H_INCLUDE_)
#define _Graph_H_INCLUDE_

using namespace std;


const int INF = INT_MAX;  //largest int value.  if it gets this big, it'll break down anyway
const int NIL = 0;


class Graph
{
public:
	Graph(int n);
	Graph(ifstream& in);
	Graph(Graph* G);
	~Graph(void);
	
	int getOrder();
	int getSize();
	int getSource();
	int getParent(int u);
	int getDist(int u);
	void getAdjacencyList(List* L, int u);
	
	void makeNull();
	void halveSize();
	void addEdge(int u, int v, int weight, int color);
	void addArc(int u, int v, int weight, int color);
	void BFS(int s, int c);
	void Prim(int s, int color);
	void printGraph();
	void printSpanningTree();
	Graph* copyGraph();
	
private:
	bool colorApprover(int, bool ,bool , bool);
	void prepGraph(int n);
	int order;
	int size;
	int source;
	char* color;
	int* distance;
	int* parent;
	List** edgeDist;
	List** edgeColor;
	List** adj;
};

#endif