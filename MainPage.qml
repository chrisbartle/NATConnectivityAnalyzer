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
    function describePortForwardType()
    {
        switch (Controller.portForwardType)
        {
            case "NONE": return "Not Supported";
            case "DOUBLE_NAT": return "Double NAT Detected";
            case "BIDIRECTIONAL": return "Bi-Directional";
            case "BIDIRECTIONAL_UNCONFIRMED": return "Bi-Directional (unconfirmed)";
            case "MONODIRECTIONAL": return "One-Directional";
            case "MONODIRECTIONAL_UNCONFIRMED": return "One-Directional (unconfirmed)";
            case "FAILED": return "Test Failed";
        }
        return ""
    }
    //Enable/disable after the program runs once
    property bool isRunOnce: false;

    ColumnLayout
    {
        anchors.fill: parent

        Flickable {
            id: scrollContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: mainColumn.implicitHeight
            clip: true // Prevents text from drawing outside this area
            // This allows the user to drag/flick to scroll
            boundsBehavior: Flickable.StopAtBounds

            Item {
                id: mainColumnWrapper
                width: scrollContainer.width

                ColumnLayout {
                    id: mainColumn
                    spacing: 20
                    width: parent.width - 40   // 20px margin on each side
                    anchors.horizontalCenter: parent.horizontalCenter

                    // TOP PADDING
                    Item { Layout.preferredHeight: 10 }

                    //Preamble
                    ColumnLayout {
                        Layout.fillWidth: true
                        property bool showThis: !isRunOnce && !Controller.isProcessingNow
                        opacity: showThis ? 1 : 0
                        visible: opacity > 0
                        Layout.preferredHeight: showThis ? implicitHeight : 0
                        Label {
                            Layout.fillWidth: true
                            text: "NAT Connectivity Analyzer"
                            wrapMode: Text.Wrap
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "Version " + Controller.applicationVersion
                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "Network Address Translation (NAT) devices are responsible for sharing a single Internet (IP) address with all of the devices on the local network."
                            wrapMode: Text.Wrap
                            //horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "In order to determine how the local NAT device works, this software must communicate with a STUN server. A STUN server is located outside of the local network. You may enter the address of a STUN server you wish to use or select one from the list below."
                            wrapMode: Text.Wrap
                            //horizontalAlignment: Text.AlignHCenter
                        }
                        Label {
                            Layout.fillWidth: true
                            text: "It is always a good idea to run multiple tests using different STUN servers."
                            wrapMode: Text.Wrap
                            //horizontalAlignment: Text.AlignHCenter
                        }
                        Behavior on opacity {
                            NumberAnimation { duration: 250 }
                        }
                        Behavior on Layout.preferredHeight {
                            NumberAnimation { duration: 250 }
                        }
                    }

                    ComboBox {
                        model: ["stun.12connect.com", "stun.schlund.de", "stun.easyvoip.com", "stun.ekiga.net", "stun.freecall.com", "stun.miwifi.com", "stun.nextcloud.com:443", "stun.voipbuster.com", "stun.voipstunt.com", "stun.xten.com"]
                        editable: true
                        font.pixelSize: 20
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 35
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
                        onClicked: {Controller.doFullAnalysis(); isRunOnce = true;}
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
                            padding: 5
                            RowLayout {
                                anchors.fill: parent
                                TextInput {
                                    Layout.fillWidth: true
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

                    //Port Forward Type Display
                    ColumnLayout {
                        Layout.fillWidth: true
                        property bool showThis: Controller.portForwardType.length > 0
                        opacity: showThis ? 1 : 0
                        visible: opacity > 0
                        Layout.preferredHeight: showThis ? implicitHeight : 0
                        spacing: 5
                        Label {
                            text: "Port Forward Type"
                            font.bold: true
                        }
                        Frame {
                            Layout.fillWidth: true
                            padding: 5
                            RowLayout {
                                anchors.fill: parent
                                TextInput {
                                    Layout.fillWidth: true
                                    text: describePortForwardType()
                                    readOnly: true
                                    selectByMouse: true
                                }
                                Button {
                                    Layout.alignment: Qt.AlignRight
                                    font.pixelSize: 20
                                    implicitWidth: 20
                                    text: "?"
                                    onClicked: mainStackView.push("PortForwardExplanation.qml")
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

                    //UPnP Router Information
                    ColumnLayout {
                        Layout.fillWidth: true
                        property bool showThis: ((Controller.upnpRouterInformation.length > 0) && (showDiagnostics.checked))
                        opacity: showThis ? 1 : 0
                        visible: opacity > 0
                        Layout.preferredHeight: showThis ? implicitHeight : 0
                        spacing: 5
                        Label {
                            text: "UPnP Router Information"
                            font.bold: true
                        }
                        Frame {
                            Layout.fillWidth: true
                            TextEdit {
                                anchors.fill: parent
                                text: Controller.upnpRouterInformation
                                readOnly: true
                                selectByMouse: true
                                wrapMode: TextEdit.Wrap
                                font.pixelSize: 10
                                font.family: "monospace"
                                // optional padding
                                leftPadding: 10
                                rightPadding: 10
                            }
                        }
                        Behavior on opacity {
                            NumberAnimation { duration: 250 }
                        }
                        Behavior on Layout.preferredHeight {
                            NumberAnimation { duration: 250 }
                        }
                    }


                    //Nat Test Log
                    ColumnLayout {
                        Layout.fillWidth: true
                        property bool showThis: ((Controller.natTestLog.length > 0) && (showDiagnostics.checked))
                        opacity: showThis ? 1 : 0
                        visible: opacity > 0
                        Layout.preferredHeight: showThis ? implicitHeight : 0
                        spacing: 5
                        Label {
                            text: "NAT Test Log"
                            font.bold: true
                        }
                        Frame {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 200
                            ScrollView {
                                anchors.fill: parent

                                TextArea {
                                    id: natLogtextDisplayer
                                    text: Controller.natTestLog
                                    readOnly: true
                                    wrapMode: TextArea.Wrap
                                    font.pixelSize: 10
                                    font.family: "monospace"

                                    // optional padding
                                    leftPadding: 10
                                    rightPadding: 10
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
                        Layout.fillWidth: true
                        BusyIndicator {
                            running: Controller.isProcessingNow
                        }
                        Label {
                            Layout.fillWidth: true
                            text: Controller.currentProcessingStatus
                            wrapMode: TextArea.Wrap
                        }
                    }

                    //Show Diagnostics checkbox. Only seen by the user after one run.
                    CheckBox {
                        id: showDiagnostics
                        text: "Show Diagnostics"
                        Layout.fillWidth: true
                        visible: isRunOnce && !Controller.isProcessingNow
                    }

                    // BOTTOM PADDING
                    Item { Layout.preferredHeight: 10 }
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
