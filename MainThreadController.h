#ifndef MAINTHREADCONTROLLER_H
#define MAINTHREADCONTROLLER_H

#include <QObject>
#include <Qthread>
#include "WorkerThreadController.h"
#include "Stunner/StunClientHelper.h"

class MainThreadController : public QObject
{
    Q_OBJECT
    Q_ENUM(NAT_TYPE)
    Q_PROPERTY(QString currentStunServer READ getCurrentStunServer WRITE setCurrentStunServer NOTIFY currentStunServerChanged)
    Q_PROPERTY(bool isProcessingNow READ getIsProcessingNow WRITE setIsProcessingNow NOTIFY isProcessingNowStatusChanged)
    Q_PROPERTY(QString currentProcessingStatus READ getCurrentProcessingStatus WRITE setCurrentProcessingStatus NOTIFY currentProcessingStatusChanged)
    Q_PROPERTY(QString internalIP READ getInternalIP WRITE setInternalIP NOTIFY internalIPChanged)
    Q_PROPERTY(QString externalIP READ getExternalIP WRITE setExternalIP NOTIFY externalIPChanged)
    Q_PROPERTY(NAT_TYPE natType READ getNatType WRITE setNatType NOTIFY natTypeChanged)
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

    NAT_TYPE getNatType() const;
    void setNatType(const NAT_TYPE &newNatType);

signals:
    void currentStunServerChanged(const QString inStunServer);
    void isProcessingNowStatusChanged(bool inIsProcessing);
    void currentProcessingStatusChanged(const QString inStatus);
    void externalIPChanged(const QString inIP);
    void internalIPChanged(const QString inIP);

    void doFullAnalysis();

    void natTypeChanged();

private:
    QThread *m_workerThread = NULL;
    WorkerThreadController *m_workerController = NULL;

    QString m_currentStunServer;
    bool m_isProcessingNow = false;
    QString m_currentProcessingStatus;
    QString m_externalIP;
    QString m_internalIP;
    NAT_TYPE m_natType;
};

#endif // MAINTHREADCONTROLLER_H
