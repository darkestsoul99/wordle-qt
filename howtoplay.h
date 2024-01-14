#ifndef HOWTOPLAY_H
#define HOWTOPLAY_H

#include <QWidget>

namespace Ui {
class HowToPlay;
}

class HowToPlay : public QWidget
{
    Q_OBJECT

public:
    explicit HowToPlay(QWidget *parent = 0);
    ~HowToPlay();
    void connectSignalsSlots();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::HowToPlay *ui;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};

#endif // HOWTOPLAY_H
