#include <iostream>

using namespace std;

void Dijkstra(int **, int, int);

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

	int s;
	cout << "Выберите номер стартовой вершины: ";
	cin >> s;

	Dijkstra(M, v, s);

	for (int i = 0; i < v; i++)
		delete[] M[i];
	delete M;

	system("pause");
	return 0;
}

void Dijkstra(int **V, int n, int s)
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

	for (int i = 0; i < n; i++)
	{
		if (dist[i] != INT32_MAX)
			cout << s << " -> " << i << " = " << dist[i] << endl;
		else
			cout << s << " -> " << i << " = Nope\n";
	}

	delete[] dist;
	delete[] visited;
}