/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:Mine about menu（扫雷游戏菜单相关）
**************************************************/

#include "menu.h"


/*************************************************
Function:       // 扫雷游戏开始菜单显示
Description:    // 扫雷：游戏开始，难度选择，排行榜，退出游戏等功能
Calls:          //
Input:          // 输入1，2，3选择游戏对应功能，4退出
Output:         // 进入游戏，模式选择，排行榜界面，游戏退出界面
Return:         // void
Others:         // 无
*************************************************/
void menu_main(void)
{
    mine_t *data = (mine_t *)malloc(sizeof(mine_t));
    SYSERR(data,==,NULL,"malloc err\n",);

/***********初始化（默认初级难度）************/
    data->row = PRI_ROW;
    data->mine_num = PRI_MINE;

    char buf[4] = "";
    while(true){
        system("clear");
        cout << "****************       MINE        *****************" << endl;
        cout << endl;
        cout << "****************   1.Start Game    *****************" << endl;
        cout << "****************   2.Riddli Level  *****************" << endl;
        cout << "****************   3.Hero  Rank    *****************" << endl;
        cout << "****************   4.Quit  Game    *****************" << endl;
        cout << endl;
        cout << "****************      Choose       *****************" << endl;

        scanf("%3s", buf);

/***********退出游戏************/
        if(buf[0] == '4'){
            cout << endl;
            cout << "************     SEE YOU     ************" << endl;
            cout << endl;
            break;
        }

        switch (buf[0])
        {
            case '1':
            {

                mine_init(data);                        //生成棋盘

                Random_Mine(data);                      //随机布雷(初始化+布雷)      0 or 1

                Init_Canvas(data);                      //初始化画布               空格 or *

                DisplayCanvas(data);                    //显示画布

                DisplayBoard(data);                     //debug mode

                FirstStep_SafeMine(data);               //第一次点击

                Exclude_Mine(data);                     //排雷 （记录排雷时间）

                break;
            }
            case '2':
                //模式选择（修改棋盘大小和雷的数量）
                Difficult_mode(data);
                break;
            case '3':
            {
                //打印排行榜
                cout << "************         Input Num to Display Rank         ******************" << endl;
                cout << endl;
                cout << "************   1.Normal mode 2.Middle mode 3.Senior mode    ************" << endl;
                cout << endl;

                char ch[4] = "";
                cin.clear();cin.sync();
                cin >> ch;

                Rank_Show(ch[0]);
                break;
            }   //case 行数较多需要加括号，不加{}会报错
            default:
            {
                cout << endl;
                cout << "************    Input Error, Please Input Again    **********************" << endl;
                cout << endl;
                cout << "************          Enter to Continue         *************************" << endl;
                getchar();
                getchar(); //接收回车
                break;
            }
        }
    }
    return;
}

/*************************************************
Function:       // 排行榜显示（仅显示前10）
Description:    // 选择不同的游戏难度进行显示排行榜
Calls:          // player_ordinary ...
Input:          // 难度模式
Output:         // 更新后的玩家数据
Return:         // 无
Others:         // 1.下载文档数据到链表2.链表排序3.打印链表前十位
*************************************************/
void Rank_Show(char mode)
{
    system("clear");
    LinkList p = LoadData(mode);        //下载数据到链表     已测试有文件
    LinkInsertSort(p);                  //链表排序
    LinkShow(p);                        //打印链表前十
}

/*************************************************
Function:       // 玩家数据下载
Description:    // 将玩家数据装载到链表中
Calls:          // PlayDataInsert
Input:          // 难度模式
Output:         // 从对于的文件拷贝数据给链表
Return:         // 填写玩家数据的链表
Others:         // 无
*************************************************/
LinkList LoadData(char mode)
{
    FILE *fp = NULL;
    if(mode == '1')
    {
        fp = fopen(FILENAME_1,"r");
        SYSERR(fp,==,NULL,"file open erro\n",NULL);
    }
    else if(mode == '2')
    {
        fp = fopen(FILENAME_2,"r");
        SYSERR(fp,==,NULL,"file open erro\n",NULL);
    }
    else if(mode == '3')
    {
        fp = fopen(FILENAME_3,"r");
        SYSERR(fp,==,NULL,"file open erro\n",NULL);
    }

    //从文件FILENAME取数据放于结构体中
    LinkList head = PlayerDataInit();
    SYSERR(head,==,NULL,"list init error\n",NULL);

/******************
    do{
        pnode_t *tmp;
        tmp = (LinkList)malloc(sizeof(pnode_t));  //开辟一个节点空间
        SYSERR(tmp,==,NULL,"tmp malloc error\n",NULL);

        tmp->mode = mode;
        fscanf(fp, "%s\t%d\n", tmp->name, &tmp->time);

        PlayerDataInsert(head, tmp);
    }while(!feof(fp));
******************/
    int tmp_time;           //存放临时时间
    char ch[16] = "";       //存放临时名字
    pnode_t *p, *q;
    p = q = head;

    while(fscanf(fp, "%s\t%d\n", ch, &tmp_time) != EOF)      //读取文件数据
    {
        q = (pnode_t*)malloc(sizeof(pnode_t));
        q->mode = mode;
        strcpy(q->name, ch);
        q->time = tmp_time;

        p->next = q;
        p = q;
        //cout << "name = " << ch << endl;                测试结果：有数据
        //cout << "time = " << tmp_time << endl;
    }
    p->next = NULL;

    fclose(fp);                                 //数据读取完毕，关闭文件指针

    return head;

}

