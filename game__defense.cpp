#include <graphics.h> 
#include "game_defense.h"
#include "game_core.h"
#include <stdio.h>
#include <conio.h>

// 三种防御塔的具体数据
const DefenseUnit tower[] = {
    {tower1,10,{},100,TOWER1_ATTACK_RADIUS},
    {tower2,20,{},200,TOWER2_ATTACK_RADIUS},
    {tower3,30,{},300,TOWER3_ATTACK_RADIUS}
};

// 大炮升级配置（等级越高攻击力增量越小，共3级）
UpgradeConfig upgradeDate[] = {
    {1,30},	// 1级→2级 加30
    {2,15},	// 2级→3级 加15
    {3,0}		// 3级满级
};

BrownBlock blocks[1000]; // 存储所有棕色方块，足够大的数组
int block_count = 0;     // 实际方块数量
Cannon cannons[MAX_CANNON];	// 大炮数组
int cannon_count = 0;			// 当前大炮数量
int selected_idx = -1;			// 选中的大炮索引
int selected_tower_type = 0;		// 选中的防御塔类型(0=未选择大炮，1=tower1，2=tower2，3=tower3)

// 错误提示文字绘制
void drawTip(const TCHAR* text) {
    settextcolor(RED);
    settextstyle(20, 0, _T("黑体"));
    outtextxy(Game_width / 2 - textwidth(text) / 2, Game_height / 2, text);
}

// 计算单个方格的射程范围（基于塔的摆放格子坐标和Gap）
GridRange calcTowerGridRange(int grid_x, int grid_y,int radius) {
    GridRange attack_range;
    // 左上坐标
    attack_range.top_left.cannon_x= (grid_x-radius) * Gap;
    attack_range.top_left.cannon_y =(grid_y-radius) * Gap;
    // 右下坐标
    attack_range.bottom_right.cannon_x = (grid_x + radius+1) * Gap;
    attack_range.bottom_right.cannon_y = (grid_y + +radius+1) * Gap;
    return attack_range;
}

//检测鼠标点击了哪种防御塔
DefenseType CheckClickedTower(int mouse_x, int mouse_y)
{
    int tower1_x = Game_width / 8;
    int tower_y = (2 * Game_height) / 3 + Game_height / 3 ;
    if((mouse_x-tower1_x)*(mouse_x-tower1_x)+(mouse_y-tower_y)*(mouse_y-tower_y)<= (Gap/2)*(Gap/2))
    {
        return tower1;
	}
    int tower2_x = 3 * Game_width / 8;
    if((mouse_x-tower2_x)*(mouse_x-tower2_x)+(mouse_y-tower_y)*(mouse_y-tower_y)<= (Gap/2)*(Gap/2))
    {
        return tower2;
    }
	int tower3_x = 5 * Game_width / 8;
    if((mouse_x-tower3_x)*(mouse_x-tower3_x)+(mouse_y-tower_y)*(mouse_y-tower_y)<= (Gap/2)*(Gap/2))
    {
        return tower3;
	}
    return invalidType; // 未点击任何防御塔按钮
}


//给出点击的防御塔类型和拥有的总金币，返回需要的防御塔或者无效防御塔
DefenseUnit GetDefense(int* totalmoney)
{
    DefenseUnit invalidUnit;
    invalidUnit.type = invalidType;
    invalidUnit.attack = 0;
    invalidUnit.attack_range = {};
    invalidUnit.price = 0;
	if (totalmoney == NULL) return invalidUnit;
	//MOUSEMSG m = GetMouseMsg();
	//if (m.uMsg != WM_LBUTTONDOWN) return invalidUnit;//仅处理鼠标左键点击事件
	//DefenseType type = CheckClickedTower(m.x, m.y);
	if (selected_tower_type == 0)return invalidUnit;//未选择防御塔类型
    if (*totalmoney < tower[selected_tower_type].price) return invalidUnit;
	return tower[selected_tower_type];
}


