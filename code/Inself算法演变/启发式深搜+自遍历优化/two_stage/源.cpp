
#include<iostream>
#include<conio.h>
#include"Inself.h"
#include"Maps.h"

#define start_position (10,10)
#define end_position (190,190)
using namespace std;


int main()
{
	//HideCursor(); //���ؿ���̨�Ĺ�� 
	DWORD start_time = GetTickCount();
	vector<vector<int>>maze(msize);
	init_map_L(maze);//��ʼ����ͼ���߽硣

	Inself ins1;
	ins1.InitInself(maze);
	Point start start_position;
	Point end end_position;
	//list<Point *> path = astar.GetfPath(start, end, true);
	list<Point *> path = ins1.Self(start, end, true);
	maze = ins1.maze;


	DWORD end_time = GetTickCount();
	
	cout << "������" << "����ʽ����+�Ա����Ż�·��" << endl;
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!"<<endl;//�������ʱ��
	cout << endl << "���ص���" << research_num(maze) << endl;
	cout << endl << "����ʽ����·������" << ins1.inspire_num << endl;
	cout << endl << "����·������" << ins1.final_num << endl;

	for (auto &p : path)
		cout << '(' << p->x << ',' << p->y << ')' << endl;
	/*
	for (int i = 0; i < msize; i++)
	{
		for (int j = 0; j < msize; j++)
		{
			cout << maze[i][j];
		}
		cout << endl;
	}
	*/
	while (1);
	return 0;
}
