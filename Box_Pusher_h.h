//----------------------------------------------------------------------------------------
//		推箱子(控制台版)		作者:EyreFree		编译环境:VS2008		2012/11/30
//----------------------------------------------------------------------------------------


//包含头文件------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "Box_Map.h"


//参数------------------------------------------------------------------------------------

POINT me,old,trend;				//当前位置，旧的位置，探测位置
int size=0;						//地图尺寸


//隐藏光标--------------------------------------------------------------------------------

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


//定位光标--------------------------------------------------------------------------------

void Gotoxy(int x,int y)
{ 
	COORD pos; 
	pos.X = x - 1; 
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos); 
}


//数据初始化------------------------------------------------------------------------------

void Init()
{
	int i=0,j=0;
	me.x =0;
	me.y =0;
	trend.x =0;
	trend.y =0;
	old.x =0;
	old.y =0;
	size=sqrt(sizeof(map)/sizeof(int));		//获取地图边长
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(map[i][j]==4)				//获取人物初始位置
			{
				me.x =i;
				me.y =j;
			}
		}
	}
	if(me.x==0||me.y==0)					//人物位置非法则返回错误信息
	{
		MessageBox(NULL,"地图数据错误！\n\nInit","加载失败！",MB_ICONEXCLAMATION);
		system("pause");
		exit(0);
	}
}


//外观调整--------------------------------------------------------------------------------

void Appearance()
{
	char screen_size[26]="mode con cols=17 lines=09";
	screen_size[14]=((size-7)*2+23)/10+48;
	screen_size[15]=((size-7)*2+23)%10+48;
	screen_size[23]=(size-7+12)/10+48;
	screen_size[24]=(size-7+12)%10+48;
	system(screen_size);					//调整大小
	system("color f0");						//调整颜色
	system("title 推箱子");					//设置标题
	HideCursor();							//隐藏光标
}


//输出图形--------------------------------------------------------------------------------

void Showscreen()
{
	int i=0,j=0,temp=0;
	Gotoxy(1,1);				//光标移至左上角
	for(i=0;i<size;i++)
	{
		printf("\n  ");
		for(j=0;j<size;j++)
		{
			switch(map[i][j])
			{
			case 0:
				temp=0;
				break;
			case 1:
				temp=35;
				break;
			case 2:
				temp=43;
				break;
			case 3:
				temp=15;
				break;
			case 4:
				temp=1;
				break;
			default:			//地图信息非法则返回错误信息
				MessageBox(NULL,"地图数据错误！\n\nShowscreen","加载失败！",MB_ICONEXCLAMATION);
				system("pause");
				exit(0);
				break;
			}
			printf("%c ",temp);
		}
	}
}


//获胜判定--------------------------------------------------------------------------------

int Is_not_win()
{
	int i=0,j=0;
	for(i=0;i<size;++i)
	{
		for(j=0;j<size;++j)
		{
			if(map_goal[i][j]==2&&map[i][j]!=3)
				return 1;
		}
	}
	return 0;
}


//恢复目标--------------------------------------------------------------------------------

void Rollback()
{
	int i=0,j=0;
	for(i=0;i<size;++i)
	{
		for(j=0;j<size;++j)
		{
			if(map_goal[i][j]==2&&map[i][j]==0)
				map[i][j]=2;
		}
	}
}


//移动处理--------------------------------------------------------------------------------

void Move(int direct)
{
	POINT temp;
	trend=me;
	switch(direct)
	{
	case 'w':
		--trend.x;
		break;
	case 's':
		++trend.x;
		break;
	case 'a':
		--trend.y;
		break;
	case 'd':
		++trend.y;
		break;
	default:
		break;
	}
	temp=trend;
	if(map[trend.x][trend.y]==0||map[trend.x][trend.y]==2)
	{
		old=me;	
		me=trend;
	}
	else if(map[trend.x][trend.y]==3)
	{
		switch(direct)
		{
		case 'w':
			--temp.x;
			break;
		case 's':
			++temp.x;
			break;
		case 'a':
			--temp.y;
			break;
		case 'd':
			++temp.y;
			break;
		default:
			break;
		}
		if(map[temp.x][temp.y]==0||map[temp.x][temp.y]==2)
		{
			map[temp.x][temp.y]=3;
			map[trend.x][trend.y]=0;
			old=me;
			me=trend;
		}
	}
	if(map[me.x][me.y]!=4)
	{
		map[old.x][old.y]=0;
		map[me.x][me.y]=4;
		old=me;
	}
	Rollback();					//恢复被覆盖的目标图形
}


//循环------------------------------------------------------------------------------------

void Run()
{
	Showscreen();				//输出图形
	while(Is_not_win())			//非获胜判定成功则继续
	{
		Move(getch());			//接受输入移动
		Showscreen();			//输出图形
	}
	system("title 你赢了！");
	printf("\n\n  你赢了！\n\n  ");
	system("pause");
}