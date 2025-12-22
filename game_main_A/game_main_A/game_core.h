#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <graphics.h>
#include <conio.h>
#include <cstdlib>  // 包含 rand() 和 srand()
#include <ctime>    // 包含 time()
using namespace std;

#define width 1024 //窗口宽度
#define height 768//窗口高度
#define Game_width 840//游戏窗口宽度
#define Game_height 600//游戏窗口高度
#define Gap 40//方格间距

extern int random_num;//随机地图选择变量

struct Enemy_BasePos {//敌方基地单个坐标结构体
	int x;
	int y;
};

/**
 * @biref :用于游戏界面初始化
 * @details :初始化EasyX窗口、加载资源、初始化游戏数据（得分、难度、最高记录）、生成初始预览防御单位
 * @param :无
 * @return :无
 */

void Game_Init(void);

/**
 * @biref :游戏主循环
 * @details :处理输入→调用各模块更新→调用绘制函数→控制帧率
 * @param :game_over_flag：int类型标志（0=游戏继续，1=游戏终止）
 * @param :restart_flag：int类型标志（0=不重开，1=玩家请求重开）
 * @param :quit_flag：int类型标志（0=不退出，1=玩家请求退出游戏返回主界面）
 * @return :无
 */

void Game_Loop(int& game_over_flag, int& restart_flag, int& quit_flag);

/**
 * @biref :结束游戏
 * @details :保存最高分(调用相应函数)、释放资源、关闭窗口
 * @param :无
 * @return :无
 */

void Game_Quit(void);

/**
 * @brief :三种游戏地图
 * @details :通过参数选择地图，红方块是敌方基地，蓝色是我方基地，棕色是防御单位可部署区域,深红色为不可通行区域
 * @param :随机值 random_num (1-3)
 * @retval :无
 */

void Game_Map(int random_num);

/**
 * @brief :侧边栏数据绘制函数
 * @details :包括积分、金币、生命值、等级等信息显示
 * @param :无
 * @retval :无
 * 
 */

void Game_SidebarData(void);

// 函数声明：生成1-3的随机地图编号
void GenerateRandomMap(void);

void DrawGameOver(void);

// 函数声明：游戏主循环逻辑（每帧执行一次）
// 参数说明：
// game_over_flag：int类型标志（0=游戏继续，1=游戏终止）
// restart_flag：int类型标志（0=不重开，1=玩家请求重开）
// quit_flag：int类型标志（0=不退出，1=玩家请求退出游戏返回主界面）

#endif
