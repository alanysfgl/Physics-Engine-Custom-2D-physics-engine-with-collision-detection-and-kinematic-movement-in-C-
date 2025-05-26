#pragma once
#include"renderer.h"


void drawSquare(Vector2D pos, float size) {
    float half = size / 2.0f;
    glBegin(GL_QUADS);
    glVertex2f(pos.X - half, pos.Y - half);
    glVertex2f(pos.X + half, pos.Y - half);
    glVertex2f(pos.X + half, pos.Y + half);
    glVertex2f(pos.X - half, pos.Y + half);
    glEnd();
};
