#ifndef GAME_RULE_H
#define GAME_RULE_H
#include <tchar.h>
#include"game_core.h"

// 声明游戏相关外部声明变量
extern int Game_Score;      // 积分,初始为0
extern int Game_Coin;     // 金币，初始为100
extern int CurrentHP;     // 血量，初始为100
extern int Current_Level;   // 等级，初始为1
extern int Highest_Game_Score;// 新增：最高积分，初始为0


//游戏按钮位置（游戏开始，游戏规则，返回）
void GameButton();//绘制游戏按钮“开始游戏”“游戏规则”“返回”的位置

/**
 * @brief : 绘制游戏主界面的按钮（第一个图相关）
 * @details : 在指定的坐标区域内绘制按钮控件，同时将传入的“游戏规则”和“开始游戏”在按钮区域内居中显示，适配游戏主界面的UI布局设计
 * @param x : 输入参数，按钮区域的左上角水平x坐标（绘制起始横坐标）
 * @param y : 输入参数，按钮区域的左上角垂直y坐标（绘制起始纵坐标）
 * @param w : 输入参数，按钮区域的宽度
 * @param h : 输入参数，按钮区域的高度
 * @param text : 输入参数，指向宽字符字符串的常量指针，需显示在按钮上的文本内容
 * @return : 无
 */
 // 图一，游戏主界面绘制
void Interface1(int x, int y, int w, int h, const TCHAR* text);//绘制按钮，文字居中

/**
 * @brief : 绘制游戏主界面的"游戏规则"和"开始游戏"文字（适配宽字符）
 * @details : 专门用于游戏主界面的文字绘制
 * @return : 无
 */
void DrawText1();// 图一绘制文字

/**
 * @brief : 绘制游戏规则界面的按钮（第二个图相关）
 * @details : 在指定区域内绘制按钮，同时将传入的“返回”在按钮区域内居中显示
 * @param x : 输入参数，按钮区域的左上角x坐标（水平起始位置）
 * @param y : 输入参数，按钮区域的左上角y坐标（垂直起始位置）
 * @param w : 输入参数，按钮区域的宽度
 * @param h : 输入参数，按钮区域的高度
 * @param text : 输入参数，指向宽字符字符串的常量指针，需显示在按钮上的文本（文字居中显示）
 * @return : 无
 */
 // 第二个图,游戏规则界面绘制
void Interface2(int x, int y, int w, int h, const TCHAR* text);//绘制按钮，文字居中

/**
 * @brief : 绘制游戏规则界面的"返回"文字（适配宽字符）
 * @details : 专门用于游戏规则界面的文字绘制，内置宽字符适配逻辑，无需额外转换字符编码，保证文字正常显示
 * @param : 无
 * @return : 无
 */
void DrawText2();// 图二绘制文字


//游戏规则
/**
 * @brief : 处理基地扣血逻辑，并累计基地受到的总伤害
 * @details : 接收基地当前实时血量和单次受到的伤害值，计算扣血后的基地新血量，
 *            同时将本次伤害值累加到总伤害量中，最终通过指针参数输出新基地血量和累计总伤害
 * @param CurrentHP : 输入参数，当前基地血量（需传入实时血量，用于计算扣血后的值）
 * @param damageAmount : 输入参数，单次扣血量（敌人每攻击一次扣10，支持正数表示扣血）
 * @param newBaseHealth : 输出参数，指向int类型变量的指针，用于存储扣血后的基地新血量（为0时基地被摧毁）
 * @param accumulatedDamage : 输出参数，指向int类型变量的指针，用于存储基地受到的累计总伤害（本次伤害会累加至该值）
 * @return : 无
 */
 //基地扣血及累计伤害
int BaseTakeDamageAndAccumulate(int CurrentHP, int damageAmount, int* newBaseHealth, int* accumulatedDamage);


/**
 * @brief : 判断游戏结束条件，标记游戏状态并生成结果描述
 * @details : 接收当前基地实时血量，通过校验血量是否满足结束条件（如血量为0），
 *            标记游戏是否结束，并根据结束原因生成标准化的游戏结果文本，
 *            最终通过指针参数输出游戏结束状态和结果信息
 * @param CurrentHP : 输入参数，当前基地血量（需传入实时血量，用于判断是否为0以触发结束条件）
 * @param isGameOver : 输出参数，指向bool类型变量的指针，用于标记游戏是否结束（true=游戏结束，false=游戏继续）
 * @param gameResult : 输出参数，指向字符缓冲区的指针，用于存储游戏结果文本（如“游戏失败”），
 *                     调用者需预先分配足够大的可写字符缓冲区
 * @return : 无
 */
 //判断结束条件
int CheckGameRuleAndEndCondition(int CurrentHP, bool* isGameOver, char* gameResult);


/**
* @brief : 获取当前游戏积分数据，包括总得分、最高得分和当前等级
* @details : 通过指针参数输出当前游戏总得分、最高得分和当前等级
* return：无
*/
void CalculateAndAddScoreByEnemy(const struct Dead_enemy_count* pDeadEnemyCount);//运用到C同学的结构体

/**
* @brief : 获取当前游戏积分数据，包括总得分、最高得分和当前等级，方便外部模块调用
* 
**/
void GetGameScoreData(int* pCurrentScore, int* pHighestScore, int* pCurrentLevel);

/**
* @brief : 计算并累加游戏积分
* @details : 接收当前游戏总得分和单次积分累加值，通过计算得到累加后的新总得分，
*            最终通过指针参数输出累加后的新游戏总得分
* @param currentTotalScore : 输入参数，（当前游戏总得分），需传入实时得分，用于计算累加后的值
* @param scoreAmount : 输入参数，（积分累加一次的值），每消灭一个敌人加10分，支持正负值，正数加分/负数扣分。
* @param newTotalScore : 输出参数，指向int类型变量的指针，用于存储累加后的（新游戏总得分）
* @return : 无
*/
//计算累加游戏积分
//int CalculateAndAddScore(int currentTotalScore, int scoreAmount, int* newTotalScore);可能用不到，先注释


#endif
