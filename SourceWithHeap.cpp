#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
using namespace std;

//*************
// CUSTOM HEAP
//*************
template <typename T2>
bool compare(T2 &A, T2 &B) {
	return A < B;  //max heap
}

template <typename T4, typename Pred4>
void j_heapify(vector<T4> &p, int N, Pred4 predicate) {
	for (int k = N / 2; k >= 1; k--) {
		j_sinkheap(p, N, k, predicate);
	}
}

template <typename T1, typename Pred1>
void j_swimheap(vector<T1> &p, int N, int k, Pred1 predicate) {
	T1 temp;
	while (k > 1 && (predicate(p[k / 2], p[k]))) {
		temp = p[k];
		p[k / 2] = temp;
		k = k / 2;
	}
}

template <typename T3, typename Pred2>
void j_sinkheap(vector<T3> &p, int N, int k, Pred2 predicate) {
	T3 temp;
	while (2 * k <= N) {
		int j = 2 * k; //the child & j++ its other child
		if (j < N && predicate(p[j], p[j + 1])) {
			j += 1;
		} //we have chosen our favourite child
		if (predicate(p[k], p[j]) == false) {
			break; //parent dominates favourite child nothing to do
		}
		else {
			//child dominates the parent so swap
			temp = p[j];
			p[j] = p[k];
			p[k] = temp;
			k = j;
		}
	}
}

//*************
//structs
//*************
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

//parses string line and looks for numbers to put into a list
//ignores all non number ascii
void split(string line, vector<int> &v)
{
	string::size_type sz;
	int NumCount = 0;
	int startPos, numLen, tracker, StringSize;
	int theNumber;
	StringSize = line.size();
	bool Num = false;
	bool temp = false;
	bool temp2 = false;
	for (int i = 0; i < StringSize; i++)

	{
		//is it a number?
		temp = false;
		if (line[i] >= 48 && line[i] <= 57)
		{
			temp = true;
		}
		//if it's not a number then 
		if (temp == false)
		{
			Num = false;
		}
		//previous char was not a number but current is: we got a new number
		if (Num == false && temp == true)
		{
			Num = true;
			startPos = i;
			numLen = 1; //we don't know yet fully
			tracker = i + 1;
			while (tracker < StringSize)
			{
				temp2 = false;
				if (line[tracker] <= 57 && line[tracker] >= 48)
				{
					temp2 = true;
				}
				if (temp2 == true)
				{
					numLen += 1;
					tracker++;
				}
				else
				{
					// don't need to check these anymore cuz we already know 
					//it's # -1 b/c we i++ in for loop
					i += numLen - 1;
					break;
				}
			}
			string test2 = line.substr(startPos, numLen);
			theNumber = stoi(test2, &sz); //convert string to long
			NumCount += 1;
			v.push_back(theNumber);
		}
	}
}

//is new value smaller than older value?
double returnMin(double *A, int &w, double comparison) {
	if (A[w - 1] > comparison) {
		return comparison;
	}
	else {
		return A[w - 1];
	}
}

//dijkstra without heap
void dijkstra(vertex *G, const int &n, const int &source) {
	//input: directed graph G=(V,E), V = vertices, E = edges
	set<int> X; X.insert(source); //X = vertices processed so far
	set<int> V; //set of vertices of graph G
	for (int i = 1; i <= n; i++) { V.insert(i); }
	double *A = new double[n];  A[source - 1] = 0; //computed shortest path distances
	for (int i = 1; i < n; i++) { A[i] = INFINITY; } //initialize as infinity
													 //need to grow X by one node
	int v, w;
	double d;
	while (X != V) {
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
					A[w - 1] = returnMin(A, w, d);
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

//for minHeap
bool compare2(edgeMin &A, edgeMin &B) {
	return (A.length > B.length);
}

//find element within heap
int find_heap(vector<edgeMin> &Heap, int Find) {
	//just linaer serach O(n)
	edgeMin temp;
	for (int i = 0; i < Heap.size(); i++) {
		if (Heap[i].wStar == Find) {
			return i;
		}
	}
	return -1; //did not find
}

//with my custom heap
void dijkstraJHeap(vertex *G, const int &n, const int &source) {
	//assume A[0] = NULL so vertices n is from A[1] to A[n]
	edgeMin temp1; temp1.length = NULL; temp1.wStar = NULL;
	vector<edgeMin> minHeap; //this is our minheap represented by vector
	minHeap.push_back(temp1); //NULL
	temp1.length = 0; temp1.wStar = source;
	minHeap.push_back(temp1);
	for (int i = 1; i <= n; i++){
		if (i != source) {
			temp1.length = INFINITY; temp1.wStar = i;
			minHeap.push_back(temp1);
		}
	}
	//heapify it
	j_heapify(minHeap,n,compare2);
	int len = n;
	int u, v, pos;
	double d, uLen;
	while (minHeap.size() > 1) {
		//remember minHeap[0] = NULL, so exit if size is 1
		temp1 = minHeap[1]; //grab min
		minHeap[1] = minHeap[minHeap.size() - 1]; //switch places
		minHeap.pop_back(); //pop the back now
		len -= 1;
		//restore heap order
		u = temp1.wStar; uLen = temp1.length;
		j_sinkheap(minHeap,len,1,compare2); //top is now from back so sink it
		cout << "s --> v, (" << source << ") --> (" << u << "), SP = " << uLen << endl;
		//explore its neighbours
		for (int i2 = 0; i2 < G[u - 1].connections.size(); i2++) {
			v = G[u - 1].connections[i2].vertex;
			temp1.wStar = v; temp1.length = G[u - 1].connections[i2].length;
			pos = find_heap(minHeap, v);
			if (pos != -1) {
				//it exists in minHeap
				if (minHeap[pos].length > (uLen + temp1.length)) {
					minHeap[pos].length = uLen + temp1.length;
					j_heapify(minHeap,len, compare2);
				}
			}
		}
	}
}

int main() {
	const string readfile = "D:/coursera/stanford data struc and algos/5/dijkstraData.txt";
	const long nVertices = 200; //# of vertices
	int sVertex = 1;
	vertex *G = new vertex[nVertices]; //graph network
	cout << "Reading input...\n";
	string line;
	vector<int>temp;
	edge temp2;
	ifstream inputFile;
	inputFile.open(readfile);
	if (inputFile.is_open()) {
		int vertex, count; count = 0;
		while (getline(inputFile, line)) {
			split(line, temp);
			count = 0;
			vertex = temp[0];
			for (int i = 1; i < temp.size(); i += 2) {
				temp2.vertex = temp[i];
				temp2.length = temp[i + 1];
				//cout << temp[i + 1] << endl;
				G[vertex - 1].connections.push_back(temp2);
				count += 1;
			}
			//cout << vertex << endl;
			temp.clear();
		}
		inputFile.close();
	}

	if (1 == 3) {
		for (int i = 1; i <= 200; i++) {
			for (int j = 0; j < G[i - 1].connections.size(); j++) {
				cout << i << "->" << G[i - 1].connections[j].vertex << ", l: " << G[i - 1].connections[j].length << endl;
			}
		}
	}

	cout << "Dijkstra without heap!\n";
	dijkstra(G, nVertices, sVertex);

	cout << "Dijkstra with custom heap!\n";
	dijkstraJHeap(G,nVertices,sVertex);

	system("PAUSE");
	return 0;
}