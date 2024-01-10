#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->connectSignalsSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignalsSlots() {
    connect(this->ui->playButton, SIGNAL(released()), this, SLOT(handleButtonPress()));
    connect(this->ui->loginButton, SIGNAL(released()), this, SLOT(handleButtonPress()));
    connect(this->ui->howToPlayButton, SIGNAL(released()), this, SLOT(handleButtonPress()));
    connect(this->ui->settingsButton, SIGNAL(released()), this, SLOT(openSettingsPage()));
}

void MainWindow::handleButtonPress() {
    QObject* senderObj = QObject::sender();
    if(senderObj) {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(senderObj);
        if(clickedButton) {
            if(clickedButton == ui->playButton) {
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
            } else if (clickedButton == ui->loginButton) {
                ui->stackedWidget->setCurrentWidget(ui->loginWidget);
            } else if (clickedButton == ui->howToPlayButton) {
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
            }
        }
    }
}

void MainWindow::openSettingsPage() {
    this->settings = new Settings();
    settings->show();
}

