#include <fstream>
#include <vector>
#include "graph.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>

using namespace std;

Graph::Graph(string s)
{
	ifstream in(s);

	in >> n;
	G = new int*[n];
	for (int i = 0; i < n; i++)
	{
		G[i] = new int[n];
		for (int j = 0; j < n; j++)
			in >> G[i][j];
	}

	int numST;
	in >> numST;
	for (int i = 0; i < numST; i++)
	{
		pair<int, int> st;
		in >> st.first >> st.second;
		S_T.push_back(st);
	}

	in >> numReq;
	for (int i = 0; i < numReq; i++)
	{
		Request R;
		in >> R.start;
		in >> R.flux;
		in >> R.time;
		in >> R.s_t;
		R.status = true;
		req.push_back(R);
	}

	for (int i = 0; i < numReq; i++)
	{
		if (curr_takt != req[i].start)
		{
			curr_takt = req[i].start;
			pair<int, int> P;
			P.first = curr_takt;
			P.second = 1;
			num_req_takt.push_back(P);
		}
		else
		{
			int size = num_req_takt.size();
			num_req_takt[size - 1].second++;
		}
	}
	curr_takt = 0;
}

Graph::~Graph()
{
	for (int i = 0; i < n; i++)
		delete[] G[i];
	delete[] G;
}

void Graph::write(string name)
{
	vector<Request> input;
	ofstream out(name);

	for (int i = 0; i < num_req_takt.size(); i++)
	{
		if (num_obtain_st() == 0)
		{
			out << "Program was stopped. Number of reach s-t is null.\n";
			break;
		}
		else if (net_resource() == 0)
		{
			out << "Program was stopped. Resource of network is null.\n";
			break;
		}

		if (num_req_takt[i].first == 1)
		{
			for (int i = 0; i < num_req_takt[i].second; i++)
			{
				Request R = req[i];
				vector<int> res = Dijkstra(S_T[R.s_t - 1].first, S_T[R.s_t - 1].second);

				if (res.size() > 1)
					input.push_back(R);
			}
			for (int i = 0; i < input.size(); i++)
			{
				Request curr = input[i];
				vector<int> path = Dijkstra(S_T[curr.s_t - 1].first, S_T[curr.s_t - 1].second);

				for (int i = 0; i < path.size() - 1; i++)
				{
					if (curr.flux > G[path[i]][path[i + 1]])
						curr.status = false;
				}
				if (curr.status == true)
				{
					for (int i = 0; i < path.size() - 1; i++)
					{
						G[path[i]][path[i + 1]] -= curr.flux;
						G[path[i + 1]][path[i]] -= curr.flux;
					}
				}
			}
			out << "Такт " << num_req_takt[i].first << endl;
			out << "min edge: " << min_e() << endl;
			out << "max edge: " << max_e() << endl;
			out << "edge exp val: " << expected_value_edge() << endl;
			out << "edge st dev: " << st_dev_edge() << endl;
			out << "min cut exp val: " << expected_value_cut() << endl;
			out << "min cut exp val: " << st_dev_cut() << endl;
			out << "num reach s-t: " << num_obtain_st() << endl;
			out << "diametr: " << diametr() << endl;
			out << "min deg: " << min_degree() << endl;
			out << "max deg: " << max_degree() << endl;
			out << "deg exp val: " << expected_value_deg() << endl;
			out << "deg st val: " << st_dev_deg() << endl;
			out << "num all edge not null: " << num_edge_not_null() << endl;
			out << "resource: " << net_resource() << endl;
			out << endl;

			for (int i = 0; i < input.size(); i++)
			{
				input[i].time--;
				if (input[i].time == 0)
					input[i].status = false;
			}
		}
		else //if (num_req_takt[i].first == 2)
		{
			int temp1 = num_req_takt[i - 1].second;
			int temp2 = num_req_takt[i].second;
			for (int i = temp1; i < temp2; i++)
			{
				Request R = req[i];
				vector<int> res = Dijkstra(S_T[R.s_t - 1].first, S_T[R.s_t - 1].second);

				if (res.size() > 1)
					input.push_back(R);
			}
			for (int i = 0; i < input.size(); i++)
			{
				Request curr = input[i];
				vector<int> path = Dijkstra(S_T[curr.s_t - 1].first, S_T[curr.s_t - 1].second);

				for (int i = 0; i < path.size() - 1; i++)
				{
					if (curr.flux > G[path[i]][path[i + 1]])
						curr.status = false;
				}
				if (curr.status == true)
				{
					for (int i = 0; i < path.size() - 1; i++)
					{
						G[path[i]][path[i + 1]] -= curr.flux;
						G[path[i + 1]][path[i]] -= curr.flux;
					}
				}
			}
			out << "Такт " << num_req_takt[i].first << endl;
			out << "min edge: " << min_e() << endl;
			out << "max edge: " << max_e() << endl;
			out << "edge exp val: " << expected_value_edge() << endl;
			out << "edge st dev: " << st_dev_edge() << endl;
			out << "min cut exp val: " << expected_value_cut() << endl;
			out << "min cut exp val: " << st_dev_cut() << endl;
			out << "num reach s-t: " << num_obtain_st() << endl;
			out << "diametr: " << diametr() << endl;
			out << "min deg: " << min_degree() << endl;
			out << "max deg: " << max_degree() << endl;
			out << "deg exp val: " << expected_value_deg() << endl;
			out << "deg st val: " << st_dev_deg() << endl;
			out << "num all edge not null: " << num_edge_not_null() << endl;
			out << "resource: " << net_resource() << endl;
			out << endl;

			for (int i = 0; i < input.size(); i++)
			{
				input[i].time--;
				if (input[i].time == 0)
					input[i].status = false;
			}
		}
	}
}

