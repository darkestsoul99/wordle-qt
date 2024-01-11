#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QQuickWidget>
namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