// 根据地图编号得到棕色方块坐标
void generate_blocks(int random_num) {
    block_count = 0; // 重置方块数量
    Point* pos = NULL;
    int pos_count = 0;
    if (random_num == 1) {
        static Point Friendly_BasePosMap1[] = {
            {5,3},{5,4} ,{5,5},{5,6},{5,7},{5,8},{5,9},{5,10},{5,11},
            {6,3},{6,4} ,{6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},
            {7,3},{7,4} ,{7,5},{7,6},{7,7},{7,8},{7,9},{7,10},{7,11},
            {13,3},{13,4} ,{13,5},{13,6},{13,7},{13,8},{13,9},{13,10},{13,11},
            {14,3},{14,4} ,{14,5},{14,6},{14,7},{14,8},{14,9},{14,10},{14,11},
            {15,3},{15,4} ,{15,5},{15,6},{15,7},{15,8},{15,9},{15,10},{15,11} };
        pos = Friendly_BasePosMap1;
        pos_count = sizeof(Friendly_BasePosMap1) / sizeof(Point);
    }
    if (random_num == 2) {
        Point Friendly_BasePosMap2[] = {
            {0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13},{0,14},
            {1,4},{1,5},{1,6},{1,7},{1,8},{1,9},{1,10},{1,11},{1,12},{1,13},{1,14},
            {2,7},{2,8},{2,9},{2,10},{2,11},{2,12},{2,13},{2,14},//以上是左侧三列防御区域，从上至下
            {3,8},{3,9},//左侧防御区突出的两处
            {17,8},{17,9},//右侧防御区突出的两处
            {18,4},{18,5},{18,6},
            {18,7},{18,8},{18,9},{18,10},{18,11},{18,12},{18,13},{18,14},
            {19,4},{19,5},{19,6},{19,7},{19,8},{19,9},{19,10},{19,11},{19,12},{19,13},{19,14},
            {20,4},{20,5},{20,6},{20,7},{20,8},{20,9},{20,10},{20,11},{20,12},{20,13},{20,14},//以上是右侧三列防御区域，从上至下
            {3,13},{4,13},{5,13},{6,13},{14,13},{15,13},{16,13},{17,13},//底部突出部分左侧和右侧
            {3,14},{4,14},{5,14},{6,14},{7,14},{8,14},{9,14},{10,14},{11,14},{12,14},{13,14},{14,14},{15,14},{16,14},{17,14},{18,14},{19,14},{20,14}// 底部防御区域
        };
        pos = Friendly_BasePosMap2;
        pos_count = sizeof(Friendly_BasePosMap2) / sizeof(Point);
    }
    if (random_num == 3) {
        Point Friendly_BasePosMap3[] = {
            {7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},
            {8,1},{9,1},{10,1},{11,1},{12,1},
            {9,2},{10,2},{11,2},
            {10,3},//以上是上部四行防御区域，从左至右
            {7,14},{8,14},{9,14},{10,14},{11,14},{12,14},{13,14},
            {8,13},{9,13},{10,13},{11,13},{12,13},
            {9,12},{10,12},{11,12},
            {10,11},//以上是下部四行防御区域，从左至右
            {4,4},{5,4},{6,4},
            {5,5},{6,5},{7,5},//左上侧Z型区域
            {14,4},{15,4},{16,4},
            {13,5},{14,5},{15,5},//右上侧Z型区域
            {4,10},{5,10},{6,10},
            {5,9},{6,9},{7,9},//左下侧Z型区域
            {14,10},{15,10},{16,10},
            {13,9},{14,9},{15,9} //右下侧Z型区域
        };
        pos = Friendly_BasePosMap3;
        pos_count = sizeof(Friendly_BasePosMap3) / sizeof(Point);
    }
    for (int i = 0; i < pos_count && block_count < 1000; i++) {
        blocks[block_count].x1 = pos[i].cannon_x * Gap;
        blocks[block_count].y1 = pos[i].cannon_y * Gap;
        blocks[block_count].x2 = blocks[block_count].x1 + Gap;
        blocks[block_count].y2 = blocks[block_count].y1 + Gap;
        blocks[block_count].is_hover = false;
        block_count++;
    }
}

