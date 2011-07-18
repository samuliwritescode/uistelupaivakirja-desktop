#ifndef REVISIONDIALOG_H
#define REVISIONDIALOG_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
    class RevisionDialog;
}

class RevisionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RevisionDialog(QWidget *parent = 0);
    ~RevisionDialog();
    void setModified(QStringList list);
    void setConflicting(QStringList list);
    void setAdded(QStringList list);
    void setRemoved(QStringList list);

    bool isDontSave();

private:
    void setItems(QStringList list, QColor color);
    Ui::RevisionDialog *ui;

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);

private:
    bool m_dontSave;
};

#endif // REVISIONDIALOG_H
