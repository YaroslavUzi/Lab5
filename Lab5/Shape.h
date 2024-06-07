#ifndef SHAPE_H
#define SHAPE_H

#include <windows.h>

class Shape {
public:
    virtual void Draw(HDC hdc) = 0;
    virtual void SetData(POINT start, POINT end) = 0;
    virtual ~Shape() {}
};

class PointShape : public Shape {
    POINT point;
public:
    void Draw(HDC hdc) override {
        SetPixel(hdc, point.x, point.y, RGB(0, 0, 0));
    }

    void SetData(POINT start, POINT end) override {
        point = start;
    }
};

class LineShape : public Shape {
    POINT start, end;
public:
    void Draw(HDC hdc) override {
        MoveToEx(hdc, start.x, start.y, NULL);
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // червоний колір для гумового сліду
        SelectObject(hdc, pen);
        LineTo(hdc, end.x, end.y);
        DeleteObject(pen);
    }

    void SetData(POINT start, POINT end) override {
        this->start = start;
        this->end = end;
    }
};

class RectangleShape : public Shape {
    POINT start, end;
public:
    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(144, 238, 144)); // світло-зелена заливка
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // чорний контур
        SelectObject(hdc, brush);
        SelectObject(hdc, pen);
        Rectangle(hdc, start.x, start.y, end.x, end.y);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    void SetData(POINT start, POINT end) override {
        this->start = start;
        this->end = end;
    }
};

class EllipseShape : public Shape {
    POINT start, end;
public:
    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0)); // жовта заливка
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // чорний контур
        SelectObject(hdc, brush);
        SelectObject(hdc, pen);
        Ellipse(hdc, start.x, start.y, end.x, end.y);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    void SetData(POINT start, POINT end) override {
        this->start = start;
        this->end = end;
    }
};

#endif // SHAPE_H
