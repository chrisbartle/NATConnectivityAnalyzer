#include "NATConnectivityAnalyzer.h"


NATConnectivityAnalyzer::NATConnectivityAnalyzer(QWidget *parent)
    : QMainWindow(parent)
    , processingThread(this)
{
    ui.setupUi(this);

    //Create the connections between the logic code and the display elements
    connect(&logic, SIGNAL(updatedStatus(QString)), ui.status, SLOT(setText(QString)));
    connect(&logic, SIGNAL(updatedInternalIP(QString)), ui.internalIP, SLOT(setText(QString)));
    connect(&logic, SIGNAL(updatedExternalIP(QString)), ui.externalIP, SLOT(setText(QString)));
    connect(&logic, SIGNAL(updatedNATType(QString)), ui.natType, SLOT(setText(QString)));

    //Link up the processing thread
    logic.moveToThread(&processingThread);
    //Set DoFullAnalysis to be called when thread starts
    connect(this, SIGNAL(StartFullAnalysisOnThread()), &logic, SLOT(DoFullAnalysis()));
    //Wire up the connections so that analysisDone is called when processing is complete
    connect(&logic, SIGNAL(fullAnalysisCompleted()), this, SLOT(analysisDone()));
    //Active the thread
    processingThread.start();
}

//Called when the user closes the application
void NATConnectivityAnalyzer::closeEvent(QCloseEvent* event)
{
    processingThread.quit();
//Ideally we should wait for termination but it only throws an exception
//in debug mode so no reason to annoy the user.
//    processingThread.wait();
}


//Called when the user triggers the analysis
void NATConnectivityAnalyzer::on_analyzeButton_clicked()
{
    startAnalysis();
}

//Call this to start the analysis
void NATConnectivityAnalyzer::startAnalysis()
{
    //Disable the Analyze button so that use doesn't press it multiple times
    ui.analyzeButton->setEnabled(false);

    //Start the analysis in its own thread
    emit StartFullAnalysisOnThread();
}

//Called when the analysis is completed
void NATConnectivityAnalyzer::analysisDone()
{
    //Re-enable the button
    ui.analyzeButton->setEnabled(true);
}