int Graph::min_e()
{
	int min = INT_MAX;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (G[i][j] < min && G[i][j] != 0)
				min = G[i][j];
	return min;
}

int Graph::max_e()
{
	int max = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (G[i][j] > max)
				max = G[i][j];
	return max;
}

double Graph::expected_value_edge()
{
	double sum = 0.0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				sum += G[i][j];
	return sum / (n * n - n);
}

double Graph::st_dev_edge()
{
	double X = expected_value_edge();
	double sumD = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j)
				sumD += pow(G[i][j] - X, 2);
	double D = sumD / (n * n - n);
	return sqrt(D);
}

double Graph::expected_value_cut()
{
	vector<int> all;
	for (int i = 0; i < S_T.size(); i++)
	{
		int min_cut = FordFulkerson(S_T[i].first, S_T[i].second);
		all.push_back(min_cut);
	}
	double sum = 0;
	for (int i = 0; i < all.size(); i++)
		sum += all[i];

	return sum / all.size();
}

double Graph::st_dev_cut()
{
	double X = expected_value_cut();
	double sumD = 0;
	for (int i = 0; i < S_T.size(); i++)
	{
		int x = FordFulkerson(S_T[i].first, S_T[i].second);
		sumD += pow(x - X, 2);
	}
	double D = sumD / S_T.size();
	return sqrt(D);
}

int Graph::num_obtain_st()
{
	int count = 0;
	for (int i = 0; i < S_T.size(); i++)
	{
		if (FordFulkerson(S_T[i].first, S_T[i].second) != 0)
			count++;
	}
	return count;
}

int Graph::diametr()
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

int Graph::min_degree()
{
	int *A = new int[n];
	for (int i = 0; i < n; i++)
	{
		int count = 0;
		for (int j = 0; j < n; j++)
			if (G[i][j] != 0)
				count++;
		A[i] = count;
	}
	ShellSort(A, n);
	return A[0];
}

int Graph::max_degree()
{
	int *A = new int[n];
	for (int i = 0; i < n; i++)
	{
		int count = 0;
		for (int j = 0; j < n; j++)
			if (G[i][j] != 0)
				count++;
		A[i] = count;
	}
	ShellSort(A, n);
	return A[n - 1];
}

double Graph::expected_value_deg()
{
	int *A = new int[n];
	for (int i = 0; i < n; i++)
	{
		int count = 0;
		for (int j = 0; j < n; j++)
			if (G[i][j] != 0)
				count++;
		A[i] = count;
	}
	double sum = 0;
	for (int i = 0; i < n; i++)
		sum += A[i];
	return sum / n;
}

double Graph::st_dev_deg()
{
	int *A = new int[n];
	for (int i = 0; i < n; i++)
	{
		int count = 0;
		for (int j = 0; j < n; j++)
			if (G[i][j] != 0)
				count++;
		A[i] = count;
	}
	double X = expected_value_deg();
	double sumD = 0;
	for (int i = 0; i < n; i++)
		sumD += pow(A[i] - X, 2);
	double D = sumD / n;
	return sqrt(D);
}

int Graph::num_edge_not_null()
{
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (G[i][j] != 0)
				count++;
		}
	}
	return count / 2;
}

int Graph::net_resource()
{
	int res = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (G[i][j] != 0)
				res += G[i][j];
		}
	}
	return res / 2;
}

int Graph::FordFulkerson(int s, int t)
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

bool Graph::BFS(int **rGraph, int n, int s, int t, int *parent)
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

void Graph::ShellSort(int *A, int n)
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

vector<int> Graph::Dijkstra(int s, int g)
{
	vector<int> result;

	int v;
	int infinity = INT_MAX;
	int p = n;

	int *x = new int[n];
	int *t = new int[n];
	int *h = new int[n];

	int u;
	for (u = 0; u < p; u++)
	{
		t[u] = infinity;
		x[u] = 0;
	}
	h[s] = 0;
	t[s] = 0;
	x[s] = 1;
	v = s;

	while (1)
	{
		for (u = 0; u<p; u++)
		{
			if (G[v][u] == 0)
				continue;
			if (x[u] == 0 && t[u]>t[v] + G[v][u])
			{
				t[u] = t[v] + G[v][u];
				h[u] = v;
			}
		}

		int w = infinity;
		v = -1;
		for (u = 0; u<p; u++)
		{
			if (x[u] == 0 && t[u]<w)
			{
				v = u;
				w = t[u];
			}
		}
		if (v == -1)
			break;
		if (v == g)
		{
			u = g;
			while (u != s)
			{
				result.push_back(u);
				u = h[u];
			}
			break;
		}
		x[v] = 1;
	}
	result.push_back(s);
	vector<int> new_res;
	for (auto rit = result.rbegin(); rit < result.rend(); ++rit)
		new_res.push_back(*rit);
	return new_res;
}