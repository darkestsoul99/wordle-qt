#include "settingspopupwidget.h"
#include "ui_settingspopupwidget.h"

SettingsPopUpWidget::SettingsPopUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPopUpWidget)
{
    ui->setupUi(this);
}

SettingsPopUpWidget::~SettingsPopUpWidget()
{
    delete ui;
}
