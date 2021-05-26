#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QTimer>
#include <QPainter>
#include <QMutex>
#include <QMouseEvent>
#include <GL/glu.h>
#include <GL/gl.h>

#include <src/player/gui/fieldview/gltext/gltext.h>
#include <src/player/gui/fieldview/fieldobjects/fieldobjects.h>
#include <include/packet.pb.h>
#include <src/player/gui/fieldview/fieldobjects/default_3v3_field.h>

class FieldView : public QOpenGLWidget
{
    Q_OBJECT
public:
    FieldView(QWidget *parent = nullptr);
    void updateDefaultGeometry();
    void updateFieldObjects(fira_message::sim_to_ref::Environment wrapperData);

protected:
    void paintGL();
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeGL(int width, int height);

private:
    // Generalist draws
    void drawQuad(QVector2D v1, QVector2D v2, float z);
    void drawQuad(QVector2D v1, QVector2D v2, QVector2D v3, QVector2D v4, float z);
    void drawArc(QVector2D center, float r1, float r2, float theta1, float theta2, float z, float dTheta = -1);
    void drawTriangle(QVector2D v1, QVector2D v2, QVector2D v3, float z);

    // Field view
    float viewScale; /// Ratio of world space to screen space coordinates
    float viewXOffset;
    float viewYOffset;
    void resetView();
    void recomputeProjection();

    // Mouse events
    bool leftButton;
    bool midButton;
    bool rightButton;
    int mouseStartX;
    int mouseStartY;

    // Text draw
    GLText glText;

    // Field Objects
    fira_message::sim_to_ref::Environment _wrapperData;
    static constexpr float fieldZ = 1.0;
    static constexpr float robotZ = 2.0;
    static constexpr float ballZ = 3.0;
    static constexpr float minZValue = -10;
    static constexpr float maxZValue = 10;
    float _fieldLength;
    float _fieldWidth;
    QList<FieldLine> _fieldLines;
    QList<FieldTriangle> _fieldTriangles;
    QList<FieldCircularArc> _fieldCircularArcs;
    void drawFieldLines();
    void drawBall(QVector2D position);
    void drawRobot(bool isYellow, quint8 robotId, QVector2D position, float orientation);

    // Mutex to control graphics
    QMutex _graphicsMutex;

public slots:
    void redraw();
};

#endif // FIELDVIEW_H
