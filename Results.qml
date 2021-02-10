import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Page {
    Connections {
        target: businessLogic
        function onUpdatedStatus(newStatus) {
            statusDisplay.text = newStatus
            statusRow.visible = true
        }
        function onUpdatedInternalIP(newIP) {
            internalIPDisplay.text = newIP
            internalIPGroupBox.visible = true
        }
        function onUpdatedExternalIP(newIP) {
            externalIPDisplay.text = newIP
            externalIPGroupBox.visible = true
        }
        function onUpdatedNATType(newNATType) {
            natTypeDisplay.text = newNATType
            natTypeGroupBox.visible = true
        }
        function onFullAnalysisCompleted() {
            statusRow.visible = false
            runButton.visible = true
            runButton.enabled = true
        }
    }

    ColumnLayout {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        RowLayout {
            id: statusRow
            spacing: 5
            visible: false
            BusyIndicator {
                running: true
            }
            Label {
                id: statusDisplay
            }
        }

        GroupBox {
            title: "Internal IP"
            id: internalIPGroupBox
            visible: false
            TextInput {
                id: internalIPDisplay
                readOnly: true
                selectByMouse: true
            }
        }

        GroupBox {
            title: "External IP"
            id: externalIPGroupBox
            visible: false
            TextInput {
                id: externalIPDisplay
                readOnly: true
                selectByMouse: true
            }
        }

        GroupBox {
            title: "NAT Type"
            id: natTypeGroupBox
            visible: false
            TextInput {
                id: natTypeDisplay
                readOnly: true
                selectByMouse: true
            }
        }

        Button {
            id: runButton
            text: "Rerun Analysis"
            visible: false
            enabled: false
            onClicked: {
                enabled = false
                businessLogic.DoFullAnalysis();
            }
        }
    }
}