// 检测鼠标悬停并绘制方块
void draw_blocks(int mouse_x,int mouse_y) {
    if (selected_tower_type != 0)//只有选中大炮类型时才检测悬停
    {
        int grid_mouse_x = mouse_x / Gap * Gap;
        int grid_mouse_y = mouse_y / Gap * Gap;
        // 绘制方块（根据悬停状态切换颜色）
        for (int i = 0; i < block_count; i++) {
            if (grid_mouse_x >= blocks[i].x1 && grid_mouse_y < blocks[i].x2 && grid_mouse_y >= blocks[i].y1 && grid_mouse_y < blocks[i].y2) {
                blocks[i].is_hover = true;
            }
            else {
				blocks[i].is_hover = false;
            }
            if (blocks[i].is_hover) {
                setfillcolor(GREEN); // 绿色悬停颜色
            }
            else {
				setfillcolor(RGB(185, 128, 71)); //移开后恢复棕色 
			}
			fillrectangle(blocks[i].x1, blocks[i].y1, blocks[i].x2, blocks[i].y2);
        }
    }
}

// 判断鼠标点击位置是否可部署大炮（返回1=可部署，0=不可部署）
int canDeployCannon(int mouse_x, int mouse_y) {
    // 1. 检测鼠标点击是否在棕色格子内
    int target_block_idx = -1;
    for (int i = 0; i < block_count; i++) 
        if (mouse_x >= blocks[i].x1 && mouse_x <= blocks[i].x2 &&
            mouse_y >= blocks[i].y1 && mouse_y <= blocks[i].y2) {
            target_block_idx = i;
            break;
        }
    // 不是棕色格子，直接返回false
    if(target_block_idx == -1) {
        return 0;
    }

    // 2. 检测该棕色格子是否已部署大炮
    int block_x = blocks[target_block_idx].x1+Gap/2;
    int block_y = blocks[target_block_idx].y1+Gap/2;
    for (int i = 0; i < cannon_count; i++) {
        if (cannons[i].x == block_x && cannons[i].y == block_y) {
            return 0; // 已有大炮，不可部署
        }
    }
    return 1; // 可部署
}

// 绘制所有大炮
void drawAllCannons() {
    for (int i = 0; i < cannon_count; i++) {
        drawCannons(cannons[i]);
    }
}

// 绘制大炮
void drawCannons(Cannon cannon) {
    // 根据类型选择颜色
	if (cannon.is_selected)//如果选中大炮则高亮显示
    {
		setfillcolor(YELLOW); // 高光颜色
		solidcircle(cannon.x, cannon.y, Gap / 2 + 5); // 略大一些的高光圈
    }
    switch (cannon.type) {
    case tower1:
        setfillcolor(RGB(255, 165, 0)); // 橙色
        break;
    case tower2:
        setfillcolor(RGB(90, 237, 206)); // 青色
        break;
    case tower3:
        setfillcolor(RGB(234, 54, 128)); // 粉色
        break;
    default:
        setfillcolor(BLACK); // 默认黑色
        break;
    }
    solidcircle(cannon.x, cannon.y, Gap / 2); // 大炮半径为Gap/2
}


// 部署大炮：先判断（GetDefense）→ 再扣钱 → 最后生成
void deployCannon(int mouse_x, int mouse_y, int* totalmoney) {
    if (!canDeployCannon(mouse_x, mouse_y) || cannon_count >= MAX_CANNON||selected_tower_type==invalidType||totalmoney==NULL) {
        return;
    }

    // 调用GetDefense判断金币是否足够、获取防御塔数据（不扣钱）
    DefenseUnit unit = tower[selected_tower_type];
    if (*totalmoney<unit.price) {
        drawTip(_T("金币不足，无法部署该大炮！"));
        return;
    }
    // 找到点击的棕色格子坐标，生成大炮,执行扣钱操作
    for (int i = 0; i < block_count; i++) {
        if (mouse_x >= blocks[i].x1 && mouse_x <= blocks[i].x2 &&
            mouse_y >= blocks[i].y1 && mouse_y <= blocks[i].y2) {
            cannons[cannon_count].type = (DefenseType)selected_tower_type;
            cannons[cannon_count].x = blocks[i].x1 + Gap / 2; // 大炮居中显示
            cannons[cannon_count].y = blocks[i].y1 + Gap / 2;
            cannons[cannon_count].level = 1;
            cannons[cannon_count].current_attack = unit.attack;
			int grid_x = blocks[i].x1 / Gap;
            int grid_y = blocks[i].y1 / Gap;
			cannons[cannon_count].attack_range = calcTowerGridRange(grid_x, grid_y,unit.attack_radius);
            *totalmoney -= unit.price;
			cannons[cannon_count].is_selected = false;// 默认未选中
			drawCannons(cannons[cannon_count]);// 立即绘制新部署的大炮
            cannon_count++;
            break;
        }
    }
}

