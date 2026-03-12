#include "MainThreadController.h"

MainThreadController::MainThreadController() {
    //Set up the worker thread
    m_workerController = new WorkerThreadController(); // No parent yet, will be moved
    m_workerThread = new QThread(this);
    m_workerController->moveToThread(m_workerThread);

    //Connect the signals
    //Signals to worker
    connect(this, &MainThreadController::doFullAnalysis, m_workerController, &WorkerThreadController::DoNATAnalysis);
    connect(this, &MainThreadController::currentStunServerChanged, m_workerController, &WorkerThreadController::ChangeStunServer);
    //Signals from woker
    connect(m_workerController, &WorkerThreadController::setIsProcessingNow, this, &MainThreadController::setIsProcessingNow);
    connect(m_workerController, &WorkerThreadController::setCurrentProcessingStatus, this, &MainThreadController::setCurrentProcessingStatus);
    connect(m_workerController, &WorkerThreadController::setExternalIP, this, &MainThreadController::setExternalIP);
    connect(m_workerController, &WorkerThreadController::setInternalIP, this, &MainThreadController::setInternalIP);
    m_workerThread->start();
}

MainThreadController::~MainThreadController() {
    if (m_workerThread)
    {
        // Proper thread cleanup
        m_workerThread->quit();
        m_workerThread->requestInterruption();
        m_workerThread->wait();
        delete m_workerThread;
    }
}

bool MainThreadController::getIsProcessingNow() const
{
    return m_isProcessingNow;
}

void MainThreadController::setIsProcessingNow(bool newIsProcessingNow)
{
    if (m_isProcessingNow == newIsProcessingNow)
        return;
    m_isProcessingNow = newIsProcessingNow;
    emit isProcessingNowStatusChanged(newIsProcessingNow);
}

QString MainThreadController::getCurrentStunServer() const
{
    return m_currentStunServer;
}

void MainThreadController::setCurrentStunServer(const QString &newCurrentStunServer)
{
    if (m_currentStunServer == newCurrentStunServer)
        return;
    m_currentStunServer = newCurrentStunServer;
    emit currentStunServerChanged(newCurrentStunServer);
}

QString MainThreadController::getCurrentProcessingStatus() const
{
    return m_currentProcessingStatus;
}

void MainThreadController::setCurrentProcessingStatus(const QString &newCurrentProcessingStatus)
{
    if (m_currentProcessingStatus == newCurrentProcessingStatus)
        return;
    m_currentProcessingStatus = newCurrentProcessingStatus;
    emit currentProcessingStatusChanged(newCurrentProcessingStatus);
}

QString MainThreadController::getExternalIP() const
{
    return m_externalIP;
}

void MainThreadController::setExternalIP(const QString &newExternalIP)
{
    if (m_externalIP == newExternalIP)
        return;
    m_externalIP = newExternalIP;
    emit externalIPChanged(newExternalIP);
}

QString MainThreadController::getInternalIP() const
{
    return m_internalIP;
}

void MainThreadController::setInternalIP(const QString &newInternalIP)
{
    if (m_internalIP == newInternalIP)
        return;
    m_internalIP = newInternalIP;
    emit internalIPChanged(newInternalIP);
}

NAT_TYPE MainThreadController::getNatType() const
{
    return m_natType;
}

void MainThreadController::setNatType(const NAT_TYPE &newNatType)
{
    if (m_natType == newNatType)
        return;
    m_natType = newNatType;
    emit natTypeChanged();
}
