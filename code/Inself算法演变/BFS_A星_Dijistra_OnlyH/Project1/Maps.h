#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<list>
#include"Astar.h"
#include<time.h>
#include<Windows.h>         //���������ϰ���
#define msize 200
#define barrier_num 30     //�ϰ�������
#define safe_distance 1    //��ȫ����
#define safe_value 9     //�ȴ�ת��Ϊ1�İ�ȫֵ
char out = '>';
 unsigned long _time=100;


 //map L
 void init_map_L(std::vector<std::vector<int>>& maze)
 {
	 for (int i = 0; i < msize; i++) {
		 maze[i].resize(msize);
	 }
	 //�߽��ϰ���
	 for (int i = 0; i < msize; i++)
	 {
		 if (i == 0 || i == msize - 1)
		 {
			 for (int j = 0; j < msize - 1; j++)
			 {
				 maze[i][j] = 1;
				 //	gotoxy(i, j);
					 //cout << maze[i][j];
			 }
		 }
		 else {
			 maze[i][0] = 1;
			 //gotoxy(i, 0);
			 //cout << maze[i][0];
			 maze[i][msize - 1] = 1;
			 //gotoxy(i, msize - 1);
			 //cout << maze[i][msize - 1];
			 for (int j = 1; j < msize - 2; j++)
				 maze[i][j] = 0;
		 }
	 }

	 //����"L"�ϰ�
	 for (int i = 50; i < 151; i++)
	 {
		 maze[i][150] = 1;
		 maze[150][i] = 1;
	 }
 }



 void resetmap(std::vector<std::vector<int>>& maze)
 {
	 for (int i = 0; i < msize; i++)
	 {
		 for (int j = 0; j < msize; j++)
		 {
			 if (maze[j][i] != 0 && maze[j][i] != 1)   //ע�⣬\n�ǻ�y�����Ա���һ��y��Ӧ��x��Ȼ��\n
				 maze[j][i] = 0;
		 }
	 }
 }

 int research_num(std::vector<std::vector<int>>& maze)
 {
	 int num = 0;
	 for (int i = 0; i < msize; i++)
	 {
		 for (int j = 0; j < msize; j++)
		 {
			 if (maze[j][i] != 0 && maze[j][i] != 1)
				 num++;
		 }
	 }
	 return num;
 }