#include <QObject>

class SwitchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state WRITE setState NOTIFY stateChanged)

public:
    explicit SwitchController(QObject *parent = nullptr) : QObject(parent), m_state(false) {}

    bool state() const {
        return m_state;
    }

public slots:
    void setState(bool newState) {
        if (m_state != newState) {
            m_state = newState;
            emit stateChanged(m_state);
        }
    }

signals:
    void stateChanged(bool newState);

private:
    bool m_state;
};
