#ifndef WORKERTHREADCONTROLLER_H
#define WORKERTHREADCONTROLLER_H

#include <QObject>
#include "Stunner/StunClientHelper.h"

class WorkerThreadController : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThreadController(QObject *parent = nullptr);

    void ChangeStunServer(const QString inStunServer) { m_stunServer = inStunServer; }

public slots:
    void DoNATAnalysis();

signals:
    void setIsProcessingNow(bool newIsProcessingNow);
    void setCurrentProcessingStatus(const QString &newCurrentProcessingStatus);
    void setExternalIP(const QString inIP);
    void setInternalIP(const QString inIP);
    void setNatType(const QString &newNatType);

private:
    QString m_stunServer;
};

#endif // WORKERTHREADCONTROLLER_H
