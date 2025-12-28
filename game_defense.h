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
//橙色(RGB(255,165,0)) = 大炮类型1颜色
//青色(RGB(90,237,206))=大炮类型2颜色
//粉色(RGB(234,54,128))=大炮类型3颜色
//薄荷绿(RGB(60,179,113))=升级按钮颜色
//猩红(RGB(220,20,60))=移除按钮颜色
#define DEPLOY_BROWN RGB(185,128,71)//可部署
#define HOVER_GREEN RGB(34,139,34)//悬浮
#define TOWER1_ATTACK_RADIUS 2//大炮1攻击范围（以格子为单位）
#define TOWER2_ATTACK_RADIUS 1//大炮2攻击范围（以格子为单位）
#define TOWER3_ATTACK_RADIUS 0//大炮3攻击范围（以格子为单位）
//按钮区域

typedef enum {
	tower1,
	tower2,
	tower3,
	invalidType
}DefenseType;//三种防御塔类型

//大炮坐标结构体（表示单个点的x,y坐标）
typedef struct {
	int cannon_x;
	int cannon_y;
}Point;

typedef struct{
	Point top_left;//方格左上角坐标
	Point bottom_right;//方格右下角坐标
}GridRange;//大炮射程范围

typedef struct {
	DefenseType type;
	int attack;
	GridRange attack_range;
	int price;
	int attack_radius;
}DefenseUnit;//每种防御塔的攻击力，射程范围，价格

typedef struct {
	int x1, y1;  // 棕色方块左上角坐标
	int x2, y2;  // 棕色方块右下角坐标
	bool is_hover; // 是否被鼠标悬停
} BrownBlock;

// 大炮升级配置（等级越高攻击力增量越小，共3级）
typedef struct {
	int level;
	int attack_add;	// 升级时的攻击力增量
} UpgradeConfig;

// 一个大炮实例结构体（包含类型、等级、位置等）
typedef struct {
	DefenseType type;	// 所属类型
	int x, y;			// 坐标
	int level;			// 当前等级
	int current_attack;	// 当前攻击力（基础+升级增量）
	GridRange attack_range; // 射程范围
	bool is_selected;	// 是否被选中
} Cannon;



void drawTip(const TCHAR* text);// 错误提示文字绘制
DefenseType CheckClickedTower(int mouse_x, int mouse_y);//检测鼠标点击了哪种防御塔
int canDeployCannon(int mouse_x, int mouse_y);// 判断鼠标点击位置是否可部署大炮（返回true=可部署，false=不可部署）
void upgradeCannon(int* totalmoney);// 升级大炮（仅提升攻击力，等级越高增量越小）
void removeCannon(int* totalmoney);// 移除大炮（返还50%购买价）
void selectCannon(int mouse_x, int mouse_y);// 选中大炮
void deployCannon(int mouse_x, int mouse_y, int* totalmoney);// 部署大炮：先判断（GetDefense）→ 再扣钱 → 最后生成
GridRange calcTowerGridRange(int grid_x, int grid_y,int radius);// 计算单个方格的射程范围（基于塔的摆放格子坐标和Gap）
void PrintDefenseAreas();//打印地图下方的防御区域
DefenseUnit GetDefense(int* totalmoney);//给出点击的防御塔类型和拥有的总金币，返回需要的防御塔或者无效防御塔
void generate_blocks(int random_num);// 根据地图编号得到棕色方块坐标
void draw_blocks(int mouse_x,int mouse_y);//检测鼠标悬停并绘制方块
void drawCannons(Cannon cannon);// 绘制大炮
void drawAllCannons();//绘制所有大炮
//sinan的函数（思南给我所有敌人的坐标，我去判断哪些在射程内，并返回在射程内的敌人结构体给思南）
//画子弹线条的函数（根据在射程内的敌人坐标和大炮坐标画线条）

#endif