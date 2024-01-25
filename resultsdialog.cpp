#include "resultsdialog.h"
#include "ui_resultsdialog.h"

ResultsDialog::ResultsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultsDialog)
{
    ui->setupUi(this);
    connect(this->ui->tryButton, SIGNAL(released()), this, SLOT(tryButtonClickedSlot()));
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}

void ResultsDialog::tryButtonClickedSlot() {
    emit tryButtonClickedSignal();
    this->hide();
}

void ResultsDialog::setText(QString infoText) {
    this->ui->informationLabel->setText(infoText);
    this->ui->informationLabel->setAlignment(Qt::AlignCenter);
}

void ResultsDialog::toggleDarkMode(bool isDark) {
    setProperty("darkMode", isDark);
    foreach(QWidget *widget, findChildren<QWidget *>()) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
}
