#include <iostream>
#include <queue>

using namespace std;

bool bfs(int **, int, int, int, int *);
int FordFulkerson(int **, int, int, int);

int main()
{
	setlocale(LC_ALL, "rus");

	int v;
	cout << "Введите количество вершин: ";
	cin >> v;

	int **M = new int*[v];
	for (int i = 0; i < v; i++)
		M[i] = new int[v];
	cout << "Заполните матрицу весов для рёбер:\n";
	for (int i = 0; i < v; i++)
		for (int j = 0; j < v; j++)
			cin >> M[i][j];

	int s, t;
	cout << "Выберите сток и источник: ";
	cin >> s >> t;

	cout << "Максимальный поток: " << FordFulkerson(M, v, s, t) << endl;;

	for (int i = 0; i < v; i++)
		delete[] M[i];
	delete M;

	system("pause");
	return 0;
}

bool bfs(int **rGraph, int n, int s, int t, int *parent)
{
	bool *visited = new bool[n];
	for (int i = 0; i < n; i++)
		visited[i] = false;

	queue <int> q;
	q.push(s);
	visited[s] = true;
	parent[s] = -1;

	while (!q.empty())
	{
		int u = q.front();
		q.pop();

		for (int v = 0; v < n; v++)
		{
			if (visited[v] == false && rGraph[u][v] > 0)
			{
				q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}

	return (visited[t] == true);
}

int FordFulkerson(int **G, int n, int s, int t)
{
	int u, v;

	int **rGraph = new int*[n];
	for (int i = 0; i < n; i++)
		rGraph[i] = new int[n];


	for (u = 0; u < n; u++)
		for (v = 0; v < n; v++)
			rGraph[u][v] = G[u][v];

	int *parent = new int[n];
	int max_flow = 0;

	while (bfs(rGraph, n, s, t, parent))
	{
		int path_flow = INT_MAX;
		for (v = t; v != s; v = parent[v])
		{
			u = parent[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}

		for (v = t; v != s; v = parent[v])
		{
			u = parent[v];
			rGraph[u][v] -= path_flow;
			rGraph[v][u] += path_flow;
		}

		max_flow += path_flow;
	}

	for (int i = 0; i < v; i++)
		delete[] rGraph[i];
	delete rGraph;

	delete[] parent;

	return max_flow;
}