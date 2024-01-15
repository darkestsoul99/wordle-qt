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
}

Settings::~Settings()
{
    delete ui;
}

void Settings::connectSignalsSlots() {
    connect(this->ui->exitButton, SIGNAL(released()), this, SLOT(hide()));
    QQuickItem *rootObject = this->ui->darkThemeWidget->rootObject();
    connect(rootObject, SIGNAL(darkThemeSignal()), this, SIGNAL(darkThemeSignal())); // Will be added to other switches in future release
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
