#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include <QWidget>
#include <QSignalMapper>

class KeyboardController : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardController(QWidget *parent = nullptr);
public slots:
    void handleKeyPress(int key);
private:
    QSignalMapper *signalMapper;
};

#endif // KEYBOARDCONTROLLER_H
