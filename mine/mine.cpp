/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:与扫雷棋盘相关的函数实现
**************************************************/

#include "mine.h"
#include "menu.h"

//extern double time_player;

/*************************************************
Function:       // 初始化扫雷游戏配置
Description:    // 给游戏开辟两个空间1.棋盘空间 2.画布空间
Calls:          // board_init(mine_t **data, int num)
Input:          // 棋盘的结构体
Output:         // 生成游戏需要的棋盘和画布
Return:         // 成功返回0，失败返回-1
Others:         // 无
*************************************************/
int mine_init(mine_t *data)
{
    SYSERR(data,==,NULL,"data is null\n",-1);
    (data->board) = (char**)malloc(sizeof(char*)*(data->row + 2));       //创建棋盘行数（存地址）
    SYSERR((data->board),==,NULL,"board malloc error\n",-2);

    (data->canvas) = (char**)malloc(sizeof(char*)*(data->row + 2));      //创建画布行数（存地址）
    SYSERR((data->canvas),==,NULL,"canvas malloc erros\n",-3);

    /*********创建棋盘和画布*********/
    board_init(data->board, data->row+2);                               /******后台布雷位置******/
    board_init(data->canvas, data->row+2);                              /******前台显示位置******/

    return 0;
}

/*************************************************
Function:       // 棋盘生成(补充棋盘的列数大小)
Description:    // 给游戏生成需要的棋盘和画布,字符填充棋盘或者画布
Calls:          // malloc,
Input:          // 扫雷结构体（二级指针为扫雷内的棋盘或者画布）num 棋面大小（行列一致）
Output:         // 生成所需大小的棋盘
Return:         // 成功返回0，失败返回-1
Others:         // 无
*************************************************/
int board_init(char **data, int num)
{
    // i为第几行， flag开辟空间成功与否标记
    int i, flag = 0;
    for(i = 0; i < num; i++){
        if((*(data+i) = (char *)malloc(num)) == NULL)
        {
            flag = 1;  // 第i行开辟失败
            printf("malloc err\n");
            break;
        }
    }
    // 将之前开辟成功的空间依次释放
    i--;
    if(flag == 1)
    {
        for(; i >= 0; i--)
        {
            free(*(data+i));
            *(data+i) = NULL;
        }
        exit(0);
    }
    return 0;
}

/*************************************************
Function:       //  难度选择
Description:    // （棋盘大小，布雷数量）代表不同的游戏难度
Calls:          //  无
Input:          //  棋盘结构体变量
Output:         //  棋盘规模和雷的数量
Return:         //  无
Others:         //  输入0，退出模式选择
*************************************************/
void Difficult_mode(mine_t *data)
{
    char buf[4] = "";
    system("clear");
    cout << "***********     choose difficulty mode      ************" << endl;
    cout << "***********         1. normal mode          ************" << endl;
    cout << "***********         2. middle mode          ************" << endl;
    cout << "***********         3. senior mode          ************" << endl;
    cout << "***********         0.    quit              ************" << endl;
    while(true){
        cin.clear(); cin.sync();                            //清空输入缓冲
        cin >> buf;

        if(buf[0] == '0')                                   //退出模式选择
            break;

        switch (buf[0]) {
        case '1':
            data->row = PRI_ROW;
            data->mine_num = PRI_MINE;
            break;
        case '2':
            data->row = MID_ROW;
            data->mine_num = MID_MINE;
            break;
        case '3':
            data->row = SEN_ROW;
            data->mine_num = SEN_MINE;
            break;
        default:
            cout << "********     Input error, please input 1|2|3     ********" << endl;
            break;
        }
        cout << "********     Input enter to continue     ********" << endl;
        getchar();
        getchar();
        cout << "********     Input 0 to back     ********" << endl;
    }
}

