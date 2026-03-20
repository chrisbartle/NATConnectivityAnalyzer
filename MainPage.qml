import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {
    function describeNatType()
    {
        switch (Controller.natType)
        {
            case "ERROR_DETECTING_NAT": return "Error Detecting NAT";
            case "FIREWALL_BLOCKS_UDP": return "Firewall is Blocking UDP";
            case "FULL_CONE_NAT": return "Full Cone NAT";
            case "OPEN_INTERNET": return "Open Internet";
            case "RESTRICTED_CONE_NAT": return "Restricted Cone NAT";
            case "RESTRICTED_PORT_CONE_NAT": return "Restricted Port Cone NAT";
            case "SYMMETRIC_NAT": return "Symmetric NAT";
            case "SYMMETRIC_UDP_FIREWALL": return "Symmetric UDP Firewall";
        }
        return ""
    }

    ColumnLayout
    {
        anchors.fill: parent

        Flickable {
            id: scrollContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: mainColumn.height
            clip: true // Prevents text from drawing outside this area
            // This allows the user to drag/flick to scroll
            boundsBehavior: Flickable.StopAtBounds

            ColumnLayout {
                id: mainColumn
                spacing: 20
                width: scrollContainer.width
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 20

                ComboBox {
                    model: ["stun.12connect.com", "stun.easyvoip.com", "stun.freecall.com", "stun.nextcloud.com:443", "stun.xten.com"]
                    editable: true
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    onEditTextChanged: Controller.currentStunServer = editText
                    Component.onCompleted: {
                        if (model.length > 0) {
                            currentIndex = Math.floor(Math.random() * model.length)
                        }
                    }
                }

                Button {
                    text: "Start"
                    font.pixelSize: 25
                    enabled: !Controller.isProcessingNow
                    Layout.fillWidth: true
                    onClicked: Controller.doFullAnalysis();
                }

                //Internal IP Display
                ColumnLayout {
                    Layout.fillWidth: true
                    property bool showThis: Controller.internalIP.length > 0
                    opacity: showThis ? 1 : 0
                    visible: opacity > 0
                    Layout.preferredHeight: showThis ? implicitHeight : 0
                    spacing: 5
                    Label {
                        text: "Internal IP"
                        font.bold: true
                    }
                    Frame {
                        Layout.fillWidth: true
                        TextEdit {
                            text: Controller.internalIP
                            readOnly: true
                            selectByMouse: true
                            wrapMode: TextEdit.WrapAnywhere
                        }
                    }
                    Behavior on opacity {
                        NumberAnimation { duration: 250 }
                    }
                    Behavior on Layout.preferredHeight {
                        NumberAnimation { duration: 250 }
                    }
                }

                //External IP Display
                ColumnLayout {
                    Layout.fillWidth: true
                    property bool showThis: Controller.externalIP.length > 0
                    opacity: showThis ? 1 : 0
                    visible: opacity > 0
                    Layout.preferredHeight: showThis ? implicitHeight : 0
                    spacing: 5
                    Label {
                        text: "External IP"
                        font.bold: true
                    }
                    Frame {
                        Layout.fillWidth: true
                        TextInput{
                            text: Controller.externalIP
                            readOnly: true
                            selectByMouse: true
                        }
                    }
                    Behavior on opacity {
                        NumberAnimation { duration: 250 }
                    }
                    Behavior on Layout.preferredHeight {
                        NumberAnimation { duration: 250 }
                    }
                }

                //Nat Type Display
                ColumnLayout {
                    Layout.fillWidth: true
                    property bool showThis: Controller.natType.length > 0
                    opacity: showThis ? 1 : 0
                    visible: opacity > 0
                    Layout.preferredHeight: showThis ? implicitHeight : 0
                    spacing: 5
                    Label {
                        text: "NAT Type"
                        font.bold: true
                    }
                    Frame {
                        Layout.fillWidth: true
                        RowLayout {
                            TextInput {
                                text: describeNatType()
                                readOnly: true
                                selectByMouse: true
                            }
                            Button {
                                Layout.alignment: Qt.AlignRight
                                font.pixelSize: 20
                                implicitWidth: 20
                                text: "?"
                                onClicked: mainStackView.push("NatTypeExplanation.qml")
                            }
                        }
                    }
                    Behavior on opacity {
                        NumberAnimation { duration: 250 }
                    }
                    Behavior on Layout.preferredHeight {
                        NumberAnimation { duration: 250 }
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
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
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
