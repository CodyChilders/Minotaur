//Cody Childers
//cchilder@ucsc.edu
//Kyle Sullivan
//kyrsulli@ucsc.edu

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <climits>
#include <vector>

#include "List.h"
#include "Graph.h"

#define and &&
#define or ||
#define not !

using namespace std;

//colors! :D
typedef enum colors{
	red, green, blue, rg, rb, gb, rgb
//   0      1     2    3   4   5   6
}colors;

//helper function to streamline the constructors
void Graph::prepGraph(int n){
	order = n;
	size = 0;
	source = NIL;
	color = new char[n+1];//static_cast<char*>(calloc(n + 1, sizeof(char)));
	distance = new int[n+1];//static_cast<int*>(calloc(n + 1, sizeof(int)));
	parent = new int[n+1];//static_cast<int*>(calloc(n + 1, sizeof(int)));
	adj = new List*[n+1];//static_cast<List**>(calloc(n + 1, sizeof(List*)));
	edgeDist = new List*[n+1];//static_cast<List**>(calloc(n + 1, sizeof(List*)));
	edgeColor = new List*[n+1];//static_cast<List**>(calloc(n + 1, sizeof(List*)));
	//discover = static_cast<int*>(calloc(n + 1, sizeof(int)));
	//finish = static_cast<int*>(calloc(n + 1, sizeof(int)));
	int i;
	for(i = 0; i <= n; i++){
		color[i] = 'w';
		distance[i] = INF;
		parent[i] = NIL;
		adj[i] = new List();
		edgeDist[i] = new List();
		edgeColor[i] = new List();
	}
}

Graph::Graph(int n){
	prepGraph(n);
}

Graph::Graph(ifstream& in){
	int n;
	in >> n;
	prepGraph(n);
	while(!in.eof()){
		int i, j, weight, color;
		in >> i;
		in >> j;
		in >> weight;
		in >> color;
		//increment values by 1 so they fit
		//into existing graph structure
		i += 1;
		j += 1;
		addEdge(i, j, weight, color);
		//cout << "added arc " << i << " " << j << endl;
	}		
}

Graph::Graph(Graph* G){
	int n = G->getOrder();
	int v;
	int color;
	int weight;
	prepGraph(n);
	for(int u = 0; u < order; u++){
		if(G->adj[u]!=NULL && G->adj[u]->isEmpty()!=true){
			List* temp = G->adj[u]->copyList();
			List* tempcolor = G->edgeColor[u]->copyList();
			List* tempweight = G->edgeDist[u]->copyList();
			temp->moveTo(0);
			tempcolor->moveTo(0);
			tempweight->moveTo(0);
			while(temp->offEnd()!=true){
				v = temp->getCurrent();
				color = tempcolor->getCurrent();
				weight = tempweight->getCurrent();
				addArc(u, v, weight, color);
				temp->moveNext();
				tempcolor->moveNext();
				tempweight->moveNext();
			}
			delete temp;
			delete tempcolor;
			delete tempweight;
		}
	}
	
	G->halveSize();
}

Graph::~Graph(){

	delete color;
	delete distance;
	delete parent;
	//delete discover;
	//delete finish;
	int i;
	for(i = 0; i <= order; i++){
		delete adj[i];
		delete edgeDist[i];
		delete edgeColor[i];
	}
	delete adj;
	delete edgeDist;
	delete edgeColor;

}

/***************** Access functions ***********************************************/

int Graph::getOrder(){
	return order;
}

int Graph::getSize(){
	return size;
}

int Graph::getSource(){
	return source;
}

int Graph::getParent (int u){
	return parent[u];
}

int Graph::getDist(int u){
	return distance[u];
}
/*
int Graph::getDiscover(int u){
	return discover[u];
}

int Graph::getFinish(int u){
	return finish[u];
}

void Graph::getPath(CLinkedList* L, int u){
	int dist = distance[u];
	int curr = u;
	int temp;
	CLinkedList* T = new CLinkedList();
	CLinkedList* R = new CLinkedList();
	while(dist > 0){
		T = adj[curr]->copyList();
		T->moveTo(0);
		while(!T->offEnd()){
			if(distance[T->getCurrent()]<dist){
				dist = getDist(T->getCurrent());
				temp = T->getCurrent();
			}
			T->moveNext();
		}
		//freeCLinkedList(&T);
		R->insertFront(curr);
		curr = temp;
	}
	R->insertFront(getSource());
	R->moveTo(0);
	while(!R->offEnd()){
		L->insertBack(R->getCurrent());
		R->moveNext();
	}
	//freeCLinkedList(&R);
}
*/
void Graph::getAdjacencyList(List* l, int u){
	if(!adj[u]->isEmpty()){
		l = adj[u]->copyList();
	}
}

