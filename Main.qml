import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Controller 1.0

Window {
    width: 300
    height: 550
    visible: true
    title: qsTr("NAT Connectivity Analyzer")

    StackView {
        id: mainStackView
        initialItem: "MainPage.qml"
        anchors.fill: parent
//        Layout.fillWidth: true
//        Layout.fillHeight: true
    }
}