/*************************************************
Function:       //  随机布雷
Description:    //  所有位置先置'0'
Calls:          //  srand() rand()
Input:          //  棋盘变量
Output:         //  给board成员变量赋值
Return:         //  无
Others:         //
*************************************************/
void Random_Mine(mine_t *data)
{    
    for(int i = 0; i < data->row+2; i++)                                //所有位置置'0'
        for(int j = 0; j < data->row+2; j++)       
            data->board[i][j] = '0';      

     srand((unsigned)time(NULL));                                       //初始化地雷位置
     int x, y;

     int count = data->mine_num;                                        //需要的布雷的数量
     while(count)
     {       
         x = rand() % data->row + 1;                                    //在[1,row]内布雷
         y = rand() % data->row + 1;

         if(data->board[x][y] == '0')                                   //布雷的有效性
         {
             data->board[x][y] = '1';
             count--;
         }
     }
}

/*************************************************
Function:       //  排雷
Description:    //  输入 a排查 b插旗 c拔旗
Calls:          //  DisplayBoard(data)、GetMineNum、OpenMine、CountMine、DisplayCanvas
Input:          //  棋盘数据
Output:         //  画布数据,棋盘数据
Return:         //  无
Others:         //  每个过程都有可能使游戏结束
*************************************************/
void Exclude_Mine(mine_t *data)
{

    time_t start, finish;                                          // 游戏计时
    start = time(NULL);                                                // 游戏开始

    int x = 0, y = 0, win = 0, flag_cycle = 1;                      // x,y 坐标；win 成功为1；flag_cycle 外部大循环

    char input[4] = "";

    int  p_trueminecount = 0;                                       //旗子对于雷的真实数
    int  flagcount = 0;                                             //统计已有的旗子数

    do{
        cout << "****************      Please Choose a|b|c       *******************" << endl;
        cout << endl;
        cout << "****************  a.Exclude b.Flag c.Pull_Flag  *******************" << endl;
        cout << endl;
        //cin.clear();cin.sync();
        setbuf(stdin, NULL);
        cin >> input;
        switch (input[0]) {
            case 'a':
                {
                    int flag_a = 1;                                  //连续排查标志
                    cout << endl;
                    cout << "************     Enter the Coordinates to be Checked    ************" << endl;      //输入要检查的坐标
                    cout << endl;
                    while(flag_a)
                    {                        
                        //cin.clear();cin.sync();
                        setbuf(stdin, NULL);
                        cin >> x >> y;

                        if(x>0 && x<=data->row && y>0 && y <= data->row)     //合法的坐标范围内
                        {
                            if(data->canvas[x][y] != '*')                    //判断是否已经排查过
                            {
                                if(data->canvas[x][y] == '#')
                                    cout << "there have flag, please pull flag check again" << endl;
                                else
                                    cout << "there have checken, please input again" << endl;
                            }
                            else if(data->board[x][y] == '1')                //判断是否踩到了雷
                            {
                                cout << "I am sorry, you step on the mine, GAME OVER" << endl;
                                DisplayBoard(data);
                                flag_a = 0;
                                break;
                            }
                            else
                            {
                                int count = GetMineNum(data, x, y);          //判断坐标周围的雷的个数
                                if(0 == count)
                                {
                                    OpenMine(data, x, y);
                                    flag_a = 0;                              //跳出a循环
                                    int ret_win = CountMine(data);           //剩余可用格子数
                                    if(ret_win == 0)
                                    {
                                        flag_cycle = 0;
                                        win = 1;
                                        break;
                                    }
                                }
                                else
                                {
                                     data->canvas[x][y] = count + '0';
                                     int ret_win = CountMine(data);
                                     flag_a = 0;
                                     if(ret_win == 0)
                                     {
                                         flag_cycle = 0;
                                         win = 1;
                                         break;
                                     }
                                }
                                DisplayCanvas(data);
                                DisplayBoard(data);   // 测试代码，实际不显示
                            }
                        }
                        else
                        {
                            cout << "Input Coordinate Error, Input Again" << endl;
                        }
                    }
                    break;
                }
            case 'b':                                                         //Flag
            {
                int flag_b = 1;
                while(flag_b)
                {
                    cout << endl;
                    cout << "************     Input Flag Coordination     ************" << endl;
                    cout << endl;

                    cin.clear();cin.sync();
                    cin >> x >> y;
                    if(x > 0 && x <= data->row && y > 0 && y <= data->row)
                    {
                        if(data->canvas[x][y] == '*')
                        {
                            data->canvas[x][y] = '#';
                            flagcount++;
                            flag_b = 0;

                            p_trueminecount = TrueMineNum(data, &flagcount);
                            //如果旗子数与对应的雷数相等，且旗子下都是真雷，游戏结束，win置1；
                            if(flagcount == data->mine_num && p_trueminecount == data->mine_num)
                            {
                                flag_b = 0;
                                win = 1;
                            }
                        }
                        else
                        {
                            cout << "Here can't place flag. please check coordinate" << endl;
                        }
                    }
                    else
                        cout << "************    Input Coordinate Error    ************" << endl;
                }
                DisplayCanvas(data);
                DisplayBoard(data);   // 测试代码，实际不显示
                break;
            }
            case 'c':                         //拔旗，存在胜负判断
            {
                int flag_c = 1;
                int ret = FlagCount(data);    //目前已有的旗子数
                while(flag_c)
                {
                    if(ret == 0)
                    {
                        cout << "the board without flag, please choose a or b" << endl;   //无旗可拔
                        flag_c = 0;                                                       //跳出循环
                    }
                    else
                    {
                        cout << "************    Input Coordinate to Pull Flag     ************" << endl;
                        cin >> x >> y;
                        if(x > 0 && x <= data->row && y > 0 && y <= data->row)
                        {
                            if(data->canvas[x][y] == '#')
                            {
                                data->canvas[x][y] = '*';
                                flagcount--;
                                flag_c = 0;
                                p_trueminecount = TrueMineNum(data, &flagcount);
                                if(flagcount == data->mine_num && p_trueminecount == data->mine_num)
                                {
                                    flag_c = 0;
                                    win = 1;
                                }
                            }
                            else
                            {
                                cout << "************    Here don't Pull Flag    ************" << endl;
                            }
                        }
                        else
                        {
                            cout << "************    Input Coordinate Error    ************" << endl;
                        }
                    }
                }
                DisplayCanvas(data);
                DisplayBoard(data);   // 测试代码，实际不显示
                break;
            }
        }

        if(win == 1)                                        //当win等于1时，表示排除了全部的雷
        {
            cout << endl;
            cout << "************   Congratulation!!! All Mine were Found   ************" << endl;
            cout << endl;
            flag_cycle = 0;
            DisplayCanvas(data);

            finish = time(NULL);                               //通关时间统计
            int time_player = difftime(finish, start);

            cout << "player win time is " << time_player << endl;    //测试代码，实际不显示

            cout << endl;
            cout << "********************    Enter Your Name     **********************" << endl;
            cout << endl;
            char playername[16] = "";                       //玩家名字
            cin >> playername;


            SaveHeroRank(data, time_player, playername);    //根据模式选择存储的文件(未排序文件)

            cout << endl;
            cout << "********************    Enter to Continue    *********************" << endl;
            cout << endl;
            getchar();
            getchar();
        }
    }while(flag_cycle);
}

