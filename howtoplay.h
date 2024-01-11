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

private:
    Ui::HowToPlay *ui;
};

#endif // HOWTOPLAY_H
