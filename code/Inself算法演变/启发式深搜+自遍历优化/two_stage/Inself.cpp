#include<math.h>
#include<queue>
#include<stack>
#include "Inself.h"//����ʽ����inspiredfs + �Ա����Ż�self

void Inself::InitInself(std::vector<std::vector<int>>& _maze)   //��ά����
{
	maze = _maze;
	inspire_num = 0;
	final_num = 0;
}

//ֻ������ʽ����
std::list<Point*> Inself::GetfPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *result = findPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	//����·��,��δ�ҵ�,�򷵻ؿ�����
	while (result)
	{
		path.push_front(new Point(result->x, result->y));//��ǰ�����,����.
		result = result->parent;

	}

	return path;
}


//����ʽ����+�Ա����Ż�
std::list<Point*> Inself::Self(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	int dx;
	int dy;
	//int degree_last;
	//int degree_tmp;
	//�����·��from����ʽ���ѣ�  ->parent�������ӽڵ���
	Point *result = findPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	while (result)//ֻҪ��Ϊ�գ��ͱ���
	{
		//degree_last = 0;
		//degree_tmp = 0;
		Point *temp = result->parent;
		//��¼��ʼ�����ڵ�һ���յ�֮��ʼ��
		if (temp)
		{
			dx = temp->x - result->x;
			dy = temp->y - result->y;
		}
		while (temp)//�ڶ�����ѭ��,�ʵ�ʱ�޸�result->parent������break��
		{
			if ((temp->parent) && (temp->parent->x - temp->x == dx) && (temp->parent->y - temp->y == dy))
				;
			//degree_tmp = calDegree(result, tmp);
			//if()
			else if (isoneline(result, temp,isIgnoreCorner))
				{
					if (islineok(result, temp))
					{
						//����
						extend(result, temp);
					}
					else
					{
						//����
						break;
					}
				}
			temp = temp->parent;
		}
		//����path����Ϊ���𣬹ʴ�ǰ������
		path.push_front(new Point(result->x, result->y));//��ǰ�����,����.
		//cout << result->x << "," << result->y << endl;
	
		final_num++;

		result = result->parent;
	}

	return path;
}





int Inself::calcG(Point * temp_start, Point * point)   //��Χ�ƶ��ķ�
{
	int extraG = (abs(point->x - temp_start->x) + abs(point->y - temp_start->y) == 1) ? kCost1 : kCost2;  //�ж��Ƿ�Ϊб��
	int parentG = (point->parent == NULL) ? 0 : point->parent->G;
	return parentG + extraG;
}

int Inself::calcH(Point * point, Point * end)   //����,�����㷨�����Ż��ռ�!
{
	return (abs(end->x - point->x) + abs(end->y - point->y))*kCost1;   //�˴��ü��㷨:ֻ����ƽ����б��
}

int Inself::calcF(Point * point)
{
	return point->G + point->H;
}

float  Inself::calDegree(Point *p1, Point *p2)
{
	return (float)(p2->y - p1->y)/(p2->x - p1->x);
}

bool  Inself::isoneline(const Point *p1, const Point *p2, bool isIgnoreCorner) //�ܷ�б���ƶ�
{
	if ((p2->x == p1->x)||(p2->y == p1->y)|| (abs(p2->y - p1->y) == abs(p2->x -p1->x)) )
	{
		if ((abs(p2->y - p1->y) == abs(p2->x - p1->x)))
			return isIgnoreCorner;
		else
			return true;
	}
	else
		return false;
}
bool  Inself::islineok(const Point *point, const Point *target)  //��ͨ��isoneline��������ҪisIgnoreCorner���������б���ƶ���ͨ�������жϲ������˺���
{
	int dx;
	int dy;
	if ((target->x - point->x) == 0)
		dx = 0;
	else
		dx = (target->x - point->x) / (abs(target->x - point->x));
	if ((target->y - point->y) == 0)
		dy = 0;
	else
		dy = (target->y - point->y) / (abs(target->y - point->y));
	int tx = point->x;
	int ty = point->y;
	while ((tx != target->x) || (ty != target->y))
	{
		tx = tx + dx;
		ty = ty + dy;
		if (maze[tx][ty]==1)//���ϰ���
			return false;
	}

	return true;
}

void Inself::extend(Point *p1, Point *p2)
{
	Point *last = p1;
	int dx;
	int dy;
	if ((p2->x - p1->x) == 0)
		dx = 0;
	else
		dx = (p2->x - p1->x) / (abs(p2->x - p1->x));
	if ((p2->y - p1->y) == 0)
		dy = 0;
	else
		dy = (p2->y - p1->y) / (abs(p2->y - p1->y));
	int tempx = p1->x;
	int tempy = p1->y;
	while (tempx != p2->x || tempy != p2->y)
	{
		tempx += dx;
		tempy += dy;
		Point *tt = new Point(tempx, tempy);
		//�ν�ָ��
		tt->parent = last->parent;
		last->parent = tt;

		last = last->parent; //���ֵ�tt
	}
	last->parent = p2->parent;//����ԭp2�������·
}


