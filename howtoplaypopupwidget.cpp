#include "howtoplaypopupwidget.h"
#include "ui_howtoplaypopupwidget.h"

HowToPlayPopUpWidget::HowToPlayPopUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HowToPlayPopUpWidget)
{
    ui->setupUi(this);
}

HowToPlayPopUpWidget::~HowToPlayPopUpWidget()
{
    delete ui;
}
