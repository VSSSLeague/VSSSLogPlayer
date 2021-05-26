#include "fieldobjects.h"

FieldLine::FieldLine(QString name, QVector2D p1, QVector2D p2, float lineThickness) {
    _name = name;
    _p1 = p1;
    _p2 = p2;
    _lineThickness = lineThickness;
}

FieldLine::FieldLine(const FieldLine& other) {
    _name = other._name;
    _p1 = other._p1;
    _p2 = other._p2;
    _lineThickness = other._lineThickness;
}

FieldCircularArc::FieldCircularArc(QString name, QVector2D center, float radius, float a1, float a2, float lineThickness) {
    _name = name;
    _center = center;
    _radius = radius;
    _a1 = a1;
    _a2 = a2;
    _lineThickness = lineThickness;
}

FieldCircularArc::FieldCircularArc(const FieldCircularArc& other) {
    _name = other._name;
    _center = other._center;
    _radius = other._radius;
    _a1 = other._a1;
    _a2 = other._a2;
    _lineThickness = other._lineThickness;
}

FieldTriangle::FieldTriangle(QString name, QVector2D p1, QVector2D p2, QVector2D p3, float lineThickness) {
    _name = name;
    _p1 = p1;
    _p2 = p2;
    _p3 = p3;
    _lineThickness = lineThickness;
}

FieldTriangle::FieldTriangle(const FieldTriangle& other) {
    _name = other._name;
    _p1 = other._p1;
    _p2 = other._p2;
    _p3 = other._p3;
    _lineThickness = other._lineThickness;
}
