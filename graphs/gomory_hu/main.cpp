#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

// ���������� ������
const int sz = 6;

// ������� ��������� ����� (������ ����������, ��� ��������� ����� 
// ��������������). � ������� ���������� ������ ��� �� ���������� 
// "������" x "������" ��� ���������������
const int graph[sz][sz] =
{
	{ 0, 1, 7, 0, 0, 0 },
	{ 1, 0, 1, 3, 2, 0 },
	{ 7, 1, 0, 0, 4, 0 },
	{ 0, 3, 0, 0, 1, 6 },
	{ 0, 2, 4, 1, 0, 2 },
	{ 0, 0, 0, 6, 2, 0 }
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ������-��
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

struct Edge;
struct Vertex;

typedef vector<int>     Row;        // ������ ������� ���������
typedef vector<Row>     Matrix;     // ������� ���������
typedef vector<Vertex*> VertexList; // ������ ������
typedef vector<Edge*>   EdgeList;   // ������ �����

									// ----------------------------------------------------------------------------

									// ������� ��� ������ ������

struct Vertex
{
	int id; // ������������� �������, ����� ��� ������ �� ����� ����
			// ������ ���������, � ��� �� � ����� ��� ����������
			// ������ ��������� ������ � ������� �������� ������
			// (����� �������� ������� ��������� ��������. ��������)

	VertexList  group;  // ������� � ������ (����� ��� ������� �������)
	EdgeList    edges;  // ������ �����

	int     flag;       // ��������������� ����, ������������ ��� ������
	Vertex* parent;     // � ������ � ��� ���-��� ��� ��������� �����

						// ������������ ������� � ��������� ������
	Vertex(int id) : id(id) { }
	Vertex(const VertexList &group) : id(-1), group(group) { }
};

// ----------------------------------------------------------------------------

// ����� (�.�. ����� ��������� � ������� � ������, �� � ����� �����������
// ������ ������ �������; ��������������, �� ������ ������� �����������
// ����������� ����� � �������� �������)

struct Edge
{
	Vertex* vertex; // ������� �������
	int c;          // ���������� �����������
	int f;          // �����

	Edge(Vertex* vertex, int c) : vertex(vertex), c(c), f(0) { }
};

// ----------------------------------------------------------------------------

// ��������� ������ ����. ������

struct MinCut
{
	int        f;  // ����. ����� ����� s � t
	Vertex*    s;  // ��������� �� s
	Vertex*    t;  // ��������� �� t
	VertexList A;  // "����������" ��������, ���������� s
	VertexList B;  // "����������" ��������, ���������� t

	MinCut(Vertex* s, Vertex* t) : s(s), t(t), f(0) {}
};

// ----------------------------------------------------------------------------

// ����� ������� � ������, ������������ �������� (���������)

VertexList::iterator findVertex(VertexList &set, Vertex* v)
{
	VertexList::iterator i = set.begin();
	VertexList::iterator j = set.end();
	while (i != j && (*i) != v) i++;
	return i;
}

// ----------------------------------------------------------------------------

// �������� ��������

VertexList setMinus(VertexList &set1, VertexList &set2)
{
	// ���������� ��������� ����
	VertexList result;
	for each(Vertex* v in set1)
	{
		// ���� ������� �� ������ �� ������� � ������, ��������� � ���������
		if (findVertex(set2, v) == set2.end()) result.push_back(v);
	}
	return result;
}

// ----------------------------------------------------------------------------

// ����������� ��������

VertexList setMul(VertexList &set1, VertexList &set2)
{
	// ���������� ��������� ����
	VertexList result;
	for each(Vertex* v in set2)
	{
		// ���� ������� �� ������� ������� � �����, ��������� � ���������
		if (findVertex(set1, v) != set1.end()) result.push_back(v);
	}
	return result;
}

// ----------------------------------------------------------------------------

// ����� ����� � ������, ������������ �������� (���������)

EdgeList::iterator findEdge(Vertex* v1, Vertex* v2)
{
	EdgeList::iterator i = v1->edges.begin();
	EdgeList::iterator j = v1->edges.end();
	while (i != j && (*i)->vertex != v2) i++;
	return i;
}

// ----------------------------------------------------------------------------

// �������� ����� V1 --[c]--> V2
// ���� addReverse == true - ��������� � V2 --[c]--> V1
// ���� ����� ��� �������, �� c ����������� � ��� ����

void addEdge(Vertex* v1, Vertex* v2, const int c, bool addReverse = true)
{
	EdgeList::iterator i = findEdge(v1, v2);
	if (i != v1->edges.end()) (*i)->c += c;
	else v1->edges.push_back(new Edge(v2, c));

	if (!addReverse) return;

	i = findEdge(v2, v1);
	if (i != v2->edges.end()) (*i)->c += c;
	else v2->edges.push_back(new Edge(v1, c));
}

// ----------------------------------------------------------------------------

// ��������� ����� (���� ����) �� V1 � V2

Edge* getEdge(Vertex* v1, Vertex* v2)
{
	EdgeList::iterator i = findEdge(v1, v2);
	return (i == v1->edges.end() ? NULL : *i);
}

// ----------------------------------------------------------------------------

// ����������� ����� �� V1 � V2

void deleteEdge(Vertex* v1, Vertex* v2)
{
	EdgeList::iterator i = findEdge(v1, v2);
	if (i == v1->edges.end()) return;

	delete (*i);
	v1->edges.erase(i);
}

// ----------------------------------------------------------------------------

// ����������� ������� v � ��������� set � ��������� �����

void deleteVertex(VertexList &set, Vertex* v)
{
	VertexList::iterator i = findVertex(set, v);
	if (i == set.end()) return;

	// ���������� �����, ��������� � ��������
	for each(Edge* e in v->edges)
	{
		// ���������� ��������� �����, ����� ���� ������� ������
		deleteEdge(e->vertex, v);
		delete e;
	}

	// ������� �������
	delete (*i);
	set.erase(i);
}

// ----------------------------------------------------------------------------

// ����������� ����� (���� ������ � ����� � ���������)

void deleteVertexList(VertexList &set)
{
	for each(Vertex* v in set)
	{
		for each(Edge* e in v->edges) delete e;
		delete v;
	}
	set.clear();
}

// ----------------------------------------------------------------------------

// �������������� ����� � ��������� {{1,2}, {3}} --> {1, 2, 3}

VertexList extractGroups(const VertexList &set)
{
	VertexList result;

	for each(Vertex* v in set)
	{
		if (v->group.size() > 0)
		{
			for each(Vertex* subv in v->group) result.push_back(subv);
		}
		else result.push_back(v);
	}

	return result;
}

// ----------------------------------------------------------------------------

// �������� ������� ��������� �� ���������

Matrix vertexListToMatrix(const VertexList &set)
{
	int n = static_cast<int>(set.size());
	Matrix m(n, Row(n, 0));

	// �������� ������� � ���� flag
	// ����� ���� ��������� �������
	for (int i = 0; i < n; i++) set[i]->flag = i;
	for each(Vertex* v in set)
	{
		for each(Edge* e in v->edges) m[v->flag][e->vertex->flag] = e->c;
	}

	return m;
}

// ----------------------------------------------------------------------------

// ���������� ������ �� id ������

void sortListById(VertexList &set)
{
	size_t n = set.size();

	for (size_t i = 0; i < n - 1; i++)
	{
		size_t k = i;
		for (size_t j = i + 1; j < n; j++)
		{
			if (set[j]->id < set[k]->id)	k = j;
		}
		if (k != i)
		{
			Vertex* v = set[k];
			set[k] = set[i];
			set[i] = v;
		}
	}
}

// ----------------------------------------------------------------------------

// ��������� ������������� ������� ��� ������

string vertexToStr(const Vertex* vertex)
{
	stringstream s;
	s << "{";
	if (vertex->group.size() == 0)
	{
		s << vertex->id << "}";
		return s.str();
	}
	else
	{
		for each(Vertex* v in vertex->group) s << vertexToStr(v) << ",";
		return s.str().substr(0, s.str().size() - 1) + "}";
	}
}

// ----------------------------------------------------------------------------

// ��������� ������������� ��������� ������ ��� ������

string vertexListToStr(const VertexList &set)
{
	stringstream s;
	s << "[ ";
	for each(Vertex* v in set) s << vertexToStr(v) << ", ";
	return s.str().substr(0, s.str().size() - 2) + " ]";
}

// ----------------------------------------------------------------------------

// ��������� ������������� ������� ��������� ��������� ������

string matrixToStr(const Matrix &m)
{
	int n = static_cast<int>(m.size());

	stringstream s;
	for (int i = 0; i < n; i++)
	{
		s << "    ";
		for (int j = 0; j < n; j++) s << setw(3) << m[i][j];
		s << endl;
	}
	return s.str();
}

// ----------------------------------------------------------------------------

// ����� ����������� ���� �� s � t
// ���������� �� ����, � �������, �������������� ��� ������,
// ������� ����������� ��� ���������� ��������� B

VertexList findPath(const VertexList &set, Vertex* s, Vertex* t)
{
	// ������� ������� �������� ������
	for each(Vertex* v in set)
	{
		v->parent = NULL;
		v->flag = 0;
	}

	size_t i = 0;
	VertexList queue;

	// ��������� � ������� ��������� ������� � �������� ��
	s->flag = 1;
	queue.push_back(s);

	// ������������ ��������� ������� �������
	while (i < queue.size())
	{
		// ������ � ������� ��� �������, ��� �� ���������� � 
		// ������� � ������� ����� ����, ������� (�.�. �����,
		// �� ������� ����� ��������� ����)
		for each(Edge* e in queue[i]->edges)
		{
			Vertex* v = e->vertex;
			if (v->flag == 0 && e->c > 0)
			{
				v->parent = queue[i];
				v->flag = 1;
				queue.push_back(v);

				// ���� ������� ������� = t - ������ ���������� ����
				if (v == t) return queue;
			}
		}
		// ��������� � ��������� � ������� �������
		i++;
	}

	return queue;
}
// ----------------------------------------------------------------------------

// ����� max ������ � ��� min ������� ����� ��������� 0 � 1 � ��������� set

MinCut findMinCut(VertexList &set)
{
	MinCut result(set[0], set[1]);

	for (;;)
	{
		// ������� ���������� ���� ����� s � t, 
		// ���� �� ������ - ����� ����������
		VertexList path = findPath(set, result.s, result.t);
		if (result.t->parent == NULL) break;

		// ������� � t � ����������� �� ������� parent-�� ��������� ��
		// ���������� ����: ������ ����������� c, �� ������� ����� 
		// ��������� ����� �� ����� ���� � ������� ����� ����� ����
		int min_c = INT_MAX;
		Vertex* v = result.t;
		EdgeList edges;
		while (v->parent != NULL)
		{
			edges.push_back(getEdge(v, v->parent));
			Edge* e = getEdge(v->parent, v);
			edges.push_back(e);
			if (e->c < min_c) min_c = e->c;
			v = v->parent;
		}

		// �������� ����� � �������� ��� �� ��������� ��������
		result.f += min_c;
		for each(Edge* e in edges)
		{
			e->f += min_c;
			e->c -= min_c;
		}
	}

	// ������� B - �������� ������� �� ������� ����� t, ������� 
	// �������� �������� ��� ������ (���������) ���� �� t � s
	// ������ ��������: A = set - B
	result.B = findPath(set, result.t, result.s);
	result.A = setMinus(set, result.B);

	// �������� � ������� ���� ����� ������
	for each(Vertex* v in set)
	{
		for each(Edge* e in v->edges)
		{
			e->c += e->f;
			e->f = 0;
		}
	}

	return result;
}

// ----------------------------------------------------------------------------

// ���������� ������ ������-��

Matrix buildGomoryHuTree(const Matrix &g)
{
	// ��� 1: �������������
	VertexList Vg = VertexList();
	VertexList Vt = VertexList();

	// ������� � Vg ������� � ����� �� ������� ���������
	size_t n = g.size();
	for (size_t i = 0; i < n; i++)
	{
		Vg.push_back(new Vertex(static_cast<int>(i)));
		for (size_t j = 0; j < i; j++)
		{
			if (g[i][j] != 0) addEdge(Vg[i], Vg[j], g[i][j]);
		}
	}

	// � Vt �������� 1 �������, ������������ ��� ������� ��������� �����
	Vt.push_back(new Vertex(Vg));

	cout << endl << "Step 1: Vt= " << vertexListToStr(Vt) << endl << endl;

	for (;;)
	{
		cout << "---------------------------------------------------------------" << endl << endl;

		// ��� 2: ����� ������ ������ �� Vt, � ������� ����� 1 �������
		// ���� ��� ������ �� ����� ������� - ������ ������
		// ����� ��������� ��������� ������ � x

		VertexList::iterator i = Vt.begin();
		VertexList::iterator j = Vt.end();
		while (i != j && (*i)->group.size() < 2) i++;
		if (i == j) break;

		Vertex* x = *i;

		cout << "Step 2: X = " << vertexListToStr(x->group) << endl << endl;

		// ��� 3: ������ ���� G
		// Vt - ��� ������. ������ �� X ������� ��������� �����, �� ��������� 
		// ����� ����� ����� ��� ����� ��� � (�.�. � ������ ��� ������).
		// ������ ����� �� � "�������" � ���� ������. � ��� ��� X ���������.
		// ����: http://upload.wikimedia.org/wikipedia/en/thumb/1/1c/Gomory�Hu_T3.svg/500px-Gomory�Hu_T3.svg.png (X = {1,4})
		// �����: http://upload.wikimedia.org/wikipedia/en/thumb/6/60/Gomory�Hu_Gp4.svg/500px-Gomory�Hu_Gp4.svg.png
		// �������, ��������� ������ ������ ����� ������� ������ ��������� �����
		// � ���������� ������� ����� �������� ��������� � ��������, �� ������
		// ������� ������� ���� ������� � ������. �.�. ������� {1}, {4} �� ��������
		// ���� ��� �� ��� � ������� ����� �� ����� ���� {{1}}, {{4}}. �������� 
		// ����� ����� ���� ����������� ������ ���������. ��� ��������� ��������
		// ����� ������������� ��������� parent, ������� � ������ ������� ������
		// ����� ��������� �� ������, � ������� ��� ���������

		VertexList G = VertexList();

		// ������� ������� �� X, ���������� �� � ������
		for each(Vertex* v in x->group)
		{
			v->parent = new Vertex(VertexList(1, v));
			G.push_back(v->parent);
		}

		// ������� �������-�����
		for each(Edge* e in x->edges)
		{
			// ������� ��� ������� �����, ������������ �� ����� e � ������ ����� ������� z.
			// ��� ����� ���������� ������� ����� �� e->vertex � x � ����� ��������������
			// ����� ����� ���� �� e->vertex � x, ��������� ����������, ����� ������� ������
			deleteEdge(e->vertex, x);
			Vertex* z = new Vertex(extractGroups(findPath(Vt, e->vertex, x)));
			addEdge(e->vertex, x, e->c, false);

			for each(Vertex* v in z->group) v->parent = z;
			G.push_back(z);
		}

		// � ������ ������� G ������������� ����� ������ �� ������ � ��������� �� "�������".
		// ��������, � ������ ������� A ���� ������� ������� 1, �� ������� ���� ����� � 
		// ������� ������� 3, parent ������� - ��������� ������� B. �������������, �������� 
		// ����� ����� ���������� ��������� A � B. ��� ����, ���� ��� ����� ����� ����, ��
		// ����� ��� �������������� � �������, ����� �������, ����� ����� ��������, ��� � 
		// ����������, ����� ����� ��������� ���.
		for each(Vertex* z in G)
		{
			for each(Vertex* v in z->group)
			{
				for each(Edge* e in v->edges)
				{
					// ����� �� ������!
					if (z != e->vertex->parent) addEdge(z, e->vertex->parent, e->c, false);
				}
			}
		}

		cout << "Step 3: G = " << vertexListToStr(G) << endl << endl;
		cout << matrixToStr(vertexListToMatrix(G)) << endl << endl;

		// ��� 4: ����� min ������� � max ������� � G
		// s � t ���������� ������� �� ������ ��������� X, ������� ��������� � ������
		// ��������� G. ��� �������� findMinCut() �������� ������� 0 � 1. 
		MinCut cut = findMinCut(G);
		cut.A = extractGroups(cut.A);
		cut.B = extractGroups(cut.B);

		cout << "Step 4: s-t   = " << vertexToStr(cut.s) << "-" << vertexToStr(cut.t) << endl;
		cout << "        max_f = " << cut.f << endl;
		cout << "        A     = " << vertexListToStr(cut.A) << endl;
		cout << "        B     = " << vertexListToStr(cut.B) << endl << endl;

		// ������� ����������� ��������� �������� ����� G
		deleteVertexList(G);

		// ��� 5: �������� ������

		// X ���������� �� X*A � X*B
		// X*A � X*B ����������� ������, ������ ���� ������
		Vertex* XA = new Vertex(setMul(x->group, cut.A));
		Vertex* XB = new Vertex(setMul(x->group, cut.B));
		addEdge(XA, XB, cut.f);

		// X ���� (��������) ������� � ���������� �������� Vi
		// ��� ���� Vi ���� ���� ������������� A, ���� ������������� B
		// �������, ���� ����� ������� �� Vi �������� � A - ������ Vi
		// ���� ������. �, ����� ��������� Vi � X*A, ����� � X*B
		for each(Edge* e in x->edges)
		{
			Vertex* v = e->vertex; // Vi (v->group[0] - ������. ������� �� Vi)
			addEdge((findVertex(cut.A, v->group[0]) != cut.A.end() ? XA : XB), v, e->c);
		}

		// ��������� X*A � X*B, ������� X
		Vt.insert(Vt.insert(i, XB), XA);
		deleteVertex(Vt, x);

		cout << "Step 5: Vt= " << vertexListToStr(Vt) << endl << endl;
		cout << matrixToStr(vertexListToMatrix(Vt)) << endl << endl;
	}

	// ���������� ������� �� ����������� ������
	for each(Vertex* v in Vt)
	{
		v->id = v->group[0]->id;
		v->group.clear();
	}

	// ���������� ������� ��������� ��������������� �� id ������
	// (����� ������� �������������� �������� �������)
	sortListById(Vt);
	Matrix m = vertexListToMatrix(Vt);

	cout << "���������:" << endl << endl;
	cout << "Vt= " << vertexListToStr(Vt) << endl << endl;
	cout << matrixToStr(m) << endl << endl;

	// ������� ����������� ��������� �������� Vg � Vt
	deleteVertexList(Vg);
	deleteVertexList(Vt);

	return m;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	// ���������, ������������� �� ����� �� ��������������� ����
	for (int j = 0; j < sz; j++)
	{
		if (graph[j][j] != 0)
		{
			cout << "  ������: ����� � ������� " << j << endl;
			return 0;
		}

		for (int i = 0; i < j; i++)
		{
			if (graph[i][j] != graph[j][i])
			{
				cout << "  ������: ���������� ���� ����� " << i << "-" << j << endl;
				return 0;
			}

			if (graph[i][j] < 0)
			{
				cout << "  ������: ������������� ��� ����� " << i << "-" << j << endl;
				return 0;
			}
		}
	}

	Matrix g(sz, Row(sz));
	for (int i = 0; i < sz; i++) for (int j = 0; j < sz; j++) g[i][j] = graph[i][j];
	buildGomoryHuTree(g);

	system("pause");
	return 0;
}