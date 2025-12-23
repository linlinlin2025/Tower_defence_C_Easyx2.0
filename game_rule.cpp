
#include "game_rule.h"
#include"game_core.h"
#include"game_enemy .h"
#include<easyx.h>  // 必须包含EasyX头文件
#include<conio.h>  // 包含_getch()的头文件
using namespace std;


//int Game_Coin = 100;     // 金币，初始为100
//int CurrentHP = 100;     // 血量，初始为100

//#define width 1024 //窗口宽度
//#define height 768//窗口高度

//图一，游戏主界面
void Interface1(int x, int y, int w, int h, const TCHAR* text)  // 改为const TCHAR*
{
	// 绘制圆角按钮底色
	setfillcolor(0xFFFACD);
	fillroundrect(x, y, x + w, y + h, 5, 5);

	// 绘制按钮文字并居中（所有字符串用_T()包裹）
	settextcolor(BLACK);
	settextstyle(40, 0, _T("微软雅黑"));  // 字体名加_T()
	int hSpace = (w - textwidth(text)) / 2;  // textwidth支持TCHAR
	int vSpace = (h - textheight(text)) / 2; // textheight支持TCHAR
	outtextxy(x + hSpace, y + vSpace, text);
}
// 绘制文字（适配宽字符）
void DrawText1()
{
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, _T("微软雅黑"));
	settextcolor(BLACK);
	
	Interface1(width /3, height/3, 280, 100, _T("开始游戏"));  // 字符串加_T()
	Interface1(width /3, height*19/30, 280, 100, _T("游戏规则"));

    outtextxy(width/3, 120, _T("历史记录:"));
}

//第二个图,游戏规则界面


void Interface2(int x, int y, int w, int h, const TCHAR* text)  // 改为const TCHAR*
{
	// 绘制圆角按钮底色

	setfillstyle(BS_SOLID); // 新增：实心填充
	setfillcolor(0xFFFACD);
	fillroundrect(x, y, x + w, y + h, 5, 5);
	// 绘制按钮文字并居中（所有字符串用_T()包裹）
	settextcolor(BLACK);
	settextstyle(40, 0, _T("微软雅黑"));  // 字体名加_T()
	int hSpace = (w - textwidth(text)) / 2;  // textwidth支持TCHAR
	int vSpace = (h - textheight(text)) / 2; // textheight支持TCHAR
	outtextxy(x + hSpace, y + vSpace, text);

}	
// 绘制文字（适配宽字符）
void DrawGameRules(int x, int y, int lineHeight)
{
	// 设置文本样式（游戏规则专用）
	setbkmode(TRANSPARENT);
	settextstyle(21, 0, _T("微软雅黑")); // 字体大小适中，便于阅读
	settextcolor(BLACK);

	// 按行拆分游戏规则（每行长度适配界面，避免超出显示范围）
	const TCHAR* rules[] = {
		_T("玩法说明: 一张空白的地图，让怪兽按照你摆的炮塔的阵型来回移动，并且可以通过搭建和贱卖出口的炮塔，"),
		_T("使怪兽来回移动，达到最长的移动距离，以便于炮塔不断的轰击怪兽。"),
		_T(""), // 空行分隔
		_T("高阶玩法提示：选择武器并部署到地图后，怪物将朝着基地进攻。提供大炮攻击怪物，另设有可充当"),
		_T("防御塔的路障。首要任务是策略性布置大炮，通过大炮和路障墙配合以限制怪物行进路线。"),
		_T("可随时升级现有武器，所有武器会自动攻击射程内的怪物。金币用于购买和升级武器，消灭怪物可增加金币。"),
		_T("生命值会因怪物抵达终点而减少，拆除任何大炮可返还50%金币。"),
		_T(""), // 空行分隔
		_T("温馨提示：抵制不良游戏，拒绝盗版游戏，注意自我保护，谨防受骗上当，适度游戏益脑，沉迷游戏伤身，"),
		_T("合理安排时间，享受健康生活。最佳视频游戏机")
	};

	// 逐行绘制文本
	int lineCount = sizeof(rules) / sizeof(rules[0]); // 计算总行数
	for (int i = 0; i < lineCount; i++)
	{
		outtextxy(x, y + i * lineHeight, rules[i]); // 按行高偏移绘制
	}
}
void DrawText2()
{
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("微软雅黑"));
	settextcolor(BLACK);
	Interface1(650, 500, 80, 60, _T("返回"));
	 DrawGameRules(50, 100, 25);
	// 字符串加_T()
}

void GameButton() {
	Interface1(width/3, height/3, 280, 100, _T("开始游戏")); 
	Interface1(width/3, height*19/30, 280, 100, _T("游戏规则"));
	Interface2(10, 10, 100, 50, _T("返回"));

}



//int AttackDamage = 5;     // 单次攻击扣5血
//int currentHealth = CurrentHP;
//int accumulatedDamage = 0;
//int enemyKillCount = 0;      // 敌人死亡数初始为0

// 积分累加，当前积分数，游戏等级判断

// ① 当前总积分初始值，游戏初始总分为0
int currentTotalScore = 0;
// ② 本次累加分数初始值（初始待累加得分为0，后续可通过敌人死亡数等更新）
int scoreAmount = 0;
// ③ 接收新总积分的变量初始值（占位初始化，会被函数覆盖）
int newTotalScore = 0;