/**************** Manipulation procedures ****************************************/
void Graph::makeNull(){
	int i = 0;
	int n = getOrder();
	for(i = 0; i <= n; i++){
		color[i] = 'w';
		distance[i] = INF;
		parent[i] = NIL;
		adj[i]->makeEmpty();
	}
	size = 0;
	source = NIL;
}

void Graph::halveSize(){
	size = size/2;
}

void Graph::addEdge(int u, int v, int weight, int color){
	if(u >= 1 && u <= getOrder() && v >= 1 && v <= getOrder()){
		//add going both directions
		addArc(u, v, weight, color);
		addArc(v, u, weight, color);
		//will increment size twice, so use this to cut it down to 1 edge
		size--;
	}
}

void Graph::addArc(int u, int v, int weight, int color){
//	if(u < 1 || u > getOrder() || v < 1 || v > getOrder()){
//		exit(1);
//	}
	//adj[u]->insertBack(v);
	size++;
	edgeColor[u]->insertBack(color);
	edgeDist[u]->insertBack(weight);
	adj[u]->insertBack(v);
}

//originally written as an equal weight breadth first search,
//this has been adapted for BFS with weighted edges using
//Dijkstra's algorithm
//function is very long, but there wasn't an easy way
//to break it down, so it was left huge
void Graph::BFS(int s, int colorCheck){ //source and color
	int testValue;
	for(int i = 0; i <= getOrder(); i++){
		color[i] = 'w';
		distance[i] = INF;
		parent[i] = NIL;
	}
	bool flag = true;
	color[s] = 'g';
	distance[s] = 0;
	source = s;//assign the source of the search
	int i;
	//create several lists, using them to compare several
	//different properties of a node at the same time
	List* l1 = new List();
	List* l2 = new List();
	List* l3 = new List();
	List* l4 = new List();
	int u = 0, v;
	int smallestVal = INF;
	for(int x = 1; x < 50; x++){//iterate through 50 nodes
		l1->insertBack(x);
	}
	while(!l1->isEmpty()){
		u = l1->getFront();
		smallestVal = distance[u];
		l1->moveTo(0);
		while(l1->getCurrent() != l1->getBack() && !l1->offEnd()){
			testValue = l1->getCurrent();
			if(distance[testValue] < distance[u])
				u = testValue;
			l1->moveNext();
		}
		if(l1->getLength() == 1)
			break;
		l1->moveTo(0);
		while(l1->getCurrent()!=u)
			l1->moveNext();
		l1->deleteCurrent();
		if(distance[u] == INF)
			break;
		if(!adj[u]->isEmpty())
			l2 = adj[u]->copyList();
		if(!edgeColor[u]->isEmpty())
			l3 = edgeColor[u]->copyList();
		if(!edgeDist[u]->isEmpty())
			l4 = edgeDist[u]->copyList();
		l2->moveTo(0);
		l3->moveTo(0);
		l4->moveTo(0);
		//check if we need to relax, do so if needed
		while(!l2->offEnd() && !l2->isEmpty()){
			l1->moveTo(0);
			v = l2->getCurrent();
			flag = false;
			while(!l1->offEnd() && l1){
				if(l1->getCurrent() == v)
					flag = true;
				l1->moveNext();
			}
			if(colorCheck != 1 && colorCheck != 0)
					flag = true;
			else{ 
				if(colorCheck == l3->getCurrent())
					flag = true;
				else
					flag = false;
			}
			if(flag == true){
				i = distance[u] + l4->getCurrent();
				if(i < distance[v]){
					distance[v] = i;
					parent[v] = u;
				}
			}
			l2->moveNext();
			l3->moveNext();
			l4->moveNext();
		}
		delete l2;
		delete l3;
		delete l4;
	}
	delete l1;
}

inline bool Graph::colorApprover(int current, bool r, bool g, bool b){
	if(current == red && r) return true;
	if(current == green && g) return true;
	if(current == blue && b) return true;
	return false;
}