/*************************************************
Function:       //  数据存档
Description:    //  记录玩家数据，姓名，通关时间，游戏难度
Calls:          //
Input:          //  棋盘数据，通关时间，名字
Output:         //  数据文件
Return:         //  无
Others:         //  记录所有游戏玩家人数
*************************************************/
void SaveHeroRank(mine_t *data, int time, char *name)
{
    switch (data->row) {
        case PRI_ROW:
        {
            FILE *fp = fopen(FILENAME_1,"a+");
            SYSERR(fp,==,NULL,"file open error\n",);
            SYSERR(data,==,NULL,"mine data is null",);

            fprintf(fp, "%s\t%d\n", name, time);
            fclose(fp);
            break;
        }
        case MID_ROW:
        {
            FILE *fp = fopen(FILENAME_2,"a+");
            SYSERR(fp,==,NULL,"file open error\n",);
            SYSERR(data,==,NULL,"mine data is null",);

            fprintf(fp, "%s\t%d\n", name, time);
            fclose(fp);
            break;
        }
        case SEN_ROW:
        {
            FILE *fp = fopen(FILENAME_3,"a+");
            SYSERR(fp,==,NULL,"file open error\n",);
            SYSERR(data,==,NULL,"mine data is null",);

            fprintf(fp, "%s\t%d\n", name, time);
            fclose(fp);
            break;
        }
    }
}

/*************************************************
Function:       // 玩家数据链表初始化
Description:    // 名字为NULL，时间为0， 模式为'0'
Calls:          // malloc
Input:          // 无
Output:         // 链表头指针
Return:         // 指针
Others:         // 无
*************************************************/
LinkList PlayerDataInit()
{
    pnode_t *p = (pnode_t *)malloc(sizeof(pnode_t));
    //strcpy(p->name,"NULL");
    //p->time = 0;
    //p->mode = '0';
    p->next = NULL;

    return p;
}

/*************************************************
Function:       // 数据插入
Description:    // 头后部插入新数据
Calls:          // 无
Input:          // 头指针，新数据指针
Output:         // 新链表
Return:         // 0
Others:         // 无
*************************************************/
int PlayerDataInsert(pnode_t *head, pnode_t *value)
{
    SYSERR(value,==,NULL,"new data is null\n",-1);
    SYSERR(head,==,NULL,"list is null\n",-1);

    value->next = head->next;
    head->next = value;

    return 0;
}

/*************************************************
Function:       // 单链表排序
Description:    // 插入法排序，节点时间成员大小
Calls:          //
Input:          // 头指针
Output:         // 拍好序的链表
Return:         // 头指针
Others:         // 无
*************************************************/
//单链表排序（从大到小）插入排序
LinkList LinkInsertSort(pnode_t *head)
{
    LinkList p, q, pre;
    p = head->next->next;
    head->next->next = NULL;
    while(p)
    {
        q = p->next;
        pre = head;
        while(pre->next != NULL && pre->next->time < p->time)
            pre = pre->next;
        p->next = pre->next;
        pre->next = p;
        p = q;
    }
    return head;
}

/*************************************************
Function:       // 显示玩家数据
Description:    // 显示前十姓名和时间
Calls:          //
Input:          // 头指针
Output:         // 姓名：时间
Return:         // void
Others:         // 头部无数据
*************************************************/
void LinkShow(pnode_t *p)
{
    int count = 0;
    LinkList tmp = p->next;
    while(tmp != NULL && count < RANKNUM)
    {
        cout << setw(6) << right << tmp->name << " : " << setw(6) << right << tmp->time<< endl;
        tmp = tmp->next;
        count++;
    }
    cout << endl;

    Free_List(p);          //打印结束释放链表

    return;
}

/*************************************************
Function:       // 玩家数据清空
Description:    // 三种难度的表都写为NULL 9999
Calls:          //
Input:          //
Output:         // 三张表
Return:         // 0
Others:         // 无
*************************************************/
int RankClear()
{
    FILE *fp1 = fopen(FILENAME_1, "w+");
    FILE *fp2 = fopen(FILENAME_2, "w+");
    FILE *fp3 = fopen(FILENAME_3, "w+");
    SYSERR(fp1,==,NULL,"FILE1 open error\n",-1);
    SYSERR(fp2,==,NULL,"FILE2 open error\n",-1);
    SYSERR(fp3,==,NULL,"FILE3 open error\n",-1);

    pnode_t *hero = NULL;
    strcpy(hero->name,"NULL");
    hero->time = 9999;

    for(int i = 0; i < RANKNUM; i++)
    {
        fscanf(fp1, "%s\t%d\n", hero->name, &hero->time);
        fscanf(fp2, "%s\t%d\n", hero->name, &hero->time);
        fscanf(fp3, "%s\t%d\n", hero->name, &hero->time);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

//单链表释放
void Free_List(LinkList head)
{
    LinkList p;
    while(head != NULL)
    {
        p = head;
        head = head->next;
        free(p);
    }
    return;
}
