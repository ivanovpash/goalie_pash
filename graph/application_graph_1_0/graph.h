#pragma once
#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <list>

using namespace std;

struct Request
{
	int start;	// Начало потока
	int flux;	// Величина потока
	int time;	// Время жизни
	int s_t;	// Номер пары s-t
	bool status;	// Отклонена или нет

	void Show()
	{
		cout << start << " " << flux << " " << time << " " << s_t << " " << status << endl;
	}
};

class Graph
{
public:
	Graph(string);
	~Graph();

	void write(string);

private:
	// 1. Найти минимальную и максимальную величину графа.
	int min_e();
	int max_e();

	// 2. Математическое ожидание и стандартное отклонение для дуг на графе.
	double expected_value_edge();
	double st_dev_edge();

	// 3. Математические ожидание и стандартное отклонение для минимальных разрезов.
	double expected_value_cut();
	double st_dev_cut();

	// 4. Количество достижимых пар s-t.
	int num_obtain_st();

	// 5. Диаметр графа.
	int diametr();

	// 6. Минимальная и максимальная степени вершин графа.
	int min_degree();
	int max_degree();

	// 7. Математические ожидание и стандартное отклонение для степеней вершин.
	double expected_value_deg();
	double st_dev_deg();

	// 8. Количество дуг всех не нулевых.
	int num_edge_not_null();

	// 9. Ресурс сети.
	int net_resource();

	// Вспомогательные функции.
	int FordFulkerson(int, int);
	bool BFS(int **, int, int, int, int *);
	void ShellSort(int *, int);
	vector<int> Dijkstra(int, int);

private:
	vector<Request> req;
	int **G = nullptr;
	vector<pair<int, int>> S_T;
	int n;
	int numReq;
	int curr_takt = 0;
	vector<pair<int, int>> num_req_takt;
};