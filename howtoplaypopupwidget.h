#ifndef HOWTOPLAYPOPUPWIDGET_H
#define HOWTOPLAYPOPUPWIDGET_H

#include <QWidget>

namespace Ui {
class HowToPlayPopUpWidget;
}

class HowToPlayPopUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HowToPlayPopUpWidget(QWidget *parent = 0);
    ~HowToPlayPopUpWidget();

private:
    Ui::HowToPlayPopUpWidget *ui;
};

#endif // HOWTOPLAYPOPUPWIDGET_H
