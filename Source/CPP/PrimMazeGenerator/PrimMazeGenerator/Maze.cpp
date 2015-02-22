#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "Graph.h"

#define and &&
#define or ||
#define not !

using namespace std;

//Prim's needs a color but we don't, so set a generic one here
const int color = 1;
//global string for the filename
const string outputFileName = "baseMaze.maze";

int coordinateConversion(int c1, int c2, int iConst);
void printMaze(Graph* g, int width, int height);
void printSolution(Graph* g);

int main(void){
	//set the seed for later
	srand(time(NULL));
	//get the i and j values
	int i, j;
	cout << "Welcome to the random maze generator program!!!" << endl;
	//get numbers from the user
	cout << "Enter the size of the maze: ";
	while(true){
		while(not (cin >> i) ){
			cout << endl << "Please enter an integer > 0: ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if(i <= 0){
			cout << "Please enter a positive integer." << endl;
			continue;
		}
		break;
	}
	
	//for some reason, it only worked for square mazes, and this was
	//faster than debugging
	j = i;

	//create our graph
	Graph* g = new Graph(i * j + 1);
	//cycle through values, adding edges of random weight
	for(int ii = 0; ii < i; ii++){
		for(int jj = 0; jj < j; jj++){
			//figure out which node we're looking at
			int current = coordinateConversion(ii, jj, i);
			
			//add the edge to the left
			if( (current - 1 > 0) and ( (current - 1) % i != 0 ) ){
				g->addEdge(current, current - 1, rand() % 100, color);
			}
			
			//to the right
			if( (current + 1 < i * j + 1) and (current % i != 0 ) ){
				g->addEdge(current, current + 1, rand() % 100, color);
			}
			
			//up
			if(current - i > 0){
				g->addEdge(current, current - i, rand() % 100, color);
			}
			
			//down
			if(current + i < i * j + 1){
				g->addEdge(current, current + i, rand() % 100, color);
			}
		}
	}
	
	//run prims to sift through edges
	g->Prim(1, color);

	printMaze(g, i, j);
	delete g;

	//we're done here
	return 0;
}

//this function returns a unique 1 index conversion, which
//corresponds to where in the graph this node goes
inline int coordinateConversion(int c1, int c2, int iConst){
        return iConst * c1 + c2 + 1;
}

//this primarily prints the maze, but it also assembles a new graph
//as it finds edges from Prim, used for dijkstra's solution to the
//maze
inline void printMaze(Graph* g, int width, int height){
	//create the file we need to write to
	ofstream maze (outputFileName, ios::out);
#define cout maze
	cout << width << endl;
	for(int i = 1; i < width * height + 1; i++){
		cout << '+';
		//see if a left/right edge is needed
		if( (g->getParent(i) == i + 1) or ( g->getParent(i + 1) == i) ){
			cout << "-";
		}else{
			cout << " ";
		}
		//return for next line
		if(i % width == 0){
			cout << endl;
			//fill in the intermediate row
			int lookingAt = i + 1;
			for(int j = 0; j < width; j++){
				if( (g->getParent(lookingAt + j) == lookingAt + j - width)
				   or (g->getParent(lookingAt + j - width) == lookingAt + j) ){
					cout << "|";
				}else{
					cout << " ";
				}
				cout << "j";
			}
			cout << endl;
		}
	}
#undef cout
}

inline void printSolution(Graph* g){
	int loopConst = g->getSize();
	for(int i = 1; i < loopConst * loopConst; i++){
		cout << i << ": " << g->getParent(i) << endl;
	}
}
