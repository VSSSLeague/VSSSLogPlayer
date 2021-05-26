#include "fieldview.h"
#define FIELD_COLOR 0.25,0.25,0.25,1.0
#define FIELD_LINES_COLOR 1.0,1.0,1.0,1.0

FieldView::FieldView(QWidget *parent) : QOpenGLWidget(parent) {
    // 4x MSAA for antialiasing
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    this->setFormat(fmt);

    // Setup glText
    QFont robotIDFont = this->font();
    robotIDFont.setWeight(QFont::Bold);
    robotIDFont.setPointSize(80);
    glText = GLText(robotIDFont);

    // Take default geometry
    updateDefaultGeometry();
}

void FieldView::updateDefaultGeometry() {
    _fieldLength = DEFAULT_3V3_FIELD::kFieldLength;
    _fieldWidth = DEFAULT_3V3_FIELD::kFieldWidth;

    _fieldLines.clear();
    for(size_t i = 0; i < DEFAULT_3V3_FIELD::kNumFieldLines; i++) {
        _fieldLines.push_back(FieldLine(DEFAULT_3V3_FIELD::kFieldLines[i]));
    }

    for(size_t i = 0; i < DEFAULT_3V3_FIELD::kNumLeftGoalLines; i++) {
        _fieldLines.push_back(FieldLine(DEFAULT_3V3_FIELD::kLeftGoalLines[i]));
    }

    for(size_t i = 0; i < DEFAULT_3V3_FIELD::kNumRightGoalLines; i++) {
        _fieldLines.push_back(FieldLine(DEFAULT_3V3_FIELD::kRightGoalLines[i]));
    }

    _fieldCircularArcs.clear();
    for(size_t i = 0; i < DEFAULT_3V3_FIELD::kNumFieldArcs; i++) {
        _fieldCircularArcs.push_back(FieldCircularArc(DEFAULT_3V3_FIELD::kFieldArcs[i]));
    }

    _fieldTriangles.clear();
    for(size_t i = 0; i < DEFAULT_3V3_FIELD::kNumFieldTriangles; i++) {
        _fieldTriangles.push_back(FieldTriangle(DEFAULT_3V3_FIELD::kFieldTriangles[i]));
    }
}

void FieldView::updateFieldObjects(fira_message::sim_to_ref::Environment wrapperData) {
    _graphicsMutex.lock();
    _wrapperData = wrapperData;
    _graphicsMutex.unlock();
}

void FieldView::paintGL() {
    _graphicsMutex.lock();

    glClearColor(FIELD_COLOR);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    drawFieldLines();

    // Get frame
    if(_wrapperData.has_frame()) {
        fira_message::Frame frame = _wrapperData.frame();

        // Draw ball
        if(frame.has_ball()) {
            fira_message::Ball ball = frame.ball();
            drawBall(QVector2D(ball.x(), ball.y())*1000.0f);
        }

        // Iterate and draw blue robots
        for(int i = 0; i < frame.robots_blue_size(); i++) {
            fira_message::Robot robot = frame.robots_blue(i);
            drawRobot(false, robot.robot_id(), QVector2D(robot.x(), robot.y())*1000.0f, robot.orientation());
        }

        // Iterate and draw yellow robots
        for(int i = 0; i < frame.robots_yellow_size(); i++) {
            fira_message::Robot robot = frame.robots_yellow(i);
            drawRobot(true, robot.robot_id(), QVector2D(robot.x(), robot.y())*1000.0f, robot.orientation());
        }
    }

    glPopMatrix();
    _graphicsMutex.unlock();
}

void FieldView::mousePressEvent(QMouseEvent* event) {
    leftButton = event->buttons().testFlag(Qt::LeftButton);
    midButton = event->buttons().testFlag(Qt::MidButton);
    rightButton = event->buttons().testFlag(Qt::RightButton);

    if(leftButton) {
        setCursor(Qt::ClosedHandCursor);
    }
    if(midButton) {
        setCursor(Qt::SizeVerCursor);
    }
    if(leftButton || midButton) {
        mouseStartX = event->x();
        mouseStartY = event->y();
    }
}