//Prim's algorithm
void Graph::Prim(int s, int colorCheck){

	//gather flags based on input
	bool red = false;
	bool green = false;
	bool blue = false;
	switch(colorCheck){
		case 0: red = true; break;
		case 1: green = true; break;
		case 2: blue = true; break;
		case 3: red = green = true; break;
		case 4: red = blue = true; break;
		case 5: green = blue = true; break;
		case 6: red = green = blue = true; break;
	}
	//G++ wouldn't let me use enum type in a switch, so it is numbers
	//cout << red << green << blue<< endl;

//Pseudocode for Prim's algorithm
//Prim(G, w, s)
////Input: undirected connected weighted graph G = (V,E) in adj list representation,
//source vertex s in V
////Output: p[1..|V|], representing the set of edges composing an MST of G
//01 for each v in V
//02 color(v) <- WHITE
//03 key(v) <- infinity
//04 p(v) <- NIL
//cout << "TEST";
	for(int i = 0; i <= getOrder(); i++){
		color[i] = 'w';
		distance[i] = INF;
		parent[i] = NIL;
	}
	int smallest = INT_MAX;
	int temp;
	int u;
	int v;
	//bool go;
//05 Q <- empty list // Q keyed by key[v]
	List* Q = new List();
//	List* adjList = new List();
//	List* weightList = new List();
//	List* edgeColorList = new List();
//06 color(s) <- GRAY
	color[s] = 'g';
//07 Insert(Q, s)
	Q->insertBack(s);
//08 key(s) <- 0
	distance[s] = 0;
//09 while Q != empty
	while(!Q->isEmpty()){
		smallest = 10000;
		temp = -1;
//10 u <- Extract-Min(Q)
		Q->moveTo(0);
		while(!Q->offEnd()){
			u = Q->getCurrent();
			if(distance[u] < smallest){
				smallest = distance[u];
				temp = u;
			}
			Q->moveNext();
		}
		Q->moveTo(0);
		//if(Q->isEmpty()){break;}
		while(!Q->offEnd()&&!Q->isEmpty()){
			if(Q->getCurrent()==temp)
				Q->deleteCurrent();
			if(!Q->isEmpty()&&!Q->offEnd()) Q->moveNext();
		}
		u = temp;
//11 for v in Adj[u]
		if(!adj[u]->isEmpty()){
		//	go = true;
			List* adjList = adj[u]->copyList();
			List* weightList = edgeDist[u]->copyList();
			List* edgeColorList = edgeColor[u]->copyList();
			adjList->moveTo(0);
			weightList->moveTo(0);
			edgeColorList->moveTo(0);
			while(!adjList->offEnd()){
				v = adjList->getCurrent();
			Q->moveTo(0);
// if color(v) = WHITE
				if(color[v] == 'w' and colorApprover(edgeColorList->getCurrent(), red, green, blue) ){
//13 then color(v) <- GRAY
					color[v] = 'g';
//14 Insert(Q,v)
					Q->insertBack(v);
//15 key(v) <- w(u,v)
					distance[v] = weightList->getCurrent();
//16 p(v) <- u
					parent[v] = u;
				}
//17 elseif color(v) = GRAY
				else if(color[v] == 'g'){
//18 then if key(v) > w(u,v)
					if(distance[v] > weightList->getCurrent() and colorApprover(edgeColorList->getCurrent(), red, green, blue)){
//19 then key(v) <- w(u,v)
						distance[v] = weightList->getCurrent();
//20 p(v) <- u
						parent[v] = u;
					}
				}
				adjList->moveNext();
				weightList->moveNext();
				edgeColorList->moveNext();
			}
//21 color(v) <- BLACK
			color[u] = 'b';
			delete adjList;
			delete weightList;
			delete edgeColorList;
		}
 //cout << "WHAT\n";
	}
	delete Q;
}
//22 return(p)




