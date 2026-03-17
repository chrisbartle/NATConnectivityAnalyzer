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
                    Layout.fillWidth: true
                    visible: Controller.internalIP.length > 0
                    TextInput {
                        text: Controller.internalIP
                        readOnly: true
                        selectByMouse: true
                    }
                }

                GroupBox {
                    title: "External IP"
                    id: externalIPGroupBox
                    Layout.fillWidth: true
                    visible: Controller.externalIP.length > 0
                    TextInput {
                        text: Controller.externalIP
                        readOnly: true
                        selectByMouse: true
                    }
                }

                GroupBox {
                    title: "NAT Type"
                    id: natTypeGroupBox
                    Layout.fillWidth: true
                    visible: Controller.natType.length > 0
                    RowLayout {
                        //Layout.fillWidth: true
                        TextInput {
                            text: describeNatType()
                            readOnly: true
                            selectByMouse: true
                            Layout.fillWidth: true
                        }
                        Button {
                            Layout.alignment: Qt.AlignRight
                            font.pixelSize: 20
                            implicitWidth: 20
                            text: "?"
                            //icon.name: "help-about"
                            //display: AbstractButton.IconOnly
                            onClicked: mainStackView.push("NatTypeExplanation.qml")
                        }
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