/*************************************************
Function:       //  显示画布(前台画面)
Description:    //  玩家每操作一次显示一次画布
Calls:          //  无
Input:          //  data->canvas[][]
Output:         //  画布数据
Return:         //  无
Others:         //  实时显示
*************************************************/
void DisplayCanvas(mine_t *data)
{
    int i, j;
    cout << "      ";
    for(i = 1; i <= data->row; i++)
        cout << setw(5) << right << i;
    cout << endl << endl << endl;
    for(i = 1; i < data->row+1; i++){
        cout << i;
        for(j = 0; j < data->row+2; j++)
            cout << setw(5) << right << data->canvas[i][j];
        cout << endl;
    cout << endl;
    }
}

/*************************************************
Function:       //  显示棋盘(雷的位置状态)
Description:    //  踩到雷后显示棋盘
Calls:          //  无
Input:          //  data->board[][]
Output:         //  棋盘数据
Return:         //  无
Others:         //  游戏结束时显示雷的位置
*************************************************/
void DisplayBoard(mine_t *data)
{
    int i, j;
    cout << "      ";
    for(i = 1; i <= data->row; i++)
        cout << setw(5) << right << i;
    cout << endl << endl << endl;
    for(i = 1; i < data->row+1; i++){
        cout << i;cout << "     ";
        for(j = 1; j < data->row+1; j++)
            cout << setw(5) << right << data->board[i][j];
        cout << endl;
    cout << endl;
    }
}

