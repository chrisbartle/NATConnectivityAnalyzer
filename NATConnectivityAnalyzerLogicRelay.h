#ifndef CNATCONNECTIVITYANALYZERLOGICRELAY_H
#define CNATCONNECTIVITYANALYZERLOGICRELAY_H

#include <QObject>

class CNATConnectivityAnalyzerLogicRelay : public QObject
{
    Q_OBJECT
public:
    explicit CNATConnectivityAnalyzerLogicRelay(QObject *parent = nullptr);

public slots:
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