void FieldView::mouseReleaseEvent(QMouseEvent* event) {
    setCursor(Qt::ArrowCursor);
}

void FieldView::mouseMoveEvent(QMouseEvent* event) {
    bool leftButton = event->buttons().testFlag(Qt::LeftButton);
    bool midButton = event->buttons().testFlag(Qt::MidButton);

    if(leftButton) {
        viewXOffset -= viewScale*double(event->x() - mouseStartX);
        viewYOffset += viewScale*double(event->y() - mouseStartY);
        mouseStartX = event->x();
        mouseStartY = event->y();
        recomputeProjection();
    }
    else if(midButton) {
        float zoomRatio = static_cast<float>(event->y() - mouseStartY)/500.0;
        viewScale = viewScale*(1.0+zoomRatio);
        recomputeProjection();
        mouseStartX = event->x();
        mouseStartY = event->y();
    }
}

void FieldView::wheelEvent(QWheelEvent* event) {
    float zoomRatio = -static_cast<float>(event->delta())/1000.0;
    viewScale = viewScale*(1.0+zoomRatio);
    recomputeProjection();
}

void FieldView::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    resetView();
}

void FieldView::drawQuad(QVector2D v1, QVector2D v2, float z) {
    glBegin(GL_QUADS);
    glVertex3d(v1.x(), v1.y(), z);
    glVertex3d(v2.x(), v1.y(), z);
    glVertex3d(v2.x(), v2.y(), z);
    glVertex3d(v1.x(), v2.y(), z);
    glEnd();
}

void FieldView::drawQuad(QVector2D v1, QVector2D v2, QVector2D v3, QVector2D v4, float z) {
    glBegin(GL_QUADS);
    glVertex3d(v1.x(), v1.y(), z);
    glVertex3d(v2.x(), v2.y(), z);
    glVertex3d(v3.x(), v3.y(), z);
    glVertex3d(v4.x(), v4.y(), z);
    glEnd();
}

void FieldView::drawArc(QVector2D center, float r1, float r2, float theta1, float theta2, float z, float dTheta) {
    static const float tesselation = 1.0;

    if(dTheta < 0) {
        dTheta = tesselation/r2;
    }

    glBegin(GL_QUAD_STRIP);
    for(float theta = theta1; theta < theta2; theta += dTheta) {
        float c1 = cos(theta);
        float s1 = sin(theta);
        glVertex3d(r2*c1 + center.x(), r2*s1 + center.y(), z);
        glVertex3d(r1*c1 + center.x(), r1*s1 + center.y(), z);
    }

    float c1 = cos(theta2);
    float s1 = sin(theta2);
    glVertex3d(r2*c1 + center.x(), r2*s1 + center.y(), z);
    glVertex3d(r1*c1 + center.x(), r1*s1 + center.y(), z);
    glEnd();
}

void FieldView::drawTriangle(QVector2D v1, QVector2D v2, QVector2D v3, float z) {
    glBegin(GL_TRIANGLES);
    glVertex3d(v1.x(), v1.y(), z);
    glVertex3d(v2.x(), v2.y(), z);
    glVertex3d(v3.x(), v3.y(), z);
    glEnd();
}

void FieldView::resetView() {
    viewScale = (_fieldLength + 100.0) / width();
    viewScale = std::max(viewScale, (_fieldWidth + 100) / height());

    viewXOffset = viewYOffset = 0.0;
    recomputeProjection();
}

void FieldView::recomputeProjection() {
    makeCurrent();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5*viewScale*width()+viewXOffset, 0.5*viewScale*width()+viewXOffset, -0.5*viewScale*height()+viewYOffset, 0.5*viewScale*height()+viewYOffset, minZValue, maxZValue);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    doneCurrent();
}

