#include <iostream>

using namespace std;

int StoerWagner(int **, int);

int main()
{
	setlocale(LC_ALL, "rus");

	int n;
	cout << "Введите количество вершин: ";
	cin >> n;

	int **G = new int*[n];
	for (int i = 0; i < n; i++)
		G[i] = new int[n];
	cout << "Заполните матрицу весов для рёбер:\n";
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> G[i][j];

	cout << "Минимальный разрез: " << StoerWagner(G, n) << endl;

	system("pause");
	return 0;
}

int StoerWagner(int **G, int n)
{
	int *v = new int[n];
	int *w = new int[n];
	int *na = new int[n];
	bool *a = new bool[n];

	for (int i = 0; i < n; i++)
		v[i] = i;

	int best = INT32_MAX;
	while (n > 1)
	{
		a[v[0]] = true;
		for (int i = 1; i < n; i++)
		{
			a[v[i]] = false;
			na[i - 1] = i;
			w[i] = G[v[0]][v[i]];
		}

		int prev = v[0];
		for (int i = 1; i < n; i++)
		{
			int zj = -1;
			for (int j = 1; j < n; j++)
				if (!a[v[j]] && (zj < 0 || w[j] > w[zj]))
					zj = j;

			a[v[zj]] = true;

			if (i == n - 1)
			{
				best = w[zj];

				for (int j = 0; j < n; j++)
				 	G[v[j]][prev] = G[prev][v[j]] += G[v[zj]][v[j]];
				v[zj] = v[--n];
				break;
			}
			prev = v[zj];

			for (int j = 1; j < n; j++)
				if (!a[v[j]])
					w[j] += G[v[zj]][v[j]];
		}
	}

	delete[] v;
	delete[] w;
	delete[] na;
	delete[] a;

	return best;
}