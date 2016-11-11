#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>

using namespace std;

// Вспомогательные функции.
void ShellSort(int *, int);
bool BFS(int **, int, int, int, int *);
int FordFulkerson(int **, int, int, int);
int* Dijkstra(int **, int, int);

// 1. Найти минимальную и максимальную величину графа.
int MinWeightEdge(int **, int);
int MaxWeightEdge(int **, int);

// 2. Математическое ожидание и стандартное отклонение для дуг на графе.
double ExpectedValueEdge(int **, int);
double StDevEdge(int **, int);

// 3. Математическое ожидание и стандартное отклонение для минимальных разрезов.
double ExpectedValueMinCut(int **, int);
double StDevMinCut(int **, int);

// 4. Количество достижимых пар "источник-сток".
int NumST(int **, int);

// 5. Диаметр графа.
int Diametr(int **, int);

int main()
{
	setlocale(LC_ALL, "rus");

	ifstream in("input.txt");

	int n;
	in >> n;

	int **G = new int*[n];
	for (int i = 0; i < n; i++)
		G[i] = new int[n];

	while (!in.eof())
	{
		for (size_t i = 0; i < n; i++)
			for (size_t j = 0; j < n; j++)
				in >> G[i][j];
	}

	ofstream out("output.txt");

	out << "Минимальная величина дуги на графе: " << MinWeightEdge(G, n) << "\n";
	out << "Максимальная величина дуги на графе: " << MaxWeightEdge(G, n) << "\n";
	out << "Математическое ожидание для дуг на графе: " << ExpectedValueEdge(G, n) << "\n";
	out << "Стандартное отклонение для дуг на графе: " << StDevEdge(G, n) << "\n";
	out << "Математическое ожидание для минимальных разрезов: " << ExpectedValueEdge(G, n) << "\n";
	out << "Стандартное отклонение для минимальных разрезов: " << StDevMinCut(G, n) << "\n";
	out << "Количество доступных пар s-t: " << NumST(G, n) << "\n";
	out << "Диаметр графа: " << Diametr(G, n) << "\n";

	out.close();

	return 0;
}

int MinWeightEdge(int **G, int n)
{
	int min = INT_MAX;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (G[i][j] < min && G[i][j] != 0)
				min = G[i][j];
	return min;
}

int MaxWeightEdge(int **G, int n)
{
	int max = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (G[i][j] > max)
				max = G[i][j];
	return max;
}

double ExpectedValueEdge(int **G, int n)
{
	double sum = 0.0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				sum += G[i][j]; 
	return sum / (n * n - n);
}

double StDevEdge(int **G, int n)
{
	double X = ExpectedValueEdge(G, n);
	double sumD = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				sumD += pow(G[i][j] - X, 2);
	double D = sumD / (n * n - n);
	return sqrt(D);
}

double ExpectedValueMinCut(int **G, int n)
{
	int **M = new int*[n];
	for (int i = 0; i < n; i++)
		M[i] = new int[n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				M[i][j] = 0;
			else
				M[i][j] = FordFulkerson(G, n, i, j);
		}
	}
	double X = ExpectedValueEdge(M, n);
	return X;
}

double StDevMinCut(int **G, int n)
{
	double X = ExpectedValueMinCut(G, n);
	double sumD = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				sumD += pow(G[i][j] - X, 2);
	double D = sumD / (n * n - n);
	return sqrt(D);
}

int NumST(int **G, int n)
{
	int **W = new int*[n];
	for (int i = 0; i < n; i++)
	{
		W[i] = new int[n];
		W[i] = Dijkstra(G, n, i);
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			W[j][i] = W[i][j];
		}
	}

	int count = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (W[i][j] != 0)
				count++;
		}
	}
	return count / 2;
}

int Diametr(int **G, int n)
{
	int *r = new int[n];
	for (int i = 0; i < n; i++)
	{
		int max = 0;
		for (int j = 0; j < n; j++)
		{
			if (G[i][j] > max)
				max = G[i][j];
		}
		r[i] = max;
	}
	ShellSort(r, n);
	int D = r[n - 1];
	delete[] r;
	return D;
}

void ShellSort(int *A, int n)
{
	int t;
	for (int k = n / 2; k > 0; k /= 2)
	{
		for (int i = k; i < n; i += 1)
		{
			t = A[i];
			int j;
			for (j = i; j >= k; j -= k)
			{
				if (t < A[j - k])
					A[j] = A[j - k];
				else
					break;
			}
			A[j] = t;
		}
	}
}

bool BFS(int **rGraph, int n, int s, int t, int *parent)
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

	bool result = (visited[t] == true);
	delete[] visited;

	return result;
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

	while (BFS(rGraph, n, s, t, parent))
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

int* Dijkstra(int **V, int n, int s)
{
	int* dist = new int[n];
	bool* visited = new bool[n];
	int index = 0;
	int u;

	for (int i = 0; i < n; i++)
	{
		dist[i] = INT32_MAX;
		visited[i] = false;
	}

	dist[s] = 0;

	for (int i = 0; i < n; i++)
	{
		int min = INT32_MAX;
		for (int j = 0; j < n; j++)
		{
			if (!visited[i] && dist[i] <= min)
			{
				min = dist[i];
				index = i;
			}
		}
		u = index;
		visited[u] = true;
		for (int i = 0; i < n; i++)
			if (!visited[i] && V[u][i] != 0 && dist[u] != INT32_MAX && dist[u] + V[u][i] < dist[i])
				dist[i] = dist[u] + V[u][i];
	}

	//for (int i = 0; i < n; i++)
	//{
	//	if (dist[i] != INT32_MAX)
	//		cout << s << " -> " << i << " = " << dist[i] << endl;
	//	else
	//		cout << s << " -> " << i << " = Nope\n";
	//}

	//delete[] dist;
	delete[] visited;

	return dist;
}