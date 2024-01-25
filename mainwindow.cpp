#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringMatcher>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPropertyAnimation>

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
    this->indexMapper = new QMap<int, QLabel*>;
}

void MainWindow::clearGame() {
    foreach (QLabel *wordBox, this->indexMapper->values()) {
        wordBox->setText("");
        wordBox->setStyleSheet("");
        wordBox->setProperty("green", false);
        wordBox->setProperty("yellow", false);
    }

    foreach(QObject *child, this->ui->keyboardWidget->children()) {
        QPushButton *key =qobject_cast<QPushButton*>(child);
        if (key) {
            key->setStyleSheet("");
        }
    }

    currentIndex = 0;
    this->getWordFromNetwork();
}

void MainWindow::showSettingsPage() {
    settings->setWindowFlags(Qt::FramelessWindowHint);
    settings->show();
}

void MainWindow::showHowToPlayPage() {
    howToPlay->setWindowFlags(Qt::FramelessWindowHint);
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
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
            } else if (clickedButton == ui->howToPlayButton) {
                ui->stackedWidget->setCurrentWidget(ui->playWidget);
                showHowToPlayPage();
            }
        }
    }
}

void MainWindow::handleKeyboardButtonClick(const QString &keyboardInput) {
    QLabel* currentLabel;

    if (QString::compare(keyboardInput.toUpper(), "ENTER") == 0 ||
            QString::compare(keyboardInput, "\r") == 0) {
        if (currentWord.length() % 5 == 0) {
            handleEnteredWord();
            numberOfDeletedIndexes = 0;
        }
    } else if (QString::compare(keyboardInput.toUpper(), "BACK") == 0 ||
               QString::compare(keyboardInput, "\b") == 0) {
        if (numberOfDeletedIndexes <= 5 && numberOfDeletedIndexes > 0) {
            currentWord.chop(1);
            currentIndex--;
            numberOfDeletedIndexes--;
            currentLabel = indexMapper->value(currentIndex);
            currentLabel->setText("");
        }
        if (currentIndex > 30 || currentIndex <= 0) {
            currentIndex = 1;
        }
    } else {
        if (currentWord.length() < 5) {
            if (currentIndex > 30 || currentIndex <= 0) {
                currentIndex = 1;
            }
            currentWord.append(keyboardInput);
            currentLabel = indexMapper->value(currentIndex);
            currentLabel->setText(keyboardInput);
            numberOfDeletedIndexes++;
            currentIndex++;
        }
    }
}

void MainWindow::handleEnteredWord() {
    int correctCount = 0;
    QSet<QChar> notifiedCharacters;
    int startofIndex = currentIndex - 5;
    for (int i = 0; i < currentWord.length(); ++i) {
        QChar character = currentWord.at(i);
        QPushButton *button = ui->keyboardWidget->findChild<QPushButton *>(QString(character) + "KeyButton", Qt::FindDirectChildrenOnly);
        if (wordOfTheDay.contains(character)) {
            if (currentWord.at(i) == wordOfTheDay.at(i)) {
                indexMapper->value(startofIndex)->setStyleSheet("background: rgb(83, 141, 78);");
                indexMapper->value(startofIndex)->setProperty("green", true);
                button->setStyleSheet("background: rgb(83, 141, 78);");
                button->setProperty("green", true);
                correctCount++;
            } else if (!notifiedCharacters.contains(character)) {
                indexMapper->value(startofIndex)->setStyleSheet("background: rgb(181, 159, 59);");
                indexMapper->value(startofIndex)->setProperty("yellow", true);
                button->setStyleSheet("background: rgb(181, 159, 59);");
                button->setProperty("yellow", true);
                notifiedCharacters.insert(character);
            }
        } else {
            indexMapper->value(startofIndex)->setStyleSheet("background: rgb(120, 124, 126);");
            button->setStyleSheet("background: rgb(120, 124, 126);");
        }
        shakeWidget(indexMapper->value(startofIndex));
        startofIndex++;
    }

    if (correctCount == 5) {
        emit showResultsDialogSignal();
        this->resultsDialog->setText("Congratulations ! \nYour answer was correct.");
    }
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
    connect(keyboardMapper, SIGNAL(mapped(QString)), this, SLOT(handleKeyboardButtonClick(QString)));
    connect(this, SIGNAL(keyPressEventSignal(QString)), this, SLOT(handleKeyboardButtonClick(QString)));
    connect(this->settings, SIGNAL(darkThemeSignal(bool)), this, SLOT(toggleDarkMode(bool)));
    connect(this->settings, SIGNAL(highContrastModeSignal(bool)), this, SLOT(toggleHighContrastMode(bool)));
    connect(this->settings, SIGNAL(keyboardOnlySignal(bool)), this, SLOT(toggleKeyboardOnlyMode(bool)));
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
        return;
    }

    foreach(QObject* child, this->ui->wordsWidget->children()) {
        QLabel *wordBox =qobject_cast<QLabel*>(child);
        if (wordBox) {
            QString labelName = wordBox->objectName();  // Assuming labels have names like "index1", "index2", etc.
            int index = labelName.mid(5).toInt();  // Extract numerical part and convert to int
            indexMapper->insert(index, wordBox);  // Use insert to add the pair to the QMap
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
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        networkAccessManager->deleteLater();
    });
}

