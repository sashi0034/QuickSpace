
enum Chip{
    Bush1,
    Bush2,
    Stone1,
    Stone2,
    Moss1,
    Moss2,
    Rock,
    Weed,
    Tree
};

void draw(int32 x, int32 y, Chip kind){
    DrawBgChip(x, y, kind);
}

void drawBushA(int x, int y){
    draw(x + 0, y + 0, Chip::Bush1);
    draw(x + 1, y + 1, Chip::Bush2);
    draw(x + 0, y + 2, Chip::Bush1);
}
void drawBushB(int x, int y){
    draw(x + 0, y + 0, Chip::Bush2);
    draw(x + 2, y + 1, Chip::Bush1);
    draw(x + 4, y + 0, Chip::Bush2);
    draw(x + 6, y + 1, Chip::Bush1);
    draw(x + 8, y + 0, Chip::Bush2);
}
void drawStoneA(int x, int y){
    draw(x, y, Chip::Stone1);
    draw(x + 2, y + 2, Chip::Stone1);
    draw(x + 4, y + 1, Chip::Stone1);
}
void drawStoneB(int x, int y){
    draw(x, y + 1, Chip::Stone2);
    draw(x + 2, y, Chip::Stone2);
}
void drawStoneC(int x, int y){
    draw(x, y + 2, Chip::Stone2);
    draw(x + 2, y + 0, Chip::Stone2);
    draw(x + 1, y + 4, Chip::Stone2);
}
void drawMossA(int x, int y){
    draw(x + 0, y + 0, Chip::Moss1);
    draw(x + 1, y + 1, Chip::Moss2);
    draw(x + 2, y + 0, Chip::Moss2);
}
void drawWeedA(int x, int y){
    draw(x + 0, y + 0, Chip::Weed);
    draw(x + 2, y + 1, Chip::Weed);
    draw(x + 4, y + 0, Chip::Weed);
}
void drawWeedB(int x, int y){
    draw(x + 0, y + 0, Chip::Weed);
    draw(x + 2, y + 1, Chip::Weed);
    draw(x + 4, y + 0, Chip::Weed);
    draw(x + 6, y + 1, Chip::Weed);
    draw(x + 8, y + 0, Chip::Weed);
}

void DrawBg(){
    drawBushA(4, 4);
    drawBushA(8, 2);
    drawBushA(-12, -12);
    drawBushB(6, 9);
    drawBushB(-3, -3);
    drawBushB(8, -8);
    drawStoneA(-6, 0);
    drawStoneB(-15, 9);
    drawStoneC(12, -3);
    drawMossA(-15, -6);
    drawMossA(-9, -9);
    drawMossA(-12, -3);
    drawMossA(5, -10);
    drawWeedA(-6, 6);
    drawWeedA(-6, -12);
    drawWeedB(-15, 3);
    drawWeedB(-10, 10);

    // side tree
    for (int y=-9; y<=9; y+=3){
        draw(-20, y, Chip::Tree);
        draw(-20 + 2, y - 1, Chip::Tree);
        draw(20 - 1, y, Chip::Tree);
        draw(20 - 1 - 2, y - 1, Chip::Tree);
    }
}

