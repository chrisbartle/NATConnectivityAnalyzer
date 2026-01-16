import QtQuick
import QtQuick.Controls

Window {
    width: 300
    height: 500
    visible: true
    title: qsTr("NAT Connectivity Analyzer")

    StackView {
        id: mainStackView
        initialItem: "Intro.qml"
        anchors.fill: parent
    }
}