// 外部变量声明（等级初始1，积分初始0）
int Current_Level = 1;//游戏等级，初始为1
int Game_Score = 0;// 游戏积分，初始值为0
// 新增：全局变量记录最高积分（初始0）
int Highest_Game_Score = 0;

// 1. 积分计算与累加核心函数（修正原函数语法错误，使用外部结构体）

void CalculateAndAddScoreByEnemy(const struct Dead_enemy_count* pDeadEnemyCount) {
     
    if (pDeadEnemyCount == NULL) {
        printf("错误：传入的敌人死亡计数结构体指针为空！\n");
        return;
    }

    // 按规则计算各类敌人死亡对应的积分（一级+10、二级+25、三级+40）
    int addScore = 0;
    addScore += pDeadEnemyCount->enemy_grade_1 * 10;   // 一级敌人积分
    addScore += pDeadEnemyCount->enemy_grade_2 * 25;   // 二级敌人积分
    addScore += pDeadEnemyCount->enemy_grade_3 * 40;   // 三级敌人积分

    // 累加至全局游戏积分Game_Score
    Game_Score += addScore;

    // 更新最高积分（仅当当前积分超过历史最高时更新）
    if (Game_Score > Highest_Game_Score) {
        Highest_Game_Score = Game_Score;
    }

    // 等级判定与更新（严格遵循你的规则）
    if (Game_Score <= 100) {
        Current_Level = 1;  // 积分≤100 → LV.1
    }
    else if (Game_Score > 100 && Game_Score <= 300) {
        Current_Level = 2;  // 100<积分≤300 → LV.2
    }
    else {
        Current_Level = 3;  // 积分>300 → LV.3
    }
}

// 2. 对外提供积分/等级数据的接口函数（方便其他模块调用）
void GetGameScoreData(int* pCurrentScore, int* pHighestScore, int* pCurrentLevel) {
    
    if (pCurrentScore != NULL) {
        *pCurrentScore = Game_Score;
    }
    if (pHighestScore != NULL) {
        *pHighestScore = Highest_Game_Score;
    }
    if (pCurrentLevel != NULL) {
        *pCurrentLevel = Current_Level;
    }

}

// 新增：读取并更新历史最高分文件的函数
void UpdateHighestScoreFile() {
    const char* filename = "历史最高分";  // 文件名
    int file_highest_score = 0;            // 存储文件中的历史最高分
    FILE* fp = NULL;
    errno_t err;  // 接收安全函数的错误码

    // 第一步：读取文件中的历史最高分
    err = fopen_s(&fp, filename, "r");  // 替换fopen为fopen_s
    if (err == 0 && fp != NULL) {       // 错误码为0表示打开成功
        fscanf_s(fp, "%d", &file_highest_score);  // 替换fscanf为fscanf_s
        fclose(fp);
        fp = NULL;
    }
    else {
        file_highest_score = 0;
    }

    // 第二步：比较并更新文件
    if (Highest_Game_Score > file_highest_score) {
        err = fopen_s(&fp, filename, "w");  // 写入模式用fopen_s
        if (err == 0 && fp != NULL) {
            fprintf(fp, "%d", Highest_Game_Score);
            fclose(fp);
            fp = NULL;
            file_highest_score = Highest_Game_Score;
        }
        else {
            printf("错误：无法打开文件 %s 进行写入！\n", filename);
        }
    }
}

// 主函数测试（可选，用于验证功能）
int main() {
    // 模拟敌人死亡数据
    struct Dead_enemy_count enemy_count = { 5, 4, 3 };

    // 计算积分
    CalculateAndAddScoreByEnemy(&enemy_count);

    // 获取并打印当前游戏数据
    int curr_score, highest_score, curr_level;
    GetGameScoreData(&curr_score, &highest_score, &curr_level);
    printf("\n当前游戏积分：%d\n当前最高积分：%d\n当前等级：%d\n",
        curr_score, highest_score, curr_level);

    // 读取并更新历史最高分文件
    UpdateHighestScoreFile();

    return 0;
}

// 测试示例（验证功能有效性）
//int main() {
//    // 模拟从游戏模块获取的敌人死亡数据（外部结构体实例）
//    struct Dead_enemy_count enemyDeadData1 = { 3, 1, 0 };  // 3个一级、1个二级、0个三级
//
//    // 第一次计算并累加积分
//    CalculateAndAddScoreByEnemy(&enemyDeadData1);
//    printf("第一次敌人死亡后：\n");
//
//    // 通过接口获取积分信息
//    int currScore, highScore, currLevel;
//    GetGameScoreData(&currScore, &highScore, &currLevel);
//    printf("当前积分：%d\n最高积分：%d\n当前等级：LV.%d\n\n",
//        currScore, highScore, currLevel);
//
//    // 模拟更多敌人死亡数据
//    struct Dead_enemy_count enemyDeadData2 = { 4, 5, 2 };  // 4个一级、5个二级、2个三级
//    CalculateAndAddScoreByEnemy(&enemyDeadData2);
//    printf("第二次敌人死亡后：\n");
//
//    // 再次获取并打印积分信息
//    GetGameScoreData(&currScore, &highScore, &currLevel);
//    printf("当前积分：%d\n最高积分：%d\n当前等级：LV.%d\n",
//        currScore, highScore, currLevel);
//
//    return 0;
//}






	
