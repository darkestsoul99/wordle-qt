#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringMatcher>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->allocateObjects();
    this->getWorkFromNetwork();
    this->mapKeyboard();
    this->mapIndexes();
    this->connectSignalsSlots();
}

MainWindow::~MainWindow()
{
    foreach(QObject *object, this->children()) {
        object->deleteLater();
    }

    delete ui;
}

void MainWindow::allocateObjects() {
    ui->setupUi(this);
    this->settings = new Settings();
    this->howToPlay = new HowToPlay();
    this->keyboardMapper = new QSignalMapper();
    this->indexMapper = new QMap<int, QLabel*>;
}

void MainWindow::showSettingsPage() {
    settings->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    settings->show();
}

void MainWindow::showHowToPlayPage() {
    howToPlay->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    howToPlay->show();
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
    QLabel* currentLabel;

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
        currentLabel = indexMapper->value(currentIndex);
        currentLabel->setText("");
    } else {
        if (currentWord.length() < 5) {
            if (currentIndex > 30 || currentIndex <= 0) {
                currentIndex = 1;
            }
            currentWord.append(keyboardInput);
            currentLabel = indexMapper->value(currentIndex);
            currentLabel->setText(keyboardInput);
            currentIndex++;
            qDebug() << "Button clicked : " << keyboardInput;
            qDebug() << "current index : " << currentLabel->objectName();
            qDebug() << "current word : " << currentWord;
        }
    }
}

void MainWindow::handleEnteredWord() {
    int correctCount = 0;
    QSet<QChar> notifiedCharacters;

    for (int i = 0; i < currentWord.length(); ++i) {
        QChar character = currentWord.at(i);

        if (wordOfTheDay.contains(character)) {
            if (currentWord.at(i) == wordOfTheDay.at(i)) {
                qDebug() << "Correct character at position " << i << "!";
                correctCount++;
            } else if (!notifiedCharacters.contains(character)) {
                qDebug() << "Correct character, but at a different position!";
                notifiedCharacters.insert(character);
            }
        } else {
            qDebug() << "Incorrect character!";
        }
    }

    qDebug() << "Number of correct characters in the right position: " << correctCount;
    if (correctCount == 5) {
        //emit signal;
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

void MainWindow::paintEvent(QPaintEvent * event) {
    QPainter painter (this);

    QWidget::paintEvent(event);
}

void MainWindow::connectSignalsSlots() {
    connect(this->ui->playButton, SIGNAL(released()), this, SLOT(handleMenuButtons()));
    connect(this->ui->howToPlayButton, SIGNAL(released()), this, SLOT(handleMenuButtons()));
    connect(this->ui->settingsButton, SIGNAL(released()), this, SLOT(showSettingsPage()));
    connect(this->ui->helpButton, SIGNAL(released()), this, SLOT(showHowToPlayPage()));
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
        QLabel *wordBox =qobject_cast<QLabel*>(child);
        if (wordBox) {
            QString labelName = wordBox->objectName();  // Assuming labels have names like "index1", "index2", etc.
            int index = labelName.mid(5).toInt();  // Extract numerical part and convert to int
            indexMapper->insert(index, wordBox);  // Use insert to add the pair to the QMap
            qDebug() << "Index of QLabel" << wordBox << "is : " << index;
        }
    }
}

void MainWindow::getWorkFromNetwork() {
    // Create a QNetworkAccessManager instance
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this);

    // Create a QNetworkRequest for the desired URL
    QNetworkRequest request(QUrl("https://random-word-api.herokuapp.com/word?length=5"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    QSslConfiguration sslConfig = request.sslConfiguration();
    // Set SSL-related configurations if needed
    // sslConfig.setCaCertificates(...);
    // sslConfig.setLocalCertificate(...);
    // sslConfig.setPrivateKey(...);
    request.setSslConfiguration(sslConfig);
    // Perform the GET request
    QNetworkReply *reply = networkAccessManager->get(request);

    // Connect the signal for when the request is finished
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Read the response data
            QByteArray responseData = reply->readAll();

            // Convert the response data to a QString (assuming it's JSON)
            QString responseString = QString::fromUtf8(responseData);

            // Print the response
            qDebug() << responseString;

            // Process the JSON response as needed
            // ...

        } else {
            // Handle the error
            qDebug() << "Error: " << reply->errorString();
        }

        // Clean up
        reply->deleteLater();
        networkAccessManager->deleteLater();
    });
}