/*************************************************
Function:       //  安全设置
Description:    //  保证第一步不会踩到雷,如果有雷，修改当前data->board
Calls:          //  无
Input:          //  data->board[][]
Output:         //  棋盘数据
Return:         //  无
Others:         //
*************************************************/
void FirstStep_SafeMine(mine_t *data)
{

    int x = 0, y = 0, ret = 1, count = 0, a, b;                 // x,y 坐标值；a,b 随机值

    cout << endl;
    cout << "************     Input First Coordination    ************" << endl;
    cout << endl;
    cin.clear();cin.sync();
    cin >> x >> y;

    if(data->board[x][y] == '1')                                //如果有雷，改为没有雷
    {
        data->board[x][y] = '0';
        while(ret)                                              //额外增加一个雷
        {
            srand((unsigned)time(NULL));
            a = rand() % data->row + 1;
            b = rand() % data->row + 1;
            if(data->board[x][y] == '0'){
                data->board[x][y] = '1';
                ret--;
            }
        }
    }

    count = GetMineNum(data, x, y);                             //统计第一个坐标周围的类的个数

    data->canvas[x][y] = count + '0';                           //修改画布上的数据

    OpenMine(data, x, y);                                       //周围没有雷就自动展开

    DisplayCanvas(data);
}

/*************************************************
Function:       //  统计周围雷的个数
Description:    //  将周围的八个位置的雷相加
Calls:          //  data->board[][]
Input:          //  棋盘结构体
Output:         //  棋盘数据
Return:         //  周围雷数
Others:         //  边框全是'0' 字符加减
*************************************************/
int GetMineNum(mine_t *data, int x, int y)
{
    return data->board[x-1][y-1] + data->board[x][y-1] + data->board[x+1][y-1] + data->board[x-1][y] + data->board[x+1][y] +\
            data->board[x-1][y+1] + data->board[x][y+1] + data->board[x+1][y+1] - 8*'0';
}

/*************************************************
Function:       //  棋盘自动展开
Description:    //  坐标周围没有雷就自动展开(递归方式),将*用空格代替
Calls:          //  OpenMine()
Input:          //  棋盘数据，坐标位置
Output:         //  画布数据
Return:         //  无
Others:         //  只要周围没有雷就给它置空，然后扩散给8个方向
*************************************************/
void OpenMine(mine_t *data, int x, int y)
{
    int tmp = 0;

    tmp = GetMineNum(data, x, y);                                               //获得该坐标附近的雷的数目
    if(tmp == 0)
    {
        data->canvas[x][y] = ' ';
        if(x > 0 && y+1 <= data->row && data->canvas[x][y+1] == '*')
            OpenMine(data, x, y+1);

        if(x-1 > 0 && y+1 <= data->row && data->canvas[x-1][y+1] == '*')
            OpenMine(data, x-1, y+1);

        if(x-1 > 0 && y > 0 && data->canvas[x-1][y] == '*')
            OpenMine(data, x-1, y);

        if(x-1 > 0 && y-1 > 0 && data->canvas[x-1][y-1] == '*')
            OpenMine(data, x-1, y-1);

        if(x > 0 && y-1 > 0 && data->canvas[x][y-1] == '*')
            OpenMine(data, x, y-1);

        if(x+1 <= data->row && y-1 > 0 && data->canvas[x+1][y-1] == '*')
            OpenMine(data, x+1, y-1);

        if(x+1 <= data->row && y > 0 && data->canvas[x+1][y] == '*')
            OpenMine(data, x+1, y);

        if(x+1 <= data->row && y+1 <= data->row && data->canvas[x+1][y+1] == '*')
            OpenMine(data, x+1, y+1);
    }
    else
        data->canvas[x][y] = '0'+ tmp;   //显示相应的字符数字
}

