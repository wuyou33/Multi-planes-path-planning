#include<iostream>
#include<conio.h>
#include"Astar.h"
#include"Adjust.h"
#define start_position (8,8)
#define end_position (192,192)
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
	init_map(maze);//��ʼ����ͼ���߽硣

	Astar astar;
	astar.InitAstar(maze);
	Point start start_position;
	Point end end_position;
	list<Point *> path = astar.GetPath(start, end, true);
	resetmap(maze);
	if (path.empty())
	{
		cout << "no path" << endl;
		exit(0);
	}
	//for (auto &p : path)
	//	cout << '(' << p->x << ',' << p->y << ')' << endl;
		
	Plane plane[10];
	for (int ik = 0; ik < 10; ik++)
		plane[ik].step = 0;

	int break_times = 0;
	startpoint(start.x, start.y, plane, maze, end.x - start.x);
	adjust(plane, maze, path, end.x - start.x, end,break_times);
	
	DWORD end_time = GetTickCount();
	gotoxy(201, 201);
	cout <<endl<< "The run time is:" << (end_time - start_time) << "ms!" ;//�������ʱ��
	for (int ik = 0; ik < 10; ik++)
		cout << endl<<ik<<"�ŷɻ����ܲ���Ϊ" << plane[ik].step;
	cout << endl << "�ܳ�ͻ��Ϊ" << break_times;
	while (1);
	return 0;
}