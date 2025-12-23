#include <graphics.h> 
#include "game_defense.h"
#include "game_core.h"
#include <stdio.h>
#include <conio.h>

// 三种防御塔的具体数据
const DefenseUnit tower[] = {
    {tower1,10,calcTowerGridRange(7,7),100},
    {tower2,20,calcTowerGridRange(5,5),200},
    {tower3,30,calcTowerGridRange(3,3),300}
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
int selected_tower_type = tower1; // 默认选中的部署类型（tower1）


// 错误提示文字绘制
void drawTip(const TCHAR* text) {
    settextcolor(RED);
    settextstyle(20, 0, _T("黑体"));
    outtextxy(Game_width / 2 - textwidth(text) / 2, Game_height / 2, text);
}

// 计算单个方格的射程范围（基于塔的摆放格子坐标和Gap）
GridRange calcTowerGridRange(int grid_x, int grid_y) {
    GridRange attack_range;
    // 左上坐标：格子坐标 * 像素间隔
    attack_range.top_left.cannon_x= grid_x * Gap;
    attack_range.top_left.cannon_y = grid_y * Gap;
    // 右下坐标：(格子坐标+1) * 像素间隔（覆盖整个方格）
    attack_range.bottom_right.cannon_x = (grid_x + 1) * Gap;
    attack_range.bottom_right.cannon_y = (grid_y + 1) * Gap;
    return attack_range;
}


//检测鼠标点击了哪种防御塔
DefenseType CheckClickedTower(int mouse_x, int mouse_y)
{
    int tower1_x = Game_width / 8;
    int tower1_y = (2 * Game_height) / 3 + Game_height / 3 ;
    if((mouse_x-tower1_x)*(mouse_x-tower1_x)+(mouse_y-tower1_y)*(mouse_y-tower1_y)<= (Gap/2)*(Gap/2))
    {
        return tower1;
	}
    int tower2_x = 3 * Game_width / 8;
    if((mouse_x-tower2_x)*(mouse_x-tower2_x)+(mouse_y-tower1_y)*(mouse_y-tower1_y)<= (Gap/2)*(Gap/2))
    {
        return tower2;
    }
	int tower3_x = 5 * Game_width / 8;
    if((mouse_x-tower3_x)*(mouse_x-tower3_x)+(mouse_y-tower1_y)*(mouse_y-tower1_y)<= (Gap/2)*(Gap/2))
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
	MOUSEMSG m = GetMouseMsg();
	if (m.uMsg != WM_LBUTTONDOWN) return invalidUnit;//仅处理鼠标左键点击事件
	DefenseType type = CheckClickedTower(m.x, m.y);
    if (*totalmoney < tower[type].price) return invalidUnit;
    else
    {
        return tower[type];
    }
}


// 根据地图编号生成棕色方块坐标
void generate_blocks(int random_num) {
    block_count = 0; // 清空原有方块
    setfillcolor(RGB(185, 128, 71));

    if (random_num == 1) {
        // 地图1的棕色方块生成逻辑
        for (int i = 5 * Gap; i < 8 * Gap; i += Gap) {
            for (int j = 3 * Gap; j < 12 * Gap; j += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
            }
        }
    }
    else if (random_num == 2) {
        // 地图2的棕色方块生成逻辑
        for (int i = 0; i < 2 * Gap; i += Gap) {
            for (int j = 4 * Gap; j < Game_height; j += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
            }
        }
        for (int i = 2 * Gap; i < 3 * Gap; i += Gap) {
            for (int j = 7 * Gap; j < Game_height; j += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
            }
        }
        for (int i = 3 * Gap; i < 4 * Gap; i += Gap) {
            for (int j = 8 * Gap; j < 10 * Gap; j += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
            }
        }
        for (int i = 3 * Gap; i < 7 * Gap; i += Gap) {
            for (int j = 13 * Gap; j < Game_height; j += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
            }
        }
        for (int i = 7 * Gap; i < Game_width - 7 * Gap; i += Gap) {
            blocks[block_count++] = { i, Game_height - Gap, i + Gap, Game_height, false };
        }
    }
    else if (random_num == 3) {
        // 地图3的棕色方块生成逻辑
        for (int j = 0; j < 3 * Gap; j += Gap) {
            for (int i = 7 * Gap + j; i < 10 * Gap; i += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { i, Game_height - j, i + Gap, Game_height - j - Gap, false };
                if (i != Gap * 10) {
                    blocks[block_count++] = { Game_width - i, Game_height - j - Gap, Game_width - i - Gap, Game_height - j, false };
                    blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
                }
            }
        }
        for (int j = 0; j < 4 * Gap; j += Gap) {
            blocks[block_count++] = { 10 * Gap, j, 11 * Gap, j + Gap, false };
            blocks[block_count++] = { 10 * Gap, Game_height - j - Gap, 11 * Gap, Game_height - j, false };
        }
        for (int j = 4 * Gap; j < 6 * Gap; j += Gap) {
            static int Move_GAP = 0;
            for (int i = 4 * Gap + Move_GAP; i < 7 * Gap + Move_GAP; i += Gap) {
                blocks[block_count++] = { i, j, i + Gap, j + Gap, false };
                blocks[block_count++] = { Game_width - i, j, Game_width - i - Gap, j + Gap, false };
                blocks[block_count++] = { i, Game_height - j - Gap, i + Gap, Game_height - j, false };
                blocks[block_count++] = { Game_width - i, Game_height - j - Gap, Game_width - i - Gap, Game_height - j, false };
            }
            Move_GAP = 40;
        }
    }
}

// 检测鼠标悬停并绘制方块
void draw_blocks() {
    static MOUSEMSG m; // 静态存储鼠标消息，避免重复获取
    // 若有鼠标消息，更新鼠标坐标
    if (MouseHit()) {
        m = GetMouseMsg();
    }
    // 遍历所有方块，检测鼠标是否悬停
    for (int i = 0; i < block_count; i++) {
        if (m.x >= blocks[i].x1 && m.x <= blocks[i].x2 &&
            m.y >= blocks[i].y1 && m.y <= blocks[i].y2) {
            blocks[i].is_hover = true;
        }
        else {
            blocks[i].is_hover = false;
        }
    }
    // 绘制方块（根据悬停状态切换颜色）
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].is_hover) {
            setfillcolor(GREEN);
        }
        else {
            setfillcolor(RGB(185, 128, 71));
        }
        fillrectangle(blocks[i].x1, blocks[i].y1, blocks[i].x2, blocks[i].y2);
    }
}

