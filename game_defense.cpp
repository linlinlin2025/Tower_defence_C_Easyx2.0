//生成防御单位
#include <graphics.h> 
#include "game_defense.h"
#include "game_core.h"
#include <stdio.h>
#include <conio.h>

// 错误提示文字绘制
void drawTip(const TCHAR *text) {
	settextcolor(RED);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(Game_width/2 - textwidth(text)/2, Game_height/2, text);
}

 //给出要的防御塔类型和拥有的总金币，返回需要的防御塔或者无效防御塔
 DefenseUnit GetDefense(DefenseType type,int &totalmoney) 
{    DefenseUnit invalidUnit;
     invalidUnit.type = invalidType;
     invalidUnit.attack = 0;
     invalidUnit.range = 0;
     invalidUnit.price = 0;
	if (totalmoney < tower[type].price) return invalidUnit;
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

// 部署大炮：先判断（GetDefense）→ 再扣钱 → 最后生成
void deployCannon(int mouse_x, int mouse_y, int& totalmoney) {
    if (!canDeployCannon(mouse_x, mouse_y) || cannon_count >= MAX_CANNON) {
        return;
    }

    // 1. 调用GetDefense判断金币是否足够、获取防御塔数据（不扣钱）
    DefenseUnit unit = GetDefense((DefenseType)selected_tower_type, totalmoney);
    if (unit.type == invalidType) {
		drawTip("金币不足，无法部署该大炮！");
        return;
    }

    // 2. 确认可部署，执行扣钱操作
    totalmoney -= unit.price;

    // 3. 找到点击的棕色格子坐标，生成大炮
    for (int i = 0; i < block_count; i++) {
        if (mouse_x >= blocks[i].x1 && mouse_x <= blocks[i].x2 &&
            mouse_y >= blocks[i].y1 && mouse_y <= blocks[i].y2) {
            cannons[cannon_count].type = (DefenseType)selected_tower_type;
            cannons[cannon_count].x = blocks[i].x1 + Gap / 2; // 大炮居中显示
            cannons[cannon_count].y = blocks[i].y1 + Gap / 2;
            cannons[cannon_count].level = 1;
            cannons[cannon_count].current_range = unit.range;
            cannons[cannon_count].is_selected = false;
            cannons[cannon_count].is_active = true;
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

// 升级大炮（仅提升射程，等级越高增量越小）
void upgradeCannon(int& totalmoney) {
    if (selected_idx == -1) return;
    Cannon* c = &cannons[selected_idx];

    if (c->level >= 3) {
		drawTip("大炮已达最高等级，无法升级！");
        return;
    }
    if (totalmoney < Upgrade_Cost) {
		drawTip("金币不足，无法升级大炮！");
        return;
    }

    // 扣除金币，提升等级，增加射程
    totalmoney -= Upgrade_Cost;
    c->level++;
    c->current_range += upgradeDate[c->level - 2].range_add; // 取对应增量
}

// 移除大炮（返还50%购买价）
void removeCannon(int& totalmoney) {
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


// 绘制大炮：橙色实心圆，无边框
void drawCannons() {
    for (int i = 0; i < cannon_count; i++) {
        if (!cannons[i].is_active) continue;

        // 绘制橙色实心圆（无边框）
        setfillcolor(RGB(255, 165, 0));
        solidcircle(cannons[i].x, cannons[i].y, Gap / 2);
}

// 绘制功能按钮
    void drawButtons(int totalmoney)
    { 
        return 0;
    }


//处理鼠标点击（部署、选中、升级、移除）
void handleMouse(int totalmoney)
{
}


void PrintDefenseAreas()
{
    initgraph(width ,height);
    setbkcolor(RGB(237, 231, 216));
    cleardevice();//用白色清屏
    setlinecolor(BLACK);//设置线条颜色为黑色
    setlinestyle(PS_SOLID, 3);//设置线条为实线，宽度为3
    line(Game_width / 4, Game_height, Game_width / 4, height);
    line(Game_width / 2, Game_height, Game_width / 2, height);
    line(Game_width *3 / 4, Game_height, Game_width *3/4 , height);
    solidcircle(Game_width / 8, (2* Game_height)/3+height/3, Gap/2);
    _getch();
    closegraph();
}
int main()
{
    PrintDefenseAreas();
    return 0;
}