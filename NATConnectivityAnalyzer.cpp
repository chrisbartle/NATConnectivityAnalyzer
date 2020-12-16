#include "NATConnectivityAnalyzer.h"


NATConnectivityAnalyzer::NATConnectivityAnalyzer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //Initialize variables
    processingThread = NULL;

    //Create the connections between the logic code and the display elements
    connect(&logic, SIGNAL(updatedStatus(QString)), ui.status, SLOT(setText(QString)));
    connect(&logic, SIGNAL(updatedInternalIP(QString)), ui.internalIP, SLOT(setText(QString)));
    connect(&logic, SIGNAL(updatedExternalIP(QString)), ui.externalIP, SLOT(setText(QString)));
    connect(&logic, SIGNAL(updatedNATType(QString)), ui.natType, SLOT(setText(QString)));

   
}

//Called when the user triggers the analysis
void NATConnectivityAnalyzer::on_analyzeButton_clicked()
{
    startAnalysis();
}

//Call this to start the analysis
void NATConnectivityAnalyzer::startAnalysis()
{
    //If the thread already exists, don't do anything
    if (processingThread)
        return;

    //Create a new processing thread and link the logic to it
    processingThread = new QThread(this);
    logic.moveToThread(processingThread);

    //Set DoFullAnalysis to be called when thread starts
    connect(processingThread, SIGNAL(started()), &logic, SLOT(DoFullAnalysis()));
    //Wire up the connections so that analysisDone is called when processing is complete
    connect(&logic, SIGNAL(fullAnalysisCompleted()), processingThread, SLOT(quit()));
    connect(processingThread, SIGNAL(finished()), this, SLOT(analysisDone()));

    //Disable the Analyze button so that use doesn't press it multiple times
    ui.analyzeButton->setEnabled(false);

    //Start the analysis in its own thread
    processingThread->start();
}

//Called when the analysis is completed
void NATConnectivityAnalyzer::analysisDone()
{
    //Clean up the thread
    delete processingThread;
    processingThread = NULL;

    //Re-enable the button
    ui.analyzeButton->setEnabled(true);
}