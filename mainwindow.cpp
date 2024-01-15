#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringMatcher>
#include <QQuickWidget>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QException>
#include <QQuickItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->allocateObjects();
    this->getWordFromNetwork();
    this->mapKeyboard();
    this->mapIndexes();
    this->connectSignalsSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::allocateObjects() {
    ui->setupUi(this);
    this->settings = new Settings();
    this->howToPlay = new HowToPlay();
    this->resultsDialog = new ResultsDialog();
    this->keyboardMapper = new QSignalMapper();
    this->indexMapper = new QMap<int, QQuickWidget*>;
}

void MainWindow::clearGame() {
    foreach (QQuickWidget *wordBox, this->indexMapper->values()) {
        //wordBox->setText("");
    }

    currentIndex = 0;
    this->getWordFromNetwork();
}

void MainWindow::showSettingsPage() {
    settings->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    settings->show();
}

void MainWindow::showHowToPlayPage() {
    howToPlay->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    howToPlay->show();
}

void MainWindow::showResultsDialog() {
    resultsDialog->show();
}

void MainWindow::handleMenuButtons() {
    QObject* senderObj = QObject::sender();
    if(senderObj) {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(senderObj);
        if(clickedButton) {
            if(clickedButton == ui->playButton) {
                ui->mainExitButton->hide();
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
            } else if (clickedButton == ui->howToPlayButton) {
                ui->mainExitButton->hide();
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
                showHowToPlayPage();
            }
        }
    }
}

void MainWindow::handleKeyboardButtonClick(const QString &keyboardInput) {
    QQuickWidget* currentWidget;

    if (QString::compare(keyboardInput.toUpper(), "ENTER") == 0 ||
            QString::compare(keyboardInput, "\r") == 0) {
        if (currentWord.length() % 5 == 0) {
            handleEnteredWord();
        }
    } else if (QString::compare(keyboardInput.toUpper(), "BACK") == 0 ||
               QString::compare(keyboardInput, "\b") == 0) {
        currentWord.chop(1);
        currentIndex--;
        if (currentIndex > 30 || currentIndex <= 0) {
            currentIndex = 1;
        }
        currentWidget = indexMapper->value(currentIndex);
        emit textChangedSignal("", currentIndex);
    } else {
        if (currentWord.length() < 5) {
            if (currentIndex > 30 || currentIndex <= 0) {
                currentIndex = 1;
            }
            currentWord.append(keyboardInput);
            currentWidget = indexMapper->value(currentIndex);
            emit textChangedSignal(keyboardInput, currentIndex);
            currentIndex++;
            qDebug() << "Button clicked : " << keyboardInput;
            qDebug() << "current index : " << currentWidget->objectName();
            qDebug() << "current word : " << currentWord;
        }
    }
}

void MainWindow::handleEnteredWord() {
    int correctCount = 0;
    int iterator = currentIndex - 5;
    QSet<QChar> notifiedCharacters;

    for (int i = 0; i < currentWord.length(); ++i) {
        QChar character = currentWord.at(i);

        if (wordOfTheDay.contains(character)) {
            if (currentWord.at(i) == wordOfTheDay.at(i)) {
                qDebug() << "Correct character at position " << i << "!";
                this->indexMapper->value(iterator)->setProperty("state", "correct");
                correctCount++;
            } else if (!notifiedCharacters.contains(character)) {
                qDebug() << "Correct character, but at a different position!";
                notifiedCharacters.insert(character);
                this->indexMapper->value(iterator)->setProperty("state", "partiallyCorrect");
            }
        } else {
            qDebug() << "Incorrect character!";
            this->indexMapper->value(iterator)->setProperty("state", "incorrect");
        }
    }

    qDebug() << "Number of correct characters in the right position: " << correctCount;
    if (correctCount == 5) {
        emit showResultsDialogSignal();
        this->resultsDialog->setText("Congratulations ! \nYour answer was correct.");
    }
    qDebug() << currentIndex;
    if (currentIndex > 30) {
        emit showResultsDialogSignal();
        this->resultsDialog->setText("You couldn't find the right answer. \nPlease try again.");
    }
    currentWord = "";
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->ui->stackedWidget->currentWidget() == ui->playWidget) {
        emit keyPressEventSignal(event->text().toUpper());
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::connectSignalsSlots() {
    connect(this->ui->playButton, SIGNAL(released()), this, SLOT(handleMenuButtons()));
    connect(this->ui->howToPlayButton, SIGNAL(released()), this, SLOT(handleMenuButtons()));
    connect(this->ui->settingsButton, SIGNAL(released()), this, SLOT(showSettingsPage()));
    connect(this->ui->helpButton, SIGNAL(released()), this, SLOT(showHowToPlayPage()));
    connect(this, SIGNAL(showResultsDialogSignal()), this, SLOT(showResultsDialog()));
    connect(resultsDialog, SIGNAL(tryButtonClickedSignal()), this, SLOT(clearGame()));
    connect(this->ui->exitButton, SIGNAL(released()), this, SLOT(close()));
    connect(this->ui->mainExitButton, SIGNAL(released()), this, SLOT(close()));
    connect(keyboardMapper, SIGNAL(mapped(QString)), this, SLOT(handleKeyboardButtonClick(QString)));
    connect(this, SIGNAL(keyPressEventSignal(QString)), this, SLOT(handleKeyboardButtonClick(QString)));
}

void MainWindow::mapKeyboard() {
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
    if(!gridLayout) {
        qDebug() << "Layout is not QGridLayout!";
        return;
    }

    foreach(QObject* child, this->ui->wordsWidget->children()) {
        QQuickWidget *wordBox =qobject_cast<QQuickWidget*>(child);
        if (wordBox) {
            wordBox->setSource(QUrl("qrc:/qml/Wordbox.qml"));
            QQuickItem *rootObject = wordBox->rootObject();

            QObject::connect(this, SIGNAL(textChangedSignal(QString, int)),
                             rootObject, SIGNAL(textChangedSignal(QString, int)));
            QString widgetName = wordBox->objectName();  // Assuming labels have names like "index1", "index2", etc.
            int index = widgetName.mid(5).toInt();  // Extract numerical part and convert to int
            indexMapper->insert(index, wordBox);  // Use insert to add the pair to the QMap
            qDebug() << "Index of QLabel" << wordBox << "is : " << index;
        }
    }
}

void MainWindow::getWordFromNetwork() {
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://random-word-api.herokuapp.com/word?length=5"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *reply = networkAccessManager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonArray jsonArray = jsonDoc.array();
            if (!jsonArray.isEmpty() && jsonArray.at(0).isString()) {
                QString word = jsonArray.at(0).toString();
                wordOfTheDay = word.toUpper();
                qDebug() << "Word: " << word;
            } else {
                qDebug() << "Invalid JSON array or no string element found.";
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
        networkAccessManager->deleteLater();
    });
}
