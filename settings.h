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
    void toggleDarkMode(bool isDark);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
signals:
    void darkThemeSignal(bool);
    void highContrastModeSignal(bool);
    void keyboardOnlySignal(bool);
private:
    Ui::Settings *ui;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    void setWidgetAttributes();
    void connectSignalsSlots();
};

#endif // SETTINGS_H