// 选中大炮
void selectCannon(int mouse_x, int mouse_y) {
    // 取消之前选中的大炮
    if (selected_idx != -1) {
        cannons[selected_idx].is_selected = false;
		drawCannons(cannons[selected_idx]);
        selected_idx = -1;
    }
    // 检测点击的大炮
    for (int i = 0; i < cannon_count; i++) {
        int dx = mouse_x - cannons[i].x;
        int dy = mouse_y - cannons[i].y;
        if (dx * dx + dy * dy <= (Gap / 2) * (Gap / 2)) { // 大炮半径为Gap/2
            cannons[i].is_selected = true;
            selected_idx = i;
			drawCannons(cannons[i]);
            break;
        }
    }
}

// 升级大炮（仅提升攻击力，等级越高增量越小）
void upgradeCannon(int* totalmoney) {
	if (totalmoney == NULL) return;
    if (selected_idx == -1) return;
    Cannon* c = &cannons[selected_idx];

    if (c->level >= 3) {
        drawTip(_T("大炮已达最高等级，无法升级！"));
        return;
    }
    if (*totalmoney < Upgrade_Cost) {
        drawTip(_T("金币不足，无法升级大炮！"));
        return;
    }

    // 扣除金币，提升等级，增加射程
    *totalmoney -= Upgrade_Cost;
    c->level++;
    c->current_attack += upgradeDate[c->level - 2].attack_add; // 取对应攻击力的增量
}

// 移除大炮（返还50%购买价）
void removeCannon(int* totalmoney) {
	if (totalmoney == NULL) return;
    if (selected_idx == -1) return;
    Cannon* c = &cannons[selected_idx];
    DefenseUnit unit = tower[c->type];

    // 返还金币
    *totalmoney += (int)(unit.price * Refund_Rate);
	// 从数组中移除该大炮
    for(int i=selected_idx; i < cannon_count - 1; i++) {
        cannons[i] = cannons[i + 1]; 
	}
	cannon_count--;

    // 移除大炮
    selected_idx = -1;
}

    //打印地图下方的防御区域
    void PrintDefenseAreas()
    {
        //initgraph(width, height);
        //setbkcolor(RGB(237, 231, 216));
        //cleardevice();//用白色清屏
        setlinecolor(BLACK);//设置线条颜色为黑色
        setlinestyle(PS_SOLID, 2);//设置线条为实线，宽度为2
        line(Game_width / 4, Game_height, Game_width / 4, height);
        line(Game_width / 2, Game_height, Game_width / 2, height);
        line(Game_width * 3 / 4, Game_height, Game_width * 3 / 4, height);
		setfillcolor(RGB(255, 165, 0));//大炮类型1颜色
        solidcircle(Game_width / 8, (2 * Game_height) / 3 + height / 3, Gap);
		setfillcolor(RGB(90, 237, 206));//大炮类型2颜色
		solidcircle(Game_width * 3 / 8, (2 * Game_height) / 3 + height / 3, Gap);
		setfillcolor(RGB(234, 54, 128));//大炮类型3颜色
		solidcircle(Game_width * 5 / 8, (2 * Game_height) / 3 + height / 3, Gap);
		settextcolor(BLACK);//文字颜色
		settextstyle(16, 0, _T("黑体"));
		outtextxy(Game_width / 8 - 30, (2 * Game_height) / 3 + height / 3 + 50, _T("价格：100"));
		outtextxy(Game_width * 3 / 8 - 30, (2 * Game_height) / 3 + height / 3 + 50, _T("价格：200"));
		outtextxy(Game_width * 5 / 8 - 30, (2 * Game_height) / 3 + height / 3 + 50, _T("价格：300"));
		setfillcolor(RGB(60, 179, 113));//升级按钮颜色
		solidroundrect(13 * Game_width / 16, height / 7 + 6 * Game_height / 7, 15 * Game_width / 16, 3*height / 7 + 4 * Game_height / 7, 10, 10);
        settextcolor(BLACK);
        settextstyle(16, 0, _T("黑体"));
		outtextxy(13 * Game_width / 16 + 38, height / 7 + 6 * Game_height / 7 + 20, _T("升级"));
		setfillcolor(RGB(220, 20, 60));//移除按钮颜色
		solidroundrect(13 * Game_width / 16, 5*height / 7 + 2 * Game_height / 8, 15 * Game_width / 16, 6*height / 7 + Game_height / 7, 10, 10);
		settextcolor(BLACK);
		settextstyle(16, 0, _T("黑体"));
		outtextxy(13 * Game_width / 16 + 38, 5*height / 7 + 2 * Game_height / 7 -5, _T("移除"));
       /* _getch();
        closegraph();*/
    }

    //// 实时监测鼠标位置及点击事件
