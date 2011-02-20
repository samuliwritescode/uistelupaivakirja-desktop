#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace Ui {
    class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();

private:
    Ui::SettingsForm *ui;

private slots:
    void on_clearCSS_clicked();
    void on_checkBoxUseCustomFields_clicked(bool checked);
    void on_customFields_cellChanged(int row, int column);
    void on_checkBoxUseSuggestions_clicked(bool checked);
    void on_openCSSFile_clicked();
    void on_openSaveFolder_clicked();
};

#endif // SETTINGSFORM_H