Point * Inself::findPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)  //�������ڴ�
{
	
	Point *curPoint = new Point(startPoint.x, startPoint.y);
	curPoint->parent = NULL;
	while (curPoint != NULL)
	{
		//��һ������ʽ������Ŀ+1
		inspire_num++;

		maze[curPoint->x][curPoint->y] = 3;//��ʶ�߹�·��
		auto surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner);   //����Χ�ĸ���
		for (auto &target : surroundPoints)
		{
			target->parent = curPoint;   //��ǰ��Ϊ�丸��
			target->G = calcG(curPoint, target);
			target->H = calcH(target, &endPoint);
			target->F = calcF(target);
			maze[target->x][target->y] = 2;  //open but not went
			
		}


		Point *resPoint = isInList(curPoint, &endPoint); //�ж�endPoint�Ƿ���openList֮��.
		if (resPoint)//endPoint is in openList
			return resPoint;   //��ͷ�����,�ʷ���resPoint�ڵ�,����endPoint

		//�ҵ�Fֵ��С�ĵ�
		if (!surroundPoints.empty())
		{
			
			curPoint = surroundPoints.front();
			for (auto &target : surroundPoints)
			{
				//openList.push_back(target);   //����openList ��

				if (target->F < curPoint->F)
				{
					curPoint = target;
				}
			}
		}
		else
		{
			curPoint = curPoint->parent; //�˻���һ��

		}
	}
	return nullptr;
}



Point * Inself::isInList(Point * curPoint, const Point * point) const
{

	if (maze[point->x][point->y] == 2)
	{
		Point *p = new Point(point->x, point->y);
		p->parent = curPoint;
		return p;
	}
	return nullptr;
}


bool Inself::isCanreach(const Point * point, const Point * target, bool isIgnoreCorner, int safe_d) const  //�Ӱ�ȫ����
{
	if (target->x<0 || target->x>maze.size() - 1
		|| target->y<0 || target->y>maze[0].size() - 1
		|| maze[target->x][target->y] == 1
		|| (target->x == point->x&&target->y == point->y)
		|| maze[target->x][target->y] == 3)//|| maze[target->x][target->y]==2,  2��Ҳ����ÿ���µ����Χ�㼯�ڣ����ܴ�����Զ����Ӱ�첻��
	{
		delete target;
		return false;
	}
	
	for (int i = -1; abs(i) < safe_d; i = (i < 0) ? (-i) : (-i - 1))//�����̽��
	{
		for (int j = -1; abs(j) < safe_d; j = (j < 0) ? (-j) : (-j - 1))
		{
			if (maze[target->x + i][target->y + j] == 1)
			{
				delete target;
				return false;
			}
		}
	}
	if (abs(point->x - target->x) + abs(point->y - target->y) == 1)
	{
		delete target;
		return true;
	}
	else
	{
		delete target;
		return isIgnoreCorner;// �ж�б�Խ��Ƿ��ס
	}
}


//̽����Χ�ɵ����
std::vector<Point*> Inself::getSurroundPoints(const Point * point, bool isIgnoreCorner)//clock 1:˳ʱ��,���Ͻǿ�ʼ,2:��ʱ��
{
	//int temp_clock = 1;
	int temp_tmp_x = 0;
	int temp_tmp_y = 0;
	std::vector<Point *> surroundPoints;
	//for (int x = point->x - 1; x <= point->x + 1; x++)
	//	for (int y = point->y - 1; y <= point->y + 1; y++)

		for (int temp_ii = 0; temp_ii < 8; temp_ii++)
		{
			switch (temp_ii) {
			case 0:
				temp_tmp_x = -1;
				temp_tmp_y = 1;
				break;
			case 1:
				temp_tmp_x = 0;
				temp_tmp_y = 1;
				break;
			case 2:
				temp_tmp_x = 1;
				temp_tmp_y = 1;
				break;
			case 3:
				temp_tmp_x = 1;
				temp_tmp_y = 0;
				break;
			case 4:
				temp_tmp_x = 1;
				temp_tmp_y = -1;
				break;
			case 5:
				temp_tmp_x = 0;
				temp_tmp_y = -1;
				break;
			case 6:
				temp_tmp_x = -1;
				temp_tmp_y = -1;
				break;
			case 7:
				temp_tmp_x = -1;
				temp_tmp_y = 0;
				break;
			default:break;
			}
			if (isCanreach(point, new Point(point->x + temp_tmp_x, point->y + temp_tmp_y), isIgnoreCorner, safe_distance))
				surroundPoints.push_back(new Point(point->x + temp_tmp_x, point->y + temp_tmp_y));
		}

	return surroundPoints;
}