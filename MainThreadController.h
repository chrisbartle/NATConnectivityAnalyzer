#ifndef MAINTHREADCONTROLLER_H
#define MAINTHREADCONTROLLER_H

#include <QObject>
#include <Qthread>

class MainThreadController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentStunServer READ getCurrentStunServer WRITE setCurrentStunServer NOTIFY currentStunServerChanged)
public:
    MainThreadController();

    //currentStunServer
    QString getCurrentStunServer() const {return m_currentStunServer;};
    void setCurrentStunServer(const QString inStunServer)
    {
        if (m_currentStunServer != inStunServer)
        {
            m_currentStunServer = inStunServer;
            emit currentStunServerChanged(inStunServer);
        }
    };

signals:
    void currentStunServerChanged(const QString inStunServer);
    void doFullAnalysis();

private:
    QString m_currentStunServer;
};

#endif // MAINTHREADCONTROLLER_H
