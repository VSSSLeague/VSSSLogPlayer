#ifndef GLTEXT_H
#define GLTEXT_H

#include <QVector>
#include <QtGui>
#include <GL/glu.h>
#include <math.h>
#include <float.h>

class GLText{
    struct Glyph{
        bool compiled;
        GLuint displayListID;
        double width;
        double height;
        double ascent;
        double descent;
        Glyph(){compiled=false;}
    };

    QVector<Glyph> glyphs;

    double characterSpacing;
    static const bool debugTesselation = false;
    QFont font;

public:
    typedef enum{
        LeftAligned,
        RightAligned,
        CenterAligned
    } HAlignOptions;

    typedef enum{
        TopAligned,
        BottomAligned,
        MedianAligned,
        MiddleAligned
    } VAlignOptions;

    GLText(QFont font = QFont());
    ~GLText();
    void drawString(QVector2D loc, double angle, double size, const char* str, GLText::HAlignOptions hAlign=LeftAligned, GLText::VAlignOptions vAlign=MiddleAligned);
    void drawGlyph(char glyph);
    void initializeGlyph(char ch);
    double getWidth(char ch);
    double getHeight(char ch);
    QVector2D getSize(char ch);
    double getWidth(const char* str);
    double getHeight(const char* str);
    double getAscent(char ch);
    double getDescent(char ch);
    double getAscent(const char* str);
    double getDescent(const char* str);

private:
    static const char* getPrimitiveType(GLenum type);
    static void tessBeginCB(GLenum which);
    static void tessEndCB();
    static void tessVertexCB(const GLvoid *data);
    static void tessErrorCB(GLenum errorCode);
    static constexpr double FontRenderSize = 1000.0;
};

#endif //GL_TEXT_H