void MainWindow::shakeWidget(QWidget *widget) {
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(100);  // Adjust the duration as needed
    animation->setLoopCount(2);    // Number of times to repeat the animation

    animation->setKeyValueAt(0, widget->pos());
    animation->setKeyValueAt(0.1, widget->pos() + QPoint(5, 0));
    animation->setKeyValueAt(0.2, widget->pos() - QPoint(5, 0));
    animation->setKeyValueAt(0.3, widget->pos() + QPoint(5, 0));
    animation->setKeyValueAt(0.4, widget->pos() - QPoint(5, 0));
    animation->setKeyValueAt(0.5, widget->pos() + QPoint(5, 0));
    animation->setKeyValueAt(0.6, widget->pos() - QPoint(5, 0));
    animation->setKeyValueAt(0.7, widget->pos() + QPoint(5, 0));
    animation->setKeyValueAt(0.8, widget->pos() - QPoint(5, 0));
    animation->setKeyValueAt(0.9, widget->pos() + QPoint(5, 0));
    animation->setKeyValueAt(1, widget->pos());

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::toggleHardMode(const bool isHardMode) {
    return;
}

void MainWindow::toggleDarkMode(const bool isDark) {
    ui->centralWidget->setProperty("darkMode", isDark);
    settings->toggleDarkMode(isDark);
    howToPlay->toggleDarkMode(isDark);
    resultsDialog->toggleDarkMode(isDark);
    foreach (auto widget, findChildren<QWidget *>()) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
}


void MainWindow::toggleHighContrastMode(const bool isHighContrast) {
    ui->wordsWidget->setProperty("highContrastMode", isHighContrast);
    foreach (auto widget, ui->centralWidget->findChildren<QWidget *>()) {
        if (isHighContrast) {
            if (widget->property("green") == true) {
                widget->setStyleSheet("background-color: rgb(245, 121, 58)");
            }
            else if (widget->property("yellow") == true) {
                widget->setStyleSheet("background-color: rgb(133, 192, 249)");
            }
        }
        else {
            if (widget->property("green") == true) {
                widget->setStyleSheet("background: rgb(83, 141, 78);");
            }
            else if (widget->property("yellow") == true) {
                widget->setStyleSheet("background: rgb(181, 159, 59);");
            }
        }
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
}

void MainWindow::toggleKeyboardOnlyMode(const bool isKeyboardOnly) {
    isKeyboardOnly ? disconnect(this, SIGNAL(keyPressEventSignal(QString)), this, SLOT(handleKeyboardButtonClick(QString))) :
                     connect(this, SIGNAL(keyPressEventSignal(QString)), this, SLOT(handleKeyboardButtonClick(QString)));                     ;
}
