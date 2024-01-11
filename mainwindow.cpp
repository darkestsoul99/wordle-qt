#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mapKeyboard();
    this->mapIndexes();
    this->connectSignalsSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettingsPage() {
    this->settings = new Settings();
    settings->show();
}

void MainWindow::showHowToPlayPage() {
    this->howToPlay = new HowToPlay();
    howToPlay->show();
}

void MainWindow::handleMenuButtons() {
    QObject* senderObj = QObject::sender();
    if(senderObj) {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(senderObj);
        if(clickedButton) {
            if(clickedButton == ui->playButton) {
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
            } else if (clickedButton == ui->howToPlayButton) {
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
                showHowToPlayPage();
            }
        }
    }
}

void MainWindow::handleKeyboardButtonClick(const QString &keyboardInput) {
    // TODO CURRENT LABEL SOLVE PROBLEM
    if (currentIndex > 30 || currentIndex <= 0) {
        currentIndex = 1;
    }
    QLabel* currentLabel = indexMapper->value(currentIndex);

    if (QString::compare(keyboardInput.toUpper(), "ENTER") == 0 ||
            QString::compare(keyboardInput, "\r") == 0) {
        if (currentIndex % 5 == 0) {
            emit checkWordSignal(currentWord);
            currentWord = "";
        }
    } else if (QString::compare(keyboardInput.toUpper(), "BACK") == 0 ||
               QString::compare(keyboardInput, "\b") == 0) {
        currentWord.chop(1);
        currentIndex--;
        currentLabel->setText("");
    } else {
        if (currentWord.length() <= 5) {
            currentWord.append(keyboardInput);
            currentLabel->setText(keyboardInput);
            currentIndex++;
        }
        qDebug() << "Button clicked : " << keyboardInput;
        qDebug() << "current index : " << currentLabel->objectName();
        qDebug() << "current word : " << currentWord;
    }
}

void MainWindow::handleEnteredWord(const QString &enteredWord) {
    qDebug() << "Entered Word :" << enteredWord;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->ui->stackedWidget->currentWidget() == ui->playWidget) {
        emit keyPressEventSignal(event->text().toUpper());
    }
}

void MainWindow::connectSignalsSlots() {
    connect(this->ui->playButton, SIGNAL(released()), this, SLOT(handleMenuButtons()));
    connect(this->ui->howToPlayButton, SIGNAL(released()), this, SLOT(handleMenuButtons()));
    connect(this->ui->settingsButton, SIGNAL(released()), this, SLOT(showSettingsPage()));
    connect(this->ui->howToPlayButton, SIGNAL(released()), this, SLOT(showHowToPlayPage()));
    connect(this->ui->helpButton, SIGNAL(released()), this, SLOT(showHowToPlayPage()));
    connect(keyboardMapper, SIGNAL(mapped(QString)), this, SLOT(handleKeyboardButtonClick(QString)));
    connect(this, SIGNAL(keyPressEventSignal(QString)), this, SLOT(handleKeyboardButtonClick(QString)));
    connect(this, SIGNAL(checkWordSignal(QString)), this, SLOT(handleEnteredWord(QString)));
}

void MainWindow::mapKeyboard() {
    this->keyboardMapper = new QSignalMapper();
    foreach (QObject *child, this->ui->keyboardWidget->children()) {
        QPushButton *button = qobject_cast<QPushButton*>(child);
        if(button) {
            connect(button, SIGNAL(clicked()), keyboardMapper, SLOT(map()));
            keyboardMapper->setMapping(button, button->text());
        }
    }
}

void MainWindow::mapIndexes() {
    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(this->ui->wordsWidget->layout());
    this->indexMapper = new QMap<int, QLabel*>;
    if(!gridLayout) {
        qDebug() << "Layout is not QGridLayout!";
        return;
    }

    foreach(QObject* child, this->ui->wordsWidget->children()) {
        QLabel *wordBox =qobject_cast<QLabel*>(child);
        if (wordBox) {
            QString labelName = wordBox->objectName();  // Assuming labels have names like "index1", "index2", etc.
            int index = labelName.mid(5).toInt();  // Extract numerical part and convert to int
            indexMapper->insert(index, wordBox);  // Use insert to add the pair to the QMap
            qDebug() << "Index of QLabel" << wordBox << "is : " << index;
        }
    }
}
