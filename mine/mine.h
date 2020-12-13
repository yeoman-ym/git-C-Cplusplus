/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:扫雷游戏设置相关头文件
**************************************************/

#ifndef MINE_H
#define MINE_H

#include "com.h"

//游戏初始化（生成棋盘和画布）
int mine_init(mine_t *data);
//棋盘和画布的列数
int board_init(char **data, int num);
//随机布雷(初始化棋盘)
void Random_Mine(mine_t *data);
//排雷
void Exclude_Mine(mine_t *data);
//显示画布（玩家）
void DisplayCanvas(mine_t *data);
//显示棋盘（后台）（游戏结束+调试）
void DisplayBoard(mine_t *data);
//第一步安全设置
void FirstStep_SafeMine(mine_t *data);
//统计一个坐标周围的雷数
int GetMineNum(mine_t *data, int x, int y);
//周围没有雷自动展开棋盘
void OpenMine(mine_t *data, int x, int y);
//初始化画布
void Init_Canvas(mine_t *data);
//统计真实雷的个数
int TrueMineNum(mine_t *data, int *pcount);
//统计剩余区域大小
int CountMine(mine_t *data);
//统计旗子个数
int FlagCount(mine_t *data);

#endif // MINE_H