// 判断鼠标点击位置是否可部署大炮（返回true=可部署，false=不可部署）
bool canDeployCannon(int mouse_x, int mouse_y) {
    // 1. 检测鼠标点击是否在棕色格子内
    int target_block_idx = -1;
    for (int i = 0; i < block_count; i++) {
        if (mouse_x >= blocks[i].x1 && mouse_x <= blocks[i].x2 &&
            mouse_y >= blocks[i].y1 && mouse_y <= blocks[i].y2) {
            target_block_idx = i;
            break;
        }
    }
    // 不是棕色格子，直接返回false
    if (target_block_idx == -1) {
        return false;
    }

    // 2. 检测该棕色格子是否已部署大炮
    int block_x = blocks[target_block_idx].x1;
    int block_y = blocks[target_block_idx].y1;
    for (int i = 0; i < cannon_count; i++) {
        if (cannons[i].x == block_x + Gap / 2 && cannons[i].y == block_y + Gap / 2 && cannons[i].is_active) {
            return false; // 已有大炮，不可部署
        }
    }
    return true; // 可部署
}

// 绘制大炮
void drawCannons(Cannon cannon) {
    if(!cannon.is_active) return; // 不存在则不绘制
    // 根据类型选择颜色
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
    if (!canDeployCannon(mouse_x, mouse_y) || cannon_count >= MAX_CANNON) {
        return;
    }

    // 1. 调用GetDefense判断金币是否足够、获取防御塔数据（不扣钱）
    DefenseUnit unit = GetDefense(totalmoney);
    if (unit.type == invalidType) {
        drawTip(_T("金币不足，无法部署该大炮！"));
        return;
    }

    // 2. 确认可部署，执行扣钱操作
    totalmoney -= unit.price;

    // 3. 找到点击的棕色格子坐标，生成大炮
	bool findBlock = false;
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
			cannons[cannon_count].attack_range = calcTowerGridRange(grid_x, grid_y);
            cannons[cannon_count].is_selected = false;
            cannons[cannon_count].is_active = true;
			drawCannons(cannons[cannon_count]);// 立即绘制新部署的大炮
            cannon_count++;
            findBlock = true;
            break;
        }
    }
}

// 选中大炮
void selectCannon(int mouse_x, int mouse_y) {
    // 取消之前选中的大炮
    if (selected_idx != -1) {
        cannons[selected_idx].is_selected = false;
        selected_idx = -1;
    }

    // 检测点击的大炮
    for (int i = 0; i < cannon_count; i++) {
        if (!cannons[i].is_active) continue;
        int dx = mouse_x - cannons[i].x;
        int dy = mouse_y - cannons[i].y;
        if (dx * dx + dy * dy <= (Gap / 2) * (Gap / 2)) { // 大炮半径为Gap/2
            cannons[i].is_selected = true;
            selected_idx = i;
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
    totalmoney += (int)(unit.price * Refund_Rate);
    // 移除大炮
    c->is_active = false;
    c->is_selected = false;
    selected_idx = -1;
}


    // 绘制功能按钮
    void drawButtons(int* totalmoney)
    {
    }


    //处理鼠标点击（部署、选中、升级、移除）
    void handleMouse(int* totalmoney)
    {
    }

    //打印地图下方的防御区域
    void PrintDefenseAreas()
    {
        initgraph(width, height);
        setbkcolor(RGB(237, 231, 216));
        cleardevice();//用白色清屏
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
		solidroundrect(13 * Game_width / 16, 5*height / 7 + 2 * Game_height / 7, 15 * Game_width / 16, 6*height / 7 + Game_height / 7, 10, 10);
		settextcolor(BLACK);
		settextstyle(16, 0, _T("黑体"));
		outtextxy(13 * Game_width / 16 + 38, 5*height / 7 + 2 * Game_height / 7 + 20, _T("移除"));
        _getch();
        closegraph();
    }
    int main()
    {
        PrintDefenseAreas();
		int totalmoney = 500; // 示例初始金币
        DefenseUnit GetDefense(int* totalmoney);
        DefenseType CheckClickedTower(int mouse_x,int mouse_y);
        
        return 0;
    }