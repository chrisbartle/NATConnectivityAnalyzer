import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: window
    width: 200
    height: 480
    visible: true
    title: qsTr("NAT Connectivity Analyzer")

    StackView {
        id: stackView
        initialItem: "Intro.ui.qml"
        anchors.fill: parent
    }
}
