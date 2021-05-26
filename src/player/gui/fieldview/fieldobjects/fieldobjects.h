#ifndef FIELDOBJECTS_H
#define FIELDOBJECTS_H

#include <QString>
#include <QVector2D>

class FieldLine
{
public:
    // Constructors
    FieldLine(QString name, QVector2D p1, QVector2D p2, float lineThickness);
    FieldLine(const FieldLine& other);

    // Getters
    QString name() const { return _name; }
    QVector2D p1() const { return _p1; }
    QVector2D p2() const { return _p2; }
    float lineThickness() const { return _lineThickness; }

private:
    QString _name;
    QVector2D _p1, _p2;
    float _lineThickness;
};

class FieldCircularArc
{
public:
    // Constructors
    FieldCircularArc(QString name, QVector2D center, float radius, float a1, float a2, float lineThickness);
    FieldCircularArc(const FieldCircularArc& other);

    // Getters
    QString name() const { return _name; }
    QVector2D center() const { return _center; }
    float radius() const { return _radius; }
    float a1() const { return _a1; }
    float a2() const { return _a2; }
    float lineThickness() const { return _lineThickness; }

private:
    QString _name;
    QVector2D _center;
    float _radius;
    float _a1, _a2;
    float _lineThickness;
};

class FieldTriangle
{
public:
    // Constructors
    FieldTriangle(QString name, QVector2D p1, QVector2D p2, QVector2D p3, float lineThickness);
    FieldTriangle(const FieldTriangle& other);

    // Getters
    QString name() const { return _name; }
    QVector2D p1() const { return _p1; }
    QVector2D p2() const { return _p2; }
    QVector2D p3() const { return _p3; }
    float lineThickness() const { return _lineThickness; }

private:
    QString _name;
    QVector2D _p1, _p2, _p3;
    float _lineThickness;
};

#endif // FIELDOBJECTS_H
