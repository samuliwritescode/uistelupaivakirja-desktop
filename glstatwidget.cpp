#include <QtGui>
#include <QtOpenGL>
#include "glstatwidget.h"

GLStatWidget::GLStatWidget(QGLWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xPos = 0;
    yPos = 0;
    zPos = 2000;

}

void GLStatWidget::initializeGL()
{
    qglClearColor(QColor::fromRgb(200,200,200));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLStatWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(xPos/160.0, -yPos/160.0, -zPos/160);

    double maxval=0;
    for(QMap<QString, QString>::iterator iter = m_stats.begin(); iter != m_stats.end(); iter++)
    {
        if(iter.value().toDouble() > maxval)
            maxval = iter.value().toDouble();
    }

    int index = 0;
    for(QMap<QString, QString>::iterator iter = m_stats.begin(); iter != m_stats.end(); iter++)
    {
        QString name = iter.key();
        double value = 5*iter.value().toDouble()/maxval;
        drawBox(index, 0.0, -1.0, 0.4, value, name);
        renderText(index, -1.0, -1.0, name);
        renderText(index, value, -1.0, iter.value());
        index++;
    }

    glTranslatef(0.5, 1.5, 0);
    glPopMatrix();

}

void GLStatWidget::setStat(const QMap<QString, QString>& p_stats)
{
    QMap<QString, QString> temp = p_stats;
    m_stats = temp;
    updateGL();
}

void GLStatWidget::drawBox(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, QString text)
{

    GLfloat vertices[] =
    {
        x+w,y+h,z+w,    x,y+h,z+w,      x,y,z+w,    x+w,y,z+w,
        x+w,y+h,z+w,    x+w,y,z+w,      x+w,y,z,    x+w,y+h,z,
        x+w,y+h,z+w,    x+w,y+h,z,      x,y+h,z,    x,y+h,z+w,
        x,y+h,z+w,      x,y+h,z,        x,y,z,      x,y,z+w,
        x,y,z,          x+w,y,z,        x+w,y,z+w,  x,y,z+w,
        x+w,y,z,        x,y,z,          x,y+h,z,    x+w,y+h,z
    };

    GLfloat normals[] =
    {
        0,0,1,      0,0,1,      0,0,1,      0,0,1,
        1,0,0,      1,0,0,      1,0,0,      1,0,0,
        0,1,0,      0,1,0,      0,1,0,      0,1,0,
        -1,0,0,     -1,0,0,     -1,0,0,     -1,0,0,
        0,-1,0,     0,-1,0,     0,-1,0,     0,-1,0,
        0,0,-1,     0,0,-1,     0,0,-1,     0,0,-1
     };

    GLfloat r1 = 1.0;
    GLfloat g1 = 0.0;
    GLfloat b1 = 0.0;

    GLfloat r2 = 0.0;
    GLfloat g2 = 1.0;
    GLfloat b2 = 0.0;

    GLfloat colors[] =
    {
        r1,g1,b1,   r1,g1,b1,  r2,g2,b2,  r2,g2,b2,
        r1,g1,b1,   r2,g2,b2,  r2,g2,b2,  r1,g1,b1,
        r1,g1,b1,   r1,g1,b1,  r1,g1,b1,  r1,g1,b1,
        r1,g1,b1,   r1,g1,b1,  r2,g2,b2,  r2,g2,b2,
        r2,g2,b2,   r2,g2,b2,  r2,g2,b2,  r2,g2,b2,
        r2,g2,b2,   r2,g2,b2,  r1,g1,b1,  r1,g1,b1
    };

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 24);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

}

void GLStatWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, static_cast<float>(width)/height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void GLStatWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLStatWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        xPos += event->x() - lastPos.x();
        yPos += event->y() - lastPos.y();
        updateGL();
    }
    else if (event->buttons() == Qt::RightButton)
    {
        zPos += event->y() - lastPos.y();
        updateGL();
    }
    lastPos = event->pos();
}
