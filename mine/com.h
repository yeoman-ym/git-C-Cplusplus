/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:扫雷公共数据头文件
**************************************************/

#ifndef COM_H
#define COM_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iomanip>
#include <time.h>
using namespace std;

#define PRI_ROW 9                               /*********初级难度*********/
#define MID_ROW 16                              /*********中级难度*********/
#define SEN_ROW 22                              /*********高级难度*********/

#define PRI_MINE 10                             /*********初级雷数*********/
#define MID_MINE 40                             /*********初级雷数*********/
#define SEN_MINE 90                             /*********初级雷数*********/

#define FILENAME_1 "hero_mine_1.data"           /*********模式一游戏存档*********/
#define FILENAME_2 "hero_mine_2.data"           /*********模式二游戏存档*********/
#define FILENAME_3 "hero_mine_3.data"           /*********模式三游戏存档*********/

#define RANKNUM 10                              /*********排行榜人数*********/

/**************玩家结构体****************/
typedef struct rank
{
    char name[16];                              /*********玩家名字*********/
    double time;                                /*********通关时间*********/
    char mode;                                  /*********游戏难度*********/
}rank_t;

/**************扫雷结构体****************/
typedef struct mine
{
    int row;                                    /*************游戏行数***************/
    int mine_num;                               /*************雷的数量***************/
    char **board;                               /*********储存地雷信息（后台）*********/
    char **canvas;                              /*********玩家显示棋盘（前台）*********/
}mine_t;

/**************玩家数据链表节点****************/
typedef struct playernode
{
    char name[16];                              /*********玩家名字*********/
    int time;                                /*********通关时间*********/
    char mode;                                  /*********游戏难度*********/
    struct playernode *next;                    /*********节点指针*********/
}pnode_t, *LinkList;

/*************************************************
Function:       // 系统安全检查
Input:          // X Y
Output:         // MESSAGE
Return:         // Z
Others:         // 无返回值,
*************************************************/
#define SYSERR(X,OPTION,Y,MESSAGE,Z) if((X)OPTION(Y))\
                                {\
                                    cout << __FILE__ << ':' << __func__ << ':' << __LINE__  << ':' << MESSAGE << endl;\
                                    return Z;\
                                }

#endif // COM_H