/*************************************************
Function:       //  画布初始化
Description:    //  将画布用*填满,边框用空格填充
Calls:          //  data->canvas[][]
Input:          //  棋盘数据
Output:         //  画布数据
Return:         //  无
Others:         //
*************************************************/
void Init_Canvas(mine_t *data)
{

    for(int i = 1; i < data->row + 1; i++)                          //中间用*填充
        for(int j = 1; j < data->row + 1; j++)
            data->canvas[i][j] = '*';
    for(int i = 0; i < data->row + 2; i++)                          //四个边框为空格
        for(int j = 0; j < data->row + 2; j++){
            data->canvas[0][j] = ' ';
            data->canvas[data->row+1][j] = ' ';
            data->canvas[i][0] = ' ';
            data->canvas[i][data->row+1] = ' ';
        }
}

/*************************************************
Function:       //  统计真实的雷
Description:    //  统计棋盘下旗子对于的真实的雷的数目
Calls:          //  *pcount 旗子数量，data->board[][] data->canvas[][]
Input:          //  棋盘数据
Output:         //  画布数据
Return:         //  旗子下面真实的雷数
Others:         //  后台计算，玩家不许访问
*************************************************/
int TrueMineNum(mine_t *data, int *pcount)
{
    int count = 0;
    if(*pcount > 0)
    {
        for(int i = 1; i <= data->row; i++)
            for(int j = 1; j <= data->row; j++)
                if(data->canvas[i][j] == '#')                            //有旗子
                    if(data->board[i][j] == '1')                         //有雷
                        count++;
    }
    return count;
}

/*************************************************
Function:       //  统计画布上剩余的区域（除去雷）
Description:    //  排除已除去的区域和雷的区域，剩下的区域大小
Calls:          //  data->canvas[][]
Input:          //  棋盘数据
Output:         //  无
Return:         //  剩余的安全数量
Others:         //
*************************************************/
int CountMine(mine_t *data)
{
    int check;
    int win = data->row*data->row - data->mine_num;                     //win = 所有位置数量 - 雷的数量
    for(int i = 1; i <= data->row; i++)
        for(int j = 1; j <= data->row; j++)
        {
            check = data->canvas[i][j] - '0';
            if(data->canvas[i][j] == ' ' || (check > 0 && check < 9))   //位置为空，或者为数字1-8，表示已经安全
                win--;
        }
    return win;
}

/*************************************************
Function:       //  统计画布上已有的旗子个数
Description:    //  计算旗子个数#
Calls:          //  棋盘data->canvas[][]
Input:          //  棋盘数据
Output:         //  旗子数
Return:         //  FlagCount
Others:         //  画布大小为(row+2)(row+2) 统计的[1,row]内的#
*************************************************/
int FlagCount(mine_t *data)
{
    int count = 0;
    for(int i = 1; i <= data->row; i++)
        for(int j = 1; j <= data->row; j++)
            if(data->canvas[i][j] == '#')
                count++;
    return count;
}
