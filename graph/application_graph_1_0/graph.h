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
	int start;	// ������ ������
	int flux;	// �������� ������
	int time;	// ����� �����
	int s_t;	// ����� ���� s-t
	bool status;	// ��������� ��� ���

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
	// 1. ����� ����������� � ������������ �������� �����.
	int min_e();
	int max_e();

	// 2. �������������� �������� � ����������� ���������� ��� ��� �� �����.
	double expected_value_edge();
	double st_dev_edge();

	// 3. �������������� �������� � ����������� ���������� ��� ����������� ��������.
	double expected_value_cut();
	double st_dev_cut();

	// 4. ���������� ���������� ��� s-t.
	int num_obtain_st();

	// 5. ������� �����.
	int diametr();

	// 6. ����������� � ������������ ������� ������ �����.
	int min_degree();
	int max_degree();

	// 7. �������������� �������� � ����������� ���������� ��� �������� ������.
	double expected_value_deg();
	double st_dev_deg();

	// 8. ���������� ��� ���� �� �������.
	int num_edge_not_null();

	// 9. ������ ����.
	int net_resource();

	// ��������������� �������.
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