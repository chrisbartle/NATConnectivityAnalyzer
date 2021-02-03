import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Page {
    Connections {
        target: businessLogic
        function onUpdatedInternalIP(newIP) {
            internalIPDisplay.text = newIP
        }
        function onUpdatedExternalIP(newIP) {
            externalIPDisplay.text = newIP
        }
        function onUpdatedNATType(newNATType) {
            natTypeDisplay.text = newNATType
        }
    }

    Column {
        spacing: 40
        width: parent.width

        ColumnLayout {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: internalIPDisplay
                text: "Internal IP"
            }

            Label {
                id: externalIPDisplay
                text: "External IP"
            }

            Label {
                id: natTypeDisplay
                text: "NAT Type"
            }
        }
    }
}
