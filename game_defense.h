#ifndef GAME_DEFENSE_H
#define GAME_DEFENSE_H
#include "game_core.h"

#define MAX_CANNON 100      //最大大炮数量
#define Upgrade_Cost 50 //升级费用
#define Refund_Rate 0.5 //移除返回比例
//红色(RGB(196, 43, 28)) = 敌方基地 | 蓝色(RGB(36, 98, 255)) = 我方基地
//棕色(RGB(185, 128, 71)) = 可部署区域 | 深红色(RGB(53, 17, 10)) = 不可通行区域
//绿色(RGB(34,139,34)) = 悬浮
//紫色（RGB(128,0,128)) = 敌人颜色
//橙色(RGB(255,165,0)) = 大炮颜色
#define DEPLOY_BROWN RGB(185,128,71)//可部署
#define HOVER_GREEN RGB(34,139,34)//悬浮
//按钮区域

typedef enum {
	tower1,
	tower2,
	tower3,
	invalidType
}DefenseType;//三种防御塔类型

typedef struct {
	DefenseType type;
	int attack;
	int range;
	int price;
}DefenseUnit;//每种防御塔的攻击力，射程范围，价格
DefenseUnit unit;

const DefenseUnit tower[] = {
	{tower1,10,2 * Gap,100},
	{tower2,20,4 * Gap,200},
	{tower3,30,6 * Gap,300}
};//具体数据

typedef struct {
	int x1, y1;  // 棕色方块左上角坐标
	int x2, y2;  // 棕色方块右下角坐标
	bool is_hover; // 是否被鼠标悬停
} BrownBlock;

// 大炮升级配置（等级越高射程增量越小，共3级）
typedef struct {
	int level;
	int range_add;	// 升级时的射程增量
} UpgradeConfig;

UpgradeConfig upgradeDate[] = {
	{1,30},	// 1级→2级 加30
	{2,15},	// 2级→3级 加15
	{3,0}		// 3级满级
};

// 大炮实例结构体（包含类型、等级、位置等）
typedef struct {
	DefenseType type;	// 所属类型
	int x, y;			// 坐标
	int level;			// 当前等级
	int current_range;	// 当前射程（基础+升级增量）
	bool is_selected;	// 是否被选中
	bool is_active;		// 是否存在
} Cannon;


BrownBlock blocks[1000]; // 存储所有棕色方块，足够大的数组
int block_count = 0;     // 实际方块数量
Cannon cannons[MAX_CANNON];	// 大炮数组
int cannon_count = 0;			// 当前大炮数量
int selected_idx = -1;			// 选中的大炮索引
int selected_tower_type = tower1; // 默认选中的部署类型（tower1）



void drawTip(const TCHAR* text);// 错误提示文字绘制
void PrintDefenseAreas();//打印地图下方的防御区域
DefenseUnit GetDefense(DefenseType type, int* totalmoney);//给出要的防御塔类型和拥有的总金币，返回需要的防御塔或者无效防御塔
void generate_blocks(int random_num);// 根据地图编号生成棕色方块坐标
void draw_blocks();//检测鼠标悬停并绘制方块
bool canDeployCannon(int mouse_x, int mouse_y);// 判断鼠标点击位置是否可部署大炮（返回true=可部署，false=不可部署）
void deployCannon(int mouse_x, int mouse_y, int* totalmoney);// 部署大炮：先判断（GetDefense）→ 再扣钱 → 最后生成
void selectCannon(int mouse_x, int mouse_y);// 选中大炮
void upgradeCannon(int* totalmoney);// 升级大炮（仅提升射程，等级越高增量越小）
void removeCannon(int* totalmoney);// 移除大炮（返还50%购买价）
void drawCannons();// 绘制大炮：橙色实心圆，无边框
void drawButtons(int* totalmoney);// 绘制功能按钮
void handleMouse(int* totalmoney);// 处理鼠标点击（部署、选中、升级、移除）
//sinan的函数

#endif