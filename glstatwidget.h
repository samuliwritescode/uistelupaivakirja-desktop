#ifndef GLSTATWIDGET_H
#define GLSTATWIDGET_H

#include <QGLWidget>
#include <QMap>

class GLStatWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLStatWidget(QGLWidget *parent = 0);
    void setStat(const QList<QMap<QString, QString> >&);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void	wheelEvent ( QWheelEvent * event );
    void drawBox(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

signals:

private:
    int xPos;
    int yPos;
    int zPos;

    int xRot;
    int yRot;

    QPoint lastPos;
    QList<QMap<QString, QString> > m_stats;

};

#endif // GLSTATWIDGET_H
