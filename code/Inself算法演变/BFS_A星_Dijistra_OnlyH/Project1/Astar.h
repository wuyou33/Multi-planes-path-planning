#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<list>
#include<math.h>
#include<queue>
#include<stack>

extern int mode; //1:���ѣ�2:A* , 3: dijistra����С���ȶ��еı�׼F��ΪԴ����G�� 4:A* ������ֻ��H
//��1������;   2&4ʹ��F��1&3ʹ��G ��  4��F=H

const int kCost1 = 10;//�ڸ�
const int kCost2 = 14;//б������
const int safe_distance = 0; //��ȫ����

//��С���ȶ��е���ʷ�������
extern int max_num;
extern int max_num_temp;

struct Point {

	friend bool operator <(Point p1, Point p2)
	{
		if(mode == 2|| mode == 4)
			return p1.F < p2.F;
		else
			return p1.G < p2.G;
	}
	friend bool operator >(Point p1, Point p2)
	{
		if (mode == 2|| mode == 4)
			return p1.F > p2.F;
		else
			return p1.G > p2.G;
	}
	int x, y;
	int F, G, H;
	Point *parent;
	Point(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(NULL)
	{
	}
	/*Point(int _x, int _y,int _G,int _H,int _F) :x(_x), y(_y), G(_G), H(_H), F(_F), parent(NULL)
	{
	}*/
};
template<>
struct std::less<Point*>
{
	bool operator()(const Point* a1, const Point* a2) const
	{
		return *a1 < *a2;
	}
};
class Astar {
public:
	void InitAstar(std::vector<std::vector<int>>&_maze);
	std::list<Point *> GetfPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner);
	std::list<Point *> GetPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner); //���յ㣬�Ƿ���б����
private:
	Point *find_GetPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner);
	std::vector<Point *>getSurroundPoints(const Point *point, bool isIgnoreCorner) ;
	bool isCanreach(const Point *point, const Point *target, bool isIgnoreCorner, int safe_d) const;//judge if there is a solution
	Point *isInList(Point * curPoint, const Point *point)const;
	
	int calcG(Point *temp_start, Point *point);
	int calcH(Point *point, Point *end);
	int calcF(Point *point);

public:
	int check_num; //���ص���
	int final_num;  //����·������

private:
	std::vector<std::vector<int>> maze;
	std::list<Point *> openlist;
};

/*
class point_queue {
public:
	struct que_point {
		Point *data;
		struct que_point *next;
		struct que_point *last;
	};
	point_queue() 
	{
		pfront = NULL;
		prear = NULL;
		max_num = 0;
		max_num_temp = 0;
	}
	bool empty()
	{
		if (pfront == NULL || prear == NULL)
		{
			pfront = NULL;
			prear = NULL;
			return 1;
		}
		else
			return 0;
	}
	void pop() {
		if (!empty())
		{
			max_num_temp--;
			pfront = pfront->next;
			if(pfront!=NULL)
			pfront->last = NULL;
		}
		if (pfront == NULL)
			prear = NULL;
	}
	Point *front()
	{
		if (!empty())
		{
			return pfront->data;
		}
		else
			return NULL;
	}
	
	int find_point_F(Point *input)
	{
		if (!empty())
		{
			struct que_point *temp = prear;
			while (temp != NULL && (temp->data->x != input->x || temp->data->y != input->y))
				temp = temp->last;
			if (temp != NULL)
				return temp->data->F;
			else
				return 0;
		}
		else
			return 0;
	}
	
	void push(Point *input)//��ͨ���
	{
		max_num_temp++;
		if (max_num_temp > max_num)
			max_num = max_num_temp; //�������ֵ
		if (empty())
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;
			temp->next = NULL;
			temp->last = NULL;
			prear = temp;
			pfront = temp;
		}
		else
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;
			temp->next = NULL;
			temp->last = prear;
			prear->next = temp;
			prear = prear->next;
		}
	}

	void push_prior(Point *input)//F��С�����롰�ӡ�����A*�㷨
	{
		max_num_temp++;
		if (max_num_temp > max_num)
			max_num = max_num_temp; //�������ֵ


		if (empty())
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;
			temp->next = NULL;
			temp->last = NULL;
			prear = temp;
			pfront = temp;
		}
		else
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;

			struct que_point * itera = pfront;
			//�ݹ鵽iteraָ����һ����input��Fֵ���λ�ã���NULL
			while (itera&&(itera->data->F <= input->F))
			{
				itera = itera->next;
			}
			//��ͷ��������
			if (itera == pfront)
			{
				pfront = temp;
				temp->last = nullptr;
				temp->next = itera;
				itera->last = temp;
			}
			//�������
			else if (itera)
			{
				temp->last = itera->last;
				temp->next = itera;
				if(itera->last)
					itera->last->next = temp;
				itera->last = temp;

			}
			//��β
			else
			{
				temp->next = NULL;
				temp->last = prear;
				prear->next = temp;
				prear = prear->next;
			}

		}
	}


	void push_d(Point *input)//G��С�����롰�ӡ�����Dijistra�㷨
	{
		max_num_temp++;
		if (max_num_temp > max_num)
			max_num = max_num_temp; //�������ֵ


		if (empty())
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;
			temp->next = NULL;
			temp->last = NULL;
			prear = temp;
			pfront = temp;
		}
		else
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;

			struct que_point * itera = pfront;
			//�ݹ鵽iteraָ����һ����input��Gֵ���λ�ã���NULL
			while (itera && (itera->data->G <= input->G))
			{
				itera = itera->next;
			}
			//��ͷ��������
			if (itera == pfront)
			{
				pfront = temp;
				temp->last = nullptr;
				temp->next = itera;
				itera->last = temp;
			}
			//�������
			else if (itera)
			{
				temp->last = itera->last;
				temp->next = itera;
				if (itera->last)
					itera->last->next = temp;
				itera->last = temp;

			}
			//��β
			else
			{
				temp->next = NULL;
				temp->last = prear;
				prear->next = temp;
				prear = prear->next;
			}

		}
	}
	void del(Point *input) //ֻɾһ�����ҽ���ʹ��
	{
		struct que_point *temp = prear;
		while (temp != NULL &&temp!=pfront && (temp->data->x != input->x || temp->data->y != input->y))
			temp = temp->last;//�Ӻ���ǰ�ҵ��˵�
		if (temp != NULL&& temp->data->x != input->x && temp->data->y != input->y)
		{
			//����ͷָ��Ҳ��βָ��
			if (temp != pfront&&temp != prear)
			{
				if (temp->last != NULL)
					temp->last->next = temp->next;
				if (temp->next != NULL)
					temp->next->last = temp->last;
			}


			
			//ɾ��ǰ�����ͷβָ������
			else if (temp == prear&&temp!=pfront)
			{
				prear = prear->last;
				prear->next = NULL;

			}
			else if (temp == pfront&&temp!=prear)
			{
				pfront = pfront->next;
				pfront->last = NULL;

			}
			else if (temp == pfront&&temp == prear)
			{
				pfront = NULL;
				prear = NULL;
			}
			
			delete temp;

			max_num_temp--;

		}
		
	}


private:
	struct que_point *pfront;//ͷָ��
	struct que_point *prear;//βָ��
};*/