#include "Astar.h"

int mode = 2; //1:���ѣ�2:A* , 3: dijistra����С���ȶ��еı�׼F��ΪԴ����G��  4:A* ������ֻ��H
//��1������;   2&4ʹ��F��1&3ʹ��G ��  4��F=H

//��С���ȶ��е���ʷ�������
int max_num;
int max_num_temp;

void Astar::InitAstar(std::vector<std::vector<int>>& _maze)   //��ά����
{
	maze = _maze;
	check_num = 0;
	final_num = 0;

}


Point * Astar::find_GetPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{

	Point *curPointa=new Point(startPoint.x, startPoint.y);
	//��Ҫ
	curPointa->G = 0;

	curPointa->parent = NULL; //���ã�Ĭ��null
	openlist.push_back(curPointa);

	while (!openlist.empty())
	{
		if(mode!=1)
		openlist.sort(less<Point *>());//����,�����в����˲�
		
		/*std::list<Point *>::iterator ita;
		for (ita = openlist.begin(); ita != openlist.end(); ita++)
		{
			cout << (*ita)->F << endl;
		}
		system("pause");*/

		Point *curPoint = openlist.front();  //�ҵ�һ��
		openlist.pop_front();
		maze[curPoint->x][curPoint->y] = 7;//��ʶ�߹�·��
		max_num_temp--;
		//test print
		//cout << curPoint->x << "," << curPoint->y << endl;
		
		auto surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner);   //����Χ�ĸ���
		for (auto &target : surroundPoints)
		{
			target->G = calcG(target, curPoint);

			if(maze[target->x][target->y] == 0)
			{
				target->parent = curPoint;   //��ǰ��Ϊ�丸��
				//target->G = calcG(target, curPoint);
				target->H = calcH(target, &endPoint);
				target->F = calcF(target);
				maze[target->x][target->y] = 6;  //open but not went
				//����openlist
				openlist.push_back(target);

				check_num++;
				max_num_temp++;
				if (max_num_temp > max_num)
					max_num = max_num_temp;
			}
			else if ((!openlist.empty()) && mode != 1 && maze[target->x][target->y] == 6) //������&�����ڵ�
			{
				std::list<Point *>::iterator it;
				for (it = openlist.begin(); it != openlist.end(); it++)
				{
					if ((*it)->x == target->x && (*it)->y == target->y)
					{
						if (((mode == 2||mode==4) && target->F < (*it)->F)||(mode ==3 && target->G < (*it)->G))   //����ģʽ����������ǰ��G
						{
							//��Ҫ����,�����ԣ���listֱ�ӸĲ���ȡ��ȫ���0������ɾ���ټ�
							openlist.erase(it);
							target->parent = curPoint;   //��ǰ��Ϊ�丸��
							//target->G = calcG(target, curPoint);
							target->H = calcH(target, &endPoint);
							target->F = calcF(target);
							openlist.push_back(target);
						}
						break;
					}
				}
			}
		}


		Point *resPoint = isInList(curPoint, &endPoint); //�ж�endPoint�Ƿ���openList֮��.
		if (resPoint)//endPoint is in openList
			return resPoint;   //��ͷ�����,�ʷ���resPoint�ڵ�,����endPoint

	}
	return nullptr;
}



std::list<Point*> Astar::GetPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *result = find_GetPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	//����·��,��δ�ҵ�,�򷵻ؿ�����
	while (result)
	{
		path.push_front(new Point(result->x, result->y));//��ǰ�����,����.
		final_num++;
		result = result->parent;
	}

	return path;
}


///////////////////////////////////////////////////////////////////////////////////////

int Astar::calcG(Point * temp_start, Point * point)   //��Χ�ƶ��ķ�,b->a
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
	if(mode == 4)
		return   point->H;
	else
		return   point->G + point->H;
}



Point * Astar::isInList(Point * curPoint, const Point * point) const
{

	if (maze[point->x][point->y] == 6|| maze[point->x][point->y] == 7)
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
		|| maze[target->x][target->y] == 7)  //7:closed   //|| maze[target->x][target->y]==2,  2��Ҳ����ÿ���µ����Χ�㼯�ڣ����ܴ�����Զ����Ӱ�첻��
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
std::vector<Point*> Astar::getSurroundPoints(const Point * point, bool isIgnoreCorner)
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