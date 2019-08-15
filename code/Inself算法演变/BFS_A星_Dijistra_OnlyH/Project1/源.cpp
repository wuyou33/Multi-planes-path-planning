#include<iostream>
#include<conio.h>
#include"Astar.h"
#include"Maps.h"
#define start_position (10,10)
#define end_position (190,190)
using namespace std;


/*
void HideCursor()//���ؿ���̨�Ĺ�� 
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}*/

int main()
{
	//HideCursor(); //���ؿ���̨�Ĺ�� 
	DWORD start_time = GetTickCount();
	vector<vector<int>>maze(msize);
	init_map_L(maze);//��ʼ����ͼ���߽硣

	Astar astar;
	astar.InitAstar(maze);
	Point start start_position;
	Point end end_position;
	list<Point *> path = astar.GetPath(start, end, true);

	DWORD end_time = GetTickCount();
	cout << endl << "������";
	switch (mode)
	{
	case 1:cout << "����" << endl; break;
	case 2:cout << "A*" << endl; break;
	case 3:cout << "Dijistra" << endl; break;
	case 4:cout << "������ֻ�е�ǰ�����յ�����A*����" << endl; break;
	}
	cout <<endl<< "The run time is:" << (end_time - start_time) << "ms!" ;//�������ʱ��
	cout << endl << "���ص���" << astar.check_num << endl;
	cout << endl << "����·������" << astar.final_num << endl;

	cout << endl << "����/��С���ȶ��е�Ԫ���������ֵ" << max_num << endl;
	for (auto &p : path)
		cout << '(' << p->x << ',' << p->y << ')' << endl;
	

	while (1);
	return 0;
}