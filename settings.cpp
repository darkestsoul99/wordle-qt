#include "settings.h"
#include "ui_settings.h"
#include <QPainter>
#include <QQuickItem>
#include <QQuickWidget>
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    connectSignalsSlots();
    setWidgetAttributes();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::connectSignalsSlots() {
    connect(this->ui->exitButton, SIGNAL(released()), this, SLOT(hide()));
    connect(this->ui->darkThemeWidget->rootObject(), SIGNAL(darkModeSignal(bool)),
            this, SIGNAL(darkThemeSignal(bool)));
    connect(this->ui->highContrastWidget->rootObject(), SIGNAL(highContrastModeSignal(bool)),
            this, SIGNAL(highContrastModeSignal(bool)));
    connect(this->ui->onscreenInputOnlyWidget->rootObject(), SIGNAL(keyboardOnlySignal(bool)),
            this, SIGNAL(keyboardOnlySignal(bool)));
}


void Settings::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void Settings::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void Settings::paintEvent(QPaintEvent * event) {
    QPainter painter (this);
    painter.setRenderHint (QPainter :: Antialiasing); // Anti-aliasing;
    painter.setBrush (QBrush (Qt::BrushStyle::NoBrush));
    painter.setPen (QPen(Qt::black, 10));
    QRect rect = this-> rect ();
    rect.setWidth (rect.width ()-1);
    rect.setHeight (rect.height ()-1);
    painter.drawRoundedRect (rect, 15, 15);
    QWidget::paintEvent(event);
}

void Settings::toggleDarkMode(bool isDark) {
    setProperty("darkMode", isDark);
    style()->unpolish(this);
    style()->polish(this);
    foreach(QLabel *label, findChildren<QLabel *>()) {
        label->style()->unpolish(label);
        label->style()->polish(label);
    }

    foreach(QQuickWidget *quickWidget, findChildren<QQuickWidget *>()) {
        qDebug() << quickWidget->objectName();
        quickWidget->rootObject()->setProperty("backgroundColor", isDark ? "#333247" : "#D9DDDC");
        quickWidget->style()->unpolish(quickWidget);
        quickWidget->style()->polish(quickWidget);
    }
}

void Settings::setWidgetAttributes() {
    foreach(QQuickWidget *quickWidget, findChildren<QQuickWidget *>()) {
        quickWidget->rootObject()->setProperty("objectName", quickWidget->objectName());
        quickWidget->setAttribute(Qt::WA_TranslucentBackground);
        quickWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
        quickWidget->setClearColor(Qt::transparent);
    }
}
