import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: window
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
