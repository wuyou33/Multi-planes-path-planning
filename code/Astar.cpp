#include<math.h>
#include<queue>
#include<stack>
#include "Astar.h"
int step = 0;
void Astar::InitAstar(std::vector<std::vector<int>>& _maze)   //��ά����
{
	maze = _maze;
}

std::list<Point*> Astar::GetfPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *result = findPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	//����·��,��δ�ҵ�,�򷵻ؿ�����
	while (result)
	{
		path.push_front(new Point(result->x, result->y));//��ǰ�����,����.
		result = result->parent;
	}
	while (!openList.empty())
	{
		auto curPoint = openList.front();
		openList.pop_front();
		delete curPoint;
	}
	//cout << step<<endl;
	return path;
}


int Astar::calcG(Point * temp_start, Point * point)   //��Χ�ƶ��ķ�
{
	int extraG = (abs(point->x - temp_start->x) + abs(point->y - temp_start->y) == 1) ? kCost1 : kCost2;  //�ж��Ƿ�Ϊб��
	int parentG = (point->parent == NULL) ? 0 : point->parent->G;
	return parentG + extraG;
}

int Astar::calcH(Point * point, Point * end)   //����,�����㷨�����Ż��ռ�!
{
	return (abs(end->x - point->x) + abs(end->y - point->y))*kCost1;   //�˴��ü��㷨:ֻ����ƽ����б��
}

int Astar::calcF(Point * point)
{
	return point->G + point->H;
}
 
/*
Point * Astar::getLeastFpoint(const Point * point,bool isIgnoreCorner)     //����Ϊֻ����Χδ�߹��ĵ���������
{
	if (point != NULL)
	{
		Point *fPoints, *temp;
		for (int x = point->x - 1; x <= point->x + 1; x++)
			for (int y = point->y - 1; y <= point->y + 1; y++)
				if (isCanreach(point, new Point(x, y), isIgnoreCorner, safe_distance))
					fPoints.push_back(new Point(x, y));
		return fPoints;

	}
	




	if (!openList.empty())
	{
		auto resPoint = openList.front();
		for (auto &point : openList)     //��openlist��ȡ��,  ***??***
			if (point->F < resPoint->F)
				resPoint = point;
		return resPoint;                   //���ؿ���������F��С�ĵ�
	}
	return nullptr; //c++�Ŀ�ָ��
}
*/
Point * Astar::findPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)  //�������ڴ�
{
	int flag=0;
	openList.push_back(new Point(startPoint.x, startPoint.y));   //���!!(ֻ��һ��
	Point * curPoint = openList.front();
	curPoint->parent = NULL;
	while (curPoint!=NULL)
	{
		step++;
		flag = 0;
		maze[curPoint->x][curPoint->y] = 3;//��ʶ�߹�·��
		auto surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner);   //����Χ�ĸ���
		for (auto &target : surroundPoints)
		{

			
			//target->parent = curPoint;
			//if(maze[target->x][target->y]==0)  //��target(��Χ��)���ڿ����б���,����  
			//{
				target->parent = curPoint;   //��ǰ��Ϊ�丸��
				target->G = calcG(curPoint, target);
				target->H = calcH(target, &endPoint);
				target->F = calcF(target);
				maze[target->x][target->y] = 2;  //open but not went
				//openList.push_back(target);   //����openList ��
			//}

			//����ѿ���,���������,��G,���G��,�򲻲���,���򻻸��ڵ�,
			//Ŀ��:���۾�������target,�ж�����curPoint���������.����,���ĸ����,ʹ������target,һ�����Ƶ�curPoint(Fֵ��С�ó�)

			/*

			else if (maze[target->x][target->y] == 2)  //��������û�߹�
						{
							//�ؼ���G
							int tempG = calcG(curPoint, target);
							//����Ƶ�curPoint����
							if (tempG + curPoint->G < target->G)  //make a correction
							{
								target->parent = curPoint;  //�����ڵ�
								target->G = tempG;
								target->F = calcF(target);   //F=G+H  H����
							}
						}
			*/
		}
	

		Point *resPoint = isInList(curPoint, &endPoint); //�ж�endPoint�Ƿ���openList֮��.
		if (resPoint)//endPoint is in openList
			return resPoint;   //��ͷ�����,�ʷ���resPoint�ڵ�,����endPoint

		//curPoint = getLeastFpoint(curPoint,isIgnoreCorner);  //�ҵ�Fֵ��С�ĵ�
		if (!surroundPoints.empty())
		{
			auto temPoint = curPoint;
			curPoint = surroundPoints.front();
			for (auto &target : surroundPoints)
			{
				openList.push_back(target);   //����openList ��
				if (target->F< curPoint->F)
				{
					curPoint = target;
				}
			}
		}
		else
		{
			curPoint = curPoint->parent;
			
		}
	}
	return nullptr;
}




