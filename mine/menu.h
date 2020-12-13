/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:扫雷菜单栏,游戏玩家数据头文件
**************************************************/

#ifndef MENU_H
#define MENU_H

#include "mine.h"

//显示菜单
void menu_main(void);
//困难模式选择
void Difficult_mode(mine_t *data);
//排行榜显示（下载文档数据进行显示）
void Rank_Show(char);
//游戏数据存档
void SaveHeroRank(mine_t *data, int time, char *name);
//游戏数据节点初始化
LinkList PlayerDataInit();
//游戏数据节点插入数据
int PlayerDataInsert(pnode_t *p, pnode_t *);
//单链表排序（从大大小）插入排序
pnode_t *LinkInsertSort(pnode_t *head);
//游戏数据下载
LinkList LoadData(char mode);
//游戏数据销毁
void RankDestory();
//游戏数据清空
int RankClear();
//打印链表前十
void LinkShow(pnode_t *p);
//链表释放
void Free_List(LinkList head);

#endif // MENU_H
