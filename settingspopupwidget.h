#ifndef SETTINGSPOPUPWIDGET_H
#define SETTINGSPOPUPWIDGET_H

#include <QWidget>

namespace Ui {
class SettingsPopUpWidget;
}

class SettingsPopUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPopUpWidget(QWidget *parent = 0);
    ~SettingsPopUpWidget();

private:
    Ui::SettingsPopUpWidget *ui;
};

#endif // SETTINGSPOPUPWIDGET_H