void FieldView::drawFieldLines() {
    glColor4f(FIELD_LINES_COLOR);

    for (int i = 0; i < _fieldLines.size(); ++i) {
        const FieldLine& line = _fieldLines[i];
        const float half_thickness = 0.5 * line.lineThickness();
        const QVector2D p1 = line.p1();
        const QVector2D p2 = line.p2();

        QVector2D perp = (p2 - p1);
        float square = sqrt(pow(perp.x(), 2) + pow(perp.y(), 2));
        perp = QVector2D(perp.x() / square, perp.y() / square);
        perp = QVector2D(-perp.y(), perp.x());

        const QVector2D v1 = p1 - half_thickness * perp;
        const QVector2D v2 = p1 + half_thickness * perp;
        const QVector2D v3 = p2 + half_thickness * perp;
        const QVector2D v4 = p2 - half_thickness * perp;
        drawQuad(v1, v2, v3, v4, fieldZ);
    }

    for (int i = 0; i < _fieldCircularArcs.size(); ++i) {
        const FieldCircularArc& arc = _fieldCircularArcs[i];
        const float half_thickness = 0.5 * arc.lineThickness();
        const float radius = arc.radius();
        const QVector2D center = arc.center();
        const float a1 = arc.a1();
        const float a2 = arc.a2();
        drawArc(center, radius - half_thickness, radius + half_thickness, a1, a2, fieldZ);
    }

    for(int i = 0; i < _fieldTriangles.size(); i++) {
        const FieldTriangle& triangle = _fieldTriangles[i];
        const QVector2D v1 = triangle.p1();
        const QVector2D v2 = triangle.p2();
        const QVector2D v3 = triangle.p3();
        drawTriangle(v1, v2, v3, fieldZ);
    }
}

void FieldView::drawBall(QVector2D position) {
    glColor3d(1.0,0.5059,0.0);
    drawArc(position, -18.5, 18.5, -M_PI, M_PI, ballZ);
    glColor3d(0.8706,0.3490,0.0);
    drawArc(position, 18.5, 21.5, -M_PI, M_PI, ballZ);
}

void FieldView::drawRobot(bool isYellow, quint8 robotId, QVector2D position, float orientation) {
    glPushMatrix();
    glLoadIdentity();
    glTranslated(position.x(), position.y(), 0);
    glRotated(orientation*(180.0/M_PI), 0, 0, 1.0);

    // Color for triangle (robot front)
    if(isYellow) {
        glColor3d(1.0, 0.9529, 0.2431);
    }
    else {
        glColor3d(0.2549, 0.4941, 1.0);
    }

    // Drawing border
    drawQuad(QVector2D(-35, 35), QVector2D(35, -35), robotZ);

    // Color for body
    if(isYellow) {
        glColor3d(0.8, 0.6157, 0.0);
    }
    else {
        glColor3d(0.0706, 0.2314, 0.6275);
    }

    // Draw robot body
    drawQuad(QVector2D(-40, 40), QVector2D(40, 35), robotZ+0.01);
    drawQuad(QVector2D(-40, -40), QVector2D(40, -35), robotZ+0.01);
    drawQuad(QVector2D(40, -40), QVector2D(35, 40), robotZ+0.01);
    drawQuad(QVector2D(-40, -40), QVector2D(-35, 40), robotZ+0.01);

    glColor3d(0.5, 0.5, 0.5);

    drawQuad(QVector2D(-20, 47), QVector2D(20, 40), robotZ+0.02);
    drawQuad(QVector2D(-20,-47), QVector2D(20, -40), robotZ+0.02);

    // Draw robot id
    glColor3d(0.0, 0.0, 0.0);
    QString playerId = QString("%1").arg(robotId);
    glText.drawString(position, 0.0, 50, playerId.toStdString().c_str(), GLText::CenterAligned, GLText::MiddleAligned);

    glPopMatrix();
}

void FieldView::redraw() {
    _graphicsMutex.lock();
    update();
    _graphicsMutex.unlock();
}
