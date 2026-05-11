import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {
    function fullDescriptionPortForward()
    {
        switch (Controller.portForwardType)
        {
            case "NONE": return qsTr("<P><B>Automatic Port Forwarding is not Supported</B></P>
<P>The local router did not respond the requests to forward a port using PMP, PCP, or UPnP protocols.</P>");
            case "DOUBLE_NAT": return qsTr("<P><B>Double NAT Detected</B></P>
<P>The local router reported a different external IP address than the STUN server detected. This indicates a double NAT.</P>");
            case "BIDIRECTIONAL": return qsTr("<P><B>Bi-Directional</B></P>
<P>The local router opened a bi-directional port. Both outgoing and incoming traffic use the same external port number.</P>");
            case "MONODIRECTIONAL": return qsTr("<P><B>One-Directional</B></P>
<P>The local router is forwarding incoming traffic from an external port but outgoing traffic uses a different port number.</P>");
            case "BLOCKED": return qsTr("3rd Party Traffic is Blocked</B></P>
<P>The router forwarded a port as instructed but the STUN server was unable to send traffic through the forwarded port.
Rerun the test using a different STUN server but if this error recurs then it indicates that something is blocking 3rd party
traffic to this device.</P>");
            case "FAILED": return qsTr("<P><B>Test Failed</B></P>
<P>The local router reported that the port has been forwarded but the STUN server is not responding correctly. Rerun
the test using a different STUN server.</P>");
        }
        return ""
    }

    ColumnLayout {
        spacing: 20
        anchors.fill: parent

        Flickable {
            id: scrollContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: textDisplayer.height
            clip: true // Prevents text from drawing outside this area
            // This allows the user to drag/flick to scroll
            boundsBehavior: Flickable.StopAtBounds
            TextEdit {
                id: textDisplayer
                width: scrollContainer.width
                leftPadding: 10
                rightPadding: 10
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.pixelSize: 15
                text: fullDescriptionPortForward()
                readOnly: true
            }
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }
        Button {
            text: "Back"
            onClicked: mainStackView.pop()
            font.pixelSize: 25
            height: 30
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Layout.fillWidth: true
        }
    }
}