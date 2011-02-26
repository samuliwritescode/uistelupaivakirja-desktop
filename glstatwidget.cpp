#include <QtGui>
#include <QtOpenGL>

#ifndef Q_OS_DARWIN
#include <GL/glext.h>
#endif

#include "glstatwidget.h"

GLStatWidget::GLStatWidget(QGLWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xPos = 0;
    yPos = 0;
    zPos = 2000;

    xRot = 0;
    yRot = 0;
    m_progress = 100;
}

void GLStatWidget::initializeGL()
{
    qglClearColor(QColor::fromRgb(200,200,200));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE_ARB);

    glEnable(GL_TEXTURE_2D);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLStatWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    qglColor(QColor::fromRgb(0,0,0));
    if(m_progress != 100)
    {
        QFont font;
        font.setPixelSize(24);
        renderText(this->width()/2-50, this->height()/2, tr("Lasken (")+QString::number(m_progress)+")%", font);
        return;
    } else if(m_stats.count() == 0)
    {
        QFont font;
        font.setPixelSize(24);
        renderText(this->width()/2-50, this->height()/2, tr("Ei tilastoa"), font);
        return;
    }

    glLoadIdentity();
    glPushMatrix();

    glTranslatef(xPos/160.0, -yPos/160.0, -zPos/160);
    glRotatef(-yRot, 1, 0, 0);
    glRotatef(-xRot, 0, 1, 0);

    for(int loop=0; loop < m_cols.count(); loop++)
    {
        QString text = m_cols.at(loop);
        renderText((double)loop, -0.3, 1.0, text);
    }

    for(int loop=0; loop < m_rows.count(); loop++)
    {
        QString text = m_rows.at(loop);
        renderText(-1, -0.3, -loop, text);
    }

    for(int loop=0; loop < m_stats.count(); loop++)
    {
        QHash<QString, QString> stats = m_stats.at(loop);
        int indexX = 0;
        foreach(QString name, m_cols)
        {
            double value = 0;
            if(m_maxVal != 0)
                value= 5*stats[name].toDouble()/m_maxVal;

            if(m_maxVal != 0)
                drawBox(indexX, 0.0, -loop, 0.4, value, stats[name].toDouble()/m_maxVal);
            else
                drawBox(indexX, 0.0, -loop, 0.4, value, 0);

            if(stats[name].isEmpty())
                renderText(indexX, value, -loop+0.3, "-");
            else
                renderText(indexX, value, -loop+0.3, stats[name]);
            //drawLine(indexX, 0.0, -loop, indexX+10, 0.0, -loop-10);
            indexX++;
        }

    }

    glTranslatef(0.5, 1.5, 0);
    glPopMatrix();

}

void GLStatWidget::addStat(const QHash<QString, QString>& p_stat, const QString& p_name)
{
    if(p_stat.count() == 0)
        return;

    m_stats.append(p_stat);
    m_rows.push_back(p_name);

    for(QHash<QString, QString>::const_iterator iter = p_stat.begin(); iter != p_stat.end(); iter++)
    {
        if(iter.value().toDouble() > m_maxVal)
            m_maxVal = iter.value().toDouble();
    }

    updateGL();
}

void GLStatWidget::clearStat()
{
    m_stats.clear();
    m_rows.clear();
    m_maxVal = 0;
    updateGL();
}

void GLStatWidget::setProgress(int p_progress)
{
    m_progress = p_progress;
    updateGL();
}

void GLStatWidget::setCols(const QList<QString>& p_cols)
{
    m_cols = p_cols;
    updateGL();
}

void GLStatWidget::drawBox(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat color)
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

    GLfloat r1 = color;
    GLfloat g1 = 1.0-color;
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

void GLStatWidget::drawLine(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{

    GLfloat vertices[] =
    {
        x1,y1,z1,
        x2,y2,z2
    };


    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);

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
        xRot += event->x() - lastPos.x();
        yRot += event->y() - lastPos.y();
        updateGL();
    }
    lastPos = event->pos();
}

void GLStatWidget::wheelEvent ( QWheelEvent * event )
{
    zPos += event->delta();
    updateGL();
}
