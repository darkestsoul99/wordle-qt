#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QLabel>
#include "settings.h"
#include "howtoplay.h"
#include "resultsdialog.h"

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
    void getWordFromNetwork();
    void shakeWidget(QWidget *widget);
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public slots:
    void handleMenuButtons();
    void showSettingsPage();
    void showResultsDialog();
    void showHowToPlayPage();
    void handleKeyboardButtonClick(const QString &keyboardInput);
    void clearGame();
    void toggleDarkMode();
    void toggleHighContrastMode();
    void darkModeColors(QWidget *widget);
    void resetColors(QWidget *widget);
signals:
    void keyPressEventSignal(const QString);
    void showResultsDialogSignal();
private:
    Ui::MainWindow *ui;
    Settings *settings;
    HowToPlay *howToPlay;
    ResultsDialog *resultsDialog;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    QSignalMapper *keyboardMapper;
    QMap<int, QLabel*> *indexMapper;
    int currentIndex = 1;
    QString currentWord;
    QString wordOfTheDay = "BRIEF";
    int numberOfDeletedIndexes = 0;
    bool darkModeEnabled = false;
    bool highContrastEnabled = false;
};

#endif // MAINWINDOW_H
