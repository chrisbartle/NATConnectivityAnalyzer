import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Page {

    ColumnLayout
    {
        anchors.fill: parent
//        anchors.top: parent.top
//        anchors.left: parent.left
//        width: parent.width
//        height: parent.height
//        Layout.fillWidth: true
//        Layout.fillHeight: true

//        ScrollView
//        {
//            anchors.top: parent.top
//            anchors.left: parent.left
//            width: parent.width
//            height: parent.height
///            Layout.fillWidth: true
//            Layout.fillHeight: true
            //ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            ColumnLayout {
                spacing: 20
                anchors.fill: parent
                anchors.margins: 20

                ComboBox {
                    model: ["stun.l.google.com:19302", "stun.ooma.com:3478", "stun.stunprotocol.org:3478", "stunserver.org:3478", "stun.xten.com"]
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
                    Layout.fillWidth: true
                    onClicked: Controller.doFullAnalysis();
                }
            }
//        }

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
