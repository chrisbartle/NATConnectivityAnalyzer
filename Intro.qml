import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Page {
    Column {
        spacing: 40
        width: parent.width


        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "NAT (Network Address Translation) Connectivity Analyzer"
        }


        ColumnLayout {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: startButton
                text: "Begin Analysis"
                onClicked: {
                    mainStackView.push("Results.qml")
                    businessLogic.DoFullAnalysis();
                }
            }
            Button {
                id: creditsButton
                text: "Credits"
            }
        }
    }
}
