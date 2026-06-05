import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {
    function fullDescriptionNatType()
    {
        switch (Controller.natType)
        {
            case "ERROR_DETECTING_NAT": return qsTr("<P><B>Error Detecting NAT</B></P>
<P>This error usually occurs due to a badly formed STUN server name. Check the name or choose a different STUN server.</P>");
            case "FIREWALL_BLOCKS_UDP": return qsTr("<P><B>Firewall is Blocking UDP</B></P>
<P>This software is unable to communicate with the STUN server. This may because a firewall is blocking UDP port 3478
but it could also be because the STUN server is not responding. Try a different STUN server.</P>");
            case "FULL_CONE_NAT": return qsTr("<P><B>Full Cone NAT (NAT 1)</B></P>
<P>A full cone NAT will accept communication with any other computer which makes it vulnerable to 3rd parties
scanning your network for security holes. The full cone NAT tends to indicate a cheaper or more basic
NAT device, usually a consumer modem.</P>
<P><B>Security:</B> Poor, 3rd parties can scan your network for exploits.</P>
<P><B>Peer to Peer:</B> Excellent, peer-to-peer products may be able to connect even without a matchmaking service.</P>");
            case "OPEN_INTERNET": return qsTr("<P><B>Open Internet</B></P>
<P>No NAT was detected, it appears that this computer is connecting openly to the Internet - old school!</P>
<P><B>Security:</B> None, there appears to be no firewall or NAT protection and this computer may be vulnerable to
scanning and attack from 3rd parties.</P>
<P><B>Peer to Peer:</B> Excellent, peer-to-peer products can easily connect without a matchmaking service.</P>");
            case "RESTRICTED_CONE_NAT": return qsTr("<P><B>Restricted Cone NAT (NAT 2)</B></P>
<P>A restricted cone NAT will accept communication with any other computer, so long as this computer
initiates the communication. This type of NAT offers a good balance between security and useability and is quite
common among consumer routers and modems.</P>
<P><B>Security:</B> Fair, 3rd parties can not contact this computer but any computer you initiate contact with may be able
to scan your computer for exploits.</P>
<P><B>Peer to Peer:</B> Good, peer-to-peer products that use a matchmaking service will be able to connect.</P>");
            case "RESTRICTED_PORT_CONE_NAT": return qsTr("<P><B>Restricted Port Cone NAT (NAT 3)</B></P>
<P>A restricted port cone NAT will accept communication with any other computer, so long as this computer
initiates the communication. This type of NAT offers a good balance between security and useability and is probably
the best option for non-business consumers.</P>
<P><B>Security:</B> Good, 3rd parties can not contact this computer.</P>
<P><B>Peer to Peer:</B> Good, peer-to-peer products that use a matchmaking service will be able to connect.</P>");
            case "SYMMETRIC_NAT": return qsTr("<P><B>Symmetric NAT (NAT 4)</B></P>
<P>A symmetric NAT generates a unique external port number for every external address and requires that this computer
initiate contact. Symmetric NATs are typically a business-class product and are designed to allow client/server
interaction while making peer-to-peer connectivity effectively impossible.</P>
<P><B>Security:</B> Good, 3rd parties can not contact this computer.</P>
<P><B>Peer to Peer:</B> Poor, most peer-to-peer technologies will be unable to connect to your computer.</P>");
            case "SYMMETRIC_UDP_FIREWALL": return qsTr("<P><B>Symmetric UDP Firewall</B></P>
<P>This computer has a global IP address but there is a firewall that only allows traffic from computers that
this computer has initiated communication with.</P>
<P><B>Security:</B> Good, 3rd parties can not interfere with your computer through the firewall.</P>
<P><B>Peer to Peer:</B> Good, peer-to-peer products that use a matchmaking service will be able to connect.</P>");
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
                leftPadding: 20
                rightPadding: 20
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                font.pixelSize: 15
                text: fullDescriptionNatType()
                readOnly: true
                //Explicitly set text color for when user changes system colors
                color: palette.windowText
                selectionColor: palette.highlight
                selectedTextColor: palette.highlightedText
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