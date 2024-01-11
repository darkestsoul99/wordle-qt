#include "keyboardcontroller.h"
#include <QPushButton>

KeyboardController::KeyboardController(QWidget *parent) : QWidget(parent)
{
    this->signalMapper = new QSignalMapper(this);
    for (int i = 0; i < texts.size(); ++i) {
        connect(button, &QPushButton::clicked, signalMapper, qOverload<>(&QSignalMapper::map));
        signalMapper->setMapping(button, texts[i]);
    }

    connect(signalMapper, &QSignalMapper::mappedString,
            this, &ButtonWidget::clicked);
}

void KeyboardController::handleKeyPress(int key)  {
    // Handle the key press here
    QChar character(static_cast<char>(key));
    // You can use 'character' as the pressed key
    qDebug() << "Key pressed: " << character;
}
