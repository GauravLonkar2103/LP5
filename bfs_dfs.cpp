#include<iostream>
#include<omp.h>
#include<stack>
#include<queue>
#include<ctime>
#include<vector>
using namespace std;
//vertices: The total number of vertices in the graph.
//edges: The total number of edges in the graph.
//graph: A 2D vector representing the adjacency list. It stores the connections between nodes.
class Graph{
	public:
		int vertices=6;
		int edges=5;
		//This is a 2D vector that stores the adjacency list of the graph. 
		//Each index of graph represents a vertex, and the corresponding vector holds the adjacent vertices (neighbors) for that vertex.
		vector<vector<int> > graph;// Adjacency list for the graph

//graph.resize(6): Initializes the graph with 6 vertices.
//The subsequent lines manually define the edges between vertices using the adjacency list representation.
	Graph(){
		graph.resize(6);
		graph[0].push_back(1);
		graph[1].push_back(0);
		graph[1].push_back(2);
		graph[1].push_back(3);
		graph[2].push_back(1);
		graph[2].push_back(4);
		graph[2].push_back(5);
		graph[3].push_back(1);
		graph[3].push_back(4);
		graph[4].push_back(2);
		graph[4].push_back(3);
		graph[5].push_back(2);
	}	
	vector<bool> visited;
	void addedge(int a, int b){//This function allows adding edges to the graph
		graph[a].push_back(b);
		graph[b].push_back(a);
	}
	//Prints the adjacency list for each vertex.
	void printGraph(){
		for(int i=0;i<vertices;i++){
			cout<<i<<"->";
			for(vector<int>::iterator j=graph[i].begin();j!=graph[i].end();j++){
				cout<<*j<<" ";
			}
			cout<<endl;
		}
	}
	void initialized_visits(){
		visited.assign(vertices,false);
	}
//	This is the standard DFS implementation using a stack. The function pushes the starting vertex i to the stack, marks it as visited, and then iterates over its neighbors.
//It uses a stack to simulate recursion in DFS, and it ensures that each vertex is visited only once.
	void dfs(int i){
		stack<int> s;
		s.push(i);
		visited[i]=true;
		while(!s.empty()){
			int curr=s.top();
			cout<<curr<< " ";
			s.pop();
			for(vector<int>::iterator j=graph[curr].begin();j!=graph[curr].end();j++){
				if(!visited[*j]){
					s.push(*j);
					visited[*j]=true;
				}
			}
		}
	}
//	This is the parallelized version of DFS using OpenMP.
//#pragma omp critical ensures that the stack operations (push and pop) are done atomically, which avoids race conditions.
//The #pragma omp parallel for is used to process all neighbors of a node in parallel. However, this is inefficient because stack modifications (push/pop) are inherently sequential.
	void parallel_dfs(int i){
		stack<int> s;
		s.push(i);
		visited[i]=true;
		while(!s.empty()){
			int current=s.top();
			cout<<current <<" ";
			#pragma omp critical
			s.pop();
			#pragma omp parallel for
			for(int k=0;k<graph[current].size();k++){
				int neighbor=graph[current][k];
				if(!visited[neighbor]){
					#pragma omp critical
					{
						s.push(neighbor);
						visited[neighbor]=true;
					}
				}
			}
		}
	}
	//This is the standard BFS implementation using a queue. It processes each node in a breadth-first manner.
	void bfs(int i){
		queue<int> q;
		q.push(i);
		visited[i]=true;
		while(!q.empty()){
			int current=q.front();
			q.pop();
			cout<<current<<" ";
			for(vector<int>::iterator j=graph[current].begin();j!=graph[current].end();j++){
				if(!visited[*j]){
					q.push(*j);
					visited[*j]=true;
				}
			}
		}
	}
	//This is the parallelized version of BFS, similar to the parallel DFS implementation, with parallelism applied to the iteration over neighbors.
	void parallel_bfs(int i){
		queue<int> q;
		q.push(i);
		visited[i]=true;
		while(!q.empty()){
			int curr=q.front();
			cout<<curr<<" ";
			#pragma omp critical
			q.pop();
			#pragma omp parallel for
			for(int k=0;k<graph[curr].size();k++){
				int neighbor=graph[curr][k];
				if(!visited[neighbor]){
					#pragma omp critical
					{
						q.push(neighbor);
						visited[neighbor]=true;
					}
				}
			}
		}
	}
};