/*
int Graph::visit( int u, int time){
	color[u] = 'g';
	discover[u] = time++;
	int temp = 0;
	if(adj[u]==NULL){
		printf("%d", u);
		exit(1);
	}
	if(adj[u]!=NULL){
		if(!isEmpty(adj[u])){
			CLinkedList* L = copyCLinkedList(adj[u]);
			moveTo(L, 0);
			while(!offEnd(L)){
			temp = getCurrent(L);
				if(color[temp]=='w'){
					parent[temp] = u;
					time = visit(G, temp, time);
				}
				moveNext(L);
			}
			freeCLinkedList(&L);
		}
	}
	color[u] = 'b';
	finish[u] = time++;
	return time;
}

CLinkedList* Graph::DFS(CLinkedList* L){
	if(getLength(L)!=getOrder(G)){
		printf("Error! Invalid Stack size!\n");
		printf("Order = %d, Length = %d", getOrder(G), getLength(L));
		exit(1);
	}
	CLinkedList* N = newCLinkedList();
	moveTo(L, 0);
	int u = 1;
	for(u = 1; u <= order; u++){
		color[u] = 'w';
		parent[u] = NIL;
	}
	int time = 1;
	while(!offEnd(L)){
		u = getCurrent(L);
		if(color[u]=='w')
			time = visit(G, u, time);
		moveNext(L);
	}
	//sort algorithm here
	moveTo(L, 0);
	int n = 0;
	int temp = 0;
	while(!offEnd(L)){
		int stop = 0;
		n = getCurrent(L);
		if(isEmpty(N))
			insertFront(N, n);
		else{
			moveTo(N, 0);
			while(!offEnd(N) && stop == 0){
				temp = getCurrent(N);
				if(finish[n] <= finish[temp]){
					insertBeforeCurrent(N, n);
					stop = 1;
				}
				moveNext(N);
			}
			if(stop == 0)
				insertBack(N, n);
		}
		moveNext(L);
	}
	return N;
}
*/

/*************** Other Functions *************************************************/

void Graph::printGraph(){
	cout<<"Note: Graph structure was set up to take in node values starting at 1" 
	    <<", so all values have been incremented by 1.  Example: what was node 5 in the"
		<<" input file is represented as 6 here"<< endl << endl;
		
	cout << "Pattern:\n NodeNumber: adj (weight, color) ..." << endl;
	cout<<"Graph represented as an edge list: " << endl;
	for(int x = 1; x <= getOrder(); x++){
		cout << x << ": ";
		//adj[x]->printList();
		adj[x]->moveTo(0);
		edgeDist[x]->moveTo(0);
		edgeColor[x]->moveTo(0);
		while(!adj[x]->offEnd() ){//since lists should be the same length, just uses 1 for exit condition
			cout << adj[x]->getCurrent() << "(" << edgeDist[x]->getCurrent() << ", ";
			     //<< edgeColor[x]->getCurrent() << ") ";
				 switch(edgeColor[x]->getCurrent()){
					case 0: cout <<"red"; break;
					case 1: cout <<"green"; break;
					case 2: cout <<"blue";break;
					default: cout <<"Unknown color";break;
				 }
				 cout <<") ";
			adj[x]->moveNext();
			edgeDist[x]->moveNext();
			edgeColor[x]->moveNext();
		}
		cout << endl;
	}
	cout << endl;
}

void Graph::printSpanningTree(){
	cout << "Spanning tree: " << endl;
	int totalDist = 0; // add to this as we go along
	
	for(int i = 1; i <= order; i++){
		if(parent[i] != NIL){
			cout << i-1 << " discovered from " << parent[i]-1 << endl;
			//see how long it was from the parent to i
			//rename to make it easy
			int source = parent[i];
			int destination = i;
			bool found = false;
			for(adj[source]->moveTo(0); not(adj[source]->offEnd()); adj[source]->moveNext()){
				if(!found){
					if(adj[source]->getCurrent() == destination){
						edgeDist[source]->moveTo(adj[source]->getIndex());
						totalDist += edgeDist[source]->getCurrent();
						found = true;
					}
				}
			}
		}else{
			cout << i-1 << " was the source vertex" << endl;
		}
	}
	cout << endl << "The weight of the minimum spanning tree is: " << totalDist << endl;
}
/*
void Graph::transpose(){
	int n = order;
	Graph* temp = new Graph(n);
	CLinkedList* L = newCLinkedList();
	int u, v;
	for(u = 1; u <= order; u++){
		if(adj[u]!=NULL){
			if(!isEmpty(adj[u])){
				L = copyCLinkedList(adj[u]);
				moveTo(L, 1);
				for(v = 0; v < getLength(L); v++){
					moveTo(L, v);
					addArc(temp, getCurrent(L), u);
					moveNext(L);
				}
				freeCLinkedList(&L);
			}
		}
	}
	return temp;
}
*/
Graph* Graph::copyGraph(){
	//return transpose(transpose());
	/*Graph* g = new Graph(getOrder());
	for(int u = 0; u < getOrder(); u++){
		if(adj[u]!=NULL && adj[u]->isEmpty== false){
		}
	}*/
	return NULL;
}

