#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>

namespace Ui {
class ResultsDialog;
}

class ResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsDialog(QWidget *parent = nullptr);
    ~ResultsDialog();
    void setText(QString infoText);
public slots:
    void tryButtonClickedSlot();
private:
    Ui::ResultsDialog *ui;
signals:
    void tryButtonClickedSignal();
};

#endif // RESULTSDIALOG_H