Point * Astar::refindPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *fresult= findPath(startPoint, endPoint, isIgnoreCorner);
	while (fresult)
	{
		maze[fresult->x][fresult->y] = 4;  //��һ���߹��ĵ���4
		fresult = fresult->parent;
	}
	
	
	//��ʼbfs
	point_queue q;
	Point *p = new Point(startPoint.x, startPoint.y);
	p->parent = NULL;
	q.push(p);
	while (!q.empty())
	{
		Point *temp = q.front();
		if (temp->x == endPoint.x&&temp->y == endPoint.y)
			return temp;
		maze[temp->x][temp->y] = 6;
		for (int i = 0; ; )
		{
			for (int j = 1; j>-2; j--)
			{
				if (maze[temp->x + i][temp->y + j] == 4)
				{
					Point *next = new Point(temp->x + i, temp->y + j);
					next->parent = temp;
					next->G = calcG(temp, next);
					next->H = calcH(next, &endPoint);
					next->F = calcF(next);
					q.push(next);
					maze[temp->x + i][temp->y + j] = 6;

				}
				//��ǩУ����
				else if (maze[temp->x + i][temp->y + j] == 6)               
				{
					Point *next = new Point(temp->x + i, temp->y + j);
					next->G = calcG(temp, next);
					next->H = calcH(next, &endPoint);
					next->F = calcF(next);
					int pre_F = q.find_point(next);
					if (pre_F <= next->F)
						delete next;
					else														//��ǩУ����
					{
						q.del(next);
						next->parent = temp;
						q.push(next);
						
					}
				}
			}
			//�趨�����е���תȡ��˳��
			if (i == 0)
				i = 1;
			else if (i == 1)
				i = -1;
			else if (i == -1)
				break;
		}

		q.pop();
	}
	return NULL;
}



std::list<Point*> Astar::GetPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *result = refindPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	//����·��,��δ�ҵ�,�򷵻ؿ�����
	while (result)
	{
		path.push_front(new Point(result->x, result->y));//��ǰ�����,����.
		result = result->parent;
	}


	
	while (!openList.empty())
	{
		auto curPoint = openList.front();
		openList.pop_front();
		delete curPoint;
	}
	//cout << step<<endl;
	return path;
}


Point * Astar::isInList( Point * curPoint, const Point * point) const
{

	if (maze[point->x][point->y] == 2)
	{
		Point *p = new Point(point->x, point->y);
		p->parent = curPoint;
		return p;
	}
	return nullptr;
}


bool Astar::isCanreach(const Point * point, const Point * target, bool isIgnoreCorner, int safe_d) const  //�Ӱ�ȫ����
{
	if (target->x<0 || target->x>maze.size() - 1
		|| target->y<0 || target->y>maze[0].size() - 1
		|| maze[target->x][target->y] == 1
		|| (target->x == point->x&&target->y == point->y)
		|| maze[target->x][target->y]==3)//|| maze[target->x][target->y]==2,  2��Ҳ����ÿ���µ����Χ�㼯�ڣ����ܴ�����Զ����Ӱ�첻��
	{
		delete target;
		return false;
	}
	for (int i = -1; abs(i)<safe_d; i = (i<0) ? (-i) : (-i - 1))
	{
		for (int j = -1; abs(j)<safe_d; j = (j<0) ? (-j) : (-j - 1))
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


std::vector<Point*> Astar::getSurroundPoints(const Point * point, bool isIgnoreCorner) 
{
	std::vector<Point *> surroundPoints;
	for (int x = point->x - 1; x <= point->x + 1; x++)
		for (int y = point->y - 1; y <= point->y + 1; y++)
			if (isCanreach(point, new Point(x, y), isIgnoreCorner, safe_distance))
				surroundPoints.push_back(new Point(x, y));
	return surroundPoints;
}