//The main function creates a Graph object and performs the four graph traversal algorithms: DFS, parallel DFS, BFS, and parallel BFS.
//It measures and prints the time taken for each algorithm using clock().
int main(){
	Graph g;
	cout<<"Adjacency List\n";
	g.printGraph();
	clock_t start,end;
	cout<<"Depth First Search:\n";
	g.initialized_visits();
	start=clock();
	g.dfs(0);
	cout<<endl;
	end=clock();
	cout<<"Time taken: "<<double(end-start)/CLOCKS_PER_SEC*1000000<<"microseconds\n";
	cout<<"Parallel Depth First Search:\n";
	g.initialized_visits();
	start=clock();
	g.parallel_dfs(0);
	cout<<endl;
	end=clock();
	cout<<"Time taken: "<<double(end-start)/CLOCKS_PER_SEC*1000000<<"microseconds\n";
	cout<<"Breadth First Search:\n";
	g.initialized_visits();
	start=clock();
	g.bfs(0);
	cout<<endl;
	end=clock();
	cout<<"Time taken: "<<double(end-start)/CLOCKS_PER_SEC*1000000<<"microseconds\n";
	cout<<"Parallel Breadth First Search:\n";
	g.initialized_visits();
	start=clock();
	g.parallel_bfs(0);
	cout<<endl;
	end=clock();
	cout<<"Time taken: "<<double(end-start)/CLOCKS_PER_SEC*1000000<<"microseconds\n";
	return 0;
}

//Advantages and Disadvantages of Parallelism in this Code
//Advantages:
//
//Faster Execution: In theory, the parallel versions of DFS and BFS can run faster, especially for large graphs, since multiple neighbors can be processed concurrently.
//
//Better Resource Utilization: Parallel execution can make better use of multi-core processors.
//
//Scalability: As the graph size increases, parallelization can improve performance.
//
//Disadvantages:
//
//Overhead: OpenMP introduces overhead due to managing threads, especially for small graphs where the sequential version might actually run faster.
//
//Complexity: The code is more complex due to the need for synchronization using #pragma omp critical, which can limit the parallelism.
//
//Inefficiency in Stack Operations: The parallelization of stack operations (push and pop) in DFS is inherently inefficient because these operations are sequential by nature.
//
//Viva Questions and Answers
//What is the difference between DFS and BFS?
//Answer: DFS (Depth First Search) explores a graph by going deep into a branch before backtracking, using a stack. BFS (Breadth First Search) explores the graph level by level, using a queue.
//
//What is OpenMP and why is it used here?
//Answer: OpenMP is an API that supports multi-platform shared memory multiprocessing programming in C, C++, and Fortran. It is used here to parallelize graph traversal algorithms (DFS and BFS) to speed up the execution on multi-core processors.
//OpenMP is like a tool that helps you make your program run faster by using multiple processors (or cores) on your computer at the same time. It's a way to split a job into smaller parts and have different processors work on them at the same time.
//In the code, OpenMP is used to make the graph traversal (DFS and BFS) faster by doing some of the work in parallel. So, instead of the program working on one node at a time, it can work on multiple nodes at the same time, which is faster, especially for large graphs.

//Why do we use #pragma omp critical in parallel versions?
//Answer: #pragma omp critical ensures that only one thread at a time can access the critical section (like pushing or popping from the stack or queue), which avoids race conditions.
//When you try to run multiple parts of the program at the same time (in parallel), sometimes they may need to use the same resource. In this case, the resource is the stack or queue that holds the nodes to visit in DFS or BFS.
//#pragma omp critical is like saying: "Only one part of the program can access this part at a time." This prevents multiple parts from messing with the same stack or queue at once, which could cause errors or unexpected results.
//So, the #pragma omp critical makes sure that while one part of the program is pushing or popping a node from the stack (or queue), no other part can do it at the same time. This helps avoid problems like two parts trying to add a node to the stack at once.

//What is the time complexity of DFS and BFS?
//Answer: Both DFS and BFS have a time complexity of O(V + E), where V is the number of vertices and E is the number of edges.

//Can parallelization always improve the performance of graph algorithms?
//Answer: Not always. Parallelization can introduce overhead, especially for smaller datasets or algorithms with dependencies that limit parallelism, like DFS stack operations.






