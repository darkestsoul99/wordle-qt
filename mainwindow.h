#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QLabel>
#include "settings.h"
#include "howtoplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void connectSignalsSlots();
    void mapKeyboard();
    void mapIndexes();
    void handleEnteredWord();
    void allocateObjects();
    void getWorkFromNetwork();
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:
    void handleMenuButtons();
    void showSettingsPage();
    void showHowToPlayPage();
    void handleKeyboardButtonClick(const QString &keyboardInput);
signals:
    void keyPressEventSignal(const QString);
private:
    Ui::MainWindow *ui;
    Settings *settings;
    HowToPlay *howToPlay;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    QSignalMapper *keyboardMapper;
    QMap<int, QLabel*> *indexMapper;
    int currentIndex = 1;
    QString currentWord;
    QString wordOfTheDay = "BRIEF";
};

#endif // MAINWINDOW_H
