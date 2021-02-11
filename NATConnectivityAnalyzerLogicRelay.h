#ifndef CNATCONNECTIVITYANALYZERLOGICRELAY_H
#define CNATCONNECTIVITYANALYZERLOGICRELAY_H

#include <QObject>

//This relay object exists so that it can sit on the main thread and then
//pass slots and signals to a worker thread. Strangely, the C++ code handles
//cross thread communication automatically but QML does not.
class CNATConnectivityAnalyzerLogicRelay : public QObject
{
    Q_OBJECT
public:
    explicit CNATConnectivityAnalyzerLogicRelay(QObject *parent = nullptr);

public slots:
    //Redirect this slot to a signal so that it can be routed to CNATConnectivityAnalyzerLogic::DoFullAnalysis
    void DoFullAnalysis() { emit startFullAnalysis(); }

signals:
    void startFullAnalysis();
    void fullAnalysisCompleted();
    void updatedStatus(const QString&);
    void updatedInternalIP(const QString&);
    void updatedExternalIP(const QString&);
    void updatedNATType(const QString&);
};

#endif // CNATCONNECTIVITYANALYZERLOGICRELAY_H
