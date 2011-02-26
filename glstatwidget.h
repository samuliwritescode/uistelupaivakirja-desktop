#ifndef GLSTATWIDGET_H
#define GLSTATWIDGET_H

#include <QGLWidget>
#include <QHash>
#include <QList>

class GLStatWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLStatWidget(QGLWidget *parent = 0);
    void addStat(const QHash<QString, QString>&, const QString&);
    void clearStat();
    void setCols(const QList<QString>&);
    void setProgress(int);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent ( QWheelEvent * event );
    void drawBox(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
    void drawLine(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);

signals:

private:
    int xPos;
    int yPos;
    int zPos;

    int xRot;
    int yRot;

    QPoint lastPos;
    QList<QHash<QString, QString> > m_stats;
    QList<QString> m_cols;
    QList<QString> m_rows;
    double m_maxVal;
    int m_progress;
};

#endif // GLSTATWIDGET_H
