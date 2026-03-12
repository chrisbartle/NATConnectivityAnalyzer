import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {

    ColumnLayout
    {
        anchors.fill: parent

            ColumnLayout {
                id: mainColumn
                spacing: 20

                Layout.leftMargin: 20
                Layout.rightMargin: 20

                ComboBox {
                    model: ["stun.l.google.com:19302", "stun.ooma.com", "stun.stunprotocol.org", "stunserver.org", "stun.xten.com"]
                    editable: true
                    font.pixelSize: 25
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    onCurrentTextChanged: Controller.currentStunServer = currentText
                    //currentText: Controller.currentStunServer
                }

                Button {
                    text: "Start"
                    font.pixelSize: 25
                    enabled: !Controller.isProcessingNow
                    Layout.fillWidth: true
                    onClicked: Controller.doFullAnalysis();
                }

                GroupBox {
                    title: "Internal IP"
                    id: internalIPGroupBox
                    visible: Controller.internalIP.length > 0
                    TextInput {
                        text: Controller.internalIP
                        readOnly: true
                        selectByMouse: true
                    }
                }

                RowLayout {
                    id: statusRow
                    visible: Controller.isProcessingNow
                    BusyIndicator {
                        running: Controller.isProcessingNow
                    }
                    Label {
                        text: Controller.currentProcessingStatus
                    }
                }
            }

        Button {
            text: 'Credits'
            onClicked: mainStackView.push("Credits.qml")
            font.pixelSize: 25
            height: 30
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Layout.fillWidth: true
        }
    }
}
