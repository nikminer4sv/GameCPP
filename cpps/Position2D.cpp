#include "../headers/Position2D.h"

Position2D::Position2D() {
    x = 0;
    y = 0;
    xf = 0;
    yf = 0;
}

Position2D::Position2D(int x, int y) {
    this->x = x;
    this->y = y;
}


Position2D::Position2D(float xf, float yf) {
    this->xf = xf;
    this->yf = yf;
}

int Position2D::GetX() {
    return x;
}

int Position2D::GetY() {
    return y;
}

void Position2D::SetX(int x) {
    this->x = x;
}

void Position2D::SetY(int y) {
    this->y = y;
}

void Position2D::SetXY(int x, int y) {
    this->x = x;
    this->y = y;
}

float Position2D::GetXF() {
    return xf;
}

float Position2D::GetYF() {
    return yf;
}

void Position2D::SetXF(float xf) {
    this->xf = xf;
}

void Position2D::SetYF(float yf) {
    this->yf = yf;
}

void Position2D::SetXYF(float xf, float yf) {
    this->xf = xf;
    this->yf = yf;
}