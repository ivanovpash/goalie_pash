#include "graph.h"
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	Graph graph("input1.txt");

	graph.write("output.txt");
	system("pause");
	return 0;
}