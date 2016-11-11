// Нахождение кратчайших путей между из заданной вершины во все остальные.
// Алгоритм Беллмана-Форда.

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Edge
{
	Edge(int i, int j, int c)
	{
		a = i;
		b = j;
		cost = c;
	}
	int a, b, cost;
};


void BellmanFord(vector<Edge>, int, int);

int main()
{
	setlocale(LC_ALL, "rus");

	vector<Edge> edges;

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

	for (int i = 0; i < v; i++)
	{
		for (int j = 0; j < v; j++)
		{
			if (M[i][j] != 0)
			{
				Edge edge(i, j, M[i][j]);
				edges.push_back(edge);
			}
		}
	}
			
	int s;
	cout << "Выберите номер стартовой вершины: ";
	cin >> s;

	BellmanFord(edges, v, s);

	for (int i = 0; i < v; i++)
		delete[] M[i];
	delete M;

	system("pause");
	return 0;
}

void BellmanFord(vector<Edge> e, int n, int s)
{
	int *d = new int[n];

	for (int i = 0; i < n; i++)
		d[i] = INT32_MAX;
	d[s] = 0;
	
	for (int i = 1; i < n - 1; i++)
	{
		for (int j = 0; j < e.size(); j++)
		{
			if (d[e[j].b] > d[e[j].a] + e[j].cost)
				d[e[j].b] = d[e[j].a] + e[j].cost;
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (d[i] != INT32_MAX)
			cout << s << " -> " << i << " = " << d[i] << endl;
		else
			cout << s << " -> " << i << " = Nope\n";
	}

	delete[] d;
}