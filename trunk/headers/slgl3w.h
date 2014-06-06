#ifndef SLGL3W_H
#define SLGL3W_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class SLGl3W
{
public:
    SLGl3W();

    static void setPatchVertices(int );
    static int getUniformLocation(unsigned int program, const QString& name);
    static int getAttributeLocation(unsigned int program, const QString& name);
    static void vertexAttrib(int loc, float value, bool isInt = false);
    static void vertexAttrib(int loc, const QVector2D& v, bool isInt = false);
    static void vertexAttrib(int loc, const QVector3D& v, bool isInt = false);
    static void vertexAttrib(int loc, const QVector4D& v, bool isInt = false);

    static void uniform(int loc, float value, bool isInt = false);
    static void uniform(int loc, const QVector2D& v, bool isInt = false);
    static void uniform(int loc, const QVector3D& v, bool isInt = false);
    static void uniform(int loc, const QVector4D& v, bool isInt = false);
};

#endif // SLGL3W_H
