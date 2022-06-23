#include"GRAF.h"
#include"al.h"
#include<iostream>
using namespace std;
int main()
{
	int al;
	cout << "������������õ��㷨:" << endl;
	cout << "1. ���������㷨      - Topological sort algorithm" << endl
		<< "2. ��Դ���·�㷨     - Dijkstra algorithm" << endl
		<< "3. ��С�������㷨     - Prim algorithm" << endl
		<< "4. ���������·���㷨 - Maximium flow (Augmented path) algorithm" << endl
		<< "5. �������㷨       - Articulation finding algorithm" << endl
		<< "6. ǿ��ͨ��֧�㷨     - Strong Connected Component algorithm" << endl
		<< "7. ŷ����·�㷨       - Euler circuit algorithm" << endl
		<< "�˳��밴Enter��" << endl;
	al = cin.get();
	switch (al)
	{
	case 49:
		topsort();
		break;
	case 50:
		Dijkstra();
		break;
	case 51:
		prim();
		break;
	case 52:
		maxflow();
		break;
	case 53:
		findArt();
		break;
	case 54:
		strong_connect();
		break;
	case 55:
		euler();
		break;
	case 10:
		exit(0);
}
}
