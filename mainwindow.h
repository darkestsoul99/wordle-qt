#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectSignalsSlots();
    ~MainWindow();
public slots:
    void handleButtonPress();
private:
    Ui::MainWindow *ui;
    Settings *settings;

signals:
    void pushButtonSignal();
};

#endif // MAINWINDOW_H
