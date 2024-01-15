#ifndef WORDBOX_H
#define WORDBOX_H

#include <qquickwidget.h>
#include <QWidget>

class WordBox : public QQuickWidget
{
    Q_OBJECT
public:
    explicit SpinnerWidget(QWidget *parent = nullptr);
    void setSpinnerSource(QString);

signals:
    void setSourceSignal(QString);
};

#endif // WORDBOX_H
