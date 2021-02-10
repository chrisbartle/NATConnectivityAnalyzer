import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Page {
    ColumnLayout {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Button {
            id: startButton
            Layout.alignment: Qt.AlignHCenter
            text: "Begin Analysis"
            onClicked: {
                mainStackView.push("Results.qml")
                businessLogic.DoFullAnalysis();
            }
        }
        Button {
            id: creditsButton
            Layout.alignment: Qt.AlignHCenter
            text: "Credits"
            onClicked: text = "blah"
        }
    }
}
