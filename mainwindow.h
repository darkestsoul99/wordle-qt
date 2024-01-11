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
    void connectSignalsSlots();
    void mapKeyboard();
    void mapIndexes();
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);
public slots:
    void handleMenuButtons();
    void showSettingsPage();
    void showHowToPlayPage();
    void handleKeyboardButtonClick(const QString &keyboardInput);
    void handleEnteredWord(const QString &enteredWord);
signals:
    void keyPressEventSignal(const QString);
    void checkWordSignal(const QString);
private:
    Ui::MainWindow *ui;
    Settings *settings;
    HowToPlay *howToPlay;
    QSignalMapper *keyboardMapper;
    QMap<int, QLabel*> *indexMapper;
    int currentIndex = 1;
    QString currentWord;
};

#endif // MAINWINDOW_H
