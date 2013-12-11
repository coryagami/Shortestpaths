#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

// comparator class for priority queue to do distance comparisons from
// the <u, w> pair format
class Comparator
{
public:
    int operator() ( const pair<int, int>& p1, const pair<int, int>& p2 )
    {
        return p1.second < p2.second;
    }
};

// Graph type is adjacency list of edges (v, u) and weights w
// Graph is a vector of root 'v' vertices, of a vector of pairs,
// where each pair is of format <u, w>
typedef vector<vector<pair<int, int> > >  Graph;

void printGraph(Graph g);
void inputGraph(string fileName, Graph& Gref);
void Dijkstras(Graph g, char source);
void SRP(Graph g, char source, int k);




int main(int argc, char* argv[])
{
	if(argc != 2)
	{cout << "Usage: " << argv[0] << " <filename>\n"; return 0;}

	Graph G;

	inputGraph(argv[1], G);
	printGraph(G);
	
	char source; int k;
	cout << "\nEnter source node for Dijkstra's: "; cin >> source;
	Dijkstras(G, source);

	cout << "\nEnter source node for Reliable Shortest Path: "; cin >> source;
	cout << "Enter 'k' for Reliable Shortest Path: "; cin >> k;
	SRP(G, source, k);

	return 0;
}



void SRP(Graph g, char source, int k)
{
	// priority queue consists of int pairs of format 
	// <vertex, distance to vertex from source>
	priority_queue<pair<int, int>, vector<pair<int, int> >, Comparator > PQ;

	// int arrays to record distances from source to vertices, and edge counts
	// from source to vertices. Support max of 50 vertices
	int dist[50], edges[50];

	// initializations of distance and path edge counts
	for(int i=0; i<g.size(); i++)
	{
		if(g[i] != g[source-65]);
		{
			dist[i] = 999;
			edges[i] = 0;
		}
	}
	dist[source-65] = 0;

	// push initial source vertex
	PQ.push(pair<int, int>(source-65, dist[source-65]));

	int v, u, w;
	while(!PQ.empty())
	{
		// pop highest priority vertex for exploring
		u = PQ.top().first;
		PQ.pop();
		int size = g[u].size();

		// only progess to neighboring vertices if edge count is not >= k
		if(edges[u] < k)
		{
			for(int i=0; i<size; i++)
			{
				v = g[u][i].first;
				w = g[u][i].second;

				// checking distances for possible update or distance and 
				// path edge count
				if(dist[v] > dist[u] + w)
				{
					dist[v] = dist[u] + w;
					PQ.push(pair<int, int>(v, dist[v]));
					edges[v] = edges[u]+1;
				}
			}
		}
	}
	cout << "\n------Shortest Reliable Path's------\nSource: " << source 
		<< ", k : " << k << endl;
	for(int i=0; i<g.size(); i++)
	{
		cout << "Node " << (char)(i+65) << ": ";
		if(dist[i] != 999)
			cout << dist[i] << ", " << edges[i] << " edges\n";
		else
			cout << "No path with edges < " << k << endl;
	}
	cout << "---End of Shortest Reliable Path's---\n\n";
}


void Dijkstras(Graph g, char source)
{
	// priority queue consists of int pairs of format 
	// <vertex, distance to vertex from source>
	priority_queue<pair<int, int>, vector<pair<int, int> >, Comparator > PQ;

	// int array to record distances from source to vertices
	int dist[50];

	// initializations
	for(int i=0; i<g.size(); i++)
	{
		if(g[i] != g[source-65]);
		{
			dist[i] = 999;
		}
	}
	dist[source-65] = 0;

	// initial push of source vertex
	PQ.push(pair<int, int>(source-65, dist[source-65]));

	int v, u, w;

	// dijkstra's algorithm
	while(!PQ.empty())
	{
		u = PQ.top().first;
		PQ.pop();
		int size = g[u].size();
		for(int i=0; i<size; i++)
		{
			v = g[u][i].first;
			w = g[u][i].second;
			if(dist[v] > dist[u] + w)
			{
				dist[v] = dist[u] + w;
				PQ.push(pair<int, int>(v, dist[v]));
			}
		}
	}

	cout << "\n-----------Dijkstra's-----------\nSource: " << source << endl;
	for(int i=0; i<g.size(); i++)
	{
		cout << "Node " << (char)(i+65) << ": ";
		if(dist[i] != 999)
			cout << dist[i] << endl;
		else
			cout << "No possible path\n";
	}
	cout << "--------End of Dijkstra's--------\n";
}


void inputGraph(string fileName, Graph& Gref)
{
	ifstream inFile(fileName.c_str());
	if(!inFile.is_open())
		cout << "FILE READ ERROR";

	char graphType; 

	string poo;
	if(inFile.get() == '#')
		getline(inFile, poo);

	// get graphType of 'U' or 'D'
	inFile.get(graphType);
	getline(inFile, poo);

	int Gsize = 1; Gref.resize(Gsize);
	char v, u; int weight;
	while(!inFile.eof())
	{
		inFile >> v >> u >> weight;
		getline(inFile, poo);
		while((v-65) >= Gref.size()-1)
		{
			Gsize++;
			Gref.resize(Gsize);
		}
		Gref[v-65].push_back(make_pair(u-65, weight));

		/// input for undirected graph (reverse edge)
		if(graphType == 'U')
		{
			while((u-65) > Gref.size()-1)
			{
				Gsize++;
				Gref.resize(Gsize);
			}
			Gref[u-65].push_back(make_pair(v-65, weight));
		}
	}
}


void printGraph(Graph G)
{
	cout << "Graph contains: \n";
	for(int i=0; i<G.size(); i++)
	{
		cout << (char)(i+65) << ":\n";
		for(int j=0; j<G[i].size(); j++)
			cout << "\t" << (char)(G[i][j].first+65) << ", " << G[i][j].second << endl;
	}
}