//void RealTimeMouseMonitor(int* totalmoney) {
//    if (totalmoney == NULL) return;
//
//    MOUSEMSG m;
//    if (MouseHit()) {
//        m = GetMouseMsg();
//        g_mouse_x = m.x;
//        g_mouse_y = m.y;
//
//        if (m.uMsg == WM_LBUTTONDOWN) {
//            // 1. 检测是否点击防御塔选择按钮（优先级最高）
//            DefenseType clicked_tower = CheckClickedTower(g_mouse_x, g_mouse_y);
//            if (clicked_tower != invalidType) {
//                selected_tower_type = clicked_tower;
//                return;
//            }
//
//            // 2. 检测是否点击升级按钮
//            int upgrade_x1 = 13 * Game_width / 16;
//            int upgrade_y1 = height / 7 + 6 * Game_height / 7;
//            int upgrade_x2 = 15 * Game_width / 16;
//            int upgrade_y2 = 3 * height / 7 + 4 * Game_height / 7;
//            if (g_mouse_x >= upgrade_x1 && g_mouse_x <= upgrade_x2 &&
//                g_mouse_y >= upgrade_y1 && g_mouse_y <= upgrade_y2) {
//                upgradeCannon(totalmoney); // 内部已做选中/等级/金币校验
//                return;
//            }
//
//            // 3. 检测是否点击移除按钮
//            int remove_x1 = 13 * Game_width / 16;
//            int remove_y1 = 5 * height / 7 + 2 * Game_height / 8;
//            int remove_x2 = 15 * Game_width / 16;
//            int remove_y2 = 6 * height / 7 + Game_height / 7;
//            if (g_mouse_x >= remove_x1 && g_mouse_x <= remove_x2 &&
//                g_mouse_y >= remove_y1 && g_mouse_y <= remove_y2) {
//                removeCannon(totalmoney); // 内部删除数组中大炮数据
//                return;
//            }
//
//            // 4. 检测是否点击已部署的大炮（选中操作）
//            selectCannon(g_mouse_x, g_mouse_y);
//
//            // 5. 检测是否部署大炮（已选塔 + 可部署位置）
//            if (selected_tower_type != invalidType) {
//                if (canDeployCannon(g_mouse_x, g_mouse_y)) {
//                    deployCannon(g_mouse_x, g_mouse_y, totalmoney);
//                    selected_tower_type = invalidType; // 部署后重置选塔类型
//                }
//                else {
//                    drawTip(_T("该位置无法部署大炮！"));
//                }
//                return;
//            }
//        }
//    }
//}