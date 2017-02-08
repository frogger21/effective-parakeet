#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

struct edge {
	int vertex;
	double length;
};

struct vertex {
	bool explored;
	vector<edge> connections;
	vertex() {
		explored = false;
	}
};

struct edgeMin {
	int wStar;
	double length;
};

void printExplored(vertex *G, const int &n) {
	cout << "Printing vertex: Explored or Unexplored\n";
	for (int i = 0; i < n; i++)	{
		if (G[i].explored == true) {
			std::cout << "Vertex " << i + 1 << " = " << "Explored\n";
		} else {
			std::cout << "Vertex " << i + 1 << " = " << "Unexplored\n";
		}
	}
}

void printGraph(vertex *G, const int &n) {
	cout << "Printing Graph!\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < G[i].connections.size(); j++) {
			std::cout << "Vertex " << i + 1 << " -> " << G[i].connections[j].vertex << ", Edge Length: ";
			std::cout << G[i].connections[j].length << endl;
		}
	}
}

//for minHeap
bool compare(edgeMin &A, edgeMin &B) {
	return (A.length > B.length);
}

//is new value smaller than older value?
double returnMin(double *A, int &w, double comparison) {
	if (A[w - 1] > comparison) {
		return comparison;
	} else {
		return A[w - 1];
	}
}

//find element within heap
int find_heap(vector<edgeMin> Heap, int Find) {
	//just linaer serach O(n)
	edgeMin temp;
	for (int i = 0; i < Heap.size(); i++) {
		if (Heap[i].wStar == Find) {
			return i;
		}
	}
	return -1; //did not find
}

//dijkstra with heap
void dijkstraHeap(vertex *G, const int &n, const int &source){
	edgeMin temp1; temp1.length = 0; temp1.wStar = source;
	vector<edgeMin> minHeap; minHeap.push_back(temp1);
	push_heap(minHeap.begin(),minHeap.end(),compare);
	for (int i = 1; i <= n; i++) {
		if (i != source) {
			temp1.length = INFINITY; temp1.wStar = i;
			minHeap.push_back(temp1);
			push_heap(minHeap.begin(), minHeap.end(), compare);
		}
	}
	int u, v, pos;
	double d, uLen;
	while (minHeap.empty()==false) {
		temp1 = minHeap.front();
		minHeap[0] = minHeap[minHeap.size() - 1];
		minHeap.pop_back();
		make_heap(minHeap.begin(), minHeap.end(), compare);
		u = temp1.wStar; uLen = temp1.length;
		cout << "s --> v, (" << source << ") = (" << u << "), SP = " << uLen << endl;
		for (int i2 = 0; i2 < G[u - 1].connections.size(); i2++) {
			v = G[u - 1].connections[i2].vertex; 
			temp1.wStar = v; temp1.length = G[u - 1].connections[i2].length;
			//is v in minHeap?
			pos = find_heap(minHeap, v);
			if(pos != -1) {
				if (minHeap[pos].length > (uLen + temp1.length)) {
					minHeap[pos].length = uLen + temp1.length;
					make_heap(minHeap.begin(),minHeap.end(),compare); //rearrange as heap
				}
			}
		}
	}
}

//dijkstra without heap
void dijkstra(vertex *G, const int &n, const int &source) {
	//input: directed graph G=(V,E), V = vertices, E = edges
	set<int> X; X.insert(source); //X = vertices processed so far
	set<int> V; //set of vertices of graph G
	for (int i = 1; i <= n; i++) {V.insert(i);}
	double *A = new double[n];  A[source - 1] = 0; //computed shortest path distances
	for (int i = 1; i < n; i++) { A[i] = INFINITY;} //initialize as infinity
	//need to grow X by one node
	int v, w;
	double d;
	while (X != V){
		//loop through all edges of set(X) -> set(V-X)
		set<int>::iterator i1;
		edgeMin temp; temp.length = INFINITY;
		for (i1 = X.begin(); i1 != X.end(); ++i1) {
			v = *i1;
			//loop through v->w where w is in set(V-X)
			for (int i2 = 0; i2 < G[v - 1].connections.size(); i2++) {
				w = G[v - 1].connections[i2].vertex;
				set<int>::iterator i3;
				i3 = X.find(w); //find w in X
				if (i3 == X.end()) {
					//w is not in X
					d = G[v - 1].connections[i2].length + A[v - 1]; //v->w eLength + A[v]
					A[w - 1] = returnMin(A,w,d);
					if (A[w - 1] < temp.length) {
						temp.length = A[w - 1];
						temp.wStar = w;
					}
				}
			}
		} //end of first for loop
		X.insert(temp.wStar);
	} //end of while loop

	std::cout << "Dijkstra Shortest Path\n";
	for (int i = 0; i < n; i++) {
		std::cout << "s --> v, (" << source << ") --> (" << i + 1 << "), SP = " << A[i] << endl;
	}
	delete[] A;
}

int main(){
	//assume vertices are numbered 1 to n
	const int nVertices = 4; //# of vertices
	const int sVertex = 1; //the source vertex
	vertex *G = new vertex[nVertices]; //graph network
	const string infile = "D:/a1.txt";
	
	//read file: each row: A B edgeLength; where: A->B with length edgeLength
	ifstream inputFile; 
	int iA, iB, max;
	max = 0;
	double iEdgeLength; //iA->iB has edge length iEdgeLength;
	edge temp;
	inputFile.open(infile);
	while (!inputFile.eof()) {
		inputFile >> iA >> iB >> iEdgeLength;
		temp.vertex = iB; temp.length = iEdgeLength;
		G[iA - 1].connections.push_back(temp); //vertex iA has connections to iB with edge length iEdgeLength
		if (iA > max) { max = iA; };
		if (iB > max) { max = iB; };
	}
	inputFile.close();
	
	//do stuff
	//printExplored(G, nVertices);
	printGraph(G, nVertices);
	cout << "dijkstra without minheap\n";
	dijkstra(G, nVertices, sVertex);
	cout << "dijkstra with minheap\n";
	dijkstraHeap(G, nVertices, sVertex);
	//done
	delete[] G;
	system("PAUSE");
	return 0;
}
