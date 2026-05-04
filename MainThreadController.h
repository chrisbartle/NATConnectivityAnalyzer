#ifndef MAINTHREADCONTROLLER_H
#define MAINTHREADCONTROLLER_H

#include <QObject>
#include <QThread>
#include "WorkerThreadController.h"

class MainThreadController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString applicationVersion READ getApplicationVersion)
    Q_PROPERTY(QString currentStunServer READ getCurrentStunServer WRITE setCurrentStunServer NOTIFY currentStunServerChanged)
    Q_PROPERTY(bool isProcessingNow READ getIsProcessingNow WRITE setIsProcessingNow NOTIFY isProcessingNowStatusChanged)
    Q_PROPERTY(QString currentProcessingStatus READ getCurrentProcessingStatus WRITE setCurrentProcessingStatus NOTIFY currentProcessingStatusChanged)
    Q_PROPERTY(QString internalIP READ getInternalIP WRITE setInternalIP NOTIFY internalIPChanged)
    Q_PROPERTY(QString externalIP READ getExternalIP WRITE setExternalIP NOTIFY externalIPChanged)
    Q_PROPERTY(QString natType READ getNatType WRITE setNatType NOTIFY natTypeChanged)
    Q_PROPERTY(QString portForwardType READ getPortForwardType WRITE setPortForwardType NOTIFY portForwardTypeChanged)
    Q_PROPERTY(QString natTestLog READ getNatTestLog WRITE setNatTestLog NOTIFY natTestLogChanged)
public:
    MainThreadController();
    ~MainThreadController();

    bool getIsProcessingNow() const;
    void setIsProcessingNow(bool newIsProcessingNow);

    QString getCurrentStunServer() const;
    void setCurrentStunServer(const QString &newCurrentStunServer);

    QString getCurrentProcessingStatus() const;
    void setCurrentProcessingStatus(const QString &newCurrentProcessingStatus);

    QString getExternalIP() const;
    void setExternalIP(const QString &newExternalIP);

    QString getInternalIP() const;
    void setInternalIP(const QString &newInternalIP);

    QString getNatType() const;
    void setNatType(const QString &newNatType);

    QString getApplicationVersion() const;

    QString getNatTestLog() const;
    void setNatTestLog(const QString &newNatTestLog);

    QString getPortForwardType() const;
    void setPortForwardType(const QString &newPortForwardType);

signals:
    void currentStunServerChanged(const QString inStunServer);
    void isProcessingNowStatusChanged(bool inIsProcessing);
    void currentProcessingStatusChanged(const QString inStatus);
    void externalIPChanged(const QString inIP);
    void internalIPChanged(const QString inIP);
    void natTypeChanged();

    void doFullAnalysis();


    void natTestLogChanged();

    void portForwardTypeChanged();

private:
    QThread *m_workerThread = NULL;
    WorkerThreadController *m_workerController = NULL;

    QString m_currentStunServer;
    bool m_isProcessingNow = false;
    QString m_currentProcessingStatus;
    QString m_externalIP;
    QString m_internalIP;
    QString m_natType;
    QString m_natTestLog;
    QString m_portForwardType;
};

#endif // MAINTHREADCONTROLLER_H
