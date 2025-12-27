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
#include "game_core.h"
#include "game_rule.h"
using namespace std;


int random_num;
ExMessage msg;
int is_Start = 0; // 是否点击开始游戏按钮的标志

//
// 定义游戏状态枚举
enum GameState {
    STATE_MAIN_MENU,   // 主界面（未点击开始游戏）
    STATE_GAME_PLAYING,// 游戏运行中
    STATE_GAME_OVER    // 游戏结束（显示再来一局）

};
GameState game_state = STATE_MAIN_MENU; // 初始状态为主界面


// 重置游戏（新增：再来一局的核心逻辑）
void ResetGame() {
    cleardevice();          // 清空界面
    GenerateRandomMap();    // 重新生成随机地图
    Game_Init();            // 重新初始化游戏
    game_state = STATE_GAME_PLAYING; // 切换到游戏运行状态
    is_Start = 1;           // 标记为“已开始游戏”（兼容你的原有逻辑）
}

// 游戏结束时调用（需你根据实际游戏结束条件触发，新增）
// 你需要在游戏逻辑中，当满足结束条件时调用此函数（比如玩家死亡、通关）
void TriggerGameOver() {
    cleardevice();
    DrawGameOver(); // 绘制游戏结束界面（需你实现：包含“再来一局”“退出”按钮）
    game_state = STATE_GAME_OVER; // 切换到游戏结束状态
}


int main() {
    // 初始化随机种子
    srand(time(NULL));
    GenerateRandomMap(); // 初始化第一个随机地图

    // 初始化图形窗口
    initgraph(width, height, EX_DBLCLKS);
    setbkcolor(RGB(237, 231, 216));
    cleardevice();
    DrawText1();

    while (true) { // 进入游戏死循环，通过状态控制退出
        switch (game_state) {
            
        case STATE_MAIN_MENU:// 状态1：主界面（未点击开始游戏）
            if (peekmessage(&msg, EX_MOUSE)) {
                // 点击“开始游戏”按钮
                if ((msg.message == WM_LBUTTONDOWN) && msg.x >= width / 3 && msg.x <= width / 3+280 && msg.y >= height / 3 && msg.y <= height / 3+100) {
                    ResetGame(); // 调用重置游戏,切换游戏状态
                }
                // 点击“游戏规则”按钮
                else if ((msg.message == WM_LBUTTONDOWN) && msg.x >= width / 3 && msg.x <= width / 3+280 && msg.y >= height * 19 / 30 && msg.y <= height * 19 / 30+100) {
                    cleardevice();
                    DrawText2();
                    int is_Return = 0;
					while (!is_Return) { //是否退出游戏规则界面
                        ExMessage rule_msg;
                        if (peekmessage(&rule_msg, EX_MOUSE)) {
                            if ((rule_msg.message == WM_LBUTTONDOWN) && rule_msg.x >= 650 && rule_msg.x <= 730 && rule_msg.y >= 500 && rule_msg.y <= 560) {
                                cleardevice();
                                is_Return = 1;
								DrawText1();//返回第一幕
                            }
                        }
                    }
                }
            }
            break;

           
        case STATE_GAME_PLAYING: // 状态2：游戏运行中
		{//加入大括号，限定变量作用域，防止初始化报错
            int game_over_flag = 0; int restart_flag = 0; int quit_flag = 0;
            // 游戏帧循环：持续执行直到 game_over_flag 变为 1
            while (game_over_flag == 0) {
                Game_Loop(game_over_flag, restart_flag, quit_flag);

                // 检测窗口关闭事件
                ExMessage temp_msg;
                if (peekmessage(&temp_msg)) {
                    // 判断消息是否是“窗口关闭”
                    if (temp_msg.message == WM_CLOSE) {
                        goto GAME_EXIT;
                    }
                }
            }if (restart_flag == 1) { // 若 Game_Loop 中标记了“重开”
                ResetGame(); // 执行重开逻辑
            }
            else if (quit_flag == 1) { // 若 Game_Loop 中标记了“退出”
                cleardevice();
                DrawText1();
                game_state = STATE_MAIN_MENU;
            }
            else { // 游戏正常结束（非重开/退出）
                TriggerGameOver();
            }
            break;
        }
            // 状态3：游戏结束
        case STATE_GAME_OVER:
            if (peekmessage(&msg, EX_MOUSE)) {
                // 点击“再来一局”按钮（坐标对应DrawGameOver中的按钮）
                if ((msg.message == WM_LBUTTONDOWN) && msg.x >= 280 && msg.x <= 560 && msg.y >= 300 && msg.y <= 400) {
					ResetGame(); // 重置游戏，重新生成随机地图，进入游戏运行状态
                }
                // 点击“退出游戏”按钮
                else if ((msg.message == WM_LBUTTONDOWN) && msg.x >= 280 && msg.x <= 560 && msg.y >= 420 && msg.y <= 520) {
                    goto GAME_EXIT; // 跳转到退出逻辑
                }
            }
            break;
        }

    }

GAME_EXIT: // 退出标记
    cleardevice();
    Game_Quit(); // 保留你的原有退出逻辑
    closegraph(); // 补充关闭图形窗口，避免资源泄漏
    return 0;
}