#include "howtoplay.h"
#include "ui_howtoplay.h"
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QDebug>

HowToPlay::HowToPlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HowToPlay)
{
    ui->setupUi(this);
    connectSignalsSlots();
}

HowToPlay::~HowToPlay()
{
    delete ui;
}

void HowToPlay::connectSignalsSlots() {
    connect(this->ui->exitButton, SIGNAL(released()), this, SLOT(hide()));
}

void HowToPlay::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void HowToPlay::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void HowToPlay::paintEvent(QPaintEvent * event) {
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

void HowToPlay::toggleDarkMode(bool isDark) {
    setProperty("darkMode", isDark);
    style()->unpolish(this);
    style()->polish(this);
    foreach(QWidget *widget, findChildren<QLabel *>()) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
}
