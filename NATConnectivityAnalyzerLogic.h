#pragma once
#include <qobject.h>


class CNATConnectivityAnalyzerLogic :
    public QObject
{
    Q_OBJECT

public slots:
    void DoFullAnalysis();

signals:
    void fullAnalysisCompleted();
    void updatedStatus(const QString&);
    void updatedInternalIP(const QString&);
    void updatedExternalIP(const QString&);
    void updatedNATType(const QString&);